#include "segment_tree.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>


using namespace std;

int main(int argc, char *argv[]) {
        const size_t all_rows = 900000;
        size_t all_read = all_rows;
        size_t adding_idx = 4;
        size_t adding_key = 5;
        size_t left_bound = 2;
        size_t right_bound = 8;

        if (argc > 5) {
                all_read = stol(argv[1]);
                adding_idx = stol(argv[2]);
                adding_key = stol(argv[3]);
                left_bound = stol(argv[4]);
                right_bound = stol(argv[5]);
        }
        all_read = (all_read <= all_rows)? all_read : all_rows;

        ifstream txt_file;
        txt_file.open("dataset.txt");
        vector<size_t> build_arr;

        string num_str;
        //loop to read the file contents
        for (size_t j = 0; j < all_read; ++j){
                getline(txt_file, num_str);
                size_t num = stol(num_str);
                build_arr.push_back(num);
        }
        txt_file.close();
        //build the tree
        auto t0_build = chrono::system_clock::now();
        segment_tree stree(build_arr);
        auto t1_build = chrono::system_clock::now();

        chrono::duration<double> build_dur = t1_build - t0_build;
        double build_time = build_dur.count();

        auto t0_add = chrono::system_clock::now();
        stree.add(adding_idx, adding_key);
        auto t1_add = chrono::system_clock::now();

        chrono::duration<double> add_dur = t1_add - t0_add;
        double adding_time = add_dur.count();

        auto t0_sum = chrono::system_clock::now();
        size_t sum2_8 = stree.sum(left_bound, right_bound);
        auto t1_sum = chrono::system_clock::now();

        chrono::duration<double> sum_dur = t1_sum - t0_sum;
        double summing_time = sum_dur.count();

        cout<<stree.stringnify_tree()<<"\n";
        cout<<sum2_8<<"\n";

        ofstream times_file;
        times_file.open("times.csv", ios::app);

        if (times_file.is_open()) {
                times_file<<all_read<<","<<build_time<<","<<adding_time<<","<<summing_time<<"\n";
                times_file.close();
        }
}
