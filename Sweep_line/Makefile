CC = g++
CFLAGS = -std=gnu++23 -Wall -O2 -Iinclude

all: sweep_line

sweep_line: Sweep_main.cpp
	$(CC) $(CFLAGS) -o SweepLine Sweep_main.cpp Sweep_Line_impl.cpp

clean:
	rm -rf SweepLine
