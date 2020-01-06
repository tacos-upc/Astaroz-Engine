#include "AABBTree.h"
#include "Math/MathAll.h"
#include "Geometry/AABB.h"
#include "ModuleScene.h"
#include "Globals.h"
#include "Application.h"

AABBTree::AABBTree()
{
}

AABBTree::~AABBTree()
{
}

AABB * AABBTree::Union(AABBTreeNode* A, AABBTreeNode* B)
{
	AABB* C = nullptr;

	if (A == nullptr) C = B->box;
	else if (B == nullptr) C = A->box;
	else C = A->Union(B);
	return C;
}

void AABBTree::insertLeaf(GameObject* objectIndex)
{
	siblingsPriorityQueue.clear();

	AABBTreeNode* leaf = createLeaf(objectIndex);
	if (count() <= 1)
	{
		root = leaf;
		return;
	}

	// Stage 1: find the best sibling for the new leaf
	AABBTreeNode* sibling = nullptr;
	if (count() <= 2) sibling = root;
	else sibling = pickBestSibling(leaf, root, math::floatMax);

	// Stage 2: create a new parent
	AABBTreeNode* newParent = createEmptyNode();
	
	newParent->parent = sibling->parent;
	
	newParent->leftChild = leaf;
	leaf->parent = newParent;

	newParent->rightChild = sibling;
	sibling->parent = newParent;

	if (sibling == root) root = newParent;

	//Stage 3: Rebalance the tree if required
	leaf->receiveDepthData(0, false, false);
	if (math::Abs(newParent->rightChild->depth - newParent->leftChild->depth) >= 2)
	{
		//The tree is out of balance
		AABBTreeNode* deepNode = newParent->getDeepestChild();
		AABBTreeNode* shallowNode = newParent->getShallowestChild();

		AABBTreeNode* deepestGrandChildren = deepNode->getDeepestChild();
		AABBTreeNode* shallowestGrandChildren = deepNode->getShallowestChild();

		//Is rotation necessary?
		if (shallowestGrandChildren->costWith(shallowNode) < newParent->cost())
		{
			deepestGrandChildren->parent = shallowNode->parent;
			if (shallowNode->isLeft()) deepestGrandChildren->parent->leftChild = deepestGrandChildren;
			else deepestGrandChildren->parent->rightChild = deepestGrandChildren;

			shallowNode->parent = deepNode;
			if (deepestGrandChildren->isLeft()) shallowNode->parent->leftChild = shallowNode;
			else shallowNode->parent->rightChild = shallowNode;
		}
	}


	// Stage 4: walk back up the tree refitting AABBs
	refit(leaf->parent);
}

void AABBTree::removeLeaf(GameObject* obj)
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		AABBTreeNode* node = nodes.at(i);
		if (node->gameObject == obj)
		{
			if (node->isLeft() && !node->isRoot()) node->parent->leftChild = nullptr;
			else if(node->isLeft() && !node->isRoot()) node->parent->rightChild = nullptr;

			refit(node->parent);
			node->parent = nullptr;
			nodes.erase(nodes.begin() + i);
			delete node;
			return;
		}
	}
}

AABBTreeNode * AABBTree::getNode(int index)
{
	AABBTreeNode* node = nullptr;
	if (index > 0 && index < nodes.size()) node = nodes.at(index);
	return node;
}

int AABBTree::count()
{
	return nodes.size();
}

AABBTreeNode* AABBTree::createLeaf(GameObject* go)
{
	AABBTreeNode* node = createEmptyNode();
	node->gameObject = go;
	node->box = new AABB(*go->fatBoundingBox);

	return node;
}

AABBTreeNode * AABBTree::createEmptyNode()
{
	AABBTreeNode* node = new AABBTreeNode();
	node->box = new AABB(float3(0,0,0), float3(0,0,0));
	nodes.push_back(node);
	return node;
}

AABBTreeNode * AABBTree::pickBestSibling(AABBTreeNode * newLeaf, AABBTreeNode* candidate, float currentBestCost)
{
	if (count() == 1) return root;

	float cost = newLeaf->costWith(candidate) + candidate->inheritedCost();
	if (cost < currentBestCost)
	{
		currentBestCost = cost;
		siblingsPriorityQueue.push_back(candidate);
	}

	float lowerBoundCost = newLeaf->cost() + candidate->cost() + candidate->inheritedCost();
	if (lowerBoundCost < currentBestCost) //It's worth the shot to look at the children
	{
		if (candidate->leftChild != nullptr) pickBestSibling(newLeaf, candidate->leftChild, currentBestCost);
		if (candidate->rightChild != nullptr) pickBestSibling(newLeaf, candidate->rightChild, currentBestCost);
	}

	return siblingsPriorityQueue.at(siblingsPriorityQueue.size() - 1);
}

void AABBTree::refit(AABBTreeNode * index)
{
	while (index != nullptr)
	{
		AABBTreeNode* child1 = index->leftChild;
		AABBTreeNode* child2 = index->rightChild;
		index->box = Union(child1, child2);
		index = index->parent;
	}
}

