CPP = g++
CPPFLAGS = -Wall -O2 -std=gnu++23

PROG = segment_tree_test

all: $(PROG)

segment_tree_test: segment_tree_test.cpp
	$(CPP) $(CPPFLAGS) -o segment_tree_test segment_tree_test.cpp segment_tree.impl.cpp

clean:
	rm -rf segment_tree_test
