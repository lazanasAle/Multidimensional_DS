#ifndef _MOVIES_KD_BTREE_HPP
#define _MOVIES_KD_BTREE_HPP

#include "rapidcsv.h"
#include "kd_btree.hpp"
#include "../movie_utils.hpp"
#include <cstdint>

using std::uint64_t;

rectangle<movie> make_movie_region_rectangle(vector<rectangle<movie> *> &movie_regions);

rectangle<movie> make_movie_point_rectangle(vector<movie *> &movie_points);

void read_csv(kd_btree<movie> &movies_kdb, size_t num_threads);

#endif /* _MOVIES_KD_BTREE_HPP */
