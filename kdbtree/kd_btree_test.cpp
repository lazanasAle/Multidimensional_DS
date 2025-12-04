#include "movies_kd_btree.hpp"
#include <iostream>

using namespace std;
extern cmp_vector<movie> movie_comp;

int main() {
        kd_btree<movie> movies_kdb(&movie_comp, make_movie_region_rectangle,
                make_movie_point_rectangle);
        read_csv(movies_kdb);

        //check if a certain movie is in
        movie mv;
        mv.id = 195554; strcpy(mv.title, "Panorama of Galveston Power House");
        mv.adult = false; strcpy(mv.org_lang, "en");
        strcpy(mv.org_country, "['US']"); mv.release_date = year_month_day{year{1900}, month{5}, day{21}};
        strcpy(mv.genre_names, "[]"); strcpy(mv.prod_comp_names, "[]");
        mv.budget = 1486.88423469876; mv.revenue = 1605.98597870523; mv.runtime = 2;
        mv.popularity = 3.4255; mv.vote_avg = 6.1; mv.vote_count = 55;

        pair<movie, movie> mv_pair = make_pair(mv, mv);
        mv_pair.second.budget += 1500; mv_pair.second.revenue += 1500; mv_pair.second.runtime +=4 ;
        mv_pair.second.popularity += 3; mv_pair.second.vote_avg += 9; mv_pair.second.vote_count += 9;
        vector<movie> mv_vec = movies_kdb.range_query(mv_pair);
        cout<<mv_vec.size()<<"\n";
        cout<<movies_kdb.n_items()<<"\n";
}
