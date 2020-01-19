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
#include "ModuleFileSystem.h"
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
	root = nullptr;
	nGameObjects = 0;
	sceneSerialized = "";

	//Begin generating a basic scene
	GenerateRoot();

	mainCamera = CreateGameObject("Main Camera", root);
	mainCamera->CreateComponent(CAMERA);

	preferedOperation = ImGuizmo::TRANSLATE;

	return true;
}

bool ModuleScene::PostStart()
{
	//Load the scene pre-saved in 'save.sav' file if it exists
	sceneSerialized = App->fileSystem->load("save.sav");
	if (sceneSerialized != "")
	{
		Serialization sceneSerial(sceneSerialized);
		App->scene->OnLoad(sceneSerial);
	}
	else
	{
		LOG("Could not load the scene. File missing or file path wrong.")
	}
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

void ModuleScene::GenerateRoot()
{
	if (root != nullptr)
	{
		root->DeleteGameObject();
		root = nullptr;
	}
	root = new GameObject("Root");
	root->id = ROOTID;
	root->isRoot = true;
	nGameObjects = 0;
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
	std::vector<Serialization> sceneSerial(gameObjects.size());
	std::vector<GameObject*> gameObjectsTree;					//we need another list for all GameObjects in scene because *order* is very important to save all of them correctly
	for (auto& childInRoot : root->childrenVector)
	{
		gameObjectsTree.push_back(childInRoot);					//first fill the list with all game objects which direct parent is ROOT (posible parents of more GameObjects)
	}

	for (unsigned int i = 0; i < gameObjectsTree.size(); ++i)
	{
		gameObjectsTree[i]->OnSave(sceneSerial[i]);				//start saving them
		for (auto& child : gameObjectsTree[i]->childrenVector)	//check if the GameObject has children in a loop
		{
			gameObjectsTree.push_back(child);					//add to the list all its children by iterating this loop
		}
	}

	serial.SaveChildrenSerial("GameObjects", sceneSerial);
	LOG("Scene saved correctly");
}

void ModuleScene::OnLoad(const Serialization& serial)
{
	selectedByHierarchy = nullptr;
	GenerateRoot();

	std::vector<Serialization> gameObjectsSerial = serial.LoadChildrenSerial("GameObjects");
	for (unsigned int i = 0; i < gameObjectsSerial.size(); ++i)
	{
		GameObject* created_game_object = CreateGameObject();
		created_game_object->OnLoad(gameObjectsSerial[i]);
	}
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