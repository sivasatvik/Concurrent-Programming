#include <iostream>
#include "tsp.h"


using namespace std;



int main(){
	srand(time(NULL)); // random numbers
	
	// creates the graph1 with parameters: number of vertexes and initial vertex
	Graph * graph1 = new Graph(5, 0);
	// add edges
	graph1->add_edge(0, 1, 1);
	graph1->add_edge(1, 0, 1);
	graph1->add_edge(0, 2, 3);
	graph1->add_edge(2, 0, 3);
	graph1->add_edge(0, 3, 4);
	graph1->add_edge(3, 0, 4);
	graph1->add_edge(0, 4, 5);
	graph1->add_edge(4, 0, 5);
	graph1->add_edge(1, 2, 1);
	graph1->add_edge(2, 1, 1);
	graph1->add_edge(1, 3, 4);
	graph1->add_edge(3, 1, 4);
	graph1->add_edge(1, 4, 8);
	graph1->add_edge(4, 1, 8);
	graph1->add_edge(2, 3, 5);
	graph1->add_edge(3, 2, 5);
	graph1->add_edge(2, 4, 1);
	graph1->add_edge(4, 2, 1);
	graph1->add_edge(3, 4, 2);
	graph1->add_edge(4, 3, 2);

	graph1->show_graph();

	Genetic genetic(graph1, 10, 1000, 5);

	const clock_t begin_time = clock(); 
	genetic.run(); 
	cout << "\n\nTime for to run the genetic algorithm: " << float(clock () - begin_time) /  CLOCKS_PER_SEC << " seconds.\n"; // shows time in seconds
	
	return 0;
}