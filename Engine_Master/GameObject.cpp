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


GameObject::GameObject()
{}

GameObject::GameObject(const char* name)
{
	this->name = name;
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
	Component* component = nullptr;

	switch(type)
	{
		case TRANSFORM:
			component = new ComponentTransform(this);
			myTransform = (ComponentTransform*)component;
			break;
		case MESH:
			component = new ComponentMesh();
			myMesh = (ComponentMesh*)component;
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
	bool objOpen = ImGui::TreeNodeEx(this, flags, name.c_str());

	if(ImGui::IsItemClicked())
	{
		App->scene->SelectObjectInHierarchy(this);
	}
	if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && ImGui::IsItemHovered())
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
			//AABB Global Update
			//Compute globalBoundingBox

			float3 globalPos, globalScale;
			float3x3 globalRot;
			myTransform->globalModelMatrix.Decompose(globalPos, globalRot, globalScale);

			globalBoundingBox->minPoint = (boundingBox->minPoint + globalPos);
			globalBoundingBox->maxPoint = (boundingBox->maxPoint + globalPos);
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

void GameObject::ComputeAABB()
{
	float3 min = float3::zero;
	float3 max = float3::zero;

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
				//Min vertex
				if (child->boundingBox->minPoint.x < min.x)
					min.x = child->boundingBox->minPoint.x;
				if (child->boundingBox->minPoint.y < min.y)
					min.y = child->boundingBox->minPoint.y;
				if (child->boundingBox->minPoint.z < min.z)
					min.z = child->boundingBox->minPoint.z;
				//Max vertex
				if (child->boundingBox->maxPoint.x > max.x)
					max.x = child->boundingBox->maxPoint.x;
				if (child->boundingBox->maxPoint.y > max.y)
					max.y = child->boundingBox->maxPoint.y;
				if (child->boundingBox->maxPoint.z > max.z)
					max.z = child->boundingBox->maxPoint.z;
			}
		}

		boundingBox = new AABB(min, max);
		//Compute globalBoundingBox
		float3 globalPos, globalScale;
		float3x3 globalRot;
		myTransform->globalModelMatrix.Decompose(globalPos, globalRot, globalScale);
		globalBoundingBox = new AABB(min + globalPos, max + globalPos);
	}
		

	for (auto vertex : myMesh->myMesh->vertices)
	{
		//Min vertex
		if (vertex.Position.x < min.x)
			min.x = vertex.Position.x;
		if (vertex.Position.y < min.y)
			min.y = vertex.Position.y;
		if (vertex.Position.z < min.z)
			min.z = vertex.Position.z;
		//Max vertex
		if (vertex.Position.x > max.x)
			max.x = vertex.Position.x;
		if (vertex.Position.y > max.y)
			max.y = vertex.Position.y;
		if (vertex.Position.z > max.z)
			max.z = vertex.Position.z;
	}
	
	boundingBox = new AABB(min, max);

	//Compute globalBoundingBox
	float3 globalPos, globalScale;
	float3x3 globalRot;
	myTransform->globalModelMatrix.Decompose(globalPos, globalRot, globalScale);
	globalBoundingBox = new AABB(min + globalPos, max + globalPos);
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

	if(boundingBox != NULL) dd::aabb(boundingBox->minPoint, boundingBox->maxPoint, float3(0.6f, 0.6f, 0.6f));

	glEnd();
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

	delete[] go_name;

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