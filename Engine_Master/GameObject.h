#ifndef __GameObject_H__
#define __GameObject_H__

#include "Globals.h"
#include "Component.h"
#include "Geometry/AABB.h"
#include "Geometry/OBB.h"
#include "Object.h"
#include <string>
#include <vector>

//Class declaration > include header
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

class GameObject : public Object
{
public:
	GameObject();
	GameObject(const char* name);
	GameObject(const GameObject& go);
	~GameObject();

	//public methods 
	void Update();
	void SetParent(GameObject* newParent);
	void RemoveChildren(GameObject* child);
	void DeleteGameObject();
	void CleanUp();

	Component* CreateComponent(ComponentType type);
	Component* GetComponent(ComponentType type);

	void DrawHierarchy(GameObject* selected);
	void drawGizmo();
	void DrawInspector();
	void createAABBs();
	void findOBBPointsForRender();
	void findOBBPoints();
	void DrawAABB();
	void Draw(GLuint program);

	std::string GetName() const;
	void SetName(const std::string &newName);

	void OnSave(Serialization& serial);
	void OnLoad(const Serialization& serial);

	//public variables
	ComponentTransform* myTransform = nullptr;
	ComponentMesh* myMesh = nullptr;
	GameObject* parent = nullptr;
	std::vector<GameObject*> childrenVector;
	std::vector<Component*> componentVector;
	bool isEnabled = true;
	bool isRoot = false;
	bool isParentOfMeshes = false;
	OBB* obb = nullptr;
	AABB* boundingBox = nullptr;
	AABB* fatBoundingBox = nullptr;					//Used for AABB tree

private:
	//private variables
	std::string myName;
	float3 obbPoints[8];

	//private methods
	void CheckDragAndDrop(GameObject* go);
	bool isfatBoxTooFat();							//Area diff between regular aabb and fat one
	void recursiveEnable(std::vector<GameObject*> childrenVector);
	bool isAncestor(GameObject*);					//Used for DragAndDrop to avoid dragging an ancestor to one of its children
	bool isOBBEnclosing = false;
};
#endif __GameObject_H__