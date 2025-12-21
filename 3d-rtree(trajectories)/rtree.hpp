#ifndef _RTREE_HPP
#define _RTREE_HPP

#include "../kd-btree/kd_btree.hpp"
#include <cmath>
#include <limits>

//struct for the 3d spatio-temporal points x,y,t
struct SpatioTemporalPoint
{
    double x,y,t; //x= longitude,y= latitude, t=time
    SpatioTemporalPoint(): x(0), y(0),t(0)
    {}

    SpatioTemporalPoint(double _x, double _y, double _t): x(_x), y(_y), t(_t) {}
    //Spatio Temporal Point=STP
    void read(fstream &file)
    {
        file.read((char*)&x, sizeof(double));
        file.read((char*)&y, sizeof(double));
        file.read((char*)&t, sizeof(double));

    }

    void write(fstream &file) const
    {
        file.write((char*)&x, sizeof(double));
        file.write((char*)&y, sizeof(double));
        file.write((char*)&t, sizeof(double));

    }
};

struct STP_comparator
{
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
        rtree(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
        function<rectangle<T> (vector<T *> &)> point_rectangle_fn);
        ~rtree(){}
};

#include "rtree.impl.hpp"
#endif /* _RTREE_HPP */

//will add commenting soon