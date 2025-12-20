#include "../../kd-btree/kd_btree.hpp"
#include "movies_kd_btree.hpp"
#include <iostream>
#include <chrono>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#define DFIELDS 4
#define LFIELDS 2
#define IV_CNT 2
#define ALL_DIMS 6

using namespace std;
extern cmp_vector<movie> movie_comp;

int main(int argc, char *argv[]) {
        kd_btree<movie, movie_compare> movies_kdb(&movie_comp, make_movie_region_rectangle,
                make_movie_point_rectangle);
        size_t num_threads = 12;
        size_t rows = 10000;

        double dfields[IV_CNT][DFIELDS];
        size_t sfields[IV_CNT][LFIELDS];
        char max_min_str[ALL_DIMS][DFIELDS];

        //parsing the cmd arguments 1 is rows, 2-13 is movie-field pairs for range-query 14-19 are max min for skyline
        if (argc > 19) {
                rows = stol(argv[1]);
                size_t k = 2;
                for (size_t i = 0; i < DFIELDS; ++i) {
                        for (size_t j = 0; j < IV_CNT; ++j)
                                dfields[j][i] = stod(argv[i+k+j]);
                        k++;
                }
                k = 10;
                for (size_t i = 0; i < LFIELDS; ++i) {
                        for (size_t j = 0; j < IV_CNT; ++j)
                                sfields[j][i] = stol(argv[i+k+j]);
                        k++;
                }
                for (size_t j = 0; j < ALL_DIMS; ++j)
                        strcpy(max_min_str[j], argv[j+14]);

        }
        else {
                cout<<"wrong argument format\n";
                return 0;
        }

        //applying basic operations and counting time

        auto ins_t0 = chrono::system_clock::now();
        read_csv(movies_kdb, num_threads, rows);
        auto ins_t1 = chrono::system_clock::now();

        chrono::duration<double> ins_dur = ins_t1 - ins_t0;
        double ins_time = ins_dur.count();

        movie mv1 = create_default_movie();
        movie mv2 = create_default_movie();

        assign_movie(dfields[0], sfields[0], mv1);
        assign_movie(dfields[1], sfields[1], mv2);

        pair<movie, movie> mv_interval = make_pair(mv1, mv2);

        auto search_t0 = chrono::system_clock::now();
        vector<movie> mv_in = movies_kdb.range_query(mv_interval);
        auto search_t1 = chrono::system_clock::now();

        chrono::duration<double> search_dur = search_t1 - search_t0;

        double search_time = search_dur.count();

        vector<best_t> bests;
        for (size_t j = 0; j < ALL_DIMS; ++j) {
                if (strcmp(max_min_str[j], "max"))
                        bests.push_back(MAXIMIZE);
                else
                        bests.push_back(MINIMIZE);
        }

        auto sky_t0 = chrono::system_clock::now();
        set<movie, movie_compare> skyline_movies = movies_kdb.skyline(bests);
        auto sky_t1 = chrono::system_clock::now();

        chrono::duration<double> sky_dur = sky_t1 - sky_t0;

        double sky_time = sky_dur.count();

        //printing results

        cout<<"interval-query results\n";

        size_t search_len = mv_in.size();

        for (size_t j = 0; j < search_len; ++j)
                cout<<mv_in[j].print_interesting()<<"\n";

        cout<<"skyline results\n";

        size_t sky_len = skyline_movies.size();

        for (auto it = skyline_movies.begin(); it != skyline_movies.end(); ++it) {
                movie mv = *it;
                cout<<mv.print_interesting()<<"\n";
        }

        cout<<"interval_length = "<<search_len<<" skyline_length = "<<sky_len<<"\n";

        //results to csv file

        ofstream outcsv;
        outcsv.open("times.csv", ios::app);

        outcsv<<rows<<","<<ins_time<<","<<search_time<<","<<sky_time<<"\n";
        outcsv.close();
}
