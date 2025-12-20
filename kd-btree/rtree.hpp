#ifndef _RTREE_HPP
#define _RTREE_HPP

#include "kd_btree.hpp"
#include <cmath>
#include <limits>

template <typename T, typename C>
class rtree: public kd_btree<T,C>
{
    private: 
        //..
}

#include "rtree.impl.hpp"
#endif /* _RTREE_HPP */