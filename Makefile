CXX = g++
CXXFLAGS = -g -O3 -Wall -std=c++11

all: clean compile run

clean:
	rm -f *.o *.a main

compile:
	$(CXX) $(CXXFLAGS) main.cpp -o main

run:
	cat ./samples-Practice/2.in | ./main
