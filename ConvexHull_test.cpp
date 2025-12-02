#include "rapidcsv.h"
#include <iostream>
#include <vector>
#include <string>
#include <stack>


using namespace std;
using namespace rapidcsv; //it needs downloading

struct movie_point{
        double budget; //the x axis
        double popularity; //the y axis

};

vector<movie_point> read_csv(string filename){
    vector<movie_point> points;
    Document doc("data_movies_clean.csv",LabelParams(0,-1));

    vector<double> budgetV=doc.GetColumn<double>("budget");
    vector<double> popularityV=doc.GetColumn<double>("popularity");

    for(auto i=0;i<budgetV.size();i++){
        movie_point point;
        point.budget=budgetV[i];
        point.popularity=popularityV[i];
        points.push_back(point);

    }
    return points;

}

//counterclockwise function-->its corrected
int  direction(movie_point p0,movie_point p1,movie_point p2){
        double orientation=(p1.budget-p0.budget)*(p2.popularity-p0.popularity)-(p1.popularity-p0.popularity)*(p2.budget-p0.budget);
        if(orientation>0) return 1;
        else if (orientation<0) return -1;
        else return 0;
}
//tie breaker
double euclideian_dist(movie_point p0,movie_point p1){
        double dist=(p0.budget-p1.budget)*(p0.budget-p1.budget) +(p0.popularity-p1.popularity)*(p0.popularity-p1.popularity);
        return dist;
}

//it works
vector<movie_point> polar_sorting(vector<movie_point> points){
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


//change that--> problemating
// i want safety dont get to empty stack
void Create_Convex_hull(vector<movie_point> points,stack<movie_point> S){


        S.push(points[0]);
        S.push(points[1]);
        S.push(points[2]);


        for(auto i=3;i<points.size();i++){
                if(direction(points[i-2], points[i-1], points[i])>0){
                        cout<<"counterclockwise 1"<<endl;
                        S.push(points[i]);
                }
                else{
                        cout<<"clockwise 1"<<endl;
                        S.pop();
                        int j=i-2;
                        while(j>=2){ // want the begining
                                if(direction(points[j-1], points[j], points[i])>0){
                                        cout<<"counterclockwise 1.2"<<endl;
                                        S.push(points[i]);
                                        j=-1;//exit the loop
                                }
                                else{
                                        cout<<"counterclockwise 1.2"<<endl;
                                        S.pop();
                                        j--;
                                }
                        }
                }

        }

}

int main(){
    //Read csv and get the columns you need-->working
    string filename="data_movies_clean.csv";
    vector<movie_point> points;
    points=read_csv(filename);



    points=polar_sorting(points);


    //create stack S and start the convex hull-->problematic
    stack<movie_point> S;
    Create_Convex_hull(points,S);






}
