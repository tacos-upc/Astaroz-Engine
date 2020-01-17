#include "AABBTreeNode.h"
#include "Geometry/AABB.h"

AABBTreeNode::AABBTreeNode()
{
	leftChild = nullptr;
	rightChild = nullptr;
	parent = nullptr;
	gameObjectID = "";

	depth = 1;
	index = 0;
}

AABBTreeNode::~AABBTreeNode()
{
	delete box;
	box = nullptr;
	leftChild = nullptr;
	rightChild = nullptr;
	parent = nullptr;
}

bool AABBTreeNode::isRoot()
{
	return parent == nullptr;
}

bool AABBTreeNode::isLeaf()
{
	return leftChild == nullptr && rightChild == nullptr;
}

bool AABBTreeNode::isLeft()
{
	return parent != nullptr && parent->leftChild == this;
}

float AABBTreeNode::cost()
{
	return this == nullptr || box == nullptr? 0.f : box->SurfaceArea();
}

float AABBTreeNode::costWith(AABBTreeNode* theOther)
{
	return Union(theOther)->SurfaceArea();
}

float AABBTreeNode::inheritedCost()
{
	if (this == nullptr || parent == nullptr) return 0.0f;
	return parent->cost() + parent->inheritedCost();
}

AABB * AABBTreeNode::Union(AABBTreeNode* B)
{
	AABB* C = nullptr;

	if (B == nullptr || B->box == nullptr || B->box == (AABB*)0xdddddddd)//This pointer is probablyy not needed
	{
		C = new AABB(*box);
	}
	else if (box == nullptr)
	{
		C = new AABB(*B->box);
	}
	else
	{
		C = new AABB();
		C->minPoint = math::Min(box->minPoint, B->box->minPoint);
		C->maxPoint = math::Max(box->maxPoint, B->box->maxPoint);
	}
	return C;
}

int AABBTreeNode::calculateDepth()
{
	if (parent == nullptr) depth = 0;
	else depth = 1 + parent->depth;

	if(rightChild != nullptr) rightChild->calculateDepth();
	if(leftChild != nullptr) leftChild->calculateDepth();

	return depth;
}

AABBTreeNode * AABBTreeNode::getDeepestChild()
{
	AABBTreeNode* deepest = nullptr;
	if (leftChild == nullptr) deepest = rightChild;
	else if (rightChild == nullptr) deepest = leftChild;
	else deepest = leftChild->depth > rightChild->depth ? leftChild : rightChild;

	return deepest;
}

AABBTreeNode * AABBTreeNode::getShallowestChild()
{
	AABBTreeNode* shallow = getDeepestChild();
	if (shallow != nullptr) shallow = shallow == leftChild ? rightChild : leftChild;
	return shallow;
}
