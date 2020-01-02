#include "ComponentMesh.h"
#include "ModuleModelLoader.h"
#include "Application.h"

ComponentMesh::ComponentMesh()
{
	myType = MESH;
}

ComponentMesh::~ComponentMesh()
{
	delete myMesh;
}

void ComponentMesh::LoadMesh(Mesh* loadedMesh)
{
	myMesh = loadedMesh;
}

void ComponentMesh::Draw(const unsigned int program) const
{
	myMesh->Draw(program);
}