#include "movies_kd_btree.hpp"
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std;
extern cmp_vector<movie> movie_comp;

int main(int argc, char *argv[]) {
        kd_btree<movie, movie_compare> movies_kdb(&movie_comp, make_movie_region_rectangle,
                make_movie_point_rectangle);
        size_t num_threads = 12;
        size_t rows = 10000;

        if (argc > 2) {
                rows = stol(argv[1]);
                num_threads = stol(argv[2]);
        }
        else if (argc > 1)
                rows = stol(argv[1]);

        auto t0 = chrono::system_clock::now();
        read_csv(movies_kdb, num_threads, rows);
        auto t1 = chrono::system_clock::now();

        chrono::duration<double> d = t1 - t0;
        double time_used = d.count();

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

        //writing the ime used to a csv file for benchmarking

        string txt = to_string(rows) + "," + to_string(time_used);

        ofstream csv_file("times.csv", ios::app);
        csv_file<<txt;
        csv_file.close();
}
