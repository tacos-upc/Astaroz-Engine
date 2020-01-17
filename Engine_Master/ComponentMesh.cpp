#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ModuleModelLoader.h"
#include "Application.h"
#include "IconsFontAwesome5.h"
#include "GameObject.h"
#include <list>
#include "Geometry/Triangle.h"

ComponentMesh::ComponentMesh()
{
	myType = MESH;
	allowMany = false;
	isLoaded = false;
}

ComponentMesh::ComponentMesh(GameObject* gameObject, ComponentMesh* componentMesh)
{
	myType = MESH;
	myGameObject = gameObject;
	myMesh = componentMesh->myMesh;
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
	if(selectedMesh >= 0) myMesh->Draw(program);
}

void ComponentMesh::DrawInspector()
{
	if (ImGui::CollapsingHeader(ICON_FA_HOME " Mesh"))
	{
		std::string meshNames = "";
		for (size_t i = 0; i < App->modelLoader->meshes.size(); i++)
		{
			Mesh* mesh = App->modelLoader->meshes.at(i);
			meshNames.append(mesh->name);
		}

		ImGui::Combo("Target Mesh", &selectedMesh, meshNames.c_str(), 3);

		if (selectedMesh >= 0 && myMesh != App->modelLoader->meshes.at(selectedMesh))
		{
			myMesh = App->modelLoader->meshes.at(selectedMesh);
			myGameObject->createAABBs();
		}

		ImGui::Separator();
	}
}

void ComponentMesh::OnSave(Serialization& serial)
{

}

void ComponentMesh::OnLoad(const Serialization& serial)
{

}