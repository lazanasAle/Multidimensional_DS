CPP = g++
CPPFLAGS = -Wall -O2 -std=gnu++23 -I..

PROG = test_rtree

all: $(PROG)

test_rtree: test_rtree.cpp rtree.hpp rtree.impl.hpp ../kd-btree/kd_btree.hpp ../kd-btree/kd_btree.impl.hpp
	$(CPP) $(CPPFLAGS) -o test_rtree test_rtree.cpp

clean:
	rm -rf test_rtree records.bin