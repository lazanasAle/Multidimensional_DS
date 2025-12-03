//headers
#ifndef _INTERVAL_TREE_HPP
#define _INTERVAL_TREE_HPP

#include <iostream>
#include <functional>

using std::vector, std::max, std::function, std::pair, std::sort;

template <typename T> //created template class for generic typw T
class interval //the interval class represents a range
{
    public:
        //the range is [low,high]
        T low;
        T high;

        interval(T l, T h); 
        // constructor with parameters l=low, h=high


};

//template <typename T>
//will update it soon
// we will for sure need a class for interval_node and one for iteration

// Run it as:
//g++ -std=c++17 test_interval_tree.cpp -o interval_test
//./interval_test

#include "interval_tree.impl.hpp"
#endif /* _INTERVAL_TREE_HPP*/