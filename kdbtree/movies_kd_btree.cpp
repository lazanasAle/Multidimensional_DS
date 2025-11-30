#include "movies_kd_btree.hpp"


cmp_vector<movie> movie_comp = {
        compare_release_dates, compare_budget, compare_revenue,
        compare_runtime, compare_popularity, compare_vote_avg,
        compare_vote_count
};


rectangle<movie> make_movie_region_rectangle(vector<rectangle<movie> *> &movie_regions) {
        movie minimum, median, maximum;

        return make_tuple(minimum, median, maximum);
}

rectangle<movie> make_movie_point_rectangle(vector<movie *> &movie_points) {
        movie minimum, median, maximum;
        return make_tuple(minimum, median, maximum);
}
