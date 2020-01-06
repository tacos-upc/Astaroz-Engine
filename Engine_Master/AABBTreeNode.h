#pragma once
#ifndef __AABBTreeNode_H__
#define __AABBTreeNode_H__
#include "Math/MathAll.h"


class AABBTreeNode
{
public:
	AABBTreeNode();
	~AABBTreeNode();

	AABBTreeNode* leftChild;
	AABBTreeNode* rightChild;
	AABBTreeNode* parent;

	AABB* box;

	bool isLeaf;
	bool isRoot;
	int objectIndex;

};
#endif __AABBTreeNode_H__

