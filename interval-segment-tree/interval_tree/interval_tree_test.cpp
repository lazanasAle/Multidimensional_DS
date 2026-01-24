#include "interval_tree.hpp"
#include <cstddef>
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

//This function is loading intervals from the CSV file
vector<interval<int>> load_intervals_from_csv(const string &filename, size_t num_of_data)
{
    vector<interval<int>> intervals;
    ifstream file(filename);

    if(!file.is_open())
    {
        cerr<< "Error: Couldn't open the file: "<<filename<<"\n";
        return intervals;
    }

    //skipping the 1st line that doesnt contain data
    string line;
    getline(file,line);
    size_t j = 0;

    while(j < num_of_data && getline(file,line))
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
                cerr << "Warning: skipping an invalid line: "<< line<< "\n";
            }
        }
        ++j;
    }

    file.close();
    cout<< "Successfully loaded "<< intervals.size()<< " intervals from " << filename<<"\n";

    return intervals;
}

void test_basic_ops() //testing basic operations
{
    cout << "---Basic Operations Test ---" <<"\n";

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
    cout<< "Inserted 6 intervals"<<"\n";

    interval<int> query(14,16); 
    cout << "\nInterval Search for [14,16]:" <<"\n";
    vector<interval<int>> result= tree.interval_search(query);

    cout << "Found " << result.size()<< " overlapping intervals:" <<"\n";
    for(const auto &inter: result)
    {
        cout<< "["<< inter.low<< ", "<< inter.high<< "]"<< "\n";
    }

    //stabbing query testing
    cout<< "\nStabbing Query for point 15:"<<"\n";
    vector<interval<int>> stab_result= tree.stabbing_query(15);
    cout<< "Found "<< stab_result.size()<< " intervals containing point 15:" <<"\n";

    for(const auto &inter: stab_result)
    {
        cout<< "[" <<inter.low<< ", " << inter.high<< "]" << "\n";
    }

    //display tree structure
    cout << "\nTree structure (inorder):" << "\n";
    cout << tree.stringify_tree() << "\n";

    //test update operation
    cout << "\nTesting update operation:" << "\n";
    interval<int> old_interval(15, 20);
    interval<int> new_interval(15, 25);
    tree.update(old_interval, new_interval);
    cout << "Updated [15,20] to [15,25]" << "\n";
    cout << "Tree after update: " << tree.stringify_tree() << "\n";
}

void test_csv()
{
    cout << "\n *** CSV imported data Testing ***" <<"\n";

    //loading intervals from the CSV file
    //The data is synthetically created using a python script
    vector<interval<int>> intervals= load_intervals_from_csv("Interval_Data.csv", 20000);

    if(intervals.empty())
    {
        cout<< "No intervals loaded!"<<"\n";
        cout<< "Exiting test" <<"\n";
        return;
    }

    //Building an Interval Tree
    interval_tree<int> tree(int_compare);

    auto start_build= chrono::system_clock::now();
    tree.build(intervals); //use build method instead of loop

    auto end_build=chrono::system_clock::now();
    auto build_time=chrono:: duration_cast<chrono::milliseconds>(end_build-start_build);

    //cout<< "Built the Interval Tree with "<< intervals.size()<< " intervals"<<"\n";
    cout<< "Built the Interval Tree with "<< tree.size()<< " intervals"<<"\n"; //using size method
    cout<< "Build Time: "<< build_time.count() << " ms"<<"\n";

    interval<int> query(50,100);
    cout<< "\nInterval Search for [50,100]:" <<"\n";

    auto start_search=chrono::system_clock::now();
    vector<interval<int>> result= tree.interval_search(query);
    auto end_search= chrono::system_clock::now();
    auto search_time= chrono::duration_cast<chrono::microseconds>(end_search-start_search);

    cout<< "Found "<< result.size()<< " over-lapping intervals" <<"\n";
    cout<< "Search Time: " << search_time.count()<< " μs"<< "\n";

    cout<< "First 10 results: "<<"\n";
    for(size_t i=0; i<min(result.size(), size_t(10)); ++i)
    {
        cout<< "[" <<result[i].low<< "," << result[i].high<< "]" <<"\n";
    }

    //Testing the stabbing query
    int point=75;  //may change it
    cout<< "\nStabbing Query for point "<<point << ":"<< "\n";

    auto start_stabbing_query= chrono::system_clock::now();
    vector<interval<int>> stab_result= tree.stabbing_query(point);

    auto end_stabbing_query= chrono:: system_clock::now();
    auto stab_time=chrono::duration_cast< chrono::microseconds>(end_stabbing_query-start_stabbing_query);

    cout<< "Found " << stab_result.size()<< " intervals containing point: "<< point<< "\n";
    cout<< "Stabbing query time: " << stab_time.count()<< " μs"<< "\n";

    cout<< "First 10 results:"<< "\n";
    for(size_t i=0; i< min(stab_result.size(), size_t(10)); ++i)
    {
        cout << "[" << stab_result[i].low << ", " << stab_result[i].high<<"]"<< "\n";
    }
}

void performance_test(size_t num_of_data)
{
    cout << "\n---- Performance Test ----" << "\n";
    vector<interval<int>> intervals= load_intervals_from_csv("Interval_Data.csv", num_of_data);

    if(intervals.empty())
    {
        cout << "There are no intervals loaded."<<"\n";
        cout<< "Exiting Test..."<< "\n";
        return;
    }

    interval_tree<int> tree(int_compare);

    //Time Calculations
    cout << "\nInsertion Performance:" <<"\n";
    auto start = chrono::system_clock::now();
    tree.build(intervals);

    auto end= chrono::system_clock::now();
    chrono::duration<double> build_dur = end - start;
    double build_time = build_dur.count();

    //cout << "Inserted " << intervals.size()<< " intervals"<< "\n";
    cout << "Inserted " << tree.size()<< " intervals"<< "\n";
    cout << "Total Time: "<< build_time<< " s" <<"\n";
    cout << "Average per insertion: " << build_time/ intervals.size()<< " s" <<"\n";

    //perform multiple searches
    cout<< "\n Search Performance (100 queries):" <<"\n";
    int num_queries=100;
    long total_results=0;

    start= chrono::system_clock::now();

    for(int i=0; i<num_queries; ++i)
    {
        int low=rand()%150; //
        int high =low+ (rand()%50);
        interval<int> query(low,high);
        vector<interval<int>> result= tree.interval_search(query);
        total_results+= result.size();
    }

    end=chrono::system_clock::now();
    chrono::duration<double> query_dur = end - start;
    double query_time = query_dur.count();

    cout << "Total Queries: " << num_queries<< "\n";
    cout<< "Total Time: "<<query_time<< " s"<< "\n";
    cout<< "Average per query: " << query_time/ num_queries<< " s"<< "\n";
    cout<< "Average Results per Query: " <<(double)total_results/ num_queries<< "\n";

    ofstream times_csv;
    times_csv.open("times.csv", ios::app);
    times_csv<<tree.size()<<","<<build_time<<","<<query_time<<"\n";
    times_csv.close();
}

int main(int argc, char *argv[])
{
    size_t num_of_data = 20000;
    if (argc > 1)
           num_of_data = stol(argv[1]);
    //Testing basic operations with the manual data
    test_basic_ops();

    //Now testing the operations with the synthetic data from CSV
    test_csv();

    performance_test(num_of_data);

    return 0;
}
