#include <iostream>
#include <vector>
#include <chrono>
#include "rtree.hpp"

using namespace std;
using namespace std::chrono;

rectangle<SpatioTemporalPoint> make_point_rect(vector<SpatioTemporalPoint*> &points)
{
    if(points.empty())
    {
        return make_tuple(SpatioTemporalPoint(), SpatioTemporalPoint(), SpatioTemporalPoint());
    }


//return
}


int main()
{
    auto cmp_x=[](const SpatioTemporalPoint &a, const SpatioTemporalPoint &b)
    {
        return a.x-b.x;
    };

    auto cmp_y=[](const SpatioTemporalPoint &a, const SpatioTemporalPoint &b)
    {
        return a.y-b.y;
    };

    auto cmp_t=[](const SpatioTemporalPoint &a, const SpatioTemporalPoint &b)
    {
        return a.t-b.t;
    };


    cmp_vector<SpatioTemporalPoint> comparators = {cmp_x, cmp_y, cmp_t};

    return 0;

}

//will make a makefile in next commit
//g++ -std=c++17 -O2 -I../3d-rtree\(trajectories)\ test_rtree.cpp -o test_rtree

//g++ -std=c++17 -O2 -I../3d-rtree\(trajectories\) test_rtree.cpp -o test_rtree


//will add commenting soon