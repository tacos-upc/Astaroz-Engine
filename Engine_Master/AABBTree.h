#pragma once
#pragma once
#ifndef __AABBTree_H__
#define __AABBTree_H__
#include "AABBTreeNode.h"
#include "GameObject.h"
#include <list>
#include <map>

class AABBTree
{
public:
	AABBTree();
	~AABBTree();

	AABBTreeNode* root;
	AABB* Union(AABBTreeNode* A, AABBTreeNode* B);

	void insertLeaf(GameObject* obj);
	void removeLeaf(GameObject* obj);
	AABBTreeNode* getNode(int);
	int count();

private:
	std::vector<AABBTreeNode*> siblingsPriorityQueue;
	std::vector<AABBTreeNode*> nodes;
	AABBTreeNode* createLeaf(GameObject*);
	AABBTreeNode* createEmptyNode();
	AABBTreeNode* pickBestSibling(AABBTreeNode*, AABBTreeNode*, float);

	void refit(AABBTreeNode*);
};
#endif __AABBTree_H__

