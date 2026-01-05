#ifndef _SWEEP_LINE_HPP
#define _SWEEP_LINE_HPP
#include <algorithm>
#pragma once

#include "rapidcsv.h"
#include <utility>
#include <cstdint>
#include <string>
#include <vector>
#include <boost/heap/fibonacci_heap.hpp>
using namespace std;
namespace fib=boost::heap;

#define EPSILON 1e-9

struct point{
        pair<double,double> cordinates;
        string line_name;
};

enum event_type:uint8_t{
        UPPER,
        LOWER,
        INTERSECT
};

typedef pair<point,point> line;

struct event{
        point p;
        event_type type;
        double current_y;

        line *fline;
        line *sline;
};

struct dataS{
        vector<point> points;
        vector<line> lines;
};

struct type_comparator{
        bool operator()(const event &e0, const event &e1) const {
                return e0.type>e1.type;
        }
};

typedef fib::fibonacci_heap<
        event,
        fib::compare<type_comparator>
> fib_pq;

dataS prepare_data(string &filename);
fib_pq InitializeEvents(vector<point> &points, vector<line> &lines);


#endif
