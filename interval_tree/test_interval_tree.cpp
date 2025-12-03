#include "interval_tree.hpp"
#include <iostream>
#include <chrono>
#include <iomanip> //provides formatting manipulators for stream

using namespace std;

int int_compare(int &a, int &b)
{
    if(a<b) return -1;
    if(a>b) return 1;
    return 0;

}


void test_basic_ops() //testing basic operations
{
    cout << "---Basic Operations Test ---" <<endl;

    interval_tree<int> tree(int_compare);
    //static data for now just to check(will change later)
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

    



}

//functions...


int main()
{
    //need to test the operations.so new function for testing
    test_basic_ops();
    
    return 0;
}
