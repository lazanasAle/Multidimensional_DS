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

bool CompareBudget(const movie_point& a,const movie_point& b){
return a.budget<b.budget;
}

//counterclockwise function-->maybe i will convert to double
double counterclockwise(movie_point p0,movie_point p1,movie_point p2){
    double inprod1=p0.budget*p1.budget+p0.popularity*p1.popularity;
    double inprod2=p0.budget*p2.budget+p0.popularity*p1.popularity;
    return (inprod1*inprod2);
}

stack<movie_point> Create_Convex_hull(vector<movie_point> points){
    stack<movie_point> S;
    
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);
    
    if(counterclockwise(points[0], points[1], points[2])<=0){
        movie_point temp=S.top();
        S.pop();
        S.pop();
        S.push(temp);
    }
    
    for(auto i=3;i<points.size();i++){
        S.push(points[i]);
        if(counterclockwise(points[i-2], points[i-1], points[i])<=0){
            movie_point temp=S.top();
            S.pop();
            S.pop();
            S.push(temp);
        }
    
    }
    return S;
    
}

int main(){
    //Read csv and get the columns you need-->working
    string filename="data_movies_clean.csv";
    vector<movie_point> points;
    points=read_csv(filename);
    
   
    
    //Sort the vector-->works
    sort(points.begin(),points.end(),CompareBudget);      
    
    //create stack S and start the convex hull
    stack<movie_point> S = Create_Convex_hull(points);
    
     
    
    
    //take them csv to visualize
    string filename1="export_test.csv";
    vector<movie_point> hullPoints;
    while (!S.empty()) {
        hullPoints.push_back(S.top());
        S.pop();
    }
    reverse(hullPoints.begin(), hullPoints.end());

    
    ofstream out(filename1);
    

    
    out << "budget,popularity,is_hull\n";

    
    for (const auto& p : points) {
        out << p.budget << "," << p.popularity << ",0\n";
    }

    
    for (const auto& p : hullPoints) {
        out << p.budget << "," << p.popularity << ",1\n";
    }

    out.close();
    std::cout << "CSV file written to " << filename1 << "\n";
    
    
    
}
