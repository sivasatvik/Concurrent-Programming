#include <iostream>
#include "tsp.h"
using namespace std;


int main()
{
	// Seed for same random cities generated for sequence and parallel code
	srand(SEED); 

	// creates random graph, parameter 3 is true is for generate the graph
	Graph * graph4 = new Graph(C, 0, true);
	// graph4->showInfoGraph();
	// graph4->showGraph();

	// parameters: the graph, population size, generations and mutation rate
	// optional parameters: show_population
	Genetic genetic(graph4, POP, GEN, 5, true);

	// gets initial my_population
	// genetic.initialPopulation(); 
	
	
	struct timeval start;
	gettimeofday(&start, 0);
	
	// Vector of created threads
	vector<thread> threads;
	for(int i = 0; i<num_proc; i++){
		threads.push_back(std::thread(&Genetic::run, &genetic, i));
	}

	// Wait for threads to finish
	for(auto &th : threads) th.join();
	struct timeval end;
	gettimeofday(&end, 0);

	// Get final minimum of results from each thread
	genetic.getResult();  
	long long duration = (end.tv_sec-start.tv_sec)*1000000LL + end.tv_usec-start.tv_usec;
	float time_val = (float)duration/1000000;

	cout << "\n\nTime for to run the genetic algorithm: " << time_val  << " seconds.\n\n"; // shows time in seconds
	cout<<"----------------------------------------------------------------------------------------------\n";

	ofstream file1;
	file1.open("Multi-threaded.csv",ios::out | ios::app);
	file1<<","<<time_val<<"\n";
	file1.close();
	return 0;
}
