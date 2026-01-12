#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/json.hpp>
#include <boost/json/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/value.hpp>
#include "rtree.hpp"
#include <rapidcsv.h>

//R-Tree implentation for spatio-temporal trajectory queries
//reads config from JSON and executes range queries on flight data
//similar to skyline/results_from_json.cpp but adapted for 3D Spatial-temporal data

using namespace std;
//using namespace std::chrono;
namespace json = boost::json;

//creates Minimum Bounding Rectangle (MBR) for a set of points
//used for leaf nodes in the R-tree
//rectangle creation functions
//everything is same as test_rtree.cpp
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

vector<SpatioTemporalPoint> read_flight_data(const string &filename, size_t max_rows = 0)
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

    //ifstream file(filename);

    /*if(!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return points;
    }

    string line;
    getline(file, line); //skip header

    size_t count = 0;*/

    /*while(getline(file, line) && (max_rows == 0 || count < max_rows))
    {
        stringstream ss(line);
        string token;
        vector<string> values;

        while(getline(ss, token, ','))
        {
            values.push_back(token);
        }

        if (values.size() >= 9)
        {
            try
            {
                int aircraft_id=stoi(values[0]);
                int year=stoi(values[1]);
                int month=stoi(values[2]);
                int day=stoi(values[3]);
                int hour=stoi(values[4]);
                int minute=stoi(values[5]);

                double second=stod(values[6]);
                double r= stod(values[7]); //r->x
                double u=stod(values[8]); //u-> y

                //convert datetime to timestamp (total seconds)
                double timestamp=second+minute*60.0+ hour* 3600.0 + day* 86400.0 + month * 2592000.0+ year*31536000.0;

                points.push_back(SpatioTemporalPoint(aircraft_id, r, u, timestamp));
                count++;
            }
            catch(const exception& e)
            {
                cerr << "Error parsing line: " << line << endl;
            }
        }
    }

    file.close();
    cout << "Successfully loaded " << count << " trajectory points from CSV file" << endl;
    return points;*/


       
}

