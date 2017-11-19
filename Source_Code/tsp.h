#ifndef TSP
#define TSP


#include <vector>
#include <map>
#include <set>
#include <utility>
#include <ctime>
#include <cstdlib>



/* Graph class to represent the graph of the city */

class Graph{
private:
	int vertices;       // Total number of cities the Salesman should travel
	int tot_edges;
	int initial_vertex;  // Starting point of the Salesman
	std::map<std::pair<int,int>, int> edge_map;   // Mapping the edges 
public:
	/* Constructor */
	Graph(int vertices, int initial_vertex);
	void add_edge(int v1, int v2, int weight);
	void show_graph();
	int exists_edge(int v1, int v2);
	friend class Genetic;
};





class Genetic{

};
