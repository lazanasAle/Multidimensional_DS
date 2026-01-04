#ifndef _CONVEXHULL_HPP
#define _CONVEXHULL_HPP

//SOS Download the rapidcsv before executing the code
#include "rapidcsv.h"
#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cstddef>
#include <list> // maybe i dont need

using namespace std;

struct movie_point{
        double budget; //the x axis
        double popularity; //the y axis

};

vector<movie_point> read_csv(string filename);
int  direction(const movie_point &p0, const movie_point &p1, const movie_point &p2);
double euclideian_dist(const movie_point &p0, const movie_point &p1);
vector<movie_point> polar_sorting(vector<movie_point> &points);
vector<movie_point> Create_Convex_hull(vector<movie_point> &points);


#endif /* _CONVEXHULL_HPP */
