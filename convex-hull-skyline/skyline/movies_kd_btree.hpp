#ifndef _MOVIES_KD_BTREE_HPP
#define _MOVIES_KD_BTREE_HPP

#include "rapidcsv.h"
#include "../../kd-btree/kd_btree.hpp"
#include "../movie_utils.hpp"
#include <cstdint>

using std::uint64_t;

struct movie_compare {
        bool operator ()(const movie &m1, const movie &m2) const {
                return (m1.id < m2.id);
        }
};

rectangle<movie> make_movie_region_rectangle(vector<rectangle<movie> *> &movie_regions);

rectangle<movie> make_movie_point_rectangle(vector<movie *> &movie_points);

double read_csv(kd_btree<movie, movie_compare> &movies_kdb, size_t num_threads, size_t rows);

#endif /* _MOVIES_KD_BTREE_HPP */
