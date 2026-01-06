#include "Sweep_Line_impl.hpp"
#include <cfloat>
#include <print>

double sweep_line::sweepY = DBL_MAX;

dataS prepare_data(string filename){
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
                l.line_name=line_name[i];
                point p0={x1[i],y1[i],0,line_name[i]};
                point p1={x2[i],y2[i],0,line_name[i]};

                if (p0.y_axis > p1.y_axis || (fabs(p0.y_axis - p1.y_axis) < EPSHILON && p0.x_axis < p1.x_axis)) {
                            l.first_point = p0;  // Encountered first (Upper)
                            l.second_point = p1; // Encountered second (Lower)
                            p0.thessi_in_line=0;
                            p1.thessi_in_line=1;
                }
                else {
                            l.first_point = p1;  // Encountered first (Upper)
                            l.second_point = p0; // Encountered second (Lower)
                            p0.thessi_in_line=1;
                            p1.thessi_in_line=0;
                }

                        // Mark positions for the Event Queue logic
                l.first_point.thessi_in_line = 0;  // UPPER
                l.second_point.thessi_in_line = 1; // LOWER
                l.line_name=line_name[i];
                        // Add both points to the points vector for event processing

                lines.push_back(l);
                points.push_back(l.first_point);
                points.push_back(l.second_point);

        }


        //sort the points by y axis descending
        sort(points.begin(),points.end(), [](const point& a, const point& b) {
                if (abs(a.y_axis - b.y_axis) > EPSHILON) return a.y_axis > b.y_axis;
                return a.x_axis < b.x_axis; // Leftmost first if on same height
        });

        input.points=points;
        input.lines=lines;

        return input;
}

//do that second
event_queue InitializeEvents(vector<point> &points, vector<line> &lines){
        event_queue EventQueue(sort_y);
        for(size_t i=0;i<points.size();i++){
                size_t firstdig=points[i].line_name.find_first_of("0123456789");
                if(firstdig!=string::npos){
                        string prefix = points[i].line_name.substr(0, firstdig);
                        string num_str = points[i].line_name.substr(firstdig);
                        int num_val = stoi(num_str);
                        event current;
                        current.current_height=points[i].y_axis;
                        current.s0=&lines[num_val];
                        current.p=points[i];
                        if(points[i].thessi_in_line==0){
                                current.type=event::UPPER;
                        }
                        else {
                                 current.type=event::LOWER;
                        }
                        EventQueue.push(current);
                }
        }
        return EventQueue;
}

//do that third

bool Collinear(point &p0, point &p1, point &p2) {
        return p1.x_axis<= max(p0.x_axis,p2.x_axis) && p1.x_axis>= min(p0.x_axis,p2.x_axis) &&
                p1.y_axis<= max(p0.y_axis,p2.y_axis) && p1.y_axis>= min(p0.y_axis,p2.y_axis);
}

int orientation(point &p0, point &p1, point &p2) {
        double determant=(p1.x_axis - p0.x_axis) * (p2.y_axis - p0.y_axis) -(p1.y_axis - p0.y_axis) * (p2.x_axis - p0.x_axis);
        if(abs(determant)<EPSHILON)
                return 0;// Collinear
        if(determant>0)
                return 2;// Clockwise
        else return 1; //Counter-Clockwise
}

bool Intersection(line &l1, line &l2) {
        point p0=l1.first_point,p1=l1.second_point;
        point p2=l2.first_point,p3=l2.second_point;

        int d0=orientation(p0, p1, p2);
        int d1=orientation(p0, p1, p3);
        int d2=orientation(p2, p3, p0);
        int d3=orientation(p2, p3, p1);

        if (((d0 == 1 && d1 == 2) || (d0 == 2 && d1 == 1)) &&
                ((d2 == 1 && d3 == 2) || (d2 == 2 && d3 == 1))) {

                return true;
            } //intersection
        if (d0 == 0 && Collinear(p0, p2, p1)) return true;
            // Check if p3 lies on segment p0p1
        if (d1 == 0 && Collinear(p0, p3, p1)) return true;
            // Check if p0 lies on segment p2p3
        if (d2 == 0 && Collinear(p2, p0, p3)) return true;
            // Check if p1 lies on segment p2p3
        if (d3 == 0 && Collinear(p2, p1, p3)) return true;

        return false;
}

