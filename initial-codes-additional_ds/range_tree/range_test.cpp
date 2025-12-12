#include "2D_range_tree.hpp"
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <cstdlib>

using namespace std;

struct test_record {
	int key1;
	int key2;
	string name;

	test_record(int key1, int key2, string name) {
		this->key1 = key1;
		this->key2 = key2;
		this->name = name;
	}
	test_record() {}
};

ostream &operator << (ostream &os, const test_record tr) {
	os<<" Key_1: "<<tr.key1<<" Key_2: "<<tr.key2<<" name: "<<tr.name<<"\n";
	return os;
}

struct comparator_first {
	int operator ()(const test_record &a, const test_record &b) const {
		return (a.key1 - b.key1);
        }
};

struct comparator_sec {
	bool operator ()(const test_record &a, const test_record &b) const {
		return (a.key2 < b.key2);
	}
};

int main() {
        two_d_range_tree<test_record, comparator_first, comparator_sec> rt;
        vector<string> names = {"Jack", "Jill", "Cameron", "Dias", "Alexia", "Markov", "Laplace", "Levoir", "Lavoisier", "Motagna"};
        for (size_t j=0; j < 10; ++j) {
                int xi = rand()%10;
    	        int yi = rand()%10 + 2;
    	        test_record tr(xi, yi, names[j]);
    	        rt.insert(tr);
    	}
    	vector<two_d_key<test_record, comparator_sec>> pr = rt.preorder();
    	vector<two_d_key<test_record, comparator_sec>> in = rt.inorder();

    	cout<<"preorder:\n";
    	for (auto &rec : pr) {
    		cout<<rec.key;
    	}
    	cout<<"inorder:\n";
    	for (auto &rec : in) {
    		cout<<rec.key;
    	}

    	test_record ts(6, 3, "Jim");
    	two_d_range_node_iterator<test_record, comparator_sec> six = rt.find(ts);
    	two_d_range_node_iterator<test_record, comparator_sec> seven = six.next();
    	cout<<"next is: "<<seven.context();
    	if (!six.is_null()) {
    		set<test_record, comparator_sec> *rbsec = six.secondary();
    		cout<<"inner tree\n";
    		for (auto &rec : *rbsec) {
    			cout<<rec;
    		}
    	}
    	test_record test_r(8, 1, "Alekan");
    	two_d_range_node_iterator<test_record, comparator_sec> upper = rt.upper_bound(test_r);
    	cout<<"upper bound of 8 is\n";
        cout<<upper.context();
        cout<<"next to the bound is\n";
        cout<<upper.next().context();

        two_d_range_node_iterator<test_record, comparator_sec> lower = rt.lower_bound(test_r);
    	cout<<"lower bound of 8 is\n";
        cout<<lower.context();
        cout<<"prev to the bound is\n";
        cout<<lower.prev().context();

        test_record three(3, 4, "Janani");
        test_record second(8, 8, "Maya");

        pair<test_record, test_record> interval = make_pair(three, second);

        vector<test_record> ve = rt.two_d_range_search(interval);

        cout<<"range_search result\n";

        for (auto &rec : ve) {
        		cout<<rec;
        }

    	for (auto &rec : in)
    	    rt.erase(rec.key);
    	cout<<rt.empty()<<"\n";
}
