#ifndef _SWEEP_LINE_IMPL_HPP
#define _SWEEP_LINE_IMPL_HPP

#include "rapidcsv.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <cstddef>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include <cmath>

#define EPSILON 1e-7

using namespace std;

struct sweep_line {
        static double sweepY;
};

struct point{
        double x_axis;
        double y_axis;
        int thessi_in_line;
        string line_name;
};

struct line{
        point first_point;
        point second_point;
        string line_name;
};
struct dataS{
        vector<point> points;
        vector<line> lines;
};

struct event{
        double current_height;
        point p;
        enum Type { UPPER, LOWER, INTERSECTION } type;
        line* s0;
        line* s1; //only for intersection
};

//comparisson for duplicates
struct PointComparator {
        bool operator()(const point& a, const point& b) const {
                if (abs(a.y_axis - b.y_axis) > EPSILON) return a.y_axis < b.y_axis;
                if (abs(a.x_axis - b.x_axis) > EPSILON) return a.x_axis < b.x_axis;
                return false;
        }
};

struct  RelativeCompareToX {
        bool operator() (const line* a, const line* b) const {
                if (a == b) return false;

                auto getX=[](const line* l, double y) {
                        if (abs(l->second_point.y_axis - l->first_point.y_axis) < EPSILON)
                                return static_cast<double>(l->first_point.x_axis); // Horizontal line
                        double  t = (y - l->first_point.y_axis) / (l->second_point.y_axis - l->first_point.y_axis);
                        return l->first_point.x_axis + t * (l->second_point.x_axis - l->first_point.x_axis);
                };
                double x0=getX(a, sweep_line::sweepY);
                double x1=getX(b, sweep_line::sweepY);
                if (abs(x0 - x1) > EPSILON) return x0 < x1;
                auto getSlope = [](const line* l) {
                        double dy = l->second_point.y_axis - l->first_point.y_axis;
                        if (abs(dy) < EPSILON) return 0.0; // Horizontal lines have 0 slope in this context
                        return (l->second_point.x_axis - l->first_point.x_axis) / dy;
                };
                double slopeA = getSlope(a);
                double slopeB = getSlope(b);
                if (abs(slopeA - slopeB) > EPSILON) return slopeA < slopeB;

                return a < b;
        }

};


dataS prepare_data(string filename);

inline auto sort_y=[](const event& e0, const event& e1) {
        if (abs(e0.current_height - e1.current_height) > EPSILON)
                return e0.current_height < e1.current_height;

        if (e0.type != e1.type) {
                static int priority_map[] = {2, 0, 1};
                return priority_map[e0.type] < priority_map[e1.type];
        }


        if (abs(e0.p.x_axis - e1.p.x_axis) > EPSILON)
                return e0.p.x_axis > e1.p.x_axis;

        return false;
};

typedef priority_queue<
        event,
        vector<event>,
        decltype(sort_y)
> event_queue;

event_queue InitializeEvents(vector<point> &points, vector<line> &lines);
bool Collinear(point &p0, point &p1, point &p2);
int orientation(point &p0, point &p1, point &p2);
bool Intersection(line &l1, line &l2);
point ComputeIntersectionpoint(line &l1, line &l2);
void InsertIntersection(line *first_in_tree, line *second_in_tree, point &p, event_queue &EventQueue,
        set<point, PointComparator> &processed, double current_y);
set<point, PointComparator> Sweep_Line(event_queue &EventQueue);


#endif /*_SWEEP_LINE_IMPL_HPP */
