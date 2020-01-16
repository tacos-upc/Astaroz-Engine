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

#include "Config.h"
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

	root = new GameObject("Root");
	root->isRoot = true;

	mainCamera = CreateGameObject("Main Camera", root);
	mainCamera->CreateComponent(CAMERA);

	gameObjects.push_back(mainCamera);

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
		gameObject->UpdateTransform();
		gameObject->Update();
	}
	
	//drawHierarchy(); --> This is being done in ModuleEditor

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
	std::string defaultName = "NewGameObject" + gameObjects.size();
	GameObject* gameObject = new GameObject(defaultName.c_str());
	gameObject->SetParent(root);
	nGameObjects++;

	return gameObject;
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* gameObject = new GameObject(name);
	gameObject->SetParent(parent);
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
	//App->modelLoader->loadModel(path);

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

	gameObjects.push_back(gameObject);
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
			LOG("You are trying to delete ROOT GameObject and it is not allowed! Please select another object in the hierarchy to delete.");
		}
	}
}

void ModuleScene::DuplicateSelectedGameObject()
{
	if (selectedByHierarchy != nullptr && selectedByHierarchy->id != root->id)
	{
		GameObject* duplicate = new GameObject(*selectedByHierarchy);
		selectedByHierarchy->parent->childrenVector.push_back(duplicate);
		gameObjects.push_back(duplicate);
	}
	else
	{
		LOG("You are trying to duplicate ROOT GameObject and it is not allowed! Please select another object in the hierarchy to duplicate.");
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
	selectedByHierarchy = selected;
}

void ModuleScene::OnSave(Serialization& serial)
{

}

void ModuleScene::OnLoad(const Serialization& serial)
{

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