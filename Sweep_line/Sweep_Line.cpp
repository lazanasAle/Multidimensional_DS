#include "Sweep_Line.hpp"
#include <algorithm>

dataS prepare_data(string &filename) {
        dataS input;
        vector<point> points;

        rapidcsv::Document doc(filename, rapidcsv::LabelParams(0, -1));
        vector<double> x1 = doc.GetColumn<double>("X1");
        vector<double> y1 = doc.GetColumn<double>("Y1");
        vector<double> x2 = doc.GetColumn<double>("X2");
        vector<double> y2 = doc.GetColumn<double>("Y2");
        vector<string> line_name = doc.GetColumn<string>("name_of_line");

        vector<line> lines;

        for (size_t i = 0; i < line_name.size(); i++){
                line l;
                point p0={make_pair(x1[i], y1[i]), line_name[i]};
                point p1={make_pair(x2[i], y2[i]), line_name[i]};

                bool sec_cond = abs(p0.cordinates.second  - p1.cordinates.second ) < EPSILON && p0.cordinates.first < p1.cordinates.first;

                if (p0.cordinates.second > p1.cordinates.second || sec_cond) {
                            l.first = p0;  // Encountered first (Upper)
                            l.second = p1; // Encountered second (Lower)
                }
                else {
                            l.first = p1;  // Encountered first (Upper)
                            l.second = p0; // Encountered second (Lower)
                }

                lines.push_back(l);
                points.push_back(l.first);
                points.push_back(l.second);
        }


        //sort the points by y axis descending
        sort(points.begin(), points.end(),
                [](const point& a, const point& b) {
                        if (abs(a.cordinates.second - b.cordinates.second) > EPSILON)
                                return a.cordinates.second > b.cordinates.second;
                        return a.cordinates.first < b.cordinates.first; // Leftmost first if on same height
        });

        input.points = move(points);
        input.lines = move(lines);

        return input;
}

queue_type InitializeEvents(vector<point> &points, vector<line> &lines){
        queue_type EventQueue;
        for (size_t i = 0; i < points.size(); i++) {
                size_t firstdig = points[i].line_name.find_first_of("0123456789");
                if (firstdig != string::npos) {
                        string prefix = points[i].line_name.substr(0, firstdig);
                        string num_str = points[i].line_name.substr(firstdig);
                        int num_val = stoi(num_str);
                        event current;
                        current.current_y = points[i].cordinates.second;
                        current.fline = &lines[num_val];
                        current.p = points[i];
                        bool is_first = current.fline->first.cordinates.first == points[i].cordinates.first &&
                                current.fline->first.cordinates.second == points[i].cordinates.second; //comparing with the first element of the line
                        if (is_first)
                                current.type = UPPER;
                        else
                                current.type = LOWER;

                        current.sline = nullptr;
                        EventQueue.insert(current);
                }
        }
        return EventQueue;
}

point *intersects(const line *l1, const line *l2) {
        //finding differences between points (computing denominator)
        double dx1 = l1->second.cordinates.first - l1->first.cordinates.first;
        double dy1 = l1->second.cordinates.second - l1->first.cordinates.second;
        double dx2 = l2->second.cordinates.first - l2->first.cordinates.first;
        double dy2 = l2->second.cordinates.second - l2->first.cordinates.second;

        double denom = dx1 * dy2 - dy1 * dx2;

        if (denom == 0) //lines are parallel or collinear
                return nullptr;

        double dx3 = l2->first.cordinates.first - l1->first.cordinates.first;
        double dy3 = l2->first.cordinates.second - l1->first.cordinates.second;

        double t = (dx3 * dy2 - dy3 * dx2) / denom;
        double u = (dx3 * dy1 - dy3 * dx1) / denom;

        bool intersection_between = t < 0 || t > 1 || u < 0 || u > 1;

        if (intersection_between)
                return nullptr;

        point *is_point = new point();
        is_point->cordinates.first = l1->first.cordinates.first + t * dx1;
        is_point->cordinates.second = l1->first.cordinates.second + t * dy1;
        is_point->line_name = l1->first.line_name + l2->first.line_name;

        return is_point;
}


event *make_intersection(const line *l1, const line *l2) {
        point *is_pt = intersects(l1, l2);

        if (is_pt) {
                event *is_event = new event();
                is_event->p = *is_pt;
                is_event->type = INTERSECT;
                is_event->current_y = is_pt->cordinates.second;
                is_event->fline = l1;
                is_event->sline = l2;

                delete(is_pt);
                return is_event;
        }
        return nullptr;
}
