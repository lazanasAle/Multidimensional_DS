#ifndef _KD_TREE_H
#define _KD_TREE_H
#pragma once

#include <functional>
#include <vector>
#include <iostream>
#include <utility>

using std::function, std::pair, std::vector, std::swap;

enum position {
	LEFT,
	RIGHT
};


template <typename T>

class kd_node {
public:
	T key;
	kd_node<T> *left;
	kd_node<T> *right;
	size_t level;

	kd_node(T key_as);
	void kd_link_node(kd_node<T> *child, position p);
};

template <typename T>

class kd_node_iterator {
private:
	kd_node<T> *node;
public:
	kd_node_iterator(kd_node<T> *node);
	T &context();
	bool is_null();
};

template <typename T>

class kd_tree {
private:
	kd_node<T> *root;
	vector<function<int (T &, T &)>> comparators;
	kd_node<T> *look_up(T &key);
	size_t cmp_size;
	void range_lookup(pair<T, T> &interval, vector<T> &vec, kd_node<T> *curr_node);
	void preorder(kd_node<T> *node, vector<T> &vec);
	void inorder(kd_node<T> *node, vector<T> &vec);
	void postorder(kd_node<T> *node, vector<T> &vec);
	bool into_rectangle(kd_node<T> *curr_node, pair<T, T> &interval);
	T find_min(vector<T> &vec, size_t dimension);
	T find_max(vector<T> &vec, size_t dimension);
	void remove_node(kd_node<T> *&node);
public:
	kd_tree(vector<function<int (T &, T &)>> &comparators);
	void insert(T &key);
	void erase(T &key);
	kd_node_iterator<T> find(T &key);
	vector<T> preorder();
	vector<T> inorder();
	vector<T> postorder();
	vector<T> range_search(pair<T, T> &interval);
	bool empty();
};



#include "kd_tree.impl.h"

#endif /* _KD_TREE_H */
