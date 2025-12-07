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

//this function is loading intervals from the CSV file
vector<interval<int>> load_intervals_from_csv(const string &filename)
{
    vector<interval<int>> intervals;
    ifstream file(filename);
    
    if(!file.is_open())
    {
        cerr<< "Error: Couldn't open the file: "<<filename<<endl;
        return intervals;
    }

    //skipping the 1st line that doesnt contain data
    string line;
    getline(file,line);

    while(getline(file,line))
    {
        stringstream ss(line);
        string x_str, y_str;

        if(getline(ss,x_str,',') && getline(ss, y_str,','))
        {
            try
            {
                int x=stoi(x_str);
                int y=stoi(y_str);

                //make sure low<=high
                int low=min(x,y);
                int high= max(x,y);

                intervals.push_back(interval<int>(low,high));
            }
            catch(const exception &e)
            {
                cerr << "Warning: skipping an invalid line: "<< line<< endl;
            }
        }
    }

    file.close();
    cout<< "Successfully loaded "<< intervals.size()<< "intervals from " << filename<<endl;

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
    cout << "\nInterval Search for [14,16]:" <<endl;
    vector<interval<int>> result= tree.interval_search(query);

    cout << "Found " << result.size()<< " overlapping intervals:" <<endl;
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
    
    //loading intervals from the CSV file
    //The data is synthetically created using a python script
    vector<interval<int>> intervals= load_intervals_from_csv("Interval_Segmnet.csv");
    // the filename has a typo. i will leave it like this for now
    //might change to Interval_Segment.csv
    // i dont wanna ruin anything else by renaming it

    if(intervals.empty())
    {
        cout<< "No intervals loaded!"<<endl;
        cout<< "Exiting test" <<endl;
        return;
    }



}

void performance_test()
{
    cout << "\n---- Performance Test ----" << endl;
    vector<interval<int>> intervals= load_intervals_from_csv("Interval_Segmnet.csv");
    //also wrong name here

    if(intervals.empty())
    {
        cout << "There are no intervals loaded."<<endl;
        cout<< "Exiting Test..."<< endl;
        return;
    }

    interval_tree<int> tree(int_compare);

    //Time Calculations
    cout << "\nInsertion Performance:" <<endl;
    auto start = chrono::high_resolution_clock::now();
    for(auto &inter : intervals)
    {
        tree.insert(inter);

    }
    
    auto end= chrono::high_resolution_clock::now();
    auto duration= chrono::duration_cast<chrono::milliseconds>(end-start);

    //cout..

}

int main()
{
    //Testing basic operations with the manual data
    test_basic_ops();

    //Now testing the operations with the synthetic data from CSV
    test_csv();

    //performance_test();
    
    return 0;
}

//comments to explain what the code actually does