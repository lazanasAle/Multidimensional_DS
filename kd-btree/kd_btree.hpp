#ifndef _KD_BTREE_HPP
#define _KD_BTREE_HPP

#include <functional>
#include <algorithm>
#include <typeinfo>
#include <utility>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <set>
#include <map>
#include <tuple>
#include <cmath>
#include <stack>
#include <cstring>
#include <iterator>
#include <ios>

#define INV_OFF -1
#define BLC_LEN 4096
#define MIN_PERC 0.35
#define EIGHT 8
#define MAX_CACHED 512

using   std::fstream, std::ios, std::string, std::vector,
        std::function, std::pair, std::tuple, std::stable_sort, std::get,
        std::move, std::abs, std::min_element, std::distance, std::make_pair, std::make_tuple,
        std::binary_search, std::strncpy, std::find, std::stack, std::set, std::map, std::prev;


static inline void null_func(){}

enum best_t: bool {MINIMIZE, MAXIMIZE};

template <typename T>

void read_vector(fstream &file, vector<T> &vec, size_t it_in_blc);

template <typename T>

void write_vector(fstream &file, vector<T> &vec, size_t it_in_blc);

template <typename T>

using cmp_vector = vector<function<double (const T &, const T &)>>;

template <typename T>

using rectangle = tuple<T, T, T>;

template <typename T>

void write_rectangle(rectangle<T> &rect, fstream &file);

template <typename T>

void read_rectangle(rectangle<T> &rect, fstream &file);

template <typename T>

class point {
public:
        T location;

        point(T p);
        point() {}
        void write(fstream &file);
        void read(fstream &file);
};

template <typename T>

bool dominates(point<T> &p1, point<T> &p2, cmp_vector<T> *cmp_vec, vector<best_t> &best);

template <typename T>

bool into_rectangle(pair<T, T> &rect, point<T> &point, cmp_vector<T> *cmp_vec);

template <typename T>

double rect_area(rectangle<T> &r, cmp_vector<T> *cmp_vec);

template <typename T>

class region {
public:
        rectangle<T> region_rec;
        long child_offset;

        region(rectangle<T> reg);
        region() {}
        void write(fstream &file);
        void read(fstream &file);
};

template <typename T>

struct region_comp {
        bool operator ()(const region<T> &r1, const region<T> &r2) const {
                return (r1.child_offset < r2.child_offset);
        }
};

template <typename T>

bool dominates(region<T> &r1, region<T> &r2, cmp_vector<T> *cmp_vec, vector<best_t> &best);

template <typename T>

bool into_rectangle(pair<T, T> &rect, region<T> &reg, cmp_vector<T> *cmp_vec);

template <typename T>

class kd_bnode {
public:
        size_t minimum_fill, maximum_fill, level, dim_len;
        long parent_offset, my_offset;
        cmp_vector<T> *comparators;
        bool tag;

        kd_bnode(cmp_vector<T> *cmp_vec);
        virtual kd_bnode<T> *split_node() = 0;
        void read_common(fstream &file, long node_offset);
        virtual ~kd_bnode() {}
};

template <typename T>

class region_kd_bnode: public kd_bnode<T> {
public:
        vector<region<T>> regions;
        function<rectangle<T> (vector<rectangle<T> *> &)> make_rectangle;

        region_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> make_rectangle_fn);
        kd_bnode<T> *split_node() override;
};

template <typename T>

class point_kd_bnode: public kd_bnode<T> {
public:
        vector<point<T>> points;
        function<rectangle<T> (vector<T *> &)> make_rectangle;

        point_kd_bnode(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<T *> &)> make_rectangle_fn);
        kd_bnode<T> *split_node() override;
};

template <typename T, typename C>

class kd_btree {
private:
        cmp_vector<T> *comparators;
        function<rectangle<T> (vector<rectangle<T> *> &)> make_region_rectangle;
        function<rectangle<T> (vector<T *> &)> make_point_rectangle;
        fstream file;
        long coffset, next_offset, root_offset;
        size_t nitems, max_cached_pnodes, max_cached_rnodes;

        map<long, point_kd_bnode<T>> pnodes_cached;
        map<long, region_kd_bnode<T>> rnodes_cached;
        stack<pair<long, bool>> eliminated_stack;

        kd_bnode<T> *load_file_node(long node_offset);
        void update_node_level(kd_bnode<T> *node);
        bool store_file_node(long node_offset, kd_bnode<T> *node);

        void evict_point_cache();
        void evict_region_cache();
        kd_bnode<T> *load_node(long node_offset);
        bool store_node(long node_offset, kd_bnode<T> *node);

        point_kd_bnode<T> *choose_leaf(T &data, long subtree_root_off);
        void propagate_split(kd_bnode<T> *org_node, kd_bnode<T> *split_org_node);

        region<T> make_parent_region(kd_bnode<T> *node);
        void update_chld_levels(region_kd_bnode<T> *node);
        void make_and_store_parent(region<T> &org_parent, region<T> &splitted_par,
                kd_bnode<T> *org_node, kd_bnode<T> *splitted_node);
        void assign_new_region(region_kd_bnode<T> *par_node, rectangle<T> &new_region_rect, long searching_off);
        void store_neighbour_after_split(region_kd_bnode<T> *neighbour, region_kd_bnode<T> *par_node,
                size_t dlen, region<T> &splitted_parent, kd_bnode<T> *splitted_node);

        void skyline_update(vector<best_t> &best, set<T, C> &skyline_set, point<T> &p);
        void skyline_region_update(vector<best_t> &best, set<region<T>, region_comp<T>> &skyline_regs, region<T> &r);

        void eliminate_node(kd_bnode<T> *node, region_kd_bnode<T> *parent, vector<T> &eliminated_data);
        void eliminate_root(kd_bnode<T> *root_node);

        void insert_rec(T &data, long subtree_root_off);
        void skyline_rec(vector<best_t> &best, set<T, C> &skyline_set, long subtree_root_off);
        void range_query_rec(pair<T, T> &rect, vector<T> &vec, long subtree_root_off);
public:
        kd_btree(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
                function<rectangle<T> (vector<T *> &)> point_rectangle_fn);
        void insert(T &data);
        vector<T> range_query(pair<T, T> &rect);
        void erase(T &data);
        set<T, C> skyline(vector<best_t> &best);
        bool empty();
        size_t n_items() {return this->nitems;}
        ~kd_btree() {this->file.close();}
};

#include "kd_btree.impl.hpp"

#endif /* _KD_BTREE_HPP */
