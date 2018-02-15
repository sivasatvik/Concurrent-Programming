all:
	reset
	g++ -std=c++14 -pthread lazylist.cpp tsp.cpp main.cpp -o main.o -DN=4 -DSEED=15 -DC=50 -DPOP=20 -DGEN=5000
	./main.o