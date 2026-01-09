#include "Sweep_Line_impl.hpp"
#include <print>
#include <chrono>
#include <fstream>


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

        println("execution time needed: {}", sweep_time);
        println("we have: {} intersection points", is_points.size());
        for (auto it = is_points.begin(); it != is_points.end(); ++it) {
                println("Intersection point in: {} {}", it->x_axis, it->y_axis);
        }

        size_t data_size = input_data.points.size();

        ofstream times_csv;
        times_csv.open("times.csv");
        times_csv<<data_size<<","<<sweep_time<<"\n";
}
