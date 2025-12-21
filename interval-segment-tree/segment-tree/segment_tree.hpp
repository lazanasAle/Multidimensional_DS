#ifndef _SEGMENT_TREE_HPP
#define _SEGMENT_TREE_HPP
#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>

using std::vector, std::max, std::min;

struct segment {
        size_t lb, rb;
        size_t sum;

        segment(size_t lb, size_t rb) {
                this->lb = min(lb, rb);
                this->rb = max(lb, rb);
        }
};

class segment_tree {
private:
        vector<segment> sg_vec;
public:
        void insert(size_t lb, size_t lr);
        void sum(size_t lb, size_t lr);
        void range_query(size_t lb, size_t lr);
        void stabbing_query(size_t pt);
};


#include "segment_tree.impl.hpp"

#endif /* _SEGMENT_TREE_HPP */
