#include "GameObject.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "debugdraw.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "SDL.h"
#include "IconsFontAwesome5.h"
#include "Geometry/AABB.h"


GameObject::GameObject()
{}

GameObject::GameObject(const char* name)
{
	this->name = name;
	isSelectedInHierarchy = false;
	CreateComponent(TRANSFORM);
}


GameObject::~GameObject()
{
	delete boundingBox;
}

void GameObject::Update()
{
	for(auto component : components)
	{
		if (component->myType != TRANSFORM)
			component->Update();
	}
}

void GameObject::SetParent(GameObject* newParent)
{
	if (parent != nullptr)
	{
		parent->RemoveChildren(this);
	}
		
	if(newParent != nullptr)
	{
		LOG("Setting new parent")
		parent = newParent;
		parent->childrenVector.push_back(this);
	}
	else
	{
		LOG("Error setting new parent. Parameter was null.");
	}
}

void GameObject::RemoveChildren(GameObject* child)
{
	if(!childrenVector.empty())
	{
		childrenVector.erase(std::remove(childrenVector.begin(), childrenVector.end(), child), childrenVector.end());
	}
}

void GameObject::DeleteGameObject()
{
	parent->RemoveChildren(this);
	App->scene->RemoveGameObject(this);
	CleanUp();
}

void GameObject::CleanUp()
{
	for(auto comp : components)
	{
		delete comp;
	}
	delete boundingBox;
	delete globalBoundingBox;
	delete this;
}

Component* GameObject::CreateComponent(ComponentType type)
{
	
	Component* component = GetComponent(type);
	if (component != nullptr && !component->allowMany) return nullptr;

	switch(type)
	{
		case TRANSFORM:
			component = new ComponentTransform(this);
			myTransform = (ComponentTransform*)component;
			break;
		case MESH:
			component = new ComponentMesh();
			myMesh = (ComponentMesh*)component;
			createAABBs();
			break;
		case MATERIAL:
			component = new ComponentMaterial();
			myMaterial = (ComponentMaterial*)component;
			break;

		case CAMERA:
			component = new ComponentCamera();
			break;
		default:
			LOG("ERROR: INVALID TYPE OF COMPONENT");
			return nullptr;
			break;
	}
	component->myGameObject = this;
	components.push_back(component);

	return component;
}

Component* GameObject::GetComponent(ComponentType type)
{
	Component* found = nullptr;

	for (auto comp : components)
	{
		if (comp->myType == type) found = comp;
	}
	
	return found;
}


void GameObject::DrawHierarchy(GameObject * selected)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | (selected == this ? ImGuiTreeNodeFlags_Selected : 0);

	ImGui::PushID(this);
	if (childrenVector.empty())
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}
	if (App->scene->selectedByHierarchy == this) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 0.52f, 1.f));
	
	bool objOpen = ImGui::TreeNodeEx(this, flags, name.c_str());
	
	if (App->scene->selectedByHierarchy == this) ImGui::PopStyleColor();
	
	if(ImGui::IsItemClicked())
	{
		if (isSelectedInHierarchy) App->scene->selectRoot();
		else App->scene->SelectObjectInHierarchy(this);

		isSelectedInHierarchy = !isSelectedInHierarchy;
	}

	if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && ImGui::IsWindowHovered())
	{
		ImGui::OpenPopup("Creation Popup");
	}

	if (ImGui::BeginPopup("Creation Popup"))
	{
		if (ImGui::Selectable("Copy"))
		{
			//TODO: Copy gameobjects
		}
		if (ImGui::Selectable("Paste"))
		{
			//TODO: Paste gameobjects
		}
		
		ImGui::Separator();

		if (ImGui::Selectable("Rename"))
		{
			//TODO: Rename gameobjects
		}
		if (ImGui::Selectable("Duplicate"))
		{
			//TODO: Duplicate gameobjects
		}
		if (ImGui::Selectable("Delete"))
		{
			//TODO: Delete gameobjects
			//DeleteGameObject();
		}

		ImGui::Separator();

		if(ImGui::Selectable("Create Empty GameObject"))
		{
			//Create empty gameobject
			App->scene->CreateEmpty(this);
		}

		if (ImGui::BeginMenu("Create 3D Object"))
		{
			if(ImGui::MenuItem("Cube"))
			{
				App->scene->CreateGameObjectShape(this, CUBE);
			}
			if (ImGui::MenuItem("Sphere"))
			{
				App->scene->CreateGameObjectShape(this, SPHERE);
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				App->scene->CreateGameObjectShape(this, CYLINDER);
			}
			if (ImGui::MenuItem("Torus"))
			{
				App->scene->CreateGameObjectShape(this, TORUS);
			}
			if (ImGui::MenuItem("Baker House"))
			{
				// TODO :CreateGameObjectBakerHouse();
				App->scene->CreateGameObjectBakerHouse(this);
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}

	CheckDragAndDrop(this);

	if(objOpen)
	{
		for(auto child : childrenVector)
		{
			child->DrawHierarchy(selected);
		}
		if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			ImGui::TreePop();
		}
	}
	ImGui::PopID();
}