//constructor range query bounds from json config
pair<SpatioTemporalPoint, SpatioTemporalPoint> range_query_routine(
    json::object& range_query_obj,
    vector<size_t>&idx_dims,
    double min_x, double max_x,
    double min_y, double max_y,
    double min_t, double max_t)
{
    //static array: coords[0] = lower bounds, coords[1] = upper bounds
    static double coords[2][3];
    coords[0][0]=min_x;
    coords[1][0]=max_x;
    coords[0][1]=min_y;
    coords[1][1]=max_y;
    coords[0][2]=min_t;
    coords[1][2]=max_t;

    for(size_t &dim : idx_dims)
    {
        string key = to_string(dim);

        if(range_query_obj.contains(key))
        {
            json::array range=range_query_obj.at(key).as_array();
            coords[0][dim]= range.at(0).as_double();
            coords[1][dim]=range.at(1).as_double();
        }
    }
    SpatioTemporalPoint lower_bound(coords[0][0], coords[0][1], coords[0][2]);
    SpatioTemporalPoint upper_bound(coords[1][0], coords[1][1], coords[1][2]);

    return make_pair(lower_bound, upper_bound);
}

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        cerr<< "Usage: "<< argv[0]<< " <number_of_rows>"<<endl;
        return 1;
    }

    //read json config
    ifstream json_file("rtree_query.json");

    if(!json_file.is_open())
    {
        cerr<< "Error! Couldn't open rtree_query.json" <<endl;
        return 1;
    }

    //parse json
    json::value root= json::parse(json_file);
    json::object root_obj=root.as_object();

    //extract config
    json::array idx_dims_json= root_obj.at("index_dimensions").as_array();
    json::object range_query_obj= root_obj.at("range_query").as_object();

    //get index dimensions
    vector<size_t> idx_dims;
    for(json::value const &v : idx_dims_json)
    {
        size_t dim=static_cast<size_t>(v.as_int64());
        idx_dims.push_back(dim);
    }

    cout<< "*****"<<endl;
    cout<<"R-Tree SpatioTemporal Query System"<< endl;
    cout<<"*****"<<endl;

    cout<< "Indexed dimensions: ";
    vector<string> dim_names={ "r (longitude/x)", "u (latitude/y)", "t (time)"};

    for(size_t dim: idx_dims)
    {
        cout<< dim_names[dim]<< " ";
    }
    cout<< endl;

    //define comparison functions for each dimension x,y,t
    //originally from test_rtree.cpp
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

    //build comparator vector with only indexed dimensions
    cmp_vector<SpatioTemporalPoint> all_comps={cmp_x, cmp_y, cmp_t};
    cmp_vector<SpatioTemporalPoint> comps_used;

    for(size_t dim: idx_dims)
    {
        comps_used.push_back(all_comps[dim]);
    }

    //create rtree with selected comparators
    rtree<SpatioTemporalPoint, STP_comparator> tree(&comps_used, make_region_rect, make_point_rect);

    //read data
    size_t rows= stol(argv[1]);

    cout<< "\nReading flight data...\n"<<endl;
    cout<< "(limit: "<< rows<< " rows)"<<endl;

    vector<SpatioTemporalPoint> trajectory=read_flight_data("flight_data_readable.csv",rows);

    if(trajectory.empty())
    {
        cerr << "No data loaded. Exit..." <<endl;
        return 1;
    }

    double min_x =trajectory[0].x, max_x = trajectory[0].x;
    double min_y =trajectory[0].y, max_y = trajectory[0].y;
    double min_t =trajectory[0].t, max_t = trajectory[0].t;

    for(auto &p: trajectory)
    {
        min_x=min(min_x, p.x);
        max_x=max(max_x, p.x);

        min_y=min(min_y, p.y);
        max_y=max(max_y, p.y);

        min_t=min(min_t, p.t);
        max_t=max(max_t, p.t);
    }

    cout << "\nDataset Statistics:" <<endl;
    cout << "  r (x) range: [" << min_x << ", "  << max_x << "]" << endl;
    cout <<"  u (y) range: [" << min_y << ", " << max_y << "]" <<endl;
    cout << "  t range: [" << min_t << ", " <<  max_t << "]" << endl;

    cout << "Building R-Tree index..."<<endl;
    auto start=chrono::high_resolution_clock::now();
    for(auto &point: trajectory)
    {
        tree.insert(point);
    }

    auto end=chrono::high_resolution_clock::now();
    auto duration=chrono::duration_cast<chrono::microseconds>(end-start);
    cout << "Inserted " << trajectory.size() << " points in " << duration.count() << " microseconds ("<< duration.count() / 1000.0 << " ms)" << endl;

    //build+execute range query
    pair<SpatioTemporalPoint,SpatioTemporalPoint> query_interval =range_query_routine(range_query_obj, idx_dims, min_x, max_x, min_y, max_y, min_t, max_t);

    cout<< "\n-----"<<endl;
    cout<<"Executing Range Query"<< endl;
    cout<< "\n-----"<<endl;

    cout << "Query bounds:" <<  endl;
    cout << "  r (x): [" << query_interval.first.x << ", " << query_interval.second.x << "]"  << endl;
    cout << "  u (y): [" << query_interval.first.y<< ", " << query_interval.second.y << "]" <<endl;
    cout << "  t: [" << query_interval.first.t << ", "  << query_interval.second.t << "]" <<endl;

    start=chrono::high_resolution_clock::now();
    vector<SpatioTemporalPoint> results=tree.range_query(query_interval);
    end =chrono::high_resolution_clock::now();
    duration=chrono::duration_cast<chrono::microseconds>(end-start);

    cout << "\nInside the typed interval exist: " << results.size() << " points"  << endl;
    cout << "Query executed in " << duration.count() / 1000.0 << " ms"<< endl;

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
