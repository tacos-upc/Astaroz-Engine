#include "ComponentMaterial.h"


ComponentMaterial::ComponentMaterial()
{
	myType = MATERIAL;
}

ComponentMaterial::ComponentMaterial(GameObject* gameObject, ComponentMaterial* componentMaterial)
{
	//TODO: Material data to be copied
	myType = MATERIAL;
	myGameObject = gameObject;
}

ComponentMaterial::~ComponentMaterial()
{}