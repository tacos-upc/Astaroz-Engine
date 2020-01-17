#include "ComponentMesh.h"
#include "ModuleModelLoader.h"
#include "Application.h"
#include "IconsFontAwesome5.h"
#include "GameObject.h"
#include <list>

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
	if (ImGui::CollapsingHeader(ICON_FA_HOME "Mesh"))
	{
		
		const char* listbox_meshNames[17] = {};
		static int selection;

		for (size_t i = 0; i < App->modelLoader->meshes.size(); i++)
		{
			Mesh* mesh = App->modelLoader->meshes.at(i);
			listbox_meshNames[i] = mesh->name;
		}

		if (myMesh != nullptr) {
			ImGui::ListBox("listbox\n(single select)", &selection, listbox_meshNames, IM_ARRAYSIZE(listbox_meshNames), 4);
		}
		selectedMesh = selection;
		if (selectedMesh >= 0 && myMesh != App->modelLoader->meshes.at(selectedMesh))
		{
			myMesh = App->modelLoader->meshes.at(selectedMesh);
			myGameObject->createAABBs();
		}

		editMat();
		ImGui::Separator();
		
	}
}

void ComponentMesh::editMat()
{
	
	ImGui::SliderFloat3("light position", (float*)&App->modelLoader->light_pos, -15.0f, 15.0f);
	ImGui::SliderFloat("ambient", (float*)&App->modelLoader->materials[myMesh->material]->k_ambient, 0.0f, 1.0f);
	ImGui::SliderFloat("diffuse", (float*)&App->modelLoader->materials[myMesh->material]->k_diffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("specular", (float*)&App->modelLoader->materials[myMesh->material]->k_specular, 0.0f, 1.0f);
	ImGui::SliderFloat3("diffuse_color", (float*)&App->modelLoader->materials[myMesh->material]->diffuse_color, -1.0f, 1.0f);
	ImGui::SliderFloat3("specular_color", (float*)&App->modelLoader->materials[myMesh->material]->specular_color, -1.0f, 1.0f);
	ImGui::SliderFloat3("emissive_color", (float*)&App->modelLoader->materials[myMesh->material]->emissive_color, -1.0f, 1.0f);
		
}
