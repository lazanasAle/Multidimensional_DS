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
}


int main()
{
    auto compare_x=[](const SpatioTemporalPoint &a, const SpatioTemporalPoint &b)
    {
        return a.x-b.x;
    };

    return 0;

}