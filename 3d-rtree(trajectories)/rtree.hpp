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

template <typename T, typename C>
class rtree: public kd_btree<T,C>
{
    private: 
        //..
    public:
        rtree(cmp_vector<T> *cmp_vec, function<rectangle<T> (vector<rectangle<T> *> &)> region_rectangle_fn,
        function<rectangle<T> (vector<T *> &)> point_rectangle_fn);
        ~rtree(){}
};

#include "rtree.impl.hpp"
#endif /* _RTREE_HPP */