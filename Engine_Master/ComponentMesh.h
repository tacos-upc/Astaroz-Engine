#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__

#include "Globals.h"
#include "Component.h"
#include "Mesh.h"

#include "Geometry/AABB.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	//public methods
	void LoadMesh(Mesh* mesh);
	void Draw(const unsigned int program) const;

	//public variables
	Mesh* myMesh;
};

#endif __ComponentMesh_H__