#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Globals.h"
#include "Component.h"
#include "ModuleTexture.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	ComponentMaterial(GameObject* gameObject, ComponentMaterial* componentMaterial);
	~ComponentMaterial();

	//public variables
	std::vector<Texture*> myTextures;
};

#endif __ComponentMaterial_H__