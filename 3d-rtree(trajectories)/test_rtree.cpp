#include <cstddef>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include "rtree.hpp"
#include <rapidcsv.h>

using namespace std;
//using namespace std::chrono;

//Testing R-tree Test Program for Aircraft trajectory queries. Reads flight data
// from the CSV and builds 3D Rtree. Then performs spatio-temporal range queries and reports performance metrics

//Creating Minimum bounding rectangle (MBR) from a vector of points. Then
// returns a tuple: (lower_bound, center,upper_bound)
rectangle<SpatioTemporalPoint> make_point_rect(vector<SpatioTemporalPoint*> &points)
{
    if(points.empty())
    {
        return make_tuple(SpatioTemporalPoint(), SpatioTemporalPoint(), SpatioTemporalPoint());
    }

    //initialise min/max with first point's coordinates
    double min_x=points[0]->x, max_x=points[0]->x;
    double min_y=points[0]->y, max_y=points[0]->y;
    double min_t=points[0]->t, max_t=points[0]->t;

    for(auto &p: points)
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

//creates MBR that encloses all given rectangles. It is used for internal Rtree nodes
rectangle<SpatioTemporalPoint> make_region_rect(vector<rectangle<SpatioTemporalPoint>*> &rectangles)
{
    if(rectangles.empty())
    {
        return make_tuple(SpatioTemporalPoint(), SpatioTemporalPoint(), SpatioTemporalPoint());
    }

    //initialise with first rectangle's bounds
    double min_x=get<0>(*rectangles[0]).x, max_x=get<2>(*rectangles[0]).x;
    double min_y=get<0>(*rectangles[0]).y, max_y=get<2>(*rectangles[0]).y;
    double min_t=get<0>(*rectangles[0]).t, max_t=get<2>(*rectangles[0]).t;

    //expand bounds to include all rectangles
    for(auto &r: rectangles)
    {
        min_x=min(min_x, get<0>(*r).x);
        max_x=max(max_x, get<2>(*r).x);
        min_y= min(min_y, get<0>(*r).y);
        max_y=max(max_y, get<2>(*r).y);
        min_t=min(min_t, get<0>(*r).t);
        max_t=max(max_t, get<2>(*r).t);
    }

    //return combined MBR
    return make_tuple(
        SpatioTemporalPoint(min_x,min_y,min_t),
        SpatioTemporalPoint((min_x+max_x)/2, (min_y+max_y)/2, (min_t+max_t)/2),
        SpatioTemporalPoint(max_x, max_y, max_t)
    );
}

vector<SpatioTemporalPoint> read_flight_data(const string &filename,size_t max_rows = 0)
{
        vector<SpatioTemporalPoint> points;
        rapidcsv::Document doc(filename, rapidcsv::LabelParams(0,-1));

        vector<int> aircraft_ids = doc.GetColumn<int>("aircraft_id");
        vector<int> years= doc.GetColumn<int>("year");
        vector<int> months= doc.GetColumn<int>("month");
        vector<int> days = doc.GetColumn<int>("day");
        vector<int> hours =doc.GetColumn<int>("hour");
        vector<int> minutes =doc.GetColumn<int>("minute");
        vector<double> seconds = doc.GetColumn<double>("second");
        vector<double> r_values = doc.GetColumn<double>("r");
        vector<double> u_values =doc.GetColumn<double>("u");

        size_t rows_to_read=(max_rows==0)? aircraft_ids.size(): min(max_rows,aircraft_ids.size());
        for(size_t i=0; i<rows_to_read; ++i)
        {
                //convert datetime to timestamp -total seconds
                double timestamp=seconds[i]+minutes[i]*60.0 + hours[i]*3600.0+ days[i]*86400.0+ months[i]*2592000.0 + years[i]*31536000.0;
                points.push_back(SpatioTemporalPoint(aircraft_ids[i], r_values[i], u_values[i], timestamp));

        }

    cout << "Successfully loaded " << points.size() << " trajectory points from CSV file" << endl;

    return points;

}

int main(int argc, char *argv[])
{
        size_t max_rows = 0;
        if (argc > 1)
                max_rows = stol(argv[1]);
    //define comparison functions for each dimension x,y,t
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

    //create comparator vector for 3D indexing
    cmp_vector<SpatioTemporalPoint> comparators = {cmp_x, cmp_y, cmp_t};

    //Initialise 3d R-tree with custom rectangle-making functions
    rtree<SpatioTemporalPoint, STP_comparator> tree(&comparators,make_region_rect,make_point_rect);
    cout<< "3D R-Tree for Spatio-temporal Queries (Flight Data)" <<endl;

    //load trajectory data from CSV
    cout <<"Reading flight data from CSV..."<< endl;
    vector<SpatioTemporalPoint> trajectory=read_flight_data("flight_data_readable.csv", max_rows);

    double min_r=0, max_r=0;
    double min_u=0, max_u=0;
    double min_t=0, max_t=0;

    //computing data range statistics
    if(!trajectory.empty())
    {
        min_r = trajectory[0].x;
        max_r = trajectory[0].x;
        min_u = trajectory[0].y;
        max_u = trajectory[0].y;
        min_t = trajectory[0].t;
        max_t = trajectory[0].t;

        for(auto &p: trajectory)
        {
            min_r=min(min_r, p.x);
            max_r=max(max_r, p.x);

            min_u=min(min_u, p.y);
            max_u=max(max_u, p.y);

            min_t=min(min_t, p.t);
            max_t=max(max_t, p.t);
        }

        cout << "\nData Statistics"<< endl;

        cout << "r (x) range: [" << min_r  << ", " <<  max_r << "]" << endl;
        cout << "u (y) range: [" <<  min_u << ", " <<  max_u << "]" << endl;
        cout << "t range: ["  << min_t << ", " <<    max_t << "]" <<   endl;
        cout<< "\n\n"<< endl;
    }
    else
    {
        cerr << "No data loaded. Exit..."<<endl;
        return 1;
    }

    // Build Rtree index
    cout<< "Inserting "<< trajectory.size()<< " trajectory points..." << endl;

    auto start=chrono::system_clock::now();
    for(auto &point: trajectory)
    {
        tree.insert(point);
    }

    auto end=chrono::system_clock::now();
    auto duration=chrono::duration_cast<chrono::microseconds>(end-start);

    cout <<"Inserted "<< tree.n_items()<<" points in "<< duration.count()<< " microseconds (" << duration.count() / 1000.0 << " ms)"  << endl;

    //Perform range query
    cout<< "\nSpatio-temporal Range Query"<< endl;
    cout << "Query: Find aircraft in specific spatial area during time interval" << endl;

    //Define query rectangle: covers 50% of space in each dimension
    SpatioTemporalPoint lower(min_r, min_u, min_t);
    SpatioTemporalPoint upper(min_r + (max_r - min_r) * 0.5, min_u + (max_u - min_u) * 0.5, min_t + (max_t - min_t) * 0.5);

    //the whole space
    //SpatioTemporalPoint lower(min_r, min_u, min_t);
    //SpatioTemporalPoint upper(max_r, max_u, max_t);

    cout <<  "Spatial range (r): ["  << lower.x << ", " << upper.x << "]" << endl;
    cout << "Spatial range (u): [" << lower.y << ", "  << upper.y << "]" << endl;
    cout << "Time range: [" << lower.t << ", " <<  upper.t << "]" <<  endl;

    //execute range query and measure time
    pair<SpatioTemporalPoint, SpatioTemporalPoint> range_query={lower,upper};
    start=chrono::system_clock::now();
    vector<SpatioTemporalPoint> results=tree.range_query(range_query);
    end =chrono::system_clock::now();

    duration=chrono::duration_cast<chrono::microseconds>(end-start);
    cout<< "Found "<<results.size()<< " points in "<< duration.count()<< " microseconds (" << duration.count() / 1000.0 << " ms)" << endl;

    cout << "\n=== First 10 Results ===" << endl;
    int display_count = min(10, (int)results.size());
    for (int i = 0; i < display_count; i++)
    {
        auto &p = results[i];
        cout << "Aircraft ID: " << p.aircraft_id
            << " | Position (r=" << p.x << ", u=" <<  p.y
            << ") | Time: " << p.t <<  endl;
    }

    return 0;

}
