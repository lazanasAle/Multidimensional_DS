CPP = g++
CFLAGS = -Wall -O2 -std=gnu++23
TARGET = kdbtree

all: $(TARGET)

kdbtree: kd_btree_test.cpp
	$(CPP) $(CFLAGS) -o kd_btree_test kd_btree_test.cpp movies_kd_btree.cpp ../movie_utils.cpp

clean:
	rm -rf kd_btree_test
