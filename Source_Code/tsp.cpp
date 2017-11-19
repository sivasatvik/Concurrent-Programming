#include <iostream>
#include <algorithm>

#include "tsp.h"

using namespace std;



Graph::Graph(int vertices, int inital_vertex){
	if(vertices<1){
		cout<<"Error: The number of cities should not be less than 1"<<endl;
		exit(1);
	}
	this->vertices = vertices;
	this->initial_vertex = initial_vertex;
	this->tot_edges = 0;
	// generate_graph();
}

/*Graph::generate_graph(){

}*/


void Graph::add_edge(int v1, int v2, int weight){
	edge_map[make_pair(v1,v2)] = weight;
}


void Graph::show_graph(){
	map<pair<int,int>, int>::iterator it;
	for(it = edge_map.begin(); it != edge_map.end(); it++){
		cout<<it->first.first<<" connected to "<<it->first.second<<" with a weight of "<<it->second<<endl;
	}
}



int Graph::exists_edge(int v1, int v2){
	map<pair<int,int>, int>::iterator it = edge_map.find(make_pair(v1,v2));
	// cout<<it->second<<endl;


	if(it != edge_map.end()){
		return it->second;
	}
	return -1;
}



Genetic::Genetic(Graph *graph, int amount_pop, int generations, int mutation_rate){
	if(amount_pop<1){
		cout<<"Error: The size of the population is too low(<1)\n";
		exit(-1);
	}
	else if(mutation_rate<0 || mutation_rate>100){
		cout<<"Error: Mutation rate should be between 0 and 100\n";
		exit(-1);
	}

	this->graph = graph;
	this->pop = amount_pop;
	this->generations = generations;
	this->mutation_rate = mutation_rate;

}


int Genetic::is_valid_solution(vector<int>& solution){
	int total_cost = 0;
	set<int> sol_set; 


	/* Elements should not be repeated, hence the usage of "set"*/
	for(int i = 0; i < graph->vertices; i++){
		sol_set.insert(i);
	}

	if(sol_set.size() != graph->vertices){
		return -1;
	}

	/* Check if connections are valid */
	for (int i = 0; i < graph->vertices; ++i){
		if(i+1 < graph->vertices){
			int cost = graph->exists_edge(solution[i], solution[i+1]);
			// cout<<cost<<endl;

			/* Checking if the connections exists between the cities */
			if(cost == -1){
				return -1;
			}
			else{
				total_cost += cost;
			}
		}
		else{
			int cost = graph->exists_edge(solution[i], solution[0]);
			
			/* Checking if the connections exists between the cities */
			if(cost == -1)
				return -1;
			else
				total_cost += cost;
			break;
		}
	}
	return total_cost;

}



bool Genetic::exists_chromosome(const vector<int>& v){
	/* Check if it exits in the population*/
	for(vector<pair<vector<int>, int> >::iterator it = population.begin(); it != population.end(); it++){
		const vector<int>& vec = (*it).first;    // Gets the vector
		if(equal(v.begin(), v.end(), vec.begin()))    // Compares the vectors
			return true;
	}
	return false;
}


/* Generates the initial population */
void Genetic::initial_population(){
	vector<int> parent;

	/* Initial vertex in the parent */
	parent.push_back(graph->initial_vertex);

	/* Creates the parent */
	for(int i = 0; i < graph->vertices; i++){
		if(i != graph->initial_vertex)
			parent.push_back(i);
		// cout<<parent[i]<<" ";
	}
	// cout<<endl;

	int total_cost = is_valid_solution(parent);

	if(total_cost != -1){ 	// Checks if the parent is valid
		population.push_back(make_pair(parent, total_cost)); // Inserts in the population
		real_size_pop++; // Increments real_size_population
	}


	// Makes random permutations "generations" times
	for(int i = 0; i < generations; i++){
		// Generates a random permutation
		random_shuffle(parent.begin() + 1, parent.begin() + (rand() % (graph->vertices - 1) + 1));
		
		int total_cost = is_valid_solution(parent); // checks if solution is valid
		
		// Checks if permutation is a valid solution and if not exists
		if(total_cost != -1 && !exists_chromosome(parent))
		{
			population.push_back(make_pair(parent, total_cost)); // Add in population
			real_size_pop++; // Increments real_size_pop in the unit
		}
		if(real_size_pop == pop) // Checks size population
			break;
	}


	// Checks if real_size_pop is 0
	if(real_size_pop == 0)
		cout << "\nEmpty initial population ;( Try again runs the algorithm...";
	else
		sort(population.begin(), population.end(), sort_pred()); // Sort population
}



