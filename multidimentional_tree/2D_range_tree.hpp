#ifndef _2D_RANGE_TREE_HPP
#define _2D_RANGE_TREE_HPP
#pragma once

#include <set>
#include <cstdint>
#include <vector>
#include <iostream>
#include <algorithm>

#define TOL 1
#define generic2(x, y) template <typename x, typename y>
#define generic3(x, y, z) template <typename x, typename y, typename z>


using std::set; 
using std::vector;
using std::max;
using std::cout;
using std::swap;

generic2(T, CMP)

struct two_d_key {
	T key;
	set<T, CMP> secondary;
	two_d_key(T key_as);
	
	void insert_in(T &element);
	void remove_from(T &element);
};

generic2(T, CMP)

void vector_to_set(vector<two_d_key<T, CMP>> &vec, set<T, CMP> &set_sec) {
	for (size_t j = 1; j < vec.size(); ++j) {
		set_sec.insert(vec[j].key);
	}
}


generic2(T, CMP)

struct two_d_range_node {
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

struct two_d_range_node_iterator {
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

struct two_d_range_tree {
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

//two_d_key functions

generic2(T, CMP)

two_d_key<T, CMP>::two_d_key(T key_as) {
	this->key = key_as;
}

generic2(T, CMP)

void two_d_key<T, CMP>::insert_in(T &element) {
	this->secondary.insert(element);
}

generic2(T, CMP)

void two_d_key<T, CMP>::remove_from(T &element) {
	this->secondary.erase(element);
}

//two_d_range_node functions

generic2(T, CMP)

two_d_range_node<T, CMP>::two_d_range_node(two_d_key<T, CMP> *ks) {
	this->key = ks;
	this->left = this->right = this->parent = nullptr;
	this->height = 0;
}

generic2(T, CMP)

int32_t two_d_range_node<T, CMP>::balance_factor() {
	int32_t left_height = (this->left)? this->left->height : -1;
        int32_t right_height = (this->right)? this->right->height : -1;
        return (left_height-right_height);
}

generic2(T, CMP)

void two_d_range_node<T, CMP>::update_height() {
	int32_t left_height = (this->left)? this->left->height : -1;
        int32_t right_height = (this->right)? this->right->height : -1;
        this->height = max(left_height, right_height) + 1;
}

generic2(T, CMP)

void two_d_range_node<T, CMP>::left_rotation() {
	if (this->right == nullptr)
		return;
        two_d_range_node<T, CMP> *varA = this->left;
        two_d_range_node<T, CMP> *varB = this->right->left;
        two_d_range_node<T, CMP> *varC = this->right->right;
        two_d_key<T, CMP> *tmp = this->key;
        this->key = this->right->key;
        this->left = new two_d_range_node<T, CMP>(tmp);
        this->left->parent = this;
        this->left->left = varA;
        if (varA)
        	varA->parent = this->left;
        this->left->right = varB;
        if (varB)
        	varB->parent = this->left;
        this->right = varC;
        if (varC)
        	varC->parent = this;
        this->left->update_height();
        this->update_height();
}

generic2(T, CMP)

void two_d_range_node<T, CMP>::right_rotation() {
	if (this->left == nullptr)
		return;
        two_d_range_node<T, CMP> *varA = this->left->left;
        two_d_range_node<T, CMP> *varB = this->left->right;
        two_d_range_node<T, CMP> *varC = this->right;
        two_d_key<T, CMP> *tmp = this->key;
        this->key = this->left->key;
        this->right = new two_d_range_node<T, CMP>(tmp);
        this->right->parent = this;
        this->left = varA;
        if (varA)
        	varA->parent = this;
        this->right->left = varB;
        if (varB)
        	varB->parent = this->right;
        this->right->right = varC;
        if (varC)
        	varC->parent = this->right;
        this->right->update_height();
        this->update_height();
}

generic2(T, CMP)

void two_d_range_node<T, CMP>::left_right_rotation() {
	if (this->left) {
		this->left->left_rotation();
		this->right_rotation();
        }
}

generic2(T, CMP)

void two_d_range_node<T, CMP>::right_left_rotation() {
	if (this->right) {
		this->right->right_rotation();
		this->left_rotation();
        }
}

generic2(T, CMP)

two_d_range_node<T, CMP>::~two_d_range_node() {
	delete(key);
}

//range_node_iterator functions

generic2(T, CMP)

two_d_range_node_iterator<T, CMP>::two_d_range_node_iterator(two_d_range_node<T, CMP> *node) {
	this->node = node;
}

generic2(T, CMP)

T two_d_range_node_iterator<T, CMP>::context() {
	return this->node->key->key;
}

generic2(T, CMP)

set<T, CMP> *two_d_range_node_iterator<T, CMP>::secondary() {
	return &this->node->key->secondary;
}

generic2(T, CMP)

two_d_range_node_iterator<T, CMP> two_d_range_node_iterator<T, CMP>::next() {
	if (this->is_null())
		return two_d_range_node_iterator<T, CMP>(nullptr);
	if (this->node->right) {
		two_d_range_node<T, CMP> *next_node = this->node->right;
		while (next_node->left)
			next_node = next_node->left;
		return two_d_range_node_iterator<T, CMP>(next_node);
	}
	two_d_range_node<T, CMP> *rn = this->node;
	while ((rn->parent) && (rn->parent->right == rn))
		rn = rn->parent;
	return two_d_range_node_iterator<T, CMP>(rn->parent);
}

generic2(T, CMP)

two_d_range_node_iterator<T, CMP> two_d_range_node_iterator<T, CMP>::prev() {
	if (this->is_null())
		return two_d_range_node_iterator<T, CMP>(nullptr);
	if (this->node->left) {
		two_d_range_node<T, CMP> *prev_node = this->node->left;
		while (prev_node->right)
			prev_node = prev_node->right;
		return two_d_range_node_iterator<T, CMP>(prev_node);
	}
	two_d_range_node<T, CMP> *prn = this->node;
	while ((prn->parent) && (prn->parent->left == prn))
		prn = prn->parent;
	return two_d_range_node_iterator<T, CMP>(prn->parent);
}

generic2(T, CMP)

bool two_d_range_node_iterator<T, CMP>::is_null() {
	return (this->node == nullptr);
}

//two_d_range_tree functions

generic3(T, CMP1, CMP2)

two_d_range_node<T, CMP2> *&two_d_range_tree<T, CMP1, CMP2>::search(T &elem, two_d_range_node<T, CMP2> *&subtree_root) {
	if (subtree_root == nullptr)
		return subtree_root;
        else if (comparator(elem, subtree_root->key->key) == 0)
        	return subtree_root;
        else if (comparator(elem, subtree_root->key->key) < 0)
        	return search(elem, subtree_root->left);
        else
        	return search(elem, subtree_root->right);
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::clear_collect_update1(two_d_range_node<T, CMP2> *&node) {
	node->key->secondary.clear();
        		
        //collect children
        vector<two_d_key<T, CMP2>> my_children;
        preorder(node, my_children);
        		
        //update inner tree
        vector_to_set(my_children, node->key->secondary);	
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::clear_collect_update2(two_d_range_node<T, CMP2> *&node1, two_d_range_node<T, CMP2> *&node2) {
	node1->key->secondary.clear();
        node2->key->secondary.clear();
        		
        //collect children
        vector<two_d_key<T, CMP2>> one_children;
        preorder(node1, one_children);
        vector<two_d_key<T, CMP2>> two_children;
        preorder(node2, two_children);
        		
        		
        //update inner trees
        vector_to_set(one_children, node1->key->secondary);
        vector_to_set(two_children, node2->key->secondary);
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::avl_balance(two_d_range_node<T, CMP2> *&node) {
	if (node == nullptr)
		return;
	else if (node->balance_factor() > TOL) {
        	if (node->left && node->left->balance_factor() < 0) {
        		node->left_right_rotation();
        		clear_collect_update2(node, node->left);
        		
        	}
        	else {
        		node->right_rotation();
        		clear_collect_update1(node);
        	}
        }
        else if (node->balance_factor() < -TOL) {
        	if (node->right && node->right->balance_factor() > 0) {
        		node->right_left_rotation();
        		clear_collect_update2(node, node->right);
        	}
        	else {
        		node->left_rotation();
        		clear_collect_update1(node);
        	}
        }
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::add_node(two_d_range_node<T, CMP2> *&new_node, two_d_range_node<T, CMP2> *&subtree_root) {
	if (subtree_root == nullptr) {
		subtree_root = new_node;
        }
        else if (comparator(new_node->key->key, subtree_root->key->key) < 0) {
        	subtree_root->key->insert_in(new_node->key->key);
        	new_node->parent = subtree_root;
        	add_node(new_node, subtree_root->left);
        }
        else if (comparator(new_node->key->key, subtree_root->key->key) > 0) {
        	subtree_root->key->insert_in(new_node->key->key);
        	new_node->parent = subtree_root;
        	add_node(new_node, subtree_root->right);
        }
        avl_balance(subtree_root);
        subtree_root->update_height();
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::remove_node(T &elem, two_d_range_node<T, CMP2> *&sub_root) {
	if (sub_root) {
		if (comparator(elem, sub_root->key->key) < 0) {
			sub_root->key->remove_from(elem);
			remove_node(elem, sub_root->left);
		}
		else if (comparator(elem, sub_root->key->key) > 0) {
			sub_root->key->remove_from(elem);
			remove_node(elem, sub_root->right);
		}
		else {
			if (sub_root->left && sub_root->right) {
				two_d_range_node<T, CMP2> *to_swap = max_node(sub_root->left);
				swap(to_swap->key->key, sub_root->key->key);
				remove_node(to_swap->key->key, sub_root->left);
			}
			else if (sub_root->left || sub_root->right) {
				two_d_range_node<T, CMP2> *&to_swap = (sub_root->left)? sub_root->left : sub_root->right;
				swap(to_swap->key->key, sub_root->key->key);
				remove_node(to_swap->key->key, to_swap);
			}
			else {
				delete(sub_root);
				sub_root = nullptr;
			}
		}
		if (sub_root) {
			avl_balance(sub_root);
			sub_root->update_height();
		}
        }
}

generic3(T, CMP1, CMP2)

two_d_range_node<T, CMP2> *two_d_range_tree<T, CMP1, CMP2>::max_node(two_d_range_node<T, CMP2> *&subtree_root) {
	two_d_range_node<T, CMP2> *roos = subtree_root;
        while (roos->right != nullptr) {
        	roos = roos->right;
        }
        return roos;
}

generic3(T, CMP1, CMP2)

two_d_range_node<T, CMP2> *two_d_range_tree<T, CMP1, CMP2>::min_node(two_d_range_node<T, CMP2> *&subtree_root) {
	two_d_range_node<T, CMP2> *roos = subtree_root;
        while (roos->left != nullptr) {
        	roos = roos->left;
        }
        return roos;
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::preorder(two_d_range_node<T, CMP2> *&subtree_root, vector<two_d_key<T, CMP2>> &pr_arr) {
	if (subtree_root != nullptr) {
		pr_arr.push_back(*subtree_root->key);
		preorder(subtree_root->left, pr_arr);
		preorder(subtree_root->right, pr_arr);
	}
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::inorder(two_d_range_node<T, CMP2> *&subtree_root, vector<two_d_key<T, CMP2>> &in_arr) {
	if (subtree_root != nullptr) {
		inorder(subtree_root->left, in_arr);
		in_arr.push_back(*subtree_root->key);
		inorder(subtree_root->right, in_arr);
	}
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::postorder(two_d_range_node<T, CMP2> *&subtree_root, vector<two_d_key<T, CMP2>> &po_arr) {
	if (subtree_root != nullptr) {
		postorder(subtree_root->left, po_arr);
		postorder(subtree_root->right, po_arr);
		po_arr.push_back(*subtree_root->key);
	}
}

generic3(T, CMP1, CMP2)

two_d_range_tree<T, CMP1, CMP2>::two_d_range_tree() {
	root = nullptr;
}

generic3(T, CMP1, CMP2)

two_d_range_node_iterator<T, CMP2> two_d_range_tree<T, CMP1, CMP2>::find(T &elem) {
	return two_d_range_node_iterator<T, CMP2>(search(elem, root));
}

generic3(T, CMP1, CMP2)

bool two_d_range_tree<T, CMP1, CMP2>::empty() {
	return (root == nullptr);
}

generic3(T, CMP1, CMP2)

two_d_key<T, CMP2> two_d_range_tree<T, CMP1, CMP2>::get_root() {
	return root->key;
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::insert(T &elem) {
	two_d_key<T, CMP2> *ks = new two_d_key<T, CMP2>(elem);
	two_d_range_node<T, CMP2> *new_node = new two_d_range_node<T, CMP2>(ks);
	add_node(new_node, root);
}

generic3(T, CMP1, CMP2)

void two_d_range_tree<T, CMP1, CMP2>::erase(T &elem) {
	remove_node(elem, root);
}

generic3(T, CMP1, CMP2)

two_d_range_node_iterator<T, CMP2> two_d_range_tree<T, CMP1, CMP2>::upper_bound(T &elem) {
	two_d_range_node<T, CMP2> *rn = root, *ret_node = nullptr;
	while (rn) {
		if (comparator(rn->key->key, elem) <= 0) {
			ret_node = rn;
			rn = rn->right;
		}
		else
			rn = rn->left;
	}
	return two_d_range_node_iterator<T, CMP2>(ret_node);
}

generic3(T, CMP1, CMP2)

two_d_range_node_iterator<T, CMP2> two_d_range_tree<T, CMP1, CMP2>::lower_bound(T &elem) {
	two_d_range_node<T, CMP2> *rn = root, *ret_node = nullptr;
	while (rn) {
		if (comparator(rn->key->key, elem) >= 0) {
			ret_node = rn;
			rn = rn->left;
		}
		else
			rn = rn->right;
	}
	return two_d_range_node_iterator<T, CMP2>(ret_node);	
}

generic3(T, CMP1, CMP2)

T two_d_range_tree<T, CMP1, CMP2>::minimum() {
	two_d_range_node<T, CMP2> mini_node = min_node(root);
	return mini_node->key->key;
}

generic3(T, CMP1, CMP2)

T two_d_range_tree<T, CMP1, CMP2>::maximum() {
	two_d_range_node<T, CMP2> maxi_node = max_node(root);
	return maxi_node->key->key;	
}

generic3(T, CMP1, CMP2)

vector<two_d_key<T, CMP2>> two_d_range_tree<T, CMP1, CMP2>::preorder() {
	vector<two_d_key<T, CMP2>> to_ret;
	preorder(root, to_ret);
	return to_ret;
}

generic3(T, CMP1, CMP2)

vector<two_d_key<T, CMP2>> two_d_range_tree<T, CMP1, CMP2>::inorder() {
	vector<two_d_key<T, CMP2>> to_ret;
	inorder(root, to_ret);
	return to_ret;
}

generic3(T, CMP1, CMP2)

vector<two_d_key<T, CMP2>> two_d_range_tree<T, CMP1, CMP2>::postorder() {
	vector<two_d_key<T, CMP2>> to_ret;
	postorder(root, to_ret);
	return to_ret;
}

generic3(T, CMP1, CMP2)

two_d_range_tree<T, CMP1, CMP2>::~two_d_range_tree() {
	if (root)
		delete(root);
}

#endif /* _2D_RANGE_TREE_HPP */