// kd_node methods

#include "kd_tree.h"
template <typename T>

kd_node<T>::kd_node(T key_as) {
	this->key = key_as;
	this->left = this->right = nullptr;
	this->level = 0;
}

template <typename T>

void kd_node<T>::kd_link_node(kd_node<T> *child, position p) {
	if (p == LEFT)
		this->left = child;
	else
		this->right = child;
	child->level = this->level + 1;
}

// kd_node iterator methods

template <typename T>

kd_node_iterator<T>::kd_node_iterator(kd_node<T> *node) {
	this->node = node;
}

template <typename T>

bool kd_node_iterator<T>::is_null() {
	return (this->node == nullptr);
}

template <typename T>

T &kd_node_iterator<T>::context() {
	T &key_ref = this->node->key;
	return key_ref;
}

// kd_tree functions

template <typename T>

kd_node<T> *kd_tree<T>::look_up(T &key) {
	kd_node<T> *ptr = this->root, *ret_ptr = nullptr;
	size_t cmp_size = this->comparators.size();
	bool found = false;

	while (ptr != nullptr && !found) {
		T this_context = ptr->key;
		int res = this->comparators[ptr->level % cmp_size](this_context, key);
		if (res == 0) {
			ret_ptr = ptr;
			found = true;
		}
		else if (res < 0)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}
	return ret_ptr;
}

template <typename T>

void kd_tree<T>::preorder(kd_node<T> *node, vector<T> &vec) {
	if (node) {
		vec.push_back(node->key);
		preorder(node->left, vec);
		preorder(node->right, vec);
	}
}

template <typename T>

void kd_tree<T>::inorder(kd_node<T> *node, vector<T> &vec) {
	if (node) {
		inorder(node->left, vec);
		vec.push_back(node->key);
		inorder(node->right, vec);
	}
}

template <typename T>

void kd_tree<T>::postorder(kd_node<T> *node, vector<T> &vec) {
	if (node) {
		postorder(node->left, vec);
		postorder(node->right, vec);
		vec.push_back(node->key);
	}
}

template <typename T>

kd_tree<T>::kd_tree(vector<function<int (T &, T &)>> &comparators) {
	this->comparators = comparators;
	this->root = nullptr;
	this->cmp_size = comparators.size();
}

template <typename T>

bool kd_tree<T>::empty() {
	return (this->root == nullptr);
}

template <typename T>

kd_node_iterator<T> kd_tree<T>::find(T &key) {
	return kd_node_iterator<T>(this->look_up(key));
}

template <typename T>

void kd_tree<T>::insert(T &key) {
	if (!this->root)
		this->root = new kd_node(key);
	else {
		kd_node<T> *node_ptr = this->root, *parent = nullptr;
		position p;

		while (node_ptr) {
			T this_context = node_ptr->key;
			parent = node_ptr;
			int res = this->comparators[node_ptr->level % cmp_size](this_context, key);
			if (res < 0) {
				node_ptr = node_ptr->right;
				p = RIGHT;
			}
			else {
				node_ptr = node_ptr->left;
				p = LEFT;
			}
		}
		kd_node<T> *nnode = new kd_node(key);
		parent->kd_link_node(nnode, p);
	}
}

template <typename T>

vector<T> kd_tree<T>::preorder() {
	vector<T> vec;
	this->preorder(this->root, vec);
	return vec;
}

template <typename T>

vector<T> kd_tree<T>::inorder() {
	vector<T> vec;
	this->inorder(this->root, vec);
	return vec;
}

template <typename T>

vector<T> kd_tree<T>::postorder() {
	vector<T> vec;
	this->postorder(this->root, vec);
	return vec;
}

template <typename T>

bool kd_tree<T>::into_rectangle(kd_node<T> *curr_node, pair<T, T> &interval) {
        T lower = interval.first;
        T upper = interval.second;
        bool greater_of_lower = false;
        bool less_than_upper = false;
        size_t cond1 = 0, cond2 = 0;

        for(auto &comp : this->comparators)
                cond1 = (comp(curr_node->key, lower) >= 0)? cond1 + 1 : cond1;
        greater_of_lower = (cond1 >= cmp_size);
        for(auto &comp : this->comparators)
                cond2 = (comp(curr_node->key, upper) <= 0)? cond2 +1 : cond2;
        less_than_upper = (cond2 >= cmp_size);
        return (greater_of_lower && less_than_upper);
}

template <typename T>

void kd_tree<T>::range_lookup(pair<T, T> &interval, vector<T> &vec, kd_node<T> *curr_node) {
        T lower = interval.first;
        T upper = interval.second;
        if (this->into_rectangle(curr_node, interval))
                vec.push_back(curr_node->key);
        if (curr_node->left) {
                if (comparators[curr_node->level % cmp_size](lower, curr_node->key) < 0)
                        range_lookup(interval, vec, curr_node->left);
        }
        if (curr_node->right) {
                if (comparators[curr_node->level % cmp_size](upper, curr_node->key) > 0)
                        range_lookup(interval, vec, curr_node->right);
        }

}

template <typename T>

T kd_tree<T>::find_min(vector<T> &vec, size_t dimension) {
	T min = vec[0];
	size_t j = 1;
	size_t vec_len = vec.size();
	while (j < vec_len) {
		if (comparators[dimension](vec[j], min) < 0)
			min = vec[j];
		j++;
	}
	return min;
}

template <typename T>

T kd_tree<T>::find_max(vector<T> &vec, size_t dimension) {
	T max = vec[0];
	size_t j = 1;
	size_t vec_len = vec.size();
	while (j < vec_len) {
		if (comparators[dimension](vec[j], max) > 0)
			max = vec[j];
		j++;
	}
	return max;
}

template <typename T>

void kd_tree<T>::remove_node(kd_node<T> *&node) {
	if (!node->left && !node->right) {
		delete(node);
		node = nullptr;
	}
	else {
		kd_node<T> *succ_ptr = (node->right)? node->right : node->left;
                vector<T> children;
		inorder(succ_ptr, children);
		T replacer = (node->right)? find_min(children, node->level % cmp_size) : find_max(children, node->level % cmp_size);
		kd_node<T> *replacement_node = look_up(replacer);
                swap(replacement_node->key, node->key);
		remove_node(replacement_node);
	}
}

template <typename T>

vector<T> kd_tree<T>::range_search(pair<T, T> &interval) {
        vector<T> ret_vec;
        range_lookup(interval, ret_vec, root);
        return ret_vec;
}

template <typename T>

void kd_tree<T>::erase(T &key) {
	kd_node<T> *node = look_up(key);
	remove_node(node);
}
