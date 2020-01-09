#pragma once
#pragma once
#ifndef __AABBTree_H__
#define __AABBTree_H__
#include "AABBTreeNode.h"
#include "GameObject.h"
#include <list>
#include <map>
#include <string>

class AABBTree
{
public:
	AABBTree();
	~AABBTree();

	AABBTreeNode* root;
	AABB* Union(AABBTreeNode* A, AABBTreeNode* B);

	void insertLeaf(std::string id);
	void removeLeaf(std::string id);
	AABBTreeNode* getNode(int);
	int count();

private:
	std::vector<AABBTreeNode*> siblingsPriorityQueue;
	std::vector<AABBTreeNode*> nodes;
	AABBTreeNode* createLeaf(std::string id);
	AABBTreeNode* createEmptyNode();
	AABBTreeNode* pickBestSibling(AABBTreeNode*, AABBTreeNode*, float);
	bool hasLeaf(std::string id);
	void refit(AABBTreeNode*);
};
#endif __AABBTree_H__

