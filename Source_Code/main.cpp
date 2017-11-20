#include <iostream>
#include "tsp.h"
using namespace std;

int main()
{
	srand(SEED); // random numbers

	// creates random graph, parameter true is for generate the graph
	Graph * graph4 = new Graph(20, 0, true);
	graph4->showInfoGraph();
  // graph4->showGraph();

	// parameters: the graph, population size, generations and mutation rate
	// optional parameters: show_population
	Genetic genetic(graph4, N, 10000, 5, true);
  // Genetic genetic1(graph4, N, 1000, 5, true);
  // Genetic genetic2(graph4, N, 1000, 5, true);
	// genetic.initialPopulation(); // gets initial my_population
	
	// exit(1);
	vector<thread> threads;
	for(int i = 0; i<num_proc; i++){
		threads.push_back(std::thread(&Genetic::run, &genetic, i));
	}

	for(auto &th : threads) th.join();

	genetic.getResult();

	
	const clock_t begin_time = clock(); // gets time
	// genetic.run(); // runs the genetic algorithm
  // genetic1.run();
  // genetic2.run();
	cout << "\n\nTime for to run the genetic algorithm: " << float(clock () - begin_time) /  CLOCKS_PER_SEC << " seconds.\n"; // shows time in seconds

	return 0;
}
