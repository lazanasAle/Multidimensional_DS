#include <kd-btree/kd_btree.hpp>
#include "movies_kd_btree.hpp"
#include <boost/json.hpp>
#include <boost/json/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/value.hpp>
#include <cstddef>
#include <iostream>
#include <chrono>
#include <cstring>
#include <string>
#include <vector>

#define DFIELDS 4
#define LFIELDS 2
#define IV_CNT 2
#define ALL_DIMS 6

extern cmp_vector<movie> movie_comp;
namespace json = boost::json;
using namespace std;

pair<movie, movie> range_query_routine(json::object &range_query_obj, vector<size_t> &idx_dims) {
        movie mv1 = create_default_movie();
        movie mv2 = create_default_movie();

        static double dfields[IV_CNT][DFIELDS];
        static size_t sfields[IV_CNT][LFIELDS];

        for (size_t &dim : idx_dims) {
                string key = to_string(dim);

                json::array range = range_query_obj.at(key).as_array();

                if (dim >= 0 && dim <= 3) { //the dimension is a double field so
                        dfields[0][dim] = range.at(0).as_double();
                        dfields[1][dim] = range.at(1).as_double();
                }
                else if (dim == 4) {
                        sfields[0][0] = static_cast<size_t>(range.at(0).as_int64());
                        sfields[1][0] = static_cast<size_t>(range.at(1).as_int64());
                }
                else {
                        sfields[0][1] = static_cast<size_t>(range.at(0).as_int64());
                        sfields[1][1] = static_cast<size_t>(range.at(1).as_int64());
                }
        }
        assign_movie(dfields[0], sfields[0], mv1);
        assign_movie(dfields[1], sfields[1], mv2);

        pair<movie, movie> mv_interval = make_pair(mv1, mv2);
        return mv_interval;
}

vector<best_t> skyline_query_routine(json::array &skyline_query_arr) {
        vector<best_t> best_dims;
        for (json::value const &v : skyline_query_arr) {
                best_t curr_best = (v.as_string().compare("max") == 0)? MAXIMIZE : MINIMIZE;
                best_dims.push_back(curr_best);
        }
        return best_dims;
}


int main(int argc, char *argv[]) {
        ifstream json_file;
        json_file.open("query.json");

        if (json_file.is_open()) { //parse the json and make the query
                json::value root = json::parse(json_file);
                json::object root_obj = root.as_object();

                //taking all fields of the json object
                json::array idx_dims_json = root_obj.at("index_dimensions").as_array();
                json::object range_query_obj = root_obj.at("range_query").as_object();
                json::array skyline_query_arr = root_obj.at("skyline_query").as_array();

                //finding the indexing dimensions
                vector<size_t> idx_dims;
                for (json::value const &v : idx_dims_json) {
                        size_t dim = static_cast<size_t>(v.as_int64());
                        idx_dims.push_back(dim);
                }
                //after finding them make your cmp_vector
                cmp_vector<movie> comps_used;
                for (size_t &dim : idx_dims)
                        comps_used.push_back(movie_comp[dim]);

                //so now comps_used is our cmp_vector. let's make the tree
                kd_btree<movie, movie_compare> movies_kdb(&comps_used, make_movie_region_rectangle,
                        make_movie_point_rectangle);

                size_t rows = stol(argv[1]); // how many data will we read
                size_t num_chunks = 12;
                read_csv(movies_kdb, num_chunks, rows); //inserting the data to the kd-btree

                //set the interval query pair
                pair<movie, movie> mv_interval = range_query_routine(range_query_obj, idx_dims);

                vector<movie> movies_in = movies_kdb.range_query(mv_interval);

                cout<<"inside the typed interval exist: "<<movies_in.size()<<" movies\n";

                for (movie &m : movies_in)
                        cout<<m.print_interesting()<<"\n";

                vector<best_t> skyline_best = skyline_query_routine(skyline_query_arr);

                set<movie, movie_compare> skyline_movies = movies_kdb.skyline(skyline_best);

                cout<<"You suggested a skyline in this skyline we have "<<skyline_movies.size()<<" movies\n";

                for (auto it = skyline_movies.begin(); it != skyline_movies.end(); ++it) {
                        movie mv = *it;
                        cout<<mv.print_interesting()<<"\n";
                }
        }
}
