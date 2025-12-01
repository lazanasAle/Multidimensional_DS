#ifndef _MOVIES_KD_BTREE_HPP
#define _MOVIES_KD_BTREE_HPP

#include "rapidcsv.h"
#include "kd_btree.hpp"
#include "../movie_utils.hpp"

rectangle<movie> make_movie_region_rectangle(vector<rectangle<movie> *> &movie_regions);

rectangle<movie> make_movie_point_rectangle(vector<movie *> &movie_points);


#endif /* _MOVIES_KD_BTREE_HPP */
