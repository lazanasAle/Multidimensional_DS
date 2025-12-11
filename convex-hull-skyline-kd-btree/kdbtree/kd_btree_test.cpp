#include "movies_kd_btree.hpp"
#include <iostream>

using namespace std;
extern cmp_vector<movie> movie_comp;

int main(int argc, char *argv[]) {
        kd_btree<movie, movie_compare> movies_kdb(&movie_comp, make_movie_region_rectangle,
                make_movie_point_rectangle);
        size_t num_threads = 12;
        if (argc > 1)
                num_threads = stol(argv[1]);

        read_csv(movies_kdb, num_threads);

        //check if a certain movie is in
        movie mv;
        mv.id = 195554; strcpy(mv.title, "Panorama of Galveston Power House");
        mv.adult = false; strcpy(mv.org_lang, "en");
        strcpy(mv.org_country, "['US']"); mv.release_date = date_t{1900, 5, 21};
        strcpy(mv.genre_names, "[]"); strcpy(mv.prod_comp_names, "[]");
        mv.budget = 1486.88423469876; mv.revenue = 1605.98597870523; mv.runtime = 2;
        mv.popularity = 3.4255; mv.vote_avg = 6.1; mv.vote_count = 55;

        pair<movie, movie> mv_pair = make_pair(mv, mv);
        vector<movie> mv_vec = movies_kdb.range_query(mv_pair);
        cout<<mv_vec.size()<<"\n";
        cout<<movies_kdb.n_items()<<"\n";
}
