#ifndef _MOVIES_KD_BTREE_HPP
#define _MOVIES_KD_BTREE_HPP

#include "rapidcsv.h"
#include "kd_btree.hpp"
#include "../movie_utils.hpp"

static inline void set_movie_str(vector<string> &str_fields, movie &mv) {
        mv.title = str_fields[0];
        mv.org_lang = str_fields[1];
        mv.org_country = str_fields[2];
        mv.genre_names = str_fields[3];
        mv.prod_comp_names = str_fields[4];
}

static inline void set_movie_dec(vector<size_t> &dec_fields, movie &mv) {
        mv.id = dec_fields[0];
        mv.runtime = dec_fields[1];
        mv.vote_count = dec_fields[2];
}

static inline void set_movie_double(vector<double> &double_fields, movie &mv) {
        mv.budget = double_fields[0];
        mv.revenue = double_fields[1];
        mv.popularity = double_fields[2];
        mv.vote_avg = double_fields[3];
}

static inline void set_movie_date(year_month_day &ymda, movie &mv) {
        mv.release_date = ymda;
}

vector<string> get_min_strings_regions(vector<rectangle<movie> *> &movie_regions);

vector<string> get_max_strings_regions(vector<rectangle<movie> *> &movie_regions);

rectangle<movie> make_movie_region_rectangle(vector<rectangle<movie> *> &movie_regions);

rectangle<movie> make_movie_point_rectangle(vector<movie *> &movie_points);


#endif /* _MOVIES_KD_BTREE_HPP */
