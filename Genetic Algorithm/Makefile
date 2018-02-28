all:
	reset
	g++ -std=c++17 lazylist.cpp tsp.cpp main.cpp -o main.o -pthread -DSEED=7 -DC=50 -DPOP=20 -DGEN=1000 
	./main.o
