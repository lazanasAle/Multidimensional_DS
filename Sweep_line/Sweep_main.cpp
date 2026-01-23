#include "Sweep_Line_impl.hpp"
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
        string filename="Swip_line_Data.csv";
        dataS input_data;

        input_data=prepare_data(filename);

        event_queue eq = InitializeEvents(input_data.points, input_data.lines);

        auto start = chrono::system_clock::now();
        set<point, PointComparator> is_points = Sweep_Line(eq);
        auto end = chrono::system_clock::now();
        chrono::duration<double> sweep_dur = end - start;
        double sweep_time = sweep_dur.count();

        cout<<"execution time needed: "<<sweep_time<<"\n";
        cout<<"we have: "<<is_points.size()<<" intersection points"<<"\n";
        for (auto it = is_points.begin(); it != is_points.end(); ++it) {
                cout<<"Intersection point in: "<<it->x_axis<<" "<<it->y_axis<<"\n";
        }

        size_t data_size = input_data.points.size() / 2;

        ofstream times_csv;
        times_csv.open("times.csv", ios::app);
        times_csv<<data_size<<","<<sweep_time<<"\n";
}
