#ifndef __Component_H__
#define __Component_H__

#include "Globals.h"
#include "Serialization.h"

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

	virtual void Enable(){}
	virtual void Disable(){}
	virtual void Update(){}
	virtual void DrawInspector(){}
	static const char* typeToString(int);

	//pure virtual functions to force implementations to those who inherits from Component
	virtual void OnSave(Serialization& serial) = 0;
	virtual void OnLoad(const Serialization& serial) = 0;
	
	GameObject* myGameObject = nullptr;
	ComponentType myType;
	bool allowMany;
};

#endif __Component_H__