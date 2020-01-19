#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__

#include "Globals.h"
#include "Component.h"
#include "Mesh.h"
#include "Geometry/AABB.h"
#include "Geometry/Triangle.h"
#include <vector>

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	ComponentMesh(GameObject* gameObject, ComponentMesh* componentMesh);
	~ComponentMesh();

	//public methods
	void LoadMesh(Mesh* mesh);
	void Draw(const unsigned int program) const;
	void DrawInspector() override;

	//serialization
	void OnSave(Serialization& serial);
	void OnLoad(const Serialization& serial);

	//public variables
	Mesh* myMesh;

	//material editor method
	void editMat();

	int selectedMesh;
	int selection;
	

};

#endif __ComponentMesh_H__