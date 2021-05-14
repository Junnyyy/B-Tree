#include "btree.h"

//-------------------------------------------------------
//BTreeNode Functions 

BNode::BNode(BNode* parent, bool leaf) {
	this->leaf = leaf;
	this->parent = parent;
	size = 0;
}

//-------------------------------------------------------
//BTree Functions 

BTree::BTree(int degree) {
	this->degree = degree;
	root = new BNode(nullptr, true);
}

//-------------------------------------------------------
//BTree Traverse Functions 

void BTree::traverse(BNode* node, int level, int targetLevel) {
	//level should be 1 by default
	if (node == nullptr) return;

	//Run through node size and output
	for (int i = 0; i < node->size; i++) {
		if (level == targetLevel) {
			cout << to_string(node->keys.at(i)) << " ";
		}
	}

	//output children if exist
	if (!node->leaf) {
		for (int i = 0; i < node->children.size(); i++) {
			traverse(node->children.at(i), level + 1, targetLevel);
		}
	}
}

//-------------------------------------------------------
//BTree Insert Functions 

void BTree::insert(BNode* node, int key) {
	//insertkey if leaf
	if (node->leaf) {
		insertKeyOnly(node, key);
		return;
	}

	//iterate to child index
	int index = 0;
	while (index < node->size && key > node->keys.at(index)) {
		index++;
	}

	//redo in child node
	insert(node->children.at(index), key);
}

void BTree::insertKeyOnly(BNode* node, int key) {
	//push key to node
	node->keys.push_back(key);
	sort(node->keys.begin(), node->keys.end());
	node->size++;

	//if maxes out degree split
	if (node->size >= degree) {
		splitNode(node);
	}
}

void BTree::splitNode(BNode* right) {
	BNode* left = new BNode(right->parent, right->leaf);
	
	//new size
	int sizeNew = degree % 2 == 0 ? degree / 2 - 1 : degree / 2;

	//creates left node;
	for (int i = 0; i < sizeNew; i++) {
		left->keys.push_back(right->keys.at(0));
		left->size++;
		right->keys.erase(right->keys.begin());
	}

	//new parent/root key
	int splitNodeKey = right->keys.at(0);
	right->keys.erase(right->keys.begin());

	if (!right->leaf) {
		//transfer children
		for (int i = 0; i < sizeNew + 1; i++) {
			left->children.push_back(right->children.at(0));
			left->children.at(i)->parent = left;
			right->children.erase(right->children.begin());
		}
	}

	right->size = degree / 2;
	if (right->parent == nullptr) {
		//new root
		BNode* nRoot = new BNode(nullptr, false);

		//transfer left and right child
		nRoot->children.push_back(left);
		nRoot->children.push_back(right);

		//change left and right child parent
		nRoot->children.at(0)->parent = nRoot;
		nRoot->children.at(1)->parent = nRoot;

		//add split key to new root
		nRoot->keys.push_back(splitNodeKey);
		nRoot->size++;

		root = nRoot;
		return;
	}

	//new parent
	BNode* parent = right->parent;

	//insert left to left position
	right->parent->children.insert(find(parent->children.begin(), parent->children.end(), right), left);
	insertKeyOnly(right->parent, splitNodeKey);
}

int BTree::getHeight(BNode* node) {
	//if nullptr tree doesn't exist
	if (node == nullptr) return 0;

	//if leaf return 1
	if(node->leaf) {
		return 1;
	}
	else {
		//recursively get height of left stack
		int height = 1 + getHeight(node->children.at(0));
		return height;
	}

}