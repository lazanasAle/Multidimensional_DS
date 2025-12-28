//headers
#ifndef _INTERVAL_TREE_HPP
#define _INTERVAL_TREE_HPP

#include <iostream>
#include <functional>
#include <vector> 
#include <algorithm>
#include <cstdint> 
#include <string>

using std::vector, std::max, std::function, std::pair, std::sort , std::string, std::to_string;

template <typename T> //created template class for generic type T
class interval //the interval class represents a range [low,high]
{
    public:
        //the range is [low,high]
        T low;
        T high;

        interval(T l, T h); 
        // constructor with parameters l=low, h=high

        interval() {} //default constructor (empty body)
        bool overlaps(interval<T> &other); //checking if tis interval overlaps with another interval
        bool contains(T point); //checking if this interval contains a point

};

template <typename T>
class interval_node
{
    public:
        interval<T> *inter; //Pointer to the interval stored inside this node
        T max_end; // max endpoint in this subtree (θα ειναι χρησιμο για το κλάδεμα)
        int32_t height; //might change later (AVL..)

        interval_node(interval<T> *i); //constructor with interval pointer
        int32_t balance_factor(); //left-right height

        //pointers to left, right (children) and parent nodes
        interval_node<T> *left;
        interval_node<T> *right;
        interval_node<T> *parent;

        void update_height(); //update the height of the tree based on the children' heights
        void update_max_end(); //update max_end based on children and its own interal
        void left_rotation(); 
        void right_rotation(); 
        void left_right_rotation();
        void right_left_rotation();
        //The rotations are useful for balanc
        //will implement these functionms inside interval_tree.impl,hpp file 
        
        ~interval_node(); //destructor to clean up memory
};

template <typename T>
class interval_node_iterator //this is a useful class for traversing the interval teee
{
    private:
        interval_node<T> *node; //pointer to the current node
    

    public:
        interval_node_iterator(interval_node<T> *n); //constructor with a node pointer
        bool is_null(); //check if the iterator points to sth null
        interval<T> *context(); //get the interval at the current position

        interval_node_iterator<T> next(); //move to the next node - inorder traversal
        interval_node_iterator<T> previous(); // same as above but previous node
};

template <typename T>
class interval_tree
{
    private:
        interval_node<T> *root; //pointer to root node of the 3
        function<int (T &, T &)>comparator; //compare function for ordering intervals
        size_t node_count; //tracking total number of nodes
        interval_node<T> *&search(interval<T> &inter, interval_node<T> *&subtree_root); //searching for an interval in subtree
        void avl_balance(interval_node<T> *&node); //balance the tree using AVL rotations
        void add_node(interval_node<T> *&new_node,interval_node<T> *&subtree_root); //adding new node to the subtree
        void remove_node(interval<T> &inter, interval_node<T> *&subtree_root); //remove an interval from subtree
        void update_rec(interval<T> &old_inter, interval<T> &new_inter, interval_node<T> *&subtree_root); //recursive update helper
        interval_node<T> *max_node(interval_node<T> *&subtree_root); //find a node with max interval in the subtree
        interval_node<T> *min_node(interval_node<T> *&subtree_root); // find a node with min interval in the subtree
        void interval_search_rec(interval<T> &inter,vector<interval<T>> &result,interval_node<T> *subtree_root); // recursive interval search
        void update_max_end_up(interval_node<T> *node); //update max_end from node to root
        void stabbing_query_rec(T point, vector<interval<T>> &result, interval_node<T> *subtree_root); //recursive stabbing query
        void inorder(interval_node<T> *subtree_root, vector<interval<T>> &vec); //inorder traversal
    public:
        interval_tree(function<int (T &, T &)> cmp); //constructor with compare function
        bool empty();
        size_t size(); //get number of nodes in tree
        void insert(interval<T> &inter); //insert an interval into the tree
        void erase(interval<T> &inter); //delete an interval from the tree
        void update(interval<T> &old_inter, interval<T> &new_inter); //update an existing interval
        interval_node_iterator<T> find(interval<T> &inter); //find an interval
        vector<interval<T>> interval_search(interval<T> &inter); //find all the intervals overlapping with given interval
        vector<interval<T>> stabbing_query(T point); //find all intervals that contain the given point
        vector<interval<T>> inorder(); //return all intervals (sorted)
        void build(vector<interval<T>> &intervals); //build method
        string stringify_tree(); //string representation of the tree for debugging
    ~interval_tree(); //destructor to clean the tree memory

};

// Run it as:
//g++ -std=c++17 test_interval_tree.cpp -o interval_test
//./interval_test

#include "interval_tree.impl.hpp" //including the implementation file
#endif /* _INTERVAL_TREE_HPP*/ //end of header