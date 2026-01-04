#include "ConvexHull.hpp"


int main(int argc, char *argv[]) {
        //Read csv and get the columns you need-->working
        string filename="../data_movies_clean.csv";
        vector<movie_point> points;
        points = read_csv(filename);
        points = polar_sorting(points);

        cout<<"Convex_hull\n";
        vector<movie_point> conv_set = Create_Convex_hull(points);

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
}
