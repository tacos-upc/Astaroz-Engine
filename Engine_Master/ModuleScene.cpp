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
	numberOfBakerHouse = 0;
	numberOfSphere = 0;
	numberOfCube = 0;
	numberOfTorus = 0;
	numberOfCylinder = 0;

	root = new GameObject("World");
	root->isRoot = true;

	mainCamera = CreateGameObject("Main Camera (root)", root);
	mainCamera->CreateComponent(CAMERA);

	gameObjects.push_back(mainCamera);

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
	std::string defaultName = "NewGameObject" + gameObjects.size(); //toString() ??
	GameObject* gameObject = new GameObject(defaultName.c_str());
	gameObject->SetParent(root);

	LOG("Creating new GameObject with name: %s", defaultName);

	nGameObjects++;

	return gameObject;
}

GameObject* ModuleScene::CreateGameObject(const char* name, GameObject* parent)
{
	GameObject* gameObject = new GameObject(name);
	gameObject->SetParent(parent);

	LOG("Creating new GameObject with name: %s", name);
	nGameObjects++;

	return gameObject;
}

GameObject* ModuleScene::getRoot()
{
	return root;
}

GameObject * ModuleScene::findById(std::string id)
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
	if (selectedByHierarchy != nullptr)	//TODO: It's never null - every frame points to ROOT if ever gets nullptr
	{
		gameObject = CreateGameObject(tempName.c_str(), selectedByHierarchy);
	}
	else
	{
		gameObject = CreateGameObject(tempName.c_str(), parent);	//we use the parameter as parent only when 'selected' is nullptr
	}
	
	gameObjects.push_back(gameObject);
}

void ModuleScene::CreateGameObjectBakerHouse(GameObject* parent)
{
	if(parent == nullptr)
	{
		LOG("ERROR: Parent is nullptr, cannot create gameObject.");
		return; //leave
	}

	LOG("Creating a GameObject with Baker House Mesh.");
	std::string defaultName = "BakerHouse" + std::to_string(numberOfBakerHouse + 1);
	GameObject* newGameObject = CreateGameObject(defaultName.c_str(), parent);
	LoadModel("../Models/baker_house/BakerHouse.fbx", newGameObject);
	++numberOfBakerHouse;

	gameObjects.push_back(newGameObject);
	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());
}

void ModuleScene::CreateGameObjectShape(GameObject* parent, ShapeType shape)
{
	/*
	if (parent == nullptr)
	{
		LOG("ERROR: Parent is nullptr, cannot create gameObject.");
		return;
	}

	std::string defaultName;
	bool correct;
	switch (shape)
	{
	case SPHERE:
		LOG("Creating a GameObject with Sphere Mesh.");
		defaultName = "Sphere" + std::to_string(numberOfSphere + 1);
		correct = App->modelLoader->LoadSphere(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(0.4f, 0.4f, 0.4f, 0.4f));
		if (!correct)
		{
			LOG("ERROR: Cannot load the sphere mesh correctly.");
			return;
		}
		++numberOfSphere;
		break;
	case CUBE:
		LOG("Creating a GameObject with cube Mesh.");
		defaultName = "Cube" + std::to_string(numberOfCube + 1);
		correct = App->modelLoader->LoadCube("cube0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, float4(0.4f, 0.4f, 0.4f, 0.4f));
		if (!correct)
		{
			LOG("ERROR: Cannot load the cube mesh correctly.");
			return;
		}
		++numberOfCube;
		break;
	case CYLINDER:
		LOG("Creating a GameObject with Cylinder Mesh.");
		defaultName = "Cylinder" + std::to_string(numberOfCylinder + 1);
		correct = App->modelLoader->LoadCylinder(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 2.0f, 1.0f, 30, 30, float4(0.4f, 0.4f, 0.4f, 0.4f));
		if (!correct)
		{
			LOG("ERROR: Cannot load the cylinder mesh correctly.");
			return;
		}
		++numberOfCylinder;
		break;
	case TORUS:
		LOG("Creating a GameObject with torus Mesh.");
		defaultName = "Torus" + std::to_string(numberOfTorus + 1);
		correct = App->modelLoader->LoadTorus(defaultName.c_str(), math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 0.5f, 0.67f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
		if (!correct)
		{
			LOG("ERROR: Cannot load the torus mesh correctly.");
			return;
		}
		++numberOfTorus;
		break;
	default:
		break;
	}


	GameObject* newGameObject = CreateGameObject(defaultName.c_str(), parent);
	

	if(!App->modelLoader->meshes.size() == 1)
	{
		LOG("ERROR: Sphere model cannot have more than one mesh. ");
		delete newGameObject;
		return;
	}



	ComponentMesh* myMeshCreated = (ComponentMesh*)newGameObject->CreateComponent(MESH);
	myMeshCreated->LoadMesh(App->modelLoader->meshes[0]);
	newGameObject->ComputeAABB();
	gameObjects.push_back(newGameObject);

	LOG("%s created with %s as parent.", defaultName.c_str(), parent->GetName());
	//Deleting model loader information
	App->modelLoader->emptyScene();
	*/
}

void ModuleScene::RemoveSelectedGameObject()
{
	if (!gameObjects.empty())
	{
		//TODO: Don't allow to delete ROOT (World) through its UID
		GameObject* toBeDeleted = nullptr;

		if (selectedByHierarchy != nullptr)
		{
			toBeDeleted = selectedByHierarchy;	//prioritize selected over param if not nullptr
			selectedByHierarchy = nullptr;		//selected GO will be deleted so it must be unasigned (will be pointing to ROOT next frame)
		}
		else
		{
			toBeDeleted = root;
		}

		toBeDeleted->DeleteGameObject();
	}
}

void ModuleScene::DuplicateGameObject(GameObject* go)
{
	//TODO: Don't allow to duplicate ROOT (World) through its UID
	GameObject* duplicate = nullptr;

	if (selectedByHierarchy != nullptr && selectedByHierarchy->GetName() != "World")
	{
		duplicate = new GameObject(*selectedByHierarchy);
		selectedByHierarchy->parent->childrenVector.push_back(duplicate);
	}
	else
	{
		duplicate = new GameObject(*go);
		go->parent->childrenVector.push_back(duplicate);
	}

	gameObjects.push_back(duplicate);
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