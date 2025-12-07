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
    cout<< "Successfully loaded "<< intervals.size()<< " intervals from " << filename<<endl;

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
    cout << "\n *** CSV imported data Testing ***" <<endl;
    
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

    //Building an Interval Tree
    interval_tree<int> tree(int_compare);

    auto start_build= chrono::high_resolution_clock::now();
    for (auto &inter: intervals)
    {
        tree.insert(inter);
    }

    auto end_build=chrono::high_resolution_clock::now();
    auto build_time=chrono:: duration_cast<chrono::milliseconds>(end_build-start_build);

    cout<< "Built the Interval Tree with "<< intervals.size()<< " intervals"<<endl;
    cout<< "Build Time: "<< build_time.count() << " ms"<<endl;

    interval<int> query(50,100);
    cout<< "\nInterval Search for [50,100]:" <<endl;

    auto start_search=chrono::high_resolution_clock::now();
    vector<interval<int>> result= tree.interval_search(query);
    auto end_search= chrono::high_resolution_clock::now();
    auto search_time= chrono::duration_cast<chrono::microseconds>(end_search-start_search);

    cout<< "Found "<< result.size()<< " over-lapping intervals" <<endl;
    cout<< "Search Time: " << search_time.count()<< " μs"<< endl;

    cout<< "First 10 results: "<<endl;
    for(size_t i=0; i<min(result.size(), size_t(10)); ++i)
    {
        cout<< "[" <<result[i].low<< "," << result[i].high<< "]" <<endl;
    }

    //Testing the stabbing query
    int point=75;  //may change it
    cout<< "\nStabbing Query for point "<<point << ":"<< endl;

    auto start_stabbing_query= chrono::high_resolution_clock::now();
    vector<interval<int>> stab_result= tree.stabbing_query(point);
    
    auto end_stabbing_query= chrono:: high_resolution_clock::now();
    auto stab_time=chrono::duration_cast< chrono::microseconds>(end_stabbing_query-start_stabbing_query);

    cout<< "Found " << stab_result.size()<< " intervals containing point: "<< point<< endl;
    cout<< "Stabbing query time: " << stab_time.count()<< " μs"<< endl;

    cout<< "First 10 results:"<< endl;
    for(size_t i=0; i< min(stab_result.size(), size_t(10)); ++i)
    {
        cout << "[" << stab_result[i].low << ", " << stab_result[i].high<<"]"<< endl;
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

    cout << "Inserted " << intervals.size()<< " intervals"<< endl;
    cout << "Total Time: "<< duration.count()<< " ms" <<endl; 
    cout << "Average per insertion: " << (double)duration.count()/ intervals.size()<< " ms" <<endl;

    //perform multiple searches
    cout<< "\n Search Performance (100 queries):" <<endl;
    int num_queries=100;
    long total_results=0;

    start= chrono::high_resolution_clock::now();

    for(int i=0; i<num_queries; ++i)
    {
        int low=rand()%150; //
        int high =low+ (rand()%50);
        interval<int> query(low,high);
        vector<interval<int>> result= tree.interval_search(query);
        total_results+= result.size();
    }

    end=chrono::high_resolution_clock::now();
    duration= chrono::duration_cast<chrono::milliseconds>(end-start);

    cout << "Total Queries: " << num_queries<< endl;
    cout<< "Total Time: "<< duration.count()<< " ms"<< endl;
    cout<< "Average per query: " << (double)duration.count()/ num_queries<< " ms"<< endl;
    cout<< "Average Results per Query: " <<(double)total_results/ num_queries<< endl;

}

int main()
{
    //Testing basic operations with the manual data
    test_basic_ops();

    //Now testing the operations with the synthetic data from CSV
    test_csv();

    performance_test();
    
    return 0;
}

//comments to explain what the code actually does