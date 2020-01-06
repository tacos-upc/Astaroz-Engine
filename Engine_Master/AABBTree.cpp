#include "AABBTree.h"
#include "Math/MathAll.h"
#include "Geometry/AABB.h"
#include "ModuleScene.h"
#include "Globals.h"
#include "Application.h"

AABBTree::AABBTree()
{
	count = 0;
}

AABBTree::~AABBTree()
{
}

AABB * AABBTree::Union(AABB * A, AABB * B)
{
	AABB* C = new AABB();
	C->minPoint = math::Min(A->minPoint, B->minPoint);
	C->maxPoint = math::Max(A->maxPoint, B->maxPoint);

	return C;
}

void AABBTree::insertLeaf(int objectIndex)
{
	count += 1;

	AABBTreeNode* leaf = createLeaf(objectIndex);
	if (count == 0)
	{
		root = leaf;
		leaf->isRoot = true;
		return;
	}
	// Stage 1: find the best sibling for the new leaf
	AABBTreeNode* bestSibling = root;
	for (int i = 0; i < count; ++i)
	{
		bestSibling = pickBestSibling(bestSibling, i);
	}

	//// Stage 2: create a new parent
	//int oldParent = tree.nodes[sibling].parentIndex;
	//int newParent = AllocateInternalNode(tree);
	//tree.nodes[newParent].parentIndex = oldParent;
	//tree.nodes[newParent].box = Union(box, tree.nodes[sibling].box);
	//if (oldParent != nullIndex)
	//{
	//	// The sibling was not the root
	//	if (tree.nodes[oldParent].child1 == sibling)
	//	{
	//		tree.nodes[oldParent].child1 = newParent;
	//	}
	//	else
	//	{
	//		tree.nodes[oldParent].child2 = newParent;
	//	}
	//	tree.nodes[newParent].child1 = sibling;
	//	tree.nodes[newParent].child2 = leafIndex;
	//	tree.nodes[sibling].parentIndex = newParent;
	//	tree.nodes[leafIndex].parentIndex = newParent;
	//}
	//else
	//{
	//	// The sibling was the root
	//	tree.nodes[newParent].child1 = sibling;
	//	tree.nodes[newParent].child2 = leafIndex;
	//	tree.nodes[sibling].parentIndex = newParent;
	//	tree.nodes[leafIndex].parentIndex = newParent;
	//	tree.rootIndex = newParent;
	//}
	// Stage 3: walk back up the tree refitting AABBs
}

AABBTreeNode* AABBTree::createLeaf(int index)
{
	AABBTreeNode* node = new AABBTreeNode();
	GameObject* obj = App->scene->gameObjects.at(index);

	node->objectIndex = index;
	node->box = obj->boundingBox;

	return node;
}

AABBTreeNode * AABBTree::pickBestSibling(AABBTreeNode *, int)
{
	return nullptr;
}
