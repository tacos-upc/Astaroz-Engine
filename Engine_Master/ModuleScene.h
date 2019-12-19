#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"


enum ShapeType
{
	SPHERE = 0,
	CUBE,
	CYLINDER,
	TORUS
};

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	//public methods
	GameObject* CreateGameObject();
	GameObject* CreateGameObject(const char* name, GameObject* parent);
	void LoadModel(const char* path, GameObject* parent);
	void CreateEmpy(GameObject* parent);
	void CreateGameObjectBakerHouse(GameObject* parent);
	void CreateGameObjectShape(GameObject* parent, ShapeType shape);
	void RemoveGameObject(GameObject* go);
	void SelectObjectInHierarchy(GameObject* selected);

	//public variables
	std::vector<GameObject*> gameObjects;
	bool showHierarchy = true;
	bool showInspector = true;
	GameObject* mainCamera = nullptr;

private:
	//private variables
	GameObject* root = nullptr;
	GameObject* selectedByHierarchy = nullptr;
	unsigned int nGameObjects = 0;
	unsigned int numberOfBakerHouse = 0;
	unsigned int numberOfSphere = 0;
	unsigned int numberOfCube = 0;
	unsigned int numberOfTorus = 0;
	unsigned int numberOfCylinder = 0;
};

#endif __ModuleScene_H__