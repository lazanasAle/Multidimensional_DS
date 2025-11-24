#ifndef _2D_RANGE_TREE_HPP
#define _2D_RANGE_TREE_HPP
#pragma once

#include <set>
#include <cstdint>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>

#define TOL 1
#define generic2(x, y) template <typename x, typename y>
#define generic3(x, y, z) template <typename x, typename y, typename z>


using std::set, std::vector, std::max, std::cout, std::swap, std::pair;

generic2(T, CMP)

class two_d_key {
public:
	T key;
	set<T, CMP> secondary;
	two_d_key(T key_as);
	
	void insert_in(T &element);
	void remove_from(T &element);
};

generic2(T, CMP)

class two_d_range_node {
public:
	two_d_key<T, CMP> *key;
	two_d_range_node<T, CMP> *left;
	two_d_range_node<T, CMP> *right;
	two_d_range_node<T, CMP> *parent;
	int32_t height;
	two_d_range_node(two_d_key<T, CMP> *ks);
	
	int32_t balance_factor();
	void update_height();
	void left_rotation();
	void right_rotation();
	void left_right_rotation();
	void right_left_rotation();
	~two_d_range_node();
	
};

generic2(T, CMP)

class two_d_range_node_iterator {
private:
	two_d_range_node<T, CMP> *node;
public:
	two_d_range_node_iterator(two_d_range_node<T, CMP> *node);
	T context();
	set<T, CMP> *secondary();
	bool is_null();
	two_d_range_node_iterator<T, CMP> next();
	two_d_range_node_iterator<T, CMP> prev();
};


generic3(T, CMP1, CMP2)

class two_d_range_tree {
private:
	two_d_range_node<T, CMP2> *root;
	CMP1 comparator;
	
	two_d_range_node<T, CMP2> *&search(T &elem, two_d_range_node<T, CMP2> *&subtree_root);
	void clear_collect_update1(two_d_range_node<T, CMP2> *&node);
	void clear_collect_update2(two_d_range_node<T, CMP2> *&node1, two_d_range_node<T, CMP2> *&node2);
	void avl_balance(two_d_range_node<T, CMP2> *&node);
	void add_node(two_d_range_node<T, CMP2> *&new_node, two_d_range_node<T, CMP2> *&subtree_root);
	void remove_node(T &elem, two_d_range_node<T, CMP2> *&subtree_root);
	two_d_range_node<T, CMP2> *max_node(two_d_range_node<T, CMP2> *&subtree_root);
	two_d_range_node<T, CMP2> *min_node(two_d_range_node<T, CMP2> *&subtree_root);
	void preorder(two_d_range_node<T, CMP2> *&subtree_root, vector<two_d_key<T, CMP2>> &pr_arr);
	void inorder(two_d_range_node<T, CMP2> *&subtree_root, vector<two_d_key<T, CMP2>> &in_arr);
	void postorder(two_d_range_node<T, CMP2> *&subtree_root, vector<two_d_key<T, CMP2>> &po_arr);
public:
	two_d_range_tree();
	two_d_range_node_iterator<T, CMP2> find(T &elem);
	two_d_range_node_iterator<T, CMP2> upper_bound(T &elem);
	two_d_range_node_iterator<T, CMP2> lower_bound(T &elem);
	vector<T> two_d_range_search(pair<T, T> &interval); 
	bool empty();
	two_d_key<T, CMP2> get_root();
	void insert(T &elem);
	void erase(T &elem);
	T minimum();
	T maximum();
	vector<two_d_key<T, CMP2>> preorder();
	vector<two_d_key<T, CMP2>> inorder();
	vector<two_d_key<T, CMP2>> postorder();
	~two_d_range_tree();
		
};

#include "2D_range_tree.impl.h"

#endif /* _2D_RANGE_TREE_HPP */
