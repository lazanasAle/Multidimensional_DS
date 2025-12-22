#ifndef _SEGMENT_TREE_HPP
#define _SEGMENT_TREE_HPP
#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>
#include <string>


using   std::vector, std::max, std::min, std::min_element, std::max_element, std::reverse,
        std::string, std::to_string;

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
        vector<segment> sg_vec;
        size_t build_rec(vector<size_t> &arr_build, size_t left, size_t right);
public:
        segment_tree(vector<size_t> &arr_build);
        segment_tree() {}
        void build(vector<size_t> &arr_build);
        string stringnify_tree();
};

#endif /* _SEGMENT_TREE_HPP */
