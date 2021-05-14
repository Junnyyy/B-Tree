#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class BNode {
private:
	vector<int> keys;
	vector<BNode*> children;
	BNode* parent;
	int size;
	bool leaf;
	BNode(BNode* parent, bool leaf);

	friend class BTree;
};

class BTree {
private:
	BNode* root;
	int degree;

	void traverse(BNode* node, int level, int targetLevel);
	void insert(BNode* insert, int key);
	void insertKeyOnly(BNode* insert, int key);
	void splitNode(BNode* right);
	int getHeight(BNode* node);
public:
	BTree(int degree);
};