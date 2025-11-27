#ifndef _KD_BTREE_HPP
#define _KD_BTREE_HPP

#include <functional>
#include <typeinfo>
#include <utility>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <cmath>
#include <ios>

#define INV_OFF -1
#define BLC_LEN 4096
#define MIN_PERC 0.35

using std::fstream, std::ios, std::string, std::vector, std::function, std::pair, std::random_device, std::mt19937, std::uniform_int_distribution;

string random_string(size_t length);

enum max_min {
        MAX,
        MIN
};

template<typename T>

class point {
public:
        string name;
        T location;
        long parent_offset;

        point(T p);
};

template<typename T>

class region {
public:
        pair<T, T> region;
        long child_offset, parent_offset;

        region(pair<T, T> &reg);
};

template<typename T>

class kd_bnode {
public:
        size_t minimum_fill, maximum_fill, level;
        function<pair<T, T> (vector<T> &)> make_rectangle;
        vector<function<int (T &, T &)>> comparators;

        kd_bnode(function<pair<T, T> (vector<T> &)> make_rect, vector<function<int (T &, T &)>> &cmp_vec);
        virtual pair<kd_bnode<T> *, kd_bnode<T> *> split_node() = 0;
};

template<typename T>

class region_kd_bnode: public kd_bnode<T> {
public:
        vector<region<T>> regions;
        pair<kd_bnode<T> *, kd_bnode<T> *> split_node() override;
};

template<typename T>

class point_kd_bnode: public kd_bnode<T> {
public:
        vector<point<T>> points;
        pair<kd_bnode<T> *, kd_bnode<T> *> split_node() override;
};

template<typename T>

class kd_btree {
private:
        fstream file;
        long coffset, root_offset;
        kd_bnode<T> *load_node(size_t node_offset);
        void range_query_rec(pair<T, T> &rect, vector<T> &vec);
        void skyline_rec(vector<max_min> &best, vector<T> &vec);
public:
        kd_btree(vector<function<int (T &, T &)>> &comparators, function<pair<T, T> (vector<T> &)> rect_fn);
        void insert(T &data);
        vector<T> range_query(pair<T, T> &rect);
        void erase(T &data);
        vector<T> skyline(vector<max_min> &best);
};

#include "kd_btree.impl.hpp"

#endif /* _KD_BTREE_HPP */
