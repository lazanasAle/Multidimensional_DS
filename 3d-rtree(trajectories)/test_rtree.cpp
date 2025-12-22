#include <iostream>
#include <vector>
#include <chrono>
#include "rtree.hpp"

using namespace std;
using namespace std::chrono;

//creating rectangles

rectangle<SpatioTemporalPoint> make_point_rect(vector<SpatioTemporalPoint*> &points)
{
    if(points.empty())
    {
        return make_tuple(SpatioTemporalPoint(), SpatioTemporalPoint(), SpatioTemporalPoint());
    }

    double min_x=points[0]->x, max_x=points[0]->x;
    double min_y=points[0]->y, max_y=points[0]->y;
    double min_t=points[0]->t, max_t=points[0]->t;

    for(auto p: points)
    {
        min_x=min(min_x, p->x);
        max_x=max(max_x, p->x);
        min_y= min(min_y, p->y);
        max_y=max(max_y, p->y);
        min_t=min(min_t, p->t);
        max_t=max(max_t, p->t);


    }

    return make_tuple(
        SpatioTemporalPoint(min_x,min_y,min_t),
        SpatioTemporalPoint((min_x+max_x)/2, (min_y+max_y)/2, (min_t+max_t)/2),
        SpatioTemporalPoint(max_x, max_y, max_t)
    );

}

rectangle<SpatioTemporalPoint> make_region_rect(vector<rectangle<SpatioTemporalPoint>*> &rectangles)
{
    if(rectangles.empty())
    {
        return make_tuple(SpatioTemporalPoint(), SpatioTemporalPoint(), SpatioTemporalPoint());
    }

    double min_x=get<0>(*rectangles[0]).x, max_x=get<2>(*rectangles[0]).x;
    double min_y=get<0>(*rectangles[0]).y, max_y=get<2>(*rectangles[0]).y;
    double min_t=get<0>(*rectangles[0]).t, max_t=get<2>(*rectangles[0]).t;
    

    for(auto r: rectangles)
    {
        min_x=min(min_x, get<0>(*r).x);
        max_x=max(max_x, get<2>(*r).x);
        min_y= min(min_y, get<0>(*r).y);
        max_y=max(max_y, get<2>(*r).y);
        min_t=min(min_t, get<0>(*r).t);
        max_t=max(max_t, get<2>(*r).t);
    }

    return make_tuple(
        SpatioTemporalPoint(min_x,min_y,min_t),
        SpatioTemporalPoint((min_x+max_x)/2, (min_y+max_y)/2, (min_t+max_t)/2),
        SpatioTemporalPoint(max_x, max_y, max_t)
    );

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
    
    //Create the 3d R-tree
    rtree<SpatioTemporalPoint, STP_comparator> tree(&comparators,make_region_rect,make_point_rect);
    cout<< "3D R-Tree for Spatio-temporal Queries" <<endl;
    cout<< "Inserting trajectory points..." << endl;


    //TEST DATA TO CHECK IF IT WORKS OK
    vector<SpatioTemporalPoint> trajectory=
    {
        {10.5,20.3,100.0}, //vehicle at position 10.5,20.3 at time=100
        {15.2,25.1,150.0},
        {18.7,22.5,200.0},
        {12.3,28.9,250.0},
        {20.1,30.5,300.0}

    };

    auto start=high_resolution_clock::now();
    for(auto &point: trajectory)
    {
        tree.insert(point);
    }

    auto end=high_resolution_clock::now();
    auto duration=duration_cast<microseconds>(end-start);

    cout <<"Inserted "<< tree.n_items()<<" points in "<< duration.count()<< " microseconds"<< endl;
    cout<< "\nSpatio-temporal Range Query"<< endl;
    cout <<"Query: Find vehicles in area [10,20]x[20,30] during time [100,250]"<< endl; 
    //range query, during time 100<=t<=250

    SpatioTemporalPoint lower(10.0,20.0,100.0);
    SpatioTemporalPoint upper(20.0,30.0,250.0);

    pair<SpatioTemporalPoint, SpatioTemporalPoint> range_query={lower,upper};
    start=high_resolution_clock::now();
    vector<SpatioTemporalPoint> results=tree.range_query(range_query);
    end =high_resolution_clock::now();

    duration=duration_cast<microseconds>(end-start);
    cout<< "Found "<<results.size()<< " points in "<< duration.count()<< " microseconds" <<endl;

    cout<< "\nResults:"<< endl;
    for(auto &p: results)
    {
        cout<< "Position ("<< p.x<< ","<<p.y<< ") at time "<<p.t<< endl;
    }
    return 0;

}

//will make a makefile in next commit

//g++ -std=c++17 -O2 -I../3d-rtree\(trajectories\) test_rtree.cpp -o test_rtree

//will add commenting soon