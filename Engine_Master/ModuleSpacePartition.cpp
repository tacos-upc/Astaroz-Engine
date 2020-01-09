#include "ModuleSpacePartition.h"
#include "Math/MathAll.h"
#include "Geometry/AABB.h"
#include "ModuleScene.h"
#include "Globals.h"
#include "Application.h"
#include "AABBTree.h"
#include "debugdraw.h"

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
