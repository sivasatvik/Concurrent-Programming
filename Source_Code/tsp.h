#ifndef TSP
#define TSP


#include <vector>
#include <map>
#include <set>
#include <utility>
#include <ctime>
#include <cstdlib>


typedef std::pair<std::vector<int>, int> my_pair;


// Sort vector with pair
struct sort_pred{
	bool operator()(const my_pair& firstElem, const my_pair& secondElem){
		return firstElem.second < secondElem.second;
	}
};



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
	//void generate_graph();     //If given a choice to generate random graph
	void show_graph();
	int exists_edge(int v1, int v2);
	friend class Genetic;
};





class Genetic{
private:
	Graph *graph;
	int generations;
	int mutation_rate;
	int pop;
	int real_size_pop;
	std::vector<std::pair<std::vector<int>, int> > population;  // This is a vector of a pair, where one is another vector and the second one is the cost
	void initial_population();
public:
	Genetic(Graph *graph, int amount_pop, int generations, int mutation_rate);
	int is_valid_solution(std::vector<int>& solution);
	void show_population();
	void cross_over(std::vector<int>& p1, std::vector<int>& p2);
	int get_best_cost();
	bool exists_chromosome(const std::vector<int>& v);
	void binary_search_insert(std::vector<int> & c1, int total_cost);
	void run();

};



#endif
