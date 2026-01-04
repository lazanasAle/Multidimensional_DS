#include "ConvexHull.hpp"
#include <cstddef>

vector<movie_point> read_csv(string filename){
        vector<movie_point> points;
        rapidcsv::Document doc(filename, rapidcsv::LabelParams(0, -1));

        vector<double> budgetV = doc.GetColumn<double>("budget");
        vector<double> popularityV = doc.GetColumn<double>("popularity");


        for(size_t i=0;i<budgetV.size();i++){
                movie_point point;
                point.budget=budgetV[i];
                point.popularity=popularityV[i];
                points.push_back(point);

        }
        return points;
}

//counterclockwise function-->its corrected
int  direction(const movie_point &p0, const movie_point &p1, const movie_point &p2){
        double orientation=(p1.budget-p0.budget)*(p2.popularity-p0.popularity)-(p1.popularity-p0.popularity)*(p2.budget-p0.budget);
        if(orientation>0) return 1;
        else if (orientation<0) return -1;
        else return 0;
}

//tie breaker
double euclideian_dist(const movie_point &p0, const movie_point &p1){
        double dist=(p0.budget-p1.budget)*(p0.budget-p1.budget) +(p0.popularity-p1.popularity)*(p0.popularity-p1.popularity);
        return dist;
}

vector<movie_point> polar_sorting(vector<movie_point> &points){
        sort(points.begin(), points.end(), [](const movie_point& a, const movie_point& b) {
                return a.budget < b.budget;
        });
        movie_point pivot_point=points[0];
        sort(points.begin(), points.end(),[&pivot_point](const movie_point& p1, const movie_point& p2){
                int x=direction(pivot_point,p1,p2);
                if(x==0){
                        return euclideian_dist(pivot_point,p1)<euclideian_dist(pivot_point,p2);
                }
                return x>0;
        });
        return points;
}

//it does exclude interior points--> it works
vector<movie_point> Create_Convex_hull(vector<movie_point> &points){
        vector<movie_point> stack;

        if(points.size() < 3)
               return points;

        stack.push_back(points[0]);
        stack.push_back(points[1]);
        stack.push_back(points[2]);

        for(size_t i=3; i<points.size();i++){
                while(stack.size()>1 && direction(*(stack.rbegin()+1),*(stack.rbegin()),points[i])<=0)
                        stack.pop_back();
                stack.push_back(points[i]);
        }

        return stack;
}
