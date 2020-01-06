#pragma once
#ifndef __AABBTreeNode_H__
#define __AABBTreeNode_H__

#include "Math/MathAll.h"
#include "GameObject.h"

class AABBTreeNode
{
public:
	AABBTreeNode();
	~AABBTreeNode();

	AABBTreeNode* leftChild;
	AABBTreeNode* rightChild;
	AABBTreeNode* parent;

	AABB* box;

	GameObject* gameObject;
	int depth;

	bool isRoot();
	bool isLeaf();
	bool isLeft();
	float cost();
	float costWith(AABBTreeNode*);
	float inheritedCost();
	AABB* Union(AABBTreeNode* B);
	void receiveDepthData(int depth, bool isLeft, bool querySibling);
	int calculateDepth();
	AABBTreeNode* getDeepestChild();
	AABBTreeNode* getShallowestChild();
};
#endif __AABBTreeNode_H__

