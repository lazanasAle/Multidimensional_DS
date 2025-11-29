#ifndef _KD_BTREE_HPP
#define _KD_BTREE_HPP

#include <functional>
#include <algorithm>
#include <typeinfo>
#include <utility>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <tuple>
#include <cmath>
#include <ios>

#define INV_OFF -1
#define BLC_LEN 4096
#define MIN_PERC 0.35

using   std::fstream, std::ios, std::string, std::vector,
        std::function, std::pair, std::tuple, std::random_device,
        std::mt19937, std::uniform_int_distribution, std::stable_sort, std::get,
        std::move, std::abs, std::min_element, std::distance, std::make_pair, std::make_tuple;

template<typename T>

void read_vector(fstream file, vector<T> &vec);

template<typename T>

void write_vector(fstream file, vector<T> &vec);

string random_string(size_t length);

template<typename T>

using cmp_vector = vector<function<double (T &, T &)>>;

template<typename T>

using rectangle = tuple<T, T, T>;

enum max_min {
        MAX,
        MIN
};

enum tag {
        POINT,
        REGION
};

template<typename T>

class point {
public:
        string name;
        T location;

        point(T p);
};

template<typename T>

bool into_rectangle(pair<T, T> &rect, point<T> &point, cmp_vector<T> *cmp_vec);

template<typename T>

double rect_area(rectangle<T> &r, cmp_vector<T> *cmp_vec);

template<typename T>

class region {
public:
        rectangle<T> region;
        long child_offset;

        region(rectangle<T> &reg);
};

template<typename T>

bool into_rectangle(pair<T, T> &rect, region<T> &reg, cmp_vector<T> *cmp_vec);

template<typename T>

class kd_bnode {
public:
        size_t minimum_fill, maximum_fill, level, dim_len;
        long parent_offset, my_offset;
        cmp_vector<T> *comparators;

        kd_bnode(cmp_vector<T> *cmp_vec);
        virtual kd_bnode<T> *split_node() = 0;
};

template<typename T>

class region_kd_bnode: public kd_bnode<T> {
public:
        vector<region<T>> regions;
        function<rectangle<T> (vector<rectangle<T> *> &)> make_rectangle;

        region_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> make_rectangle_fn);
        kd_bnode<T> *split_node() override;
};

template<typename T>

class point_kd_bnode: public kd_bnode<T> {
public:
        vector<point<T>> points;
        function<rectangle<T> (vector<T *> &)> make_rectangle;

        point_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<T *> &)> make_rectangle_fn);
        kd_bnode<T> *split_node() override;
};

template<typename T>

class kd_btree {
private:
        cmp_vector<T> *comparators;
        function<rectangle<T> (vector<rectangle<T> *> &)> make_region_rectangle;
        function<rectangle<T> (vector<T *> &)> make_point_rectangle;
        fstream file;
        long coffset, root_offset, next_offset;

        kd_bnode<T> *load_node(size_t node_offset);
        void update_node_level(kd_bnode<T> &node);
        bool store_node(size_t node_offset, kd_bnode<T> *node);
        void range_query_rec(pair<T, T> &rect, vector<T> &vec, long subtree_root_off);
        point_kd_bnode<T> *choose_leaf(T &data, long subtree_root_off);
        void propagate_split(kd_bnode<T> *org_node, kd_bnode<T> *split_org_node);
        void insert_rec(T &data, long subtree_root_off);
        void skyline_rec(vector<max_min> &best, vector<T> &vec, long subtree_root_off);
public:
        kd_btree(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
                function<rectangle<T> (vector<T *> &)> point_rectangle_fn);
        void insert(T &data);
        vector<T> range_query(pair<T, T> &rect);
        void erase(T &data);
        vector<T> skyline(vector<max_min> &best);
        bool empty();
};

#include "kd_btree.impl.hpp"

#endif /* _KD_BTREE_HPP */
