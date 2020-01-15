#include "ModuleSpacePartition.h"
#include "Math/MathAll.h"
#include "Geometry/AABB.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
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

void ModuleSpacePartition::drawTree(ImVec4 color)
{
	for (size_t i = 0; i < tree->count(); i++)
	{
		AABBTreeNode* node = tree->getNode(i);
		if (node != nullptr && node->box != nullptr)
		{
			dd::aabb(node->box->minPoint, node->box->maxPoint, float3(color.x, color.y, color.z));
		}
	}
}