point ComputeIntersectionpoint(line &l1, line &l2) {
        point intersection;
        double x1 = l1.first_point.x_axis,  y1 = l1.first_point.y_axis;
        double x2 = l1.second_point.x_axis, y2 = l1.second_point.y_axis;
        double x3 = l2.first_point.x_axis,  y3 = l2.first_point.y_axis;
        double x4 = l2.second_point.x_axis, y4 = l2.second_point.y_axis;

        double determinant = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (fabs(determinant) < EPSHILON) return {};
        intersection.x_axis=((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/determinant;
        intersection.y_axis=((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/determinant;
        intersection.thessi_in_line=0;

        return intersection;
}

void InsertIntersection(line *first_in_tree, line *second_in_tree, point &p, event_queue &EventQueue,
        set<point, PointComparator> &processed, double current_y){

                if (processed.find(p) != processed.end()) {
                        println("a if print the point {} {}",p.x_axis,p.y_axis);
                        return;
                }
                if (p.y_axis > current_y - EPSHILON){
                        println("a if print the point {} {}",p.x_axis,p.y_axis);
                        return;
                }

                event tmp;
                tmp.current_height=p.y_axis;
                tmp.s0=first_in_tree;
                tmp.s1=second_in_tree;
                tmp.p=p;
                tmp.type=event::INTERSECTION;

                EventQueue.push(tmp);
                processed.insert(p);
}

set<point, PointComparator> Sweep_Line(event_queue &EventQueue) {
        //create a line stat
        set<point, PointComparator> processedIntersections;
        set<line*, RelativeCompareToX> segment_line;

        while(!EventQueue.empty()){
                event current=EventQueue.top();
                EventQueue.pop();
                sweep_line::sweepY=current.current_height;
                if(current.type==event::UPPER){
                        auto it=segment_line.insert(current.s0).first;
                        if(it!=segment_line.end()){
                                if(it!=segment_line.begin()){
                                        auto predeccesor = prev(it);
                                        if (*predeccesor != nullptr && current.s0 != nullptr && Intersection(*current.s0, **predeccesor)){
                                                point p;
                                                p=ComputeIntersectionpoint(*current.s0, **predeccesor);
                                                println("the intersection point is:{} {}",p.x_axis,p.y_axis);
                                                InsertIntersection(*predeccesor,current.s0, p, EventQueue, processedIntersections,current.current_height);
                                        }
                                }
                                auto successor=next(it);
                                if(successor !=segment_line.end()){
                                        if (*successor!= nullptr && current.s0 != nullptr && Intersection(*current.s0, **successor)){
                                                point p;
                                                p=ComputeIntersectionpoint(*current.s0, **successor);
                                                println("the intersection point is:{} {}",p.x_axis,p.y_axis);
                                                InsertIntersection(current.s0,*successor ,p, EventQueue, processedIntersections,current.current_height);
                                        }
                                }

                        }
                }
                else if (current.type==event::LOWER) {
                        auto it = segment_line.find(current.s0);

                        if (it != segment_line.end()) {
                                if (it != segment_line.begin()) {
                                        auto predeccesor = std::prev(it);
                                        auto successor = std::next(it);
                                        if (successor != segment_line.end()) {
                                                if (Intersection(**predeccesor, **successor)) {
                                                        point p = ComputeIntersectionpoint(**predeccesor, **successor);
                                                        println("the intersection point is:{} {}",p.x_axis,p.y_axis);
                                                        InsertIntersection(*predeccesor, *successor, p, EventQueue,processedIntersections, sweep_line::sweepY);
                                                }
                                        }
                                }
                                segment_line.erase(it);
                        }
                }
                else{
                        segment_line.erase(current.s0);
                        segment_line.erase(current.s1);

                        sweep_line::sweepY -= EPSHILON;

                        auto p = segment_line.insert(current.s1).first;
                        auto p1 = segment_line.insert(current.s0).first;

                        if(p!=segment_line.begin()){
                                auto neighbour=prev(p);
                                if (Intersection(*current.s1, **neighbour)) {
                                        point p;
                                        p=ComputeIntersectionpoint(*current.s1, **neighbour);
                                        println("the intersection point is:{} {}",p.x_axis,p.y_axis);
                                        InsertIntersection(*neighbour,current.s1, p, EventQueue, processedIntersections,current.current_height);
                                }
                        }
                        auto neighbour1=next(p1);
                        if (neighbour1!=segment_line.end()) {
                                if (Intersection(*current.s0, **neighbour1)) {
                                        point p;
                                        p=ComputeIntersectionpoint(*current.s0, **neighbour1);
                                        println("the intersection point is:{} {}",p.x_axis,p.y_axis);
                                        InsertIntersection(current.s0,*neighbour1, p, EventQueue, processedIntersections,current.current_height);
                                }
                        }
                }
        }
        return processedIntersections;
}
