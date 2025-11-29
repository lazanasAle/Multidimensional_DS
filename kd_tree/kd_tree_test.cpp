#include <iostream>
#include <string>
#include <cstdlib>
#include "kd_tree.hpp"

using std::function, std::pair, std::vector, std::string, std::cout, std::make_pair;

struct record {
	int d1;
	double d2;
	string d3;
};

int cmp1(record &r1, record &r2) {
	return (r1.d1 - r2.d1);
}

int cmp2(record &r1, record &r2) {
	return (r1.d2 - r2.d2);
}

int cmp3(record &r1, record &r2) {
	return r1.d3.compare(r2.d3);
}

int main() {
	vector<function<int (record &, record &)>> comparators = {cmp1, cmp2, cmp3};
	vector<string> names = {"Jack", "Jill", "Mary", "Mark", "John", "Uwe", "Andreas", "Lawren"};
	kd_tree<record> kdt(comparators);
	
	for (size_t j = 0; j < names.size(); ++j) {
		int random_num = rand();
		int jj = j;
		record r = {random_num % 11 + jj, random_num % 11 + 0.78 + jj, names[j]};
		kdt.insert(r);
	}
	vector<record> pre = kdt.preorder();
	vector<record> in = kdt.inorder();
	
	cout<<"preorder\n";
	
	for (record &r : pre) {
		cout<<"d1 = "<<r.d1<<" d2 = "<<r.d2<<" d3 = "<<r.d3<<"\n";
	}
	
	cout<<"inoder\n";
	
	for (record &r : in) {
		cout<<"d1 = "<<r.d1<<" d2 = "<<r.d2<<" d3 = "<<r.d3<<"\n";
	}
	
	record rec = {9, 10, "kyrio Markos"};
	
	kd_node_iterator<record> rec_it = kdt.find(rec);
	
	record r = rec_it.context();
	cout<<"found:\n";
	cout<<"d1 = "<<r.d1<<" d2 = "<<r.d2<<" d3 = "<<r.d3<<"\n";
	kdt.erase(r);
	
	record rec1 = {5, 4, "A"};
	record rec2 = {9, 10, "Z"};
	pair<record, record> interval = make_pair(rec1, rec2);
	vector<record> vec_rectangle = kdt.range_search(interval);
	
	cout<<"found them all in rectangle\n";
	for (record &r : vec_rectangle) {
		cout<<"d1 = "<<r.d1<<" d2 = "<<r.d2<<" d3 = "<<r.d3<<"\n";
	}
	
}
