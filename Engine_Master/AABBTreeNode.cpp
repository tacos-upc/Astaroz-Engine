#include "AABBTreeNode.h"

AABBTreeNode::AABBTreeNode()
{
	isLeaf = false;
	isRoot = false;
	leftChild = nullptr;
	rightChild = nullptr;
	parent = nullptr;
}

AABBTreeNode::~AABBTreeNode()
{
	delete leftChild;
	delete rightChild;
}
