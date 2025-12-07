#include "interval_tree.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip> //provides formatting manipulators for stream
#include <vector>
#include <string>

using namespace std;

int int_compare(int &a, int &b)
{
    if(a<b) return -1;
    if(a>b) return 1;
    return 0;

}

//Will import data from Stamatia's synthetic dataset

vector<interval<int>> load_intervals_from_csv(const string &filename)
{
    vector<interval<int>> intervals;
    ifstream file(filename);
    //....

    return intervals;
}


void test_basic_ops() //testing basic operations
{
    cout << "---Basic Operations Test ---" <<endl;

    interval_tree<int> tree(int_compare);

    //Manual test data
    //for example:
    interval<int> i1(15,20);
    interval<int> i2(10,30);
    interval<int> i3(17,19);
    interval<int> i4(5,20);
    interval<int> i5(12,15);
    interval<int> i6(30,40);

    tree.insert(i1);
    tree.insert(i2);
    tree.insert(i3);
    tree.insert(i4);
    tree.insert(i5);
    tree.insert(i6);
    cout<< "Inserted 6 intervals"<<endl;
    
    interval<int> query(14,16); //just testing. will delete these numbers soon
    cout << "\n Interval Search for [14,16]:" <<endl;
    vector<interval<int>> result= tree.interval_search(query);

    cout << "Found" << result.size()<< " overlapping intervals:" <<endl;
    for(const auto &inter: result)
    {
        cout<< "["<< inter.low<< ", "<< inter.high<< "]"<< endl;

    }

    //stabbing query testing
    cout<< "\nStabbing Query for point 15:"<<endl;
    vector<interval<int>> stab_result= tree.stabbing_query(15);
    cout<< "Found "<< stab_result.size()<< " intervals containing point 15:" <<endl;

    for(const auto &inter: stab_result)
    {
        cout<< "[" <<inter.low<< ", " << inter.high<< "]" << endl;
    }
}

void test_csv()
{
    cout << "\n *** CSV Import Test ***" <<endl;
    
    //vector<
}

void performance_test()
{

}

int main()
{
    //need to test the operations.so new function for testing
    test_basic_ops();

    
    return 0;
}

//comments to explain what the code actually does