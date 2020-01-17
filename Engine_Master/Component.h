#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include <string>

enum ComponentType
{
	COMPONENT_TYPE_NONE = -1,
	//Don't add anything above here
	TRANSFORM = 0,
	MESH,
	CAMERA,
	LIGHT,

	//Don't add anything below here
	COMPONENT_TYPE_COUNT
};

class GameObject;

class Component
{
public:
	Component(){};

	Component(const char* componentName)
	{
		myName = componentName;
	}

	virtual void Enable(){}
	virtual void Disable(){}
	virtual void Update(){}
	virtual void DrawInspector(){}
	static const char* typeToString(int);
	
	//GameObject myParent;
	GameObject* myGameObject = nullptr;
	ComponentType myType = TRANSFORM;
	bool isActive = true;
	bool allowMany;
	std::string myName = "NewComponent";
};

#endif __Component_H__