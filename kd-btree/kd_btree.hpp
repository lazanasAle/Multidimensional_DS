#ifndef _KD_BTREE_HPP
#define _KD_BTREE_HPP

#include <cstdint>
#include <algorithm>
#include <utility>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <set>
#include <unordered_map>
#include <tuple>
#include <cmath>
#include <stack>
#include <cstring>
#include <iterator>
#include <ios>

#define INV_OFF -1
#define BLC_LEN 4096
#define MIN_PERC 0.35
#define MAX_CACHED 512

#define HASH_SHIFT_F 30
#define HASH_SHIFT_S 27
#define HASH_SHIFT_T 31

#define HASH_MUL_F UINT64_C(0xbf58476d1ce4e5b9)
#define HASH_MUL_S UINT64_C(0x94d049bb133111eb)

using   std::fstream, std::ios, std::string, std::vector,
        std::pair, std::tuple, std::stable_sort, std::get,
        std::move, std::abs, std::min_element, std::distance, std::make_pair, std::make_tuple,
        std::find_if, std::stack, std::set, std::unordered_map;


static inline void null_func(){}

enum best_t: bool {MINIMIZE, MAXIMIZE};

template <typename T>

void read_vector(fstream &file, vector<T> &vec, size_t it_in_blc);

template <typename T>

void write_vector(fstream &file, vector<T> &vec, size_t it_in_blc);

template <typename T>

using cmp_vector = vector<double (*)(const T &, const T &)>;

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

        point(T &p);
        point() {}
        void write(fstream &file);
        void read(fstream &file);
};

template <typename T>

bool dominates(const point<T> &p1, const point<T> &p2, cmp_vector<T> *cmp_vec, vector<best_t> &best);

template <typename T>

bool into_rectangle(pair<T, T> &rect, point<T> &point, cmp_vector<T> *cmp_vec);

template <typename T>

double rect_area(rectangle<T> &r, cmp_vector<T> *cmp_vec);

template <typename T>

class region {
public:
        rectangle<T> region_rec;
        long child_offset;

        region(rectangle<T> &reg);
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

bool dominates(const region<T> &r1, const region<T> &r2, cmp_vector<T> *cmp_vec, vector<best_t> &best);

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
        rectangle<T>  (*make_rectangle)(vector<rectangle<T> *> &);

        region_kd_bnode(cmp_vector<T> *cmp_vec, rectangle<T>  (*make_rectangle_fn)(vector<rectangle<T> *> &));
        kd_bnode<T> *split_node() override;
};

template <typename T>

class point_kd_bnode: public kd_bnode<T> {
public:
        vector<point<T>> points;
        rectangle<T>  (*make_rectangle)(vector<T *> &);

        point_kd_bnode(cmp_vector<T> *cmp_vec, rectangle<T>  (*make_rectangle_fn)(vector<T *> &));
        kd_bnode<T> *split_node() override;
};


static inline uint64_t split_mix_hash(uint64_t x) {
        x = (x ^ (x >> HASH_SHIFT_F)) * HASH_MUL_F;
        x = (x ^ (x >> HASH_SHIFT_S)) * HASH_MUL_S;
        x ^= (x >> HASH_SHIFT_T);
        return x;
}


struct split_mix_hasher {
        size_t operator ()(const long &key) const {
                return split_mix_hash(static_cast<uint64_t>(key));
        }
};

template <typename T>

using point_hash_map = unordered_map<
        long,
        point_kd_bnode<T>,
        split_mix_hasher
>;

template <typename T>

using region_hash_map = unordered_map<
        long,
        region_kd_bnode<T>,
        split_mix_hasher
>;

using size_hash_map = unordered_map<
        long,
        size_t,
        split_mix_hasher
>;


// r-tree (1st question must) inherit this class
template <typename T, typename C>

class kd_btree {
protected:
        cmp_vector<T> *comparators;
        rectangle<T> (*make_region_rectangle)(vector<rectangle<T> *> &);
        rectangle<T> (*make_point_rectangle)(vector<T *> &);
        fstream file;
        long coffset, root_offset;
        size_t nitems, max_cached_pnodes, max_cached_rnodes, recent_cnt;

        point_hash_map<T> pnodes_cached;
        region_hash_map<T> rnodes_cached;
        size_hash_map off_cnt_points;
        size_hash_map off_cnt_regions;
        stack<pair<long, bool>> eliminated_stack;

        kd_bnode<T> *load_file_node(long node_offset);
        void update_node_level(kd_bnode<T> *node);
        bool store_file_node(long node_offset, kd_bnode<T> *node);

        long next_pos(bool node_to_store);

        void evict_point_cache();
        void evict_region_cache();
        kd_bnode<T> *load_node(long node_offset);
        bool store_node(long node_offset, kd_bnode<T> *node);

        virtual kd_bnode<T> *split_node(kd_bnode<T> *node) {return node->split_node();} //r-tree must overwrite this method with the quadratic split algoithm based on Guttman's paper

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
        void skyline_region_prune(vector<best_t> &best, set<region<T>, region_comp<T>> &skyline_regs, set<T, C> &skyline_set);

        void eliminate_node(kd_bnode<T> *node, region_kd_bnode<T> *parent, vector<T> &eliminated_data);
        void eliminate_root(kd_bnode<T> *root_node);

        void insert_rec(T &data, long subtree_root_off);
        void skyline_rec(vector<best_t> &best, set<T, C> &skyline_set, long subtree_root_off);
        void range_query_rec(pair<T, T> &rect, vector<T> &vec, long subtree_root_off);
public:
        kd_btree(cmp_vector<T> *cmp_vec, rectangle<T> (*region_rectangle_fn)(vector<rectangle<T> *> &),
                rectangle<T> (*point_rectangle_fn)(vector<T *> &));
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
