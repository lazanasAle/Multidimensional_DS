CPP = g++
CPPFLAGS = -Wall -O2 -std=gnu++23 -I..
BOOST_LIBS = -lboost_system

PROG = test_rtree
PROG_JSON = test_rtree_json


all: $(PROG) $(PROG_JSON)

test_rtree: test_rtree.cpp rtree.hpp rtree.impl.hpp ../kd-btree/kd_btree.hpp ../kd-btree/kd_btree.impl.hpp
	$(CPP) $(CPPFLAGS) -o test_rtree test_rtree.cpp

test_rtree_json: test_rtree_json.cpp rtree.hpp rtree.impl.hpp ../kd-btree/kd_btree.hpp ../kd-btree/kd_btree.impl.hpp
	$(CPP) $(CPPFLAGS) -o test_rtree_json test_rtree_json.cpp $(BOOST_LIBS)

clean:
	rm -rf test_rtree test_rtree_json records.bin rtree_query.json