#include "kd_btree.hpp"
#include "movies_kd_btree.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <sstream>
#include <string>

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

        //insertion_time measuring
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

        cout<<"the dimensions you can use for the skyline are: budget, revenue, popularity, runtime, vote_avg, vote_count."<<
                "use them with the same order and comma-separate the words minimize and maximize\n";
        cout<<"eg: If you want to minimize the budget, maximize the revenue and popularity minimize runtime and maximize vote_avg and vote_count you write:"<<
                "MINIMIZE,MAXIMIZE,MAXIMIZE,MINIMIZE,MAXIMIZE, MAXIMIZE\n";
        string input;
        getline(cin, input);

        stringstream is(input);
        string item;
        vector<best_t> mbest;

        while (getline(is, item, ',')) {
                best_t best_entry;
                transform(item.begin(), item.end(), item.begin(), ::toupper);
                if (item.compare("MAXIMIZE") == 0)
                        best_entry = MAXIMIZE;
                else
                        best_entry = MINIMIZE;
                mbest.push_back(best_entry);
        }

        //skyline query time measure
        auto t2 = chrono::system_clock::now();
        set<movie, movie_compare> skyline = movies_kdb.skyline(mbest);
        auto t3 = chrono::system_clock::now();

        chrono::duration<double> durat = t3 - t2;
        double skyline_time = durat.count();

        cout<<"skyline_length:"<<skyline.size()<<"\n";

        for (auto movie_it = skyline.begin(); movie_it != skyline.end(); ++movie_it) {
                movie mv = *movie_it;
                cout<<" movie is: "<<mv.print_interesting()<<"\n";
        }

        //writing the time used to a csv file for benchmarking

        string txt = to_string(rows) + "," + to_string(time_used) + "," + to_string(skyline_time);

        ofstream csv_file("times.csv", ios::app);
        csv_file<<txt;
        csv_file.close();
}
