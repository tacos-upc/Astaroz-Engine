#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"
#include "ImGUI/ImGuizmo.h"


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
	GameObject* findById(std::string id);
	void selectRoot();
	void LoadModel(const char* path, GameObject* parent);
	void CreateEmpty(GameObject* parent);
	//void CreateGameObjectBakerHouse(GameObject* parent);
	void RemoveSelectedGameObject();
	void DuplicateSelectedGameObject();
	void SelectGameObjectInHierarchy(GameObject* selected);
	void drawHierarchy();
	void drawAllBoundingBoxes();
	void eraseGameObject(GameObject* go);
	void OnSave(Serialization& serial);
	void OnLoad(const Serialization& serial);

	//public variables
	std::vector<GameObject*> gameObjects;
	bool showHierarchy;
	bool showInspector;
	GameObject* mainCamera;
	GameObject* selectedByHierarchy;
	ImGuizmo::OPERATION preferedOperation;

	std::string sceneSerialized;
	std::string savedRootID;

private:
	//private variables
	GameObject* root;
	unsigned int nGameObjects;
};

#endif __ModuleScene_H__