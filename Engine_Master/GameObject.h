#ifndef __GameObject_H__
#define __GameObject_H__

#include "Globals.h"
#include "Component.h"
#include "Geometry/AABB.h"
#include "Geometry/OBB.h"
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
	Component* GetComponent(ComponentType type);

	//Hierarchy
	void DrawHierarchy(GameObject* selected);

	//Update
	void UpdateTransform();
	void DrawInspector();
	void createAABBs();
	void findOBBPointsForRender();
	void findOBBPoints();

	void DrawAABB();
	void Draw(GLuint program);
	void SetName(const std::string &newName);
	std::string GetName() const;

	//public variables
	ComponentTransform* myTransform = nullptr;
	ComponentMesh* myMesh = nullptr;
	ComponentMaterial* myMaterial = nullptr;
	GameObject* parent = nullptr;
	std::vector<GameObject*> childrenVector;
	std::vector<Component*> components;
	bool isEnabled = true;
	bool isRoot = false;
	bool isParentOfMeshes = false;
	bool isStatic = false;
	OBB* obb = nullptr;
	AABB* boundingBox = nullptr;

private:
	//private variables
	std::string name;
	bool isSelectedInHierarchy;
	float3 obbPoints[8];

	//private methods
	void CheckDragAndDrop(GameObject* go);
};
#endif __GameObject_H__