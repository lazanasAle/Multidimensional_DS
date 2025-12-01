#include "movies_kd_btree.hpp"
#include <iostream>

using namespace std;
extern cmp_vector<movie> movie_comp;

int main() {
        kd_btree<movie> movies_kdb(&movie_comp, make_movie_region_rectangle,
                make_movie_point_rectangle);

        movie mv = {
                .title = "I am your father",
                .org_lang = "en",
                .org_country = "us",
                .genre_names = "drama",
                .prod_comp_names = "amazon prime",
                .budget = 10000,
                .revenue = 1000,
                .popularity = 1000,
                .vote_avg = 6.8,
                .runtime = 3,
                .vote_count = 11
        };

        movies_kdb.insert(mv);
        pair<movie, movie> mv_pair = {mv, mv};
        vector<movie> mn_se = movies_kdb.range_query(mv_pair);

        cout<<mn_se[0].title<<"\n";
}
