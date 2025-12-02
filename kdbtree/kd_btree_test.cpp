#include "movies_kd_btree.hpp"
#include <iostream>

using namespace std;
extern cmp_vector<movie> movie_comp;

int main() {
        kd_btree<movie> movies_kdb(&movie_comp, make_movie_region_rectangle,
                make_movie_point_rectangle);
        read_csv(movies_kdb);

        movie mv;

        mv.id = 195554;
        strcpy(mv.title, "Panorama of Galveston Power House");
        mv.adult = false;
        strcpy(mv.org_lang, "en");
        strcpy(mv.org_country, "['US']");
        mv.release_date = std::chrono::year_month_day{std::chrono::year{1900}, std::chrono::month{5}, std::chrono::day{21}};
        strcpy(mv.genre_names, "[]");
        strcpy(mv.prod_comp_names, "[]");
        mv.budget = 2426.7031427901;
        mv.revenue = 3322.06597655286;
        mv.runtime = 1;
        mv.popularity = 6.3707;
        mv.vote_avg = 4;
        mv.vote_count = 4;

        pair<movie, movie> tst_range = make_pair(mv, mv);
        vector<movie> exact = movies_kdb.range_query(tst_range);

        cout<<exact.size()<<"\n";
}