void GameObject::UpdateTransform()
{
	if(myTransform != nullptr)
	{
		if(parent != nullptr && parent->myTransform != nullptr)
		{
			myTransform->SetGlobalMatrix(parent->myTransform->globalModelMatrix);
		}
		myTransform->UpdateMatrices();

		if(globalBoundingBox != nullptr && boundingBox != nullptr)
		{
			createAABBs();
		}
	}
}

void GameObject::SetName(const std::string &newName)
{
	name = newName;
}

std::string GameObject::GetName() const
{
	return name;
}

void GameObject::createAABBs()
{
	float3 min = float3::zero;
	float3 max = float3::zero;
	boundingBox = new AABB(min, max);
	obb = new OBB(*boundingBox);
	globalBoundingBox = new AABB(min, max);

	if(myMesh == nullptr)
	{
		LOG("This gameObject does not have a Mesh thus we compute the AABB from his childs.");

		if(childrenVector.size() == 0)
		{
			LOG("Cannot compute the AABB because gameObject does not have children.");
			return;	//leave at this point
		}

		for(auto child : childrenVector)
		{
			if(child->boundingBox != nullptr)
			{
				boundingBox->Enclose(*child->boundingBox);
			}
		}
	}
		
	if (myMesh->myMesh != nullptr)
	{
		for (Vertex vertex : myMesh->myMesh->vertices)
		{
			float3 v = float3(vertex.Position.x, vertex.Position.y, vertex.Position.z);
			obb->Enclose(v);
		}
	}

	findOBBPoints();
	boundingBox->Enclose(obbPoints, 8);
	boundingBox->TransformAsAABB(myTransform->localModelMatrix);
	obb->Transform(myTransform->localModelMatrix);
}

void GameObject::DrawAABB()
{
	//Bounding Box
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	glColor4f(0.6f, 0.6f, 0.6f, 1.0f);

	ComponentCamera* cam = (ComponentCamera*) GetComponent(CAMERA);
	if (cam != nullptr) cam->DrawFrustum();

	if (obb != nullptr)
	{
		findOBBPointsForRender();
		dd::box(obbPoints, float3(0.7f, 0.7f, 0.7f));
	}
	if (boundingBox != nullptr) dd::aabb(boundingBox->minPoint, boundingBox->maxPoint, float3(0.6f, 0.6f, 0.6f));
	glEnd();
}

void GameObject::Draw(GLuint program)
{
	if (myMesh != nullptr)
	{
		myMesh->Draw(program);
		DrawAABB();
	}
}

void GameObject::DrawInspector()
{
	ImGui::Checkbox("", &isEnabled); ImGui::SameLine();
	
	char* go_name = new char[64];
	strcpy(go_name, name.c_str());
	if(ImGui::InputText("##Name", go_name, 64))
	{
		name = std::string(go_name);
	}
	ImGui::SameLine();

	delete go_name;

	ImGui::Checkbox("Static", &isStatic);

	//Components
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->DrawInspector();
	}

	//Change EulerRotation to Quat
	myTransform->EulerToQuat();
}

void GameObject::CheckDragAndDrop(GameObject* go)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
		ImGui::SetDragDropPayload("DRAG", &go, sizeof(GameObject*));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG");
		if (payload != nullptr) {
			GameObject* newChild = *reinterpret_cast<GameObject**>(payload->Data);
			newChild->SetParent(go);
			if(newChild->parent->myTransform != nullptr)
				newChild->myTransform->SetLocalMatrix(newChild->parent->myTransform->globalModelMatrix);
		}
		ImGui::EndDragDropTarget();
	}
}

void GameObject::findOBBPointsForRender()
{
	obbPoints[0] = obb->CornerPoint(0);
	obbPoints[1] = obb->CornerPoint(1);
	obbPoints[2] = obb->CornerPoint(3);
	obbPoints[3] = obb->CornerPoint(2);
	obbPoints[4] = obb->CornerPoint(4);
	obbPoints[5] = obb->CornerPoint(5);
	obbPoints[6] = obb->CornerPoint(7);
	obbPoints[7] = obb->CornerPoint(6);
}

void GameObject::findOBBPoints()
{
	obbPoints[0] = obb->CornerPoint(0);
	obbPoints[1] = obb->CornerPoint(1);
	obbPoints[2] = obb->CornerPoint(2);
	obbPoints[3] = obb->CornerPoint(3);
	obbPoints[4] = obb->CornerPoint(4);
	obbPoints[5] = obb->CornerPoint(5);
	obbPoints[6] = obb->CornerPoint(6);
	obbPoints[7] = obb->CornerPoint(7);
}
