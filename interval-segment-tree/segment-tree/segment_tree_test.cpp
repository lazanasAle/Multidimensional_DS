#include "segment_tree.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>


using namespace std;

int main() {
        ifstream txt_file;
        txt_file.open("dataset.txt");
        vector<size_t> build_arr;

        string num_str;
        //loop to read the file contents
        while (getline(txt_file, num_str)) {
                size_t num = stol(num_str);
                build_arr.push_back(num);
        }
        txt_file.close();
        //build the tree
        segment_tree stree(build_arr);

        cout<<stree.stringnify_tree()<<"\n";
}