void Genetic::show_population(){
	cout << "\nShowing solutions...\n\n";

	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it){
		const vector<int>& vec = (*it).first; // Gets the vector
		
		for(int i = 0; i < graph->vertices; i++)
			cout << vec[i] << " ";
		cout<<graph->initial_vertex;
		cout<<" | Cost: "<<(*it).second<<"\n\n";
	}
	cout<<"\nPopulation size: "<<real_size_pop<<endl;
}


void Genetic::binary_search_insert(std::vector<int> & c1, int total_cost){
	int min_ = 0;
	int max_ = real_size_pop - 1;
	
	while(max_ >= min_){
		int mid_ = min_ + (max_ - min_) / 2;
		
		if(total_cost == population[mid_].second){
			population.insert(population.begin() + mid_, make_pair(c1, total_cost));
			return;
		}
		else if(total_cost > population[mid_].second)
			min_ = mid_ + 1;
		else
			max_ = mid_ - 1;
	}
	population.insert(population.begin() + min_, make_pair(c1, total_cost));
}



/*
	Makes the crossover
	This crossover selects two random points
	These points generates substrings in both parents
	The substring inverted of parent1 is placed in parent2 and
	the substring inverted of parent2 is placed in parent1
	
	Example:
		parent1: 1 2 3 4 5
		parent2: 1 2 4 5 3
		
		substring in parent1: 2 3 4
		substring in parent2: 2 4 5
		
		substring inverted in parent1: 4 3 2
		substring inverted in parent2: 5 4 2
		
		child1: 1 5 4 2 5
		child2: 1 4 3 2 3
		
		Children are invalids: 5 appears 2x in child1 and 3 appears 2x in child2
		Solution: map of genes that checks if genes are not used
*/


void Genetic::cross_over(vector<int>& p1, vector<int>& p2){
	vector<int> c1, c2;

	// Map of genes, checks if already are selected
	map<int, int> genes1, genes2;

	for(int i = 0; i < graph->vertices; i++){
		genes1[p1[i]] = 0;
		genes2[p2[i]] = 0;
	}

	// Generates random points
	int point1 = rand() % (graph->vertices - 1) + 1;
	int point2 = rand() % (graph->vertices - point1) + point1;

	// If the above two points are equal, adjust
	if(point1 == point2){
		if(point1-1>1){
			point1--;
		}
		else if(point2+1<graph->vertices){
			point2++;
		}
		else{
			// Randomly choose which point to change
			int random = rand() % 10 + 1;
			if(random<=5)
				point1--;
			else
				point2++;
		}
	}

	// Generate child next

	/* Until point1, child1 gets the genes of parent1
	 * and child2 gets the genes of parent2
	 */
	for (int i = 0; i < point1; ++i){
		// Adding the genes to the child
	 	c1.push_back(p1[i]);
	 	c2.push_back(p2[i]);
	 	// Marking the genes to indicate they have been passed
	 	genes1[p1[i]] = 1;
	 	genes2[p2[i]] = 1;
	}

	//  From point2 to the end of parents, child1 gets
	// * the genes of parent1 and child2 gets the genes 
	// * of parent2
	
	// for (int i = point2+1; i < graph->vertices; ++i){
	//  	/* code */
	//  } 

	/* Marking the remaining genes */
	for(int i = point2+1; i<graph->vertices; i++){
		genes1[p1[i]] = 1;
		genes2[p2[i]] = 1;
	}

	/* Now, child1 gets the inverted genes of parent2 and 
	* child2 gets the inverted genes of parent1 from point1
	* to point2
	*/

	for (int i = point2+1; i >= point1; ++i){
		if(genes1[p2[i]]==0){   //If the gene is not already used
			c1.push_back(p2[i]);
			genes1[p2[i]] = 1;   //Marking the gene
		}
		else{    //If the gene is already in use, choose the gene which is not in use
			for(map<int, int>::iterator it = genes1.begin(); it != genes1.end(); ++it){
				if(it->second == 0){ // Check if is not used
					c1.push_back(it->first);
					genes1[it->first] = 1; // Mark as used
					break; // Leave the loop
				}
			}
		}


		if(genes2[p1[i]]==0){   //If the gene is not already used
			c2.push_back(p1[i]);
			genes2[p1[i]] = 1;   //Marking the gene
		}
		else{    //If the gene is already in use, choose the gene which is not in use
			for(map<int, int>::iterator it = genes2.begin(); it != genes2.end(); ++it){
				if(it->second == 0){ // Check if is not used
					c2.push_back(it->first);
					genes2[it->first] = 1; // Mark as used
					break; // Leave the loop
				}
			}
		}
	}

	/* Remaining genes will go as follows:
	* child1 gets the genes of parent1 and 
	* child2 gets the genes of parent2 from 
	* point2 till end
	*/

	for (int i = point2+1; i < graph->vertices; ++i){
		c1.push_back(p1[i]);
		c2.push_back(p2[i]);
	}


	/* Now, coming to mutation */
	// Mutation will get a value in [1-100](random number)
	int mutation = rand() % 100 + 1;
	if(mutation <= mutation_rate){  // Make a mutation if the mutation is less than the mutation rate
		// We change two genes when mutation happens
		int i_gene1, i_gene2;
		i_gene1 = rand() % (graph->vertices - 1) + 1;
		i_gene2 = rand() % (graph->vertices - 1) + 1;

		// Mutation for child1
		int aux = c1[i_gene1];
		c1[i_gene1] = c1[i_gene2];
		c1[i_gene2] = aux;


		// Mutation for child2
		aux = c2[i_gene1];
		c2[i_gene1] = c2[i_gene2];
		c2[i_gene2] = aux;

	}

	int total_cost_c1 = is_valid_solution(c1);
	int total_cost_c2 = is_valid_solution(c2);

	/* Check whether the above solution we got is a valid one or not*/

	if(total_cost_c1 != -1 && !exists_chromosome(c1)){
		// Adding the child in the population
		binary_search_insert(c1, total_cost_c1);
		real_size_pop++;
	}
	if(total_cost_c2 != -1 && !exists_chromosome(c2)){
		// Adding the child in the population
		binary_search_insert(c2, total_cost_c2);
		real_size_pop++;
	}

}


