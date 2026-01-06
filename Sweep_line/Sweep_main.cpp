#include "Sweep_Line_impl.hpp"

int main(){
        string filename="Swip_line_Data.csv";
        dataS input_data;

        input_data=prepare_data(filename);

        event_queue eq = InitializeEvents(input_data.points, input_data.lines);

        Sweep_Line(eq);
}
