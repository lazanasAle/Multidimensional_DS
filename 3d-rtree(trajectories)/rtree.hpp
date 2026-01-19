#ifndef _RTREE_HPP
#define _RTREE_HPP

#include <kd-btree/kd_btree.hpp>
#include <cmath>
#include <limits>

//This file implements a 3d R-tree data structure designed for indexing and quering
//spatio-temporal trajectory data. The R-tree extends the kd_btree implementation with
//Guttman's quadratic split alg., which is optimised for spatial data with bounding rectangles

//Spatio Temporal Point=STP
//The STP represents a point in 3D space with cordinates x,y,t and an aircraft identifier

//struct for the 3d spatio-temporal points x,y,t
struct SpatioTemporalPoint
{
    int aircraft_id;
    double x,y,t; //x= longitude,y= latitude, t=time

    //initialising to 0
    SpatioTemporalPoint(): aircraft_id(0),x(0), y(0),t(0)
    {}

    //constructor with coordinates only. Creates a point at position (x,y,t) with defualt aircraft_id=0
    SpatioTemporalPoint(double _x, double _y, double _t): aircraft_id(0), x(_x), y(_y), t(_t) {}

    //constructor with full data
    SpatioTemporalPoint(int _id,double _x, double _y, double _t): aircraft_id(_id), x(_x), y(_y), t(_t) {}

    //reads the raw bytes and re-interprets them as the appropriate types
    //reinterpret_cast is the correct cast type for binary I/O ops
    void read(fstream &file)
    {
        file.read(reinterpret_cast<char*>(&aircraft_id),sizeof(int));
        // read aircraft_id, convert &aircraft_id address to char* and read sizeof(int) bytes. Same with the rest
        file.read(reinterpret_cast<char*>(&x), sizeof(double));
        file.read(reinterpret_cast<char*>(&y), sizeof(double));
        file.read(reinterpret_cast<char*>(&t), sizeof(double));
    }

    // Write to binary file. Serializes a STP to a binary file stream and writes the raw bytes
    //of each member variable to the file
    void write(fstream &file) const
    {
        file.write(reinterpret_cast<const char*>(&aircraft_id) ,sizeof(int));
        file.write(reinterpret_cast<const char*>(&x), sizeof(double));
        file.write(reinterpret_cast<const char*>(&y), sizeof(double));
        file.write(reinterpret_cast<const char*>(&t), sizeof(double));

    }
};

//This struct orders STP points in containers
struct STP_comparator
{
    //Comparison. Returns true if point a should come before point b in sorted order
    bool operator()(const SpatioTemporalPoint &a,const SpatioTemporalPoint &b) const
    {
        if(a.x !=b.x) return a.x<b.x;
        if(a.y !=b.y) return a.y<b.y;
        return a.t< b.t;
    }
};

template <typename T, typename C>
class rtree: public kd_btree<T,C>
{
    private:
        double area_increase(vector<point<T>> &group, point<T> &entry);
        double area_increase(vector<region<T>> &group, region<T> &entry);

        //picking seeds. finds the entries with the most spatial waste
        pair<size_t,size_t> pick_seeds_points(vector<point<T>> &entries);
        pair<size_t,size_t> pick_seeds_regions(vector<region<T>> &entries);

        //picking next point/region
        size_t pick_next_point(vector<point<T>> &remaining,
                        vector<point<T>> &group1,
                        vector<point<T>> &group2);

        size_t pick_next_region(vector<region<T>> &remaining,
                        vector<region<T>> &group1,
                        vector<region<T>> &group2);

        bool assign_to_group1(vector<point<T>> &group1, vector<point<T>> &group2, point<T> &entry);
        bool assign_to_group1(vector<region<T>> &group1, vector<region<T>> &group2, region<T> &entry);


    protected:
        kd_bnode<T> *split_node(kd_bnode<T> *node) override;
        //overriding the split node with Guttman's quadratic split algorithm

    public:
        rtree(cmp_vector<T> *cmp_vec, rectangle<T> (*region_rectangle_fn)(vector<rectangle<T> *> &),
        rectangle<T> (*point_rectangle_fn)(vector<T *> &));
        ~rtree(){}
};

#include "rtree.impl.hpp"
#endif /* _RTREE_HPP */
