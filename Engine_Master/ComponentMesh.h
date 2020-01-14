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
	~ComponentMesh();

	//public methods
	void LoadMesh(Mesh* mesh);
	void Draw(const unsigned int program) const;
	void DrawInspector() override;

	//public variables
	Mesh* myMesh = nullptr;

private:
	int selectedMesh;
	bool isLoaded;
};

#endif __ComponentMesh_H__