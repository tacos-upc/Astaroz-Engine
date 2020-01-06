#pragma once
#pragma once
#ifndef __AABBTree_H__
#define __AABBTree_H__
#include "AABBTreeNode.h"

class AABBTree
{
	AABBTree();
	~AABBTree();

public:
	AABBTreeNode* root;
	int count;
	AABB* Union(AABB* A, AABB* B);

	void insertLeaf(int objectIndex);

private:
	AABBTreeNode* createLeaf(int);
	AABBTreeNode* pickBestSibling(AABBTreeNode*, int);
};
#endif __AABBTree_H__

