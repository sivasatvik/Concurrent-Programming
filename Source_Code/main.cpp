#include <iostream>
#include "tsp.h"
using namespace std;

int main()
{
	srand(SEED); // random numbers

	// creates random graph, parameter true is for generate the graph
	Graph * graph4 = new Graph(C, 0, true);
	// graph4->showInfoGraph();
  // graph4->showGraph();

	// parameters: the graph, population size, generations and mutation rate
	// optional parameters: show_population
	Genetic genetic(graph4, POP, GEN, 5, true);
	// genetic.initialPopulation(); // gets initial my_population
	
	// exit(1);

	struct timeval start;
	gettimeofday(&start, 0);
	// genetic.run(); // runs the genetic algorithm
	// const clock_t begin_time = clock(); // gets time
	vector<thread> threads;
	for(int i = 0; i<num_proc; i++){
		threads.push_back(std::thread(&Genetic::run, &genetic, i));
	}

	for(auto &th : threads) th.join();

	genetic.getResult();

  
	struct timeval end;
	gettimeofday(&end, 0);
	long long duration = (end.tv_sec-start.tv_sec)*1000000LL + end.tv_usec-start.tv_usec;
	float time_val = (float)duration/1000000;
	cout << "\n\nTime for to run the genetic algorithm: " << time_val/*float(clock() - begin_time)/CLOCKS_PER_SEC*/ << " seconds.\n\n"; // shows time in seconds
	cout<<"----------------------------------------------------------------------------------------------\n";

	ofstream file1;
	file1.open("Multi-threaded.csv",ios::out | ios::app);
	file1<<","<<time_val<<"\n";
	file1.close();
	return 0;
}
