#include "Sweep_Line.hpp"
#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>
#include <utility>
dataS prepare_data(string &filename){
        dataS input;
        vector<point> points;

        rapidcsv::Document doc(filename, rapidcsv::LabelParams(0, -1));
        vector<double> x1 = doc.GetColumn<double>("X1");
        vector<double> y1 = doc.GetColumn<double>("Y1");
        vector<double> x2 = doc.GetColumn<double>("X2");
        vector<double> y2 = doc.GetColumn<double>("Y2");
        vector<string> line_name = doc.GetColumn<string>("name_of_line");

        vector<line> lines;

        for(size_t i=0;i<line_name.size();i++){
                line l;
                point p0={make_pair(x1[i], y1[i]),line_name[i]};
                point p1={make_pair(x2[i], y2[i]),line_name[i]};

                if (p0.cordinates.second > p1.cordinates.second || (abs(p0.cordinates.second  - p1.cordinates.second ) < EPSILON && p0.cordinates.first < p1.cordinates.first)) {
                            l.first = p0;  // Encountered first (Upper)
                            l.second= p1; // Encountered second (Lower)

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
        sort(points.begin(),points.end(), [](const point& a, const point& b) {
                if (abs(a.cordinates.second - b.cordinates.second) > EPSILON) return a.cordinates.second > b.cordinates.second;
                return a.cordinates.first< b.cordinates.first; // Leftmost first if on same height
        });

        input.points=move(points);
        input.lines=move(lines);

        return input;
}

fib_pq InitializeEvents(vector<point> &points, vector<line> &lines){
        fib_pq EventQueue;
        for(size_t i=0;i<points.size();i++){
                size_t firstdig=points[i].line_name.find_first_of("0123456789");
                if(firstdig!=std::string::npos){
                        string prefix = points[i].line_name.substr(0, firstdig);
                        string num_str = points[i].line_name.substr(firstdig);
                        int num_val = stoi(num_str);
                        event current;
                        current.current_y=points[i].cordinates.second;
                        current.fline=&lines[num_val];
                        current.p=points[i];
                        bool is_first = current.fline->first.cordinates.first == points[i].cordinates.first &&
                                current.fline->first.cordinates.second == points[i].cordinates.second; //comparing with the first element of the line
                        if(is_first){
                                current.type=UPPER;
                        }
                        else {
                                current.type=LOWER;
                        }
                        EventQueue.push(current);
                }
        }
        return EventQueue;
}
