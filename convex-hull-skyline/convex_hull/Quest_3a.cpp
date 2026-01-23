#include "ConvexHull.hpp"
#include <chrono>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

        size_t rows = 100000;
        if (argc > 1)
                rows = stol(argv[1]);
        //Read csv and get the columns you need-->working
        string filename="../data_movies_clean.csv";
        vector<movie_point> points;
        points = read_csv(filename, rows);
        auto sort_first = chrono::system_clock::now();
        points = polar_sorting(points);
        auto sort_last = chrono::system_clock::now();

        chrono::duration<double> sorting_dur = sort_last - sort_first;

        double sorting_time = sorting_dur.count();

        cout<<"Convex_hull\n";
        auto conv_first = chrono::system_clock::now();
        vector<movie_point> conv_set = Create_Convex_hull(points);
        auto conv_last = chrono::system_clock::now();

        chrono::duration<double> conv_dur = conv_last - conv_first;

        double conv_time = conv_dur.count();

        //print the convex hull
        for (auto it = conv_set.begin(); it != conv_set.end(); ++it)
                cout<<"X: "<<it->budget<<", Y: "<<it->popularity<<"\n";
        cout<<"\n";

        //export data to visualize its for testing only
        ofstream file("export_data.csv");

        if (!file.is_open()) {
                cerr<<"Failed to open file.\n";
                return 1;
        }
        file<<"budget,popularity\n";

        for (const auto& row : conv_set)
                file<< row.budget << ","<< row.popularity << "\n";
        file.close();
        cout<<"CSV exported successfully!\n";

        double overall_time = sorting_time + conv_time;

        ofstream times_file("times.csv", ios::app);

        times_file<<rows<<","<<overall_time<<"\n";
}
