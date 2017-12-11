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
  // Genetic genetic1(graph4, 10, 10000, 5, true);
  // Genetic genetic2(graph4, 10, 10000, 5, true);

	struct timeval start;
	gettimeofday(&start, 0);

	// const clock_t begin_time = clock(); // gets time
	genetic.run(); // runs the genetic algorithm
  // genetic1.run();
  // genetic2.run();
	struct timeval end;
	gettimeofday(&end, 0);
	long long duration = (end.tv_sec-start.tv_sec)*1000000LL + end.tv_usec-start.tv_usec;
	float time_val = (float)duration/1000000;
	// float time_val = float(clock () - begin_time) /  CLOCKS_PER_SEC;
	cout << "\n\nTime for to run the genetic algorithm: " << time_val/*float(clock () - begin_time) /  CLOCKS_PER_SEC*/ << " seconds.\n"; // shows time in seconds
	cout<<"----------------------------------------------------------------------------------------------\n";

	ofstream file1;
	file1.open("Plain.csv",ios::out | ios::app);
	file1<<","<<time_val<<"\n";
	file1.close();

	return 0;
}