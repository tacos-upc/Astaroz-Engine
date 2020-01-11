#include "AABBTree.h"
#include "Math/MathAll.h"
#include "Geometry/AABB.h"
#include "ModuleScene.h"
#include "Globals.h"
#include "Application.h"
#include <algorithm>  // remove and remove_if


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

void AABBTree::insertLeaf(std::string objectIndex)
{
	LOG("------------------------------------------------- INSERT LEAF ---------------------------------------------------------");
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

	LOG("I've chosen my sibling: ");
	printNode(sibling, -1);

	// Stage 2: create a new parent
	AABBTreeNode* newParent = createEmptyNode();
	LOG("The new parent: ");
	printNode(newParent, -1);

	newParent->parent = sibling->parent;
	
	if (sibling->parent != nullptr)
	{
		if (sibling->isLeft()) sibling->parent->leftChild = newParent;
		else sibling->parent->rightChild = newParent;
	}
	newParent->leftChild = leaf;
	leaf->parent = newParent;

	newParent->rightChild = sibling;
	sibling->parent = newParent;
	

	if (sibling == root) root = newParent;

	LOG("After insertion");
	printTree();

	//Stage 3: Rebalance the tree if required
	root->calculateDepth();
	LOG("After depth calculation");
	printTree();

	if (math::Abs(newParent->rightChild->depth - newParent->leftChild->depth) >= 2)
	{
		//The tree is out of balance
		AABBTreeNode* deepNode = newParent->getDeepestChild();
		AABBTreeNode* shallowNode = newParent->getShallowestChild();

		AABBTreeNode* deepestGrandChildren = deepNode->getDeepestChild();
		AABBTreeNode* shallowestGrandChildren = deepNode->getShallowestChild();

		//Is rotation necessary?
		if (shallowestGrandChildren != nullptr && shallowestGrandChildren->costWith(shallowNode) < newParent->cost())
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

	LOG("Finished Tree");
	printTree();
}


void AABBTree::removeLeaf(std::string id)
{
	LOG("------------------------------------------------- REMOVE LEAF ---------------------------------------------------------");
	printTree();

	for (size_t i = 0; i < nodes.size(); i++)
	{
		AABBTreeNode* node = nodes.at(i);
		if (node->gameObjectID == id)
		{
			LOG("I've found it: %d", i);

			AABBTreeNode* parent = node->parent;
			
			if (parent != nullptr)
			{
				LOG("Node %x has a parent: %x", nodes.at(i), nodes.at(i)->parent);

				AABBTreeNode* granpa = parent->parent;

				if (node->isLeft())
				{
					LOG("The node is left");
					parent->rightChild->parent = granpa;
					printTree();

					if (granpa != nullptr)
					{
						LOG("There's a granpa %x", granpa);

						if (parent->isLeft())
						{
							LOG("The parent is left");
							granpa->leftChild = parent->rightChild;
							printTree();
						}
						else
						{
							LOG("The parent is right");
							granpa->rightChild = parent->rightChild;
							printTree();
						}
					}
					else
					{
						LOG("The granpa is null");
						root = parent->rightChild;
						printTree();
					}
				}
				else
				{
					LOG("The node is right");
					parent->leftChild->parent = granpa;
					printTree();

					if (granpa != nullptr)
					{
						LOG("There's a granpa %x", granpa);

						if (parent->isLeft())
						{
							LOG("The parent is left");
							granpa->leftChild = parent->leftChild;
							printTree();
						}
						else
						{
							LOG("The parent is right");
							granpa->rightChild = parent->leftChild;
							printTree();
						}
					}
					else
					{
						LOG("The granpa is null");
						root = parent->leftChild;
						printTree();
					}
				}
			}

			nodes[i] = nullptr;
			printTree();

			if(parent != nullptr) nodes[parent->index] = nullptr;
			printTree();


			delete node;
			printTree();

			delete parent;
			printTree();

			break;
		}
	}

	//Redo all indices
	nodes.erase(std::remove(nodes.begin(), nodes.end(), nullptr), nodes.end());
	printTree();

	for (size_t i = 0; i < count(); i++) nodes.at(i)->index = i;
	printTree();

	if(nodes.size() > 0) root->calculateDepth();
}

AABBTreeNode * AABBTree::getNode(int index)
{
	AABBTreeNode* node = nullptr;
	if (index >= 0 && index < nodes.size()) node = nodes.at(index);
	return node;
}

int AABBTree::count()
{
	return nodes.size();
}

AABBTreeNode* AABBTree::createLeaf(std::string go)
{
	AABBTreeNode* node = createEmptyNode();
	node->gameObjectID = go;
	node->box = new AABB(*App->scene->findById(go)->fatBoundingBox);

	return node;
}

AABBTreeNode * AABBTree::createEmptyNode()
{
	AABBTreeNode* node = new AABBTreeNode();
	node->box = new AABB(float3(0, 0, 0), float3(0, 0, 0));
	nodes.push_back(node);
	node->index = count() - 1;
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

void AABBTree::printTree()
{
	LOG("---------------------- TREE STATE ------------------------");
	int depth = 0;
	int counting = 0;
	while (counting < nodes.size())
	{
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if (nodes.at(i) == nullptr)
			{
				counting += 1;
				continue;
			}

			if(nodes.at(i)->depth == depth)
			{
				printNode(nodes.at(i), depth);
				counting += 1;
			}
		}
		depth += 1;
	}
	LOG("--------------------------------------------------------------------");
}

void AABBTree::printNode(AABBTreeNode* node, int depth)
{
	LOG("Index: %d, Address: %x, Depth: %d, ID: %s, parent address: %x, leftChild: %x, rightChild: %x",
		node->index,
		node,
		depth,
		node->gameObjectID.c_str(),
		node->parent == nullptr ? 0 : node->parent,
		node->leftChild == nullptr ? 0 : node->leftChild,
		node->rightChild == nullptr ? 0 : node->rightChild
	);
}
