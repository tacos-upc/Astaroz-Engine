#include "AABBTreeNode.h"
#include "Geometry/AABB.h"

AABBTreeNode::AABBTreeNode()
{
	leftChild = nullptr;
	rightChild = nullptr;
	parent = nullptr;
	gameObject = nullptr;

	depth = 1;
}

AABBTreeNode::~AABBTreeNode()
{
	delete leftChild;
	delete rightChild;
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
	return box == nullptr? 0.f : box->SurfaceArea();
}

float AABBTreeNode::costWith(AABBTreeNode* theOther)
{
	return Union(theOther)->SurfaceArea();
}

float AABBTreeNode::inheritedCost()
{
	if (parent == nullptr) return 0.0f;
	return parent->cost() + parent->inheritedCost();
}

AABB * AABBTreeNode::Union(AABBTreeNode* B)
{
	AABB* C = nullptr;

	if (B == nullptr || B->box == nullptr)
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

void AABBTreeNode::receiveDepthData(int depthReceived, bool isLeft, bool querySibling)
{
	depth = 1;
	if (querySibling)
	{
		int otherDepth = isLeft ? rightChild->calculateDepth() : leftChild->calculateDepth();
		depth = depth += math::Max(depthReceived, otherDepth);
	}
	else depth += depthReceived;

	if (parent == nullptr) return;
	else parent->receiveDepthData(depth, parent->leftChild == this, false);
}

int AABBTreeNode::calculateDepth()
{
	depth = 1;
	int rightDepth = rightChild == nullptr ? 0 : rightChild->calculateDepth();
	int leftDepth = leftChild == nullptr ? 0 : leftChild->calculateDepth();

	depth = depth + math::Max(leftDepth, rightDepth);

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
