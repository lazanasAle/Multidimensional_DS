#ifndef _SEGMENT_TREE_HPP
#define _SEGMENT_TREE_HPP
#pragma once

#include <cstddef>
#include <vector>
#include <algorithm>
#include <string>
#include <climits>
#include <cstdint>
#include <numeric>


using   std::vector, std::max, std::min, std::min_element, std::max_element, std::accumulate,
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
        void build_rec(vector<size_t> &arr_build, size_t node, size_t left, size_t right);
        void add_rec(size_t place, size_t val, size_t idx);
        size_t sum_rec(size_t left, size_t right, size_t idx);
public:
        segment_tree(vector<size_t> &arr_build);
        segment_tree() {}
        void build(vector<size_t> &arr_build);
        void add(size_t place, size_t val);
        size_t sum(size_t left, size_t right);
        string stringnify_tree();
};

#endif /* _SEGMENT_TREE_HPP */
