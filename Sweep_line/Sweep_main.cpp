#include "Sweep_Line_impl.hpp"

int main(){
        string filename="Swip_line_Data.csv";
        dataS input_data;

        input_data=prepare_data(filename);


        auto EventQueue=InitializeEvents(input_data.points, input_data.lines);
        //cout<<"size of event Queue:"<<EventQueue.size()<<"\n";

        set<point,PointComparator> testing=Sweep_Line(EventQueue);

        std::cout << "Detected Intersections (" << testing.size() << "):" << std::endl;
        for (const auto& p : testing) {
            std::cout << "Point: (" << p.x_axis << ", " << p.y_axis << ")" << std::endl;
        }




}
