#include "ConvexHull.hpp"


int main(){
    //Read csv and get the columns you need-->working
        string filename="data_movies_clean.csv";
        vector<movie_point> points;
        points=read_csv(filename);

        points=polar_sorting(points);

        //print the polar points
        cout<<"Polar sorting"<<endl;
        for(size_t i=0;i<points.size();i++){
            cout << "X: " << points[i].budget << ", Y: " << points[i].popularity << std::endl;
        }
        cout << endl;

        cout<<"Convex_hull"<<endl;
        vector<movie_point> S;
        S=Create_Convex_hull(points);

        //print the convex hull
        for (auto it = S.begin(); it != S.end(); ++it) {
                cout << "X: " << it->budget << ", Y: " << it->popularity << std::endl;
        }
        cout << endl;

        //export data to visualize its for testing only
        ofstream file("export_data.csv");

            if (!file.is_open()) {
                cerr << "Failed to open file.\n";
                return 1;
            }


            file << "budget,popularity\n";


            for (const auto& row : S) {
                file << row.budget << ","
                     << row.popularity << "\n";
            }

            file.close();
            cout << "CSV exported successfully!\n";



}
