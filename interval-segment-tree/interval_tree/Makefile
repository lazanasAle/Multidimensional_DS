CPP = g++
CPPFLAGS = -Wall -O2 -std=gnu++23
PROG = interval_tree_test

all: $(PROG)

interval_tree_test: interval_tree_test.cpp interval_tree.hpp interval_tree.impl.hpp
	$(CPP) $(CPPFLAGS) -o interval_tree_test interval_tree_test.cpp 

clean:
	rm -rf interval_tree_test

