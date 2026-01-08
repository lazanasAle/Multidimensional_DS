CPP = g++
CPPFLAGS = -Wall -O2 -std=gnu++23 -I..
BOOST_LIBS = -lboost_json

PROG = test_rtree
PROG_JSON = rtree_from_json

all: $(PROG) $(PROG_JSON)

test_rtree: test_rtree.cpp rtree.hpp rtree.impl.hpp ../kd-btree/kd_btree.hpp ../kd-btree/kd_btree.impl.hpp
	$(CPP) $(CPPFLAGS) -o test_rtree test_rtree.cpp

rtree_from_json: rtree_from_json.cpp rtree.hpp rtree.impl.hpp ../kd-btree/kd_btree.hpp ../kd-btree/kd_btree.impl.hpp
	$(CPP) $(CPPFLAGS) -o rtree_from_json rtree_from_json.cpp $(BOOST_LIBS)

clean:
	rm -rf test_rtree rtree_from_json records.bin rtree_query.json