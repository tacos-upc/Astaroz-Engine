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
	GameObject* getRoot();
	void selectRoot();
	void LoadModel(const char* path, GameObject* parent);
	void CreateEmpty(GameObject* parent);
	void CreateGameObjectBakerHouse(GameObject* parent);
	void CreateGameObjectShape(GameObject* parent, ShapeType shape);
	void RemoveGameObject(GameObject* go);
	void DuplicateGameObject(GameObject* go);
	void SelectObjectInHierarchy(GameObject* selected);
	void drawHierarchy();
	void drawAllBoundingBoxes();

	//public variables
	std::vector<GameObject*> gameObjects;
	bool showHierarchy;
	bool showInspector;
	GameObject* mainCamera;
	GameObject* selectedByHierarchy = nullptr;

private:
	//private variables
	GameObject* root;
	unsigned int nGameObjects;
	unsigned int numberOfBakerHouse;
	unsigned int numberOfSphere;
	unsigned int numberOfCube;
	unsigned int numberOfTorus;
	unsigned int numberOfCylinder;
};

#endif __ModuleScene_H__