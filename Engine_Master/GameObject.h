#ifndef __GameObject_H__
#define __GameObject_H__

#include "Globals.h"
#include "Component.h"
#include "Geometry/AABB.h"
#include <string>
#include <vector>

//Class declaration > include header
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

class GameObject
{
public:
	GameObject();
	GameObject(const char* name);
	~GameObject();

	void Update();
	void SetParent(GameObject* newParent);
	void RemoveChildren(GameObject* child);
	void DeleteGameObject();
	void CleanUp();

	//public methods 
	Component* CreateComponent(ComponentType type);
	void DrawHierarchy(GameObject* selected);
	void DrawCamera();
	void UpdateTransform();
	void DrawInspector(bool &showInspector);
	void ComputeAABB();
	void DrawAABB() const;
	void SetName(const std::string &newName);
	std::string GetName() const;

	//public variables
	ComponentTransform* myTransform = nullptr;
	ComponentMesh* myMesh = nullptr;
	ComponentMaterial* myMaterial = nullptr;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
	bool isEnabled = true;
	bool isRoot = false;
	bool isParentOfMeshes = false;
	bool isStatic = false;
	AABB* boundingBox = nullptr;
	AABB* globalBoundingBox = nullptr;

private:
	//private variables
	std::string name;

	//private methods
	void CheckDragAndDrop(GameObject* go);
};

#endif __GameObject_H__