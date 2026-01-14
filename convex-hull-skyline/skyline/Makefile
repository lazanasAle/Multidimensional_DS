CPP = g++
CFLAGS = -Wall -O2 -std=gnu++23
LDFLAGS = -lboost_json
TARGET = kdbtree from_json

PROJ_ROOT := $(abspath ../..)
QUESTION_ROOT := $(abspath ..)

CFLAGS += -I$(QUESTION_ROOT)
CFLAGS += -I$(PROJ_ROOT)

all: $(TARGET)

kdbtree: kd_btree_test.cpp
	$(CPP) $(CFLAGS) -o kd_btree_test kd_btree_test.cpp movies_kd_btree.cpp ../movie_utils.cpp

from_json: results_from_Json.cpp
	$(CPP) $(CFLAGS) -o from_json results_from_Json.cpp movies_kd_btree.cpp ../movie_utils.cpp $(LDFLAGS)

clean:
	rm -rf kd_btree_test from_json