void Genetic::run(){
	initial_population();   // Initializing population

	if(real_size_pop == 0)
		return;

	for(int i = 0; i<generations; i++){
		int old_size_pop = real_size_pop;
		/* Select two parents(if exits) which will take part in reproduction*/
		if(real_size_pop >= 2){
			if(real_size_pop == 2){
				cross_over(population[0].first, population[1].first);
			}
			/* If there are more than two parents*/
			else{
				int parent1, parent2;
				parent1 = rand() % real_size_pop;
				parent2 = rand() % real_size_pop;
				while(parent1 == parent2){
					parent1 = rand() % real_size_pop;
					parent2 = rand() % real_size_pop;
				}
				cross_over(population[parent1].first, population[parent2].first);
			}

			/* Check whether the population grew after "breeding" */
			int diff_size_pop = real_size_pop - old_size_pop;

			if(diff_size_pop == 2){
				if(real_size_pop > pop){
					/* Remove the two worst parents of the population*/
					population.pop_back();
					population.pop_back();


					/* Updating the population size*/
					real_size_pop-=2;
				}
			}
			else if(diff_size_pop == 1){
				if(real_size_pop > pop){
					/* Removing the worst possible parent*/
					population.pop_back();

					/* Updating the population size*/
					real_size_pop--;
				}
			}
		}
		else{     // If population has only one parent
			/* Crossing the parent with itself */
			cross_over(population[0].first, population[0].first);  //Gauranteed to give children with mutations
			if(real_size_pop > pop){
				/* Removing the worst possible parent*/
				population.pop_back();

				/* Updating the population size*/
				real_size_pop--;
			}
		}
	}

	cout<<"\nThe best possible solution is: ";
	const vector<int>& vec = population[0].first;
	for(int i = 0; i<graph->vertices; i++){
		cout<<vec[i]<<" ";
	}
	cout<<graph->initial_vertex;
	cout<<" | Cost: "<<population[0].second;


}




int Genetic::get_best_cost(){
	if(real_size_pop>0){
		return population[0].second;
	}
	return -1;
}

