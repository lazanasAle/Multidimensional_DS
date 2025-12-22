#ifndef _SEGMENT_TREE_HPP
#define _SEGMENT_TREE_HPP
#pragma once

#include <cstddef>
#include <list>
#include <algorithm>

using std::list, std::max, std::min;

struct segment {
        size_t lb, rb;
        size_t sum;

        segment() {this->lb = this->rb = this->sum = 0;}

        segment(size_t lb, size_t rb) {
                this->lb = min(lb, rb);
                this->rb = max(lb, rb);
        }
};

class segment_tree {
private:
        list<segment> sg_vec;
        void insert_rec(size_t lb, size_t lr);
public:
        segment_tree(size_t len, vector<size_t> arr_build);
};

#endif /* _SEGMENT_TREE_HPP */
