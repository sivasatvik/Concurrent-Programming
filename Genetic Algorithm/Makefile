all:
	reset
	g++ -std=c++17 lazylist.cpp tsp.cpp main.cpp -o main.o -pthread -DSEED=11 -DC=500 -DPOP=20 -DGEN=5000 -DINF=9999
	./main.o
