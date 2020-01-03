#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include <string>

enum ComponentType
{
	TRANSFORM = 0,
	MESH,
	MATERIAL,
	CAMERA
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
	virtual void drawInspector(){}

	//GameObject myParent;
	GameObject* myGameObject = nullptr;
	ComponentType myType = TRANSFORM;
	bool isActive = true;
	std::string myName = "NewComponent";
};

#endif __Component_H__