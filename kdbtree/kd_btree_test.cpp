#include "movies_kd_btree.hpp"
#include <iostream>

using namespace std;
extern cmp_vector<movie> movie_comp;

int main() {
        kd_btree<movie> movies_kdb(&movie_comp, make_movie_region_rectangle,
                make_movie_point_rectangle);
        read_csv(movies_kdb);
}
