#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
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


vector<SpatioTemporalPoint> read_flight_data(const string& filename)
{
    vector<SpatioTemporalPoint> points;
    ifstream file(filename);

    if(!file.is_open())
    {
        cerr<< "Error: Could not open file "<<filename <<endl;
        return points;
    }

    string line;
    getline(file,line);
    //skip 1st line (header)

    int count=0;
    while(getline(file,line))
    {
        stringstream ss(line);
        string token;
        vector<string> values;

        while(getline(ss, token, ','))
        {
            values.push_back(token);
        }

        if (values.size()>=9)
        {

            try
            {
                int airfraft_id=(int)stod(values[0]);
                int year=(int)stod(values[1]);
                int month=(int)stod(values[2]);
                int day=(int)stod(values[3]);
                int hour=(int)stod(values[4]);
                int minute=(int)stod(values[5]);
                
                double second=stod(values[6]);
                double r= stod(values[7]); //r->x
                double u=stod(values[8]); //u-> y

                double timestamp=second+minute*60.0+ hour* 3600.0 + day* 86400.0 + month * 2592000.0;
                // 86400 is the #seconds in a day: 24 x 60 x 60 = 86400
                //2592000 is the #seconds in a month (if 1 month=30days) 24 x 60 x 60 x30=25920000

                //migth edit later

                points.push_back(SpatioTemporalPoint(airfraft_id,r,u,timestamp));
                count++;

            }
            catch(const exception& e)
            {
                cerr << "Error parsing the line: "<<line<< endl;
            }
        }
    }

    file.close();
    cout<< "Succesfully loaded "<< count<< " trajectory points from CSV file"<< endl;
    return points;
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
    cout<< "3D R-Tree for Spatio-temporal Queries (Flight Data)" <<endl;

    cout <<"Reading flight data from CSV..."<< endl;
    vector<SpatioTemporalPoint> trajectory=read_flight_data("flight_data_readable.csv");

    double min_r=0, max_r=0;
    double min_u=0, max_u=0;
    double min_t=0, max_t=0;

    if(!trajectory.empty())
    {
        //double min_r= trajectory[0].x, max_r= trajectory[0].x;
        //double min_u= trajectory[0].y, max_u=trajectory[0].y;
        //double min_t= trajectory[0].t, max_t=trajectory[0].t;

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

    cout<< "Inserting "<< trajectory.size()<< " trajectory points..." << endl;

    auto start=high_resolution_clock::now();
    for(auto &point: trajectory)
    {
        tree.insert(point);
    }

    auto end=high_resolution_clock::now();
    auto duration=duration_cast<microseconds>(end-start);

    cout <<"Inserted "<< tree.n_items()<<" points in "<< duration.count()<< " microseconds (" << duration.count() / 1000.0 << " ms)"  << endl;

    cout<< "\nSpatio-temporal Range Query"<< endl;
    cout << "Query: Find aircraft in specific spatial area during time interval" << endl;

    //SpatioTemporalPoint lower(16000000.0, -1.30, 0.0);
    //SpatioTemporalPoint upper(16500000.0,  -1.28, 200000.0);

    SpatioTemporalPoint lower(min_r+ (max_r-min_r)*0.2, min_u+(max_u-min_u)*0.2, min_t+(max_t-min_t)*0.1);
    SpatioTemporalPoint upper(min_r+ (max_r-min_r)*0.4, min_u+(max_u-min_u)*0.4, min_t+(max_t-min_t)*0.3);

    cout <<  "Spatial range (r): ["  << lower.x << ", " << upper.x << "]" << endl;
    cout << "Spatial range (u): [" << lower.y << ", "  << upper.y << "]" << endl;
    cout << "Time range: [" << lower.t << ", " <<  upper.t << "]" <<  endl;

    pair<SpatioTemporalPoint, SpatioTemporalPoint> range_query={lower,upper};
    start=high_resolution_clock::now();
    vector<SpatioTemporalPoint> results=tree.range_query(range_query);
    end =high_resolution_clock::now();

    duration=duration_cast<microseconds>(end-start);
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

    /*cout<< "\nResults:"<< endl;
    for(auto &p: results)
    {
        cout<< "Position ("<< p.x<< ","<<p.y<< ") at time "<<p.t<< endl;
    }*/
    return 0;

}

//will make a makefile in next commit

//cd "3d-rtree(trajectories)"
//g++ -std=gnu++23 -O2 -I../3d-rtree\(trajectories\) test_rtree.cpp -o test_rtree
// ./test_rtree

//will add commenting soon