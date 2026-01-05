#ifndef _SWEEP_LINE_HPP
#define _SWEEP_LINE_HPP
#pragma once

#include "rapidcsv.h"
#include <utility>
#include <cstdint>
#include <string>
#include <iterator>
#include <vector>
#include <set>
using namespace std;

#define EPSILON 1e-9

struct sweep_line {
        static double sweepY;
};

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

struct event_comparator {
        bool operator() (const event &e0, const event &e1) const {
                return e0.p.cordinates.first < e1.p.cordinates.first;
        }
};

static inline double x_at_y(const line *l, double y) {
        if (l->first.cordinates.second == l->second.cordinates.second)
                return min(l->first.cordinates.first, l->second.cordinates.first); //horisontal line
        double t = (y - l->first.cordinates.second) / (l->second.cordinates.second - l->first.cordinates.second);
        return l->first.cordinates.first + t * (l->second.cordinates.first - l->first.cordinates.first);
}

struct status_comp {
        bool operator() (const line *l1, const line *l2) const {
                if (l1 != l2) {
                        double xa = x_at_y(l1, sweep_line::sweepY);
                        double xb = x_at_y(l2, sweep_line::sweepY);
                        return xa < xb;
                }
                return false;
        }
};

typedef multiset<event, event_comparator> queue_type;
typedef multiset<line *, status_comp> status_bst;

dataS prepare_data(string &filename);
queue_type InitializeEvents(vector<point> &points, vector<line> &lines);
point *intersects(const line *l1, const line *l2);
event *make_intersection(line *l1, line *l2);
void handle_leftCase(event &e, queue_type &event_queue, status_bst &status_tree, vector<point> &is_points);
vector<point> sweep_line(queue_type &event_queue);

#endif /* _SWEEP_LINE_HPP */
