#include "AABBTree.h"
#include "Math/MathAll.h"
#include "Geometry/AABB.h"
#include "ModuleScene.h"
#include "Globals.h"
#include "Application.h"

AABBTree::AABBTree()
{
	root = new AABBTreeNode();
	root->box = new AABB(float3::zero, float3::zero);
	root->isRoot = true;
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

void AABBTree::insertLeaf(std::string id)
{
	if (hasLeaf(id)) return;
	LOG("Entro a insertar un nodo: %d", count());

	siblingsPriorityQueue.clear();

	AABBTreeNode* leaf = createLeaf(id);
	if (count() <= 1)
	{
		leaf->parent = root;
		root->leftChild = leaf;
		return;
	}

	// Stage 1: find the best sibling for the new leaf
	AABBTreeNode* sibling = nullptr;
	if (count() <= 2)
	{
		sibling = sibling = root->leftChild;
	}
	else sibling = pickBestSibling(leaf, root->leftChild, math::floatMax);

	// Stage 2: create a new parent
	AABBTreeNode* newParent = createEmptyNode();
	
	newParent->parent = sibling->parent;
	
	newParent->leftChild = leaf;
	leaf->parent = newParent;

	newParent->rightChild = sibling;
	sibling->parent = newParent;

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

void AABBTree::removeLeaf(std::string id)
{
	//We never include the root
	for (size_t i = 1; i < nodes.size(); i++)
	{
		AABBTreeNode* node = nodes.at(i);
		if (node->gameObjectID == id)
		{
			if (!node->isRoot)
			{
				if (node->isLeft())
				{
					node->parent->rightChild->parent = node->parent->parent;
				}
				else
				{
					node->parent->leftChild->parent = node->parent->parent;
				}
			}
			refit(node->parent);
			nodes.erase(nodes.begin() + i);

			if (!node->parent->isRoot)
			{
				nodes.erase(nodes.begin() + (node->parent->index >= count() ? node->parent->index - 1 : node->parent->index));
				delete node->parent;
			}
			delete node;

			break;
		}
	}

	//Redo all indices
	for (size_t i = 0; i < count(); i++) nodes.at(i)->index = i;
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

AABBTreeNode* AABBTree::createLeaf(std::string id)
{
	AABBTreeNode* node = createEmptyNode();
	node->gameObjectID = id;
	node->box = new AABB(*App->scene->findById(id)->fatBoundingBox);

	return node;
}

AABBTreeNode * AABBTree::createEmptyNode()
{
	AABBTreeNode* node = new AABBTreeNode();
	node->box = new AABB(float3(0,0,0), float3(0,0,0));
	nodes.push_back(node);
	node->index = count() - 1;
	return node;
}

AABBTreeNode * AABBTree::pickBestSibling(AABBTreeNode * newLeaf, AABBTreeNode* candidate, float currentBestCost)
{
	if (count() <= 1) return candidate;

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

bool AABBTree::hasLeaf(std::string id)
{
	bool isThere = false;
	for (size_t i = 0; i < count(); i++)
	{
		if (nodes.at(i)->gameObjectID == id)
		{
			isThere = true;
			break;
		}
	}
	return isThere;
}

void AABBTree::refit(AABBTreeNode * index)
{
	if(index->isRoot)
	{
		if (index->gameObjectID != "")
		{
			index->box = App->scene->findById(index->gameObjectID)->fatBoundingBox;
		}
	}
	else
	{
		while (index != nullptr)
		{
			AABBTreeNode* child1 = index->leftChild;
			AABBTreeNode* child2 = index->rightChild;
			index->box = Union(child1, child2);
			index = index->parent;
		}
	}
}

