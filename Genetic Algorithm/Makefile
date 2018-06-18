all:
	reset
	g++ -std=c++17 -D_REENTRANT -fno-strict-aliasing lockfree.cpp tsp.cpp main.cpp -o main.o -lpthread -lrt -lm -DM=4 -DSEED=11 -DC=500 -DPOP=20 -DGEN=5000 -DINF=9999
	./main.o
