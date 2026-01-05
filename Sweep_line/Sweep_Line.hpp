#ifndef _SWEEP_LINE_HPP
#define _SWEEP_LINE_HPP
#pragma once

#include "rapidcsv.h"
#include <utility>
#include <cstdint>
#include <string>
#include <vector>
#include <set>
using namespace std;

#define EPSILON 1e-9

struct point {
        pair<double,double> cordinates;
        string line_name;
};

enum event_type: uint8_t {
        UPPER,
        LOWER,
        INTERSECT
};

typedef pair<point,point> line;

struct event {
        point p;
        event_type type;
        double current_y;

        line *fline;
        line *sline;
};

struct dataS {
        vector<point> points;
        vector<line> lines;
};

struct type_comparator {
        bool operator() (const event &e0, const event &e1) const {
                return e0.type>e1.type;
        }
};

typedef multiset<event, type_comparator> queue_type;

dataS prepare_data(string &filename);
queue_type InitializeEvents(vector<point> &points, vector<line> &lines);
point *intersects(const line *l1, const line *l2);
event *make_intersection(const line *l1, const line *l2);
vector<point> sweep_line(queue_type &event_queue);

#endif /* _SWEEP_LINE_HPP */
