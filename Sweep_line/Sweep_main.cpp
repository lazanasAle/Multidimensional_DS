#include "Sweep_Line_impl.hpp"
#include <print>

int main(int argc, char *argv[]) {
        string filename="Swip_line_Data.csv";
        dataS input_data;

        input_data=prepare_data(filename);

        event_queue eq = InitializeEvents(input_data.points, input_data.lines);

        set<point, PointComparator> is_points = Sweep_Line(eq);

        println("we have: {} intersection points", is_points.size());
        for (auto it = is_points.begin(); it != is_points.end(); ++it) {
                println("Intersection point in: {} {}", it->x_axis, it->y_axis);
        }
}
