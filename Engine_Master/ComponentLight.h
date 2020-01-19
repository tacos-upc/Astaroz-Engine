#ifndef __ComponentLight_H__
#define __ComponentLight_H__

#include "Globals.h"
#include "Component.h"
#include "Mesh.h"
#include "ModuleModelLoader.h"
#include "Geometry/AABB.h"

class ComponentLight : public Component
{
public:
	ComponentLight();
	~ComponentLight();

	//public methods
	void DrawInspector() override;
	void OnSave(Serialization& serial);
	void OnLoad(const Serialization& serial);

	//public variables
	struct Light light;
};

#endif __ComponentLight_H__