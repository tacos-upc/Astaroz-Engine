#include "ModuleSpacePartition.h"
#include "Math/MathAll.h"
#include "Geometry/AABB.h"
#include "ModuleScene.h"
#include "Globals.h"
#include "Application.h"
#include "AABBTree.h"
#include "AABBTreeNode.h"
#include "debugdraw.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"


ModuleSpacePartition::ModuleSpacePartition()
{
}

ModuleSpacePartition::~ModuleSpacePartition()
{
}

bool ModuleSpacePartition::Init()
{
	tree = new AABBTree();
	return true;
}

bool ModuleSpacePartition::Start()
{
	return true;
}

update_status ModuleSpacePartition::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleSpacePartition::Update()
{
	for (size_t i = 0; i < tree->count(); i++)
	{
		AABBTreeNode* node = tree->getNode(i);
		if (node != nullptr && node->box != nullptr)
		{
			dd::aabb(node->box->minPoint, node->box->maxPoint, float3(0.9f, 0.9f, 0.9f));
		}
	}

	//if(ImGui::Begin("Tree Debug", NULL, ImGuiWindowFlags_NoCollapse))
	//{
	//	drawTree(0);
	//	ImGui::End();
	//}

	return UPDATE_CONTINUE;
}

update_status ModuleSpacePartition::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleSpacePartition::CleanUp()
{
	return true;
}

void ModuleSpacePartition::recalculateTree(GameObject* obj)
{
	tree->removeLeaf(obj->id);
	tree->insertLeaf(obj->id);
}

void ModuleSpacePartition::drawTree(int index)
{
	if (tree->count() <= 0 || tree->getNode(index) == nullptr) return;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	ImGui::PushID(tree->getNode(index));
	if (tree->getNode(index)->rightChild == nullptr && tree->getNode(index)->leftChild == nullptr)
	{
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	bool objOpen = ImGui::TreeNodeEx(tree->getNode(index), flags, tree->getNode(index)->gameObjectID.c_str());

	if (objOpen)
	{
		if (tree->getNode(index)->leftChild != nullptr) drawTree(tree->getNode(index)->leftChild->index);
		if (tree->getNode(index)->rightChild != nullptr) drawTree(tree->getNode(index)->rightChild->index);
		
		if (!(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
		{
			ImGui::TreePop();
		}
	}
	ImGui::PopID();
}
