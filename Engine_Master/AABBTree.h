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
	std::vector<AABBTreeNode*> nodes;
	std::vector<AABBTreeNode*> siblingsPriorityQueue;
	AABBTreeNode* createLeaf(std::string id);
	AABBTreeNode* createEmptyNode();
	AABBTreeNode* pickBestSibling(AABBTreeNode*, AABBTreeNode*, float);
	void refit(AABBTreeNode*);
	void printTree();
	void printNode(AABBTreeNode*, int);
};
#endif __AABBTree_H__

