#include "ModuleScene.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Math/float4.h"
#include "IconsFontAwesome5.h"

#include <stack>


ModuleScene::ModuleScene()
{}


ModuleScene::~ModuleScene()
{}

bool ModuleScene::Init()
{
	//Init variables
	showHierarchy = true;
	showInspector = true;
	selectedByHierarchy = nullptr;
	nGameObjects = 0;
	sceneSerialized = "";
	savedRootID = "";

	root = new GameObject("Root");
	root->isRoot = true;

	mainCamera = CreateGameObject("Main Camera", root);
	mainCamera->CreateComponent(CAMERA);

	preferedOperation = ImGuizmo::TRANSLATE;

	return true;
}

update_status ModuleScene::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleScene::Update()
{
	for(auto gameObject : gameObjects)
	{
		gameObject->Update();
	}
	
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp()
{
	for(auto GO : gameObjects)
	{
		delete GO;
	}
	delete root;

	return true;
}

GameObject* ModuleScene::CreateGameObject()
{
	std::string defaultName = "NewGameObject" + std::to_string(nGameObjects + 1);
	GameObject* gameObject = new GameObject(defaultName.c_str());
	gameObject->SetParent(root);
	gameObjects.push_back(gameObject);
	nGameObjects++;

	return gameObject;
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* gameObject = new GameObject(name);
	gameObject->SetParent(parent);
	gameObjects.push_back(gameObject);
	nGameObjects++;

	return gameObject;
}

GameObject* ModuleScene::getRoot()
{
	return root;
}

GameObject* ModuleScene::findById(std::string id)
{
	GameObject* found = nullptr;
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects.at(i)->id == id) found = gameObjects.at(i);
	}

	return found;
}

void ModuleScene::selectRoot()
{
	selectedByHierarchy = root;
}

void ModuleScene::LoadModel(const char* path, GameObject* parent)
{
	LOG("Trying to load model in path : %s", path);
	//App->modelLoader->LoadModel(path);

	int numObject = 0;
	std::string name = App->modelLoader->modelName;
	LOG("Creating parent gameObject %s", name.c_str());
	parent->SetName(name);

	LOG("For each mesh of the model we create a gameObject.");
	for(auto mesh : App->modelLoader->meshes)
	{
		std::string newName = name + std::to_string(numObject);
		GameObject* newMeshObject = CreateGameObject(newName.c_str(), parent);
		ComponentMesh* myMeshCreated = (ComponentMesh*)newMeshObject->CreateComponent(MESH);
		
		myMeshCreated->LoadMesh(mesh);
		newMeshObject->createAABBs();
		gameObjects.push_back(newMeshObject);

		numObject++;
	}

	LOG("Deleting info from ModelLoader");
	App->modelLoader->emptyScene();
	parent->createAABBs();

	//Setting parent as a meshParent
	parent->isParentOfMeshes = true;
}

void ModuleScene::CreateEmpty(GameObject* parent)
{
	std::string tempName = "NewGameObject" + std::to_string(nGameObjects + 1);
	GameObject* gameObject = nullptr;
	if (selectedByHierarchy != nullptr)
	{
		gameObject = CreateGameObject(tempName.c_str(), selectedByHierarchy);
	}
	else
	{
		gameObject = CreateGameObject(tempName.c_str(), parent);	//only when 'selected' is nullptr, we use the parameter as parent
	}
}

//void ModuleScene::CreateGameObjectBakerHouse(GameObject* parent)
//{
//	if(parent == nullptr)
//	{
//		LOG("ERROR: Parent is nullptr, cannot create gameObject.");
//		return; //leave
//	}
//
//	LOG("Creating a GameObject with Baker House Mesh.");
//	std::string defaultName = "BakerHouse" + std::to_string(numberOfBakerHouse + 1);
//	GameObject* newGameObject = CreateGameObject(defaultName.c_str(), parent);
//	LoadModel("../Models/baker_house/BakerHouse.fbx", newGameObject);
//	++numberOfBakerHouse;
//
//	gameObjects.push_back(newGameObject);
//	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());
//}

void ModuleScene::RemoveSelectedGameObject()
{
	if (!gameObjects.empty())
	{
		if (selectedByHierarchy != nullptr && selectedByHierarchy->id != root->id)
		{
			selectedByHierarchy->DeleteGameObject();
			selectedByHierarchy = nullptr;		//selected GO will be deleted so it must be unasigned (will be pointing to ROOT next frame)
		}
		else
		{
			LOG("You are trying to delete ROOT GameObject and it is not allowed! Please select an object in the hierarchy to delete.");
		}
	}
}

void ModuleScene::DuplicateSelectedGameObject()
{
	if (selectedByHierarchy != nullptr && selectedByHierarchy->id != root->id && selectedByHierarchy->id != mainCamera->id)
	{
		GameObject* duplicate = new GameObject(*selectedByHierarchy);
		selectedByHierarchy->parent->childrenVector.push_back(duplicate);
		gameObjects.push_back(duplicate);
	}
	else
	{
		LOG("You are trying to duplicate ROOT GameObject or MainCamera and it is not allowed! Please select an object in the hierarchy to duplicate.");
	}
}

void ModuleScene::eraseGameObject(GameObject* go)
{
	if (!gameObjects.empty())
	{
		gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), go), gameObjects.end());
	}
}

void ModuleScene::SelectGameObjectInHierarchy(GameObject* selected)
{
	if (selected != nullptr)
	{
		selectedByHierarchy = selected;
	}
	else
	{
		selectedByHierarchy = root;
	}
}

void ModuleScene::OnSave(Serialization& serial)
{
	std::vector<Serialization> game_objects_config(gameObjects.size());
	std::stack<GameObject*> pending_objects;
	unsigned int current_index = 0;

	for (auto& child_game_object : root->childrenVector)
	{
		pending_objects.push(child_game_object);
	}

	while (!pending_objects.empty())
	{
		GameObject* current_game_object = pending_objects.top();
		pending_objects.pop();

		current_game_object->OnSave(game_objects_config[current_index]);
		current_index++;

		for (auto& child_game_object : current_game_object->childrenVector)
		{
			pending_objects.push(child_game_object);
		}
	}
	//assert(current_index == game_objects_ownership.size());

	serial.AddChildrenSerial("GameObjects", game_objects_config);
	LOG("Scene saved correctly");
}

void ModuleScene::OnLoad(const Serialization& serial)
{
	selectedByHierarchy = nullptr;
	savedRootID = root->id;
	root->DeleteGameObject();
	root = new GameObject("Root");
	root->isRoot = true;

	std::vector<Serialization> game_objects_config;
	serial.GetChildrenSerial("GameObjects", game_objects_config);
	for (unsigned int i = 0; i < game_objects_config.size(); ++i)
	{
		GameObject* created_game_object = CreateGameObject();
		created_game_object->OnLoad(game_objects_config[i]);
	}
	//App->renderer->GenerateQuadTree();
	LOG("Scene loaded correctly");
}

void ModuleScene::drawHierarchy()
{
	if (selectedByHierarchy == nullptr)
	{
		selectedByHierarchy = root;
	}

	for (unsigned int i = 0; i < root->childrenVector.size(); i++)
	{
		root->childrenVector[i]->DrawHierarchy(root->childrenVector[i]);
	}
}

void ModuleScene::drawAllBoundingBoxes()
{
	for (unsigned int i = 0; i < root->childrenVector.size(); i++)
	{
		root->childrenVector[i]->DrawAABB();
	}
}