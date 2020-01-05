#include "ComponentMesh.h"
#include "ModuleModelLoader.h"
#include "Application.h"
#include "IconsFontAwesome5.h"

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
	myMesh->Draw(program);
}

void ComponentMesh::DrawInspector()
{
	if (ImGui::CollapsingHeader(ICON_FA_HOME " Mesh"))
	{
		
		ImGui::InputText("Mesh Path", meshPath, 1000);
		
		ImGui::SameLine();
		if (ImGui::Button("Load") && !isLoaded)
		{
			App->modelLoader->LoadModel(meshPath);
			isLoaded = true;
		}

		ImGui::Separator();
	}
}
