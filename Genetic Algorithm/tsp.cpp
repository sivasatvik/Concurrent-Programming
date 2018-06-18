#include "tsp.h"
using namespace std;



// constructor for graph
Graph::Graph(int V, int initial_vertex, bool random_graph) // constructor of Graph
{
	if(V < 1) // checks if number of vertexes is less than 1
	{
		cout << "Error: number of vertexes <= 0\n";
		exit(1);
	}

	this->V = V; // assigns the number of vertices
	this->initial_vertex = initial_vertex; // assigns initial vertex
	this->total_edges = 0; // initially the total of edges is 0

	// if 3rd argument is true
	
	generatesGraph();
	// showGraph();
	floydWarshall();
	// showGraph();
	cout << "FloydWarshall distace == calculated\n\n";
}



// Generates random graph
void Graph::generatesGraph()
{
	// Vector of vertices
	vector<int> vec;
	for(int i = 0; i < V; i++)
		vec.push_back(i);

	// generates a random permutation of vertices
	random_shuffle(vec.begin(), vec.end());

	initial_vertex = vec[0]; // initial vertex = 0

	int i, weight;
	// Connect all the edges of graph
	for(i = 0; i <= V; i++)
	{
		weight = rand() % 100 + 1; // random weight in range [1,100]
		// cout<<weight<<endl;
		if(i + 1 < V)
			addEdge(vec[i], vec[i + 1], weight);
		else
		{
			// add last edge
			addEdge(vec[i], vec[0], weight);
			break;
		}
	}

	int limit_edges = V * (V - 1); // calculates the limit of edges
	int size_edges = rand() % (2 * limit_edges) + limit_edges;

	// add some extra edges in graph
	for(int i = 0; i < 2*V; i++)
	{
		int src = rand() % V; // random source
		int dest = rand() % V; // random destination
		weight = rand() % 100 + 1; // random weight in range [1,V]
		if(src != dest)
		{
			if(rand()%10>7){
				weight = INF;
				addEdge(vec[src], vec[dest], weight);
			}
			else{
				addEdge(vec[src], vec[dest], weight);
			}
			if(rand()%10>7){
				weight=INF;
				addEdge(vec[dest], vec[src], weight);
			}	
			else{
				addEdge(vec[dest], vec[src], weight);
			}
		}
	}
}

// Prints info about graph (i.e  V, num_edges )
void Graph::showInfoGraph()
{
	cout << "Showing info of graph:\n\n";
	cout << "Number of vertices: " << V;
	cout << "\nNumber of edges: " << map_edges.size() << "\n";
}


// Adds and edge in graph
void Graph::addEdge(int src, int dest, int weight) // add a edge
{
	map_edges[make_pair(src, dest)] = weight; // adds edge in the map
}



// Print graph
void Graph::showGraph() // shows all connections of the graph
{
	map<pair<int, int>, int>::iterator it;
	for(it = map_edges.begin(); it != map_edges.end(); ++it)
		cout << it->first.first << " linked to vertex " << it->first.second << " with weight " << it->second << endl;
}

// Checks if edge exist in graph 
int Graph::existsEdge(int src, int dest) // checks if exists a edge
{
	map<pair<int, int>,int>::iterator it = map_edges.find(make_pair(src, dest));

	if(it != map_edges.end())
		return it->second; // returns cost

	// cout <<   __LINE__  << " -->  " <<  it->second <<  " " << src << " " << dest << endl;
	return 0;
}


// constructor of Genetic 
Genetic::Genetic(Graph* graph, int size_population, int generations, int mutation_rate, bool show_population)
{
	if(size_population < 1) // checks if size of population is less than 1
	{
		cout << "Error: size_population < 1\n";
		exit(1);
	}
	else if(mutation_rate < 0 || mutation_rate > 100) // checks if mutation rate is less than 0
	{
		cout << "Error: mutation_rate must be >= 0 and <= 100\n";
		exit(1);
	}
	this->graph = graph;
	this->size_population = size_population;
	this->the_list = list_new();
	// each thread's stack polulation size is initialized to zero
	this->real_size_population = 0;
	this->generations = generations;
	this->mutation_rate = mutation_rate;
	this->show_population = show_population;

	// cout<<__LINE__<<" In genetic constructor before initialPopulation()"<<endl;
	initialPopulation();
	// cout<<__LINE__<<" In genetic constructor after initialPopulation()"<<endl;
}


// checks if is a valid solution, then return total cost of path else return -1
int Genetic::isValidSolution(vector<int>& solution)
{
	int total_cost = 0;
	set<int> set_solution;

	// cout<<__LINE__<<" Creating a set for solution checking"<<endl;
	// checks if not contains elements repeated
	for(int i = 0; i < graph->V; i++)
		set_solution.insert(solution[i]);

	// If all vertices are not there then invalid
	if(set_solution.size() != (unsigned)graph->V)
		return -1;


	// cout<<__LINE__<<" Checking if the connections in the solution are valid"<<endl;
	// checks if connections are valid
	for(int i = 0; i < graph->V; i++)
	{
		if(i + 1 <  graph->V)
		{
			int cost = graph->existsEdge(solution[i], solution[i+1]);

			// checks if exists connection
			if(cost == -1)
				return -1;
			else
				total_cost += cost;
		}
		else
		{
			int cost = graph->existsEdge(solution[i], solution[0]);

			// checks if exists connection
			if(cost == -1)
				return -1;
			else
				total_cost += cost;
			break;
		}
	}

	// cout<<__LINE__<<" Returning the total cost calculated"<<endl;
	return total_cost;
}

// Check if vector v already in thread's population stack
/*bool Genetic::existsChromosome(const vector<int> & v)
{
	// checks if exists in the population
	for(vector<pair<vector<int>, int> >::iterator it=th_population[thread_id].begin(); it!=th_population[thread_id].end(); ++it)
	{
		const vector<int>& vec = (*it).first; // gets the vector
		if(equal(v.begin(), v.end(), vec.begin())) // compares vectors
			return true;
	}
	return false;
}*/
// Check if vector v already in thread's population stack
// bool Genetic::existsChromosome(const vector<int> & v)
// {
// 	// checks if exists in the population
// 	for(forward_list<my_pair>::iterator it=population2.begin(); it!=population2.end(); ++it)
// 	{
// 		const vector<int>& vec = (*it).first; // gets the vector
// 		if(equal(v.begin(), v.end(), vec.begin())) // compares vectors
// 			return true;
// 	}
// 	return false;
// }

// Creats n random solution at start
void Genetic::initialPopulation() // generates the initial population
{
	vector<int> parent;
	main_pop_size = 0;
	// inserts initial vertex in the parent
	parent.push_back(graph->initial_vertex);

	// creates the parent
	for(int i = 0; i < graph->V; i++)
	{
		if(i != graph->initial_vertex)
			parent.push_back(i);
	}

	int total_cost = isValidSolution(parent);

	// cout<<__LINE__<<" Created initial parent"<<endl;

	if(total_cost != -1) // checks if the parent is valid
	{
		// population2.push_front(make_pair(parent, total_cost)); // inserts in the population
		// population3.add(make_pair(parent, total_cost));
		list_add(the_list,make_pair(parent, total_cost));
		main_pop_size++; // increments main_pop_size
	}


	// cout<<__LINE__<<" Added a solution to the lazy list"<<endl;

	//cout<<__LINE__<<" "<<population3<<endl;

	// makes random permutations "generations" times
	for(int i = 0; i < generations; i++)
	{
		// generates a random permutation
		random_shuffle(parent.begin() + 1, parent.begin() + (rand() % (graph->V - 1) + 1));

		int total_cost = isValidSolution(parent); // checks if solution is valid

		// cout<<__LINE__<<" Checking whether it's a solution or not"<<endl;

		// checks if permutation is a valid solution and if not exists
		if(total_cost != -1 && !list_contains(the_list,make_pair(parent,total_cost)))
		{
			// population32.push_front(make_pair(parent, total_cost)); // add in population3
			// cout<<__LINE__<<" Adding solution to the lazy list"<<endl;
			// population3.add(make_pair(parent, total_cost));
			list_add(the_list,make_pair(parent, total_cost));
			main_pop_size++; // increments main_pop_size in the unit

			// cout<<__LINE__<<" Solution == added to the lazy list, cost: "<<total_cost<<endl;
      // cout<<main_pop_size<<endl;
		}
		if(main_pop_size == size_population) // checks size population
		{
			break;
		}
	}

	// cout<<__LINE__<<" Created a number of solutions for the problem"<<endl;

	// checks if main_pop_size is 0
	if(main_pop_size == 0)
		cout << "\nEmpty initial population ;( Try again\n";
	else
		// forward_list::sort(population2.begin(), population2.end(), sort_pred()); // sort population
	


	// This thing not needed in solution2
	/*int i = 0;
	for(i = 0; i+num_proc<main_pop_size; i+=num_proc)
	{
		for (int j = 0; j < num_proc; j++)
		{
	  		th_population[j].push_back(population[i+j]);
	  	}
	}

	int z = main_pop_size-i;
	for(int j = 0; j<z; j++){
		th_population[j].push_back(population[i+j]);
	}*/

	/*for(int o=0;o<num_proc;o++)
	{
		real_size_population[o] = th_population[o].size();
		// cout << "Initial Thread Populations \n";
		// showPopulation(o);
	}*/
	// cout<<__LINE__<<" Population 3 ka size: "<<population3.get_size()<<endl;
	real_size_population = list_size(the_list);
	cout<<"----------------------------------------------------------------------------------------------"<<endl;
	// showPopulation();
	return;
}



/* Should Modify this to work with Lazy list*/


// Show population stack of thread
/*void Genetic::showPopulation(int thread_id)
{
	cout << "\nShowing " << thread_id << "'s Population...\n\n";
	for(vector<pair<vector<int>, int> >::iterator it=th_population[thread_id].begin(); it!=th_population[thread_id].end(); ++it)
	{
		const vector<int>& vec = (*it).first; // gets the vector

		for(int i = 0; i < graph->V; i++)
			cout << vec[i] << " ";
		cout << graph->initial_vertex;
		cout << " | Cost: " << (*it).second << "\n\n";
	}
	cout << "\nthread_id = " << thread_id << ", Population size: " << real_size_population[thread_id] << endl;
}*/
// Show main population stack
void Genetic::showPopulation()
{
	cout << "\nInitialPopulation complete...\n\n";
	for(forward_list<my_pair>::iterator it=population2.begin(); it!=population2.end(); ++it)
	{
		const vector<int>& vec = (*it).first; // gets the vector

		for(int i = 0; i < graph->V; i++)
			cout << vec[i] << " ";
		cout << graph->initial_vertex;
		cout << " | Cost: " << (*it).second << "\n\n";
	}
	cout << "\nPopulation size: " << main_pop_size << endl;
}

/*--------------------------------------------------------------------------*/
// inserts in the vector using binary search
/*void Genetic::insertBinarySearch(vector<int>& child, int total_cost, int thread_id)
{
	// cout << "insertBinarySearch start " << total_cost << " \n" ; 
	int imin = 0;
	int imax = real_size_population[thread_id] - 1;

	while(imax >= imin)
	{
		int imid = imin + (imax - imin) / 2;

		if(total_cost == th_population[thread_id][imid].second)
		{
			th_population[thread_id].insert(th_population[thread_id].begin() + imid, make_pair(child, total_cost));
			return;
		}
		else if(total_cost > th_population[thread_id][imid].second)
			imin = imid + 1;
		else
			imax = imid - 1;
	}
	th_population[thread_id].insert(th_population[thread_id].begin() + imin, make_pair(child, total_cost));
	// cout << "insertBinarySearch end \n";
}*/

// inserts in the vector using binary search
/*void Genetic::insertBinarySearch(vector<int>& child, int total_cost)
{

	int imin = 0;
	int imax = main_pop_size - 1;
	for(forward_list<my_pair>::iterator it = population2.begin(); it != population2.end() ;  ++it)
	{
		if(total_cost <= it.second)
		{
			population2.insert_before(it, make_pair(child, total_cost));
			return;	
		}
	}
}*/




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
void Genetic::crossOver(vector<int> parent1, vector<int> parent2)
{
	vector<int> child1, child2;
	// map of genes, checks if already are selected
	map<int, int> genes1, genes2;
	
	for(int i = 0; i < graph->V; i++)
	{
		// initially the genes not are used
		genes1[parent1[i]] = 0;
		genes2[parent2[i]] = 0;
	}
	// generates random points
	int point1 = rand() % (graph->V - 1) + 1;
	int point2 = rand() % (graph->V - point1) + point1;


	// adjusts the points if they are equal
	if(point1 == point2)
	{
		if(point1 - 1 > 1)
			point1--;
		else if(point2 + 1 < graph->V)
			point2++;
		else
		{
			// point1 or point2 ?? random...
			int point = rand() % 10 + 1; // number in the range 1 to 10
			if(point <= 5)
				point1--;
			else
				point2++;
		}
	}
	// generates childs

	// until point1, child1 receives genes of the parent1
	// and child2 receives genes of the parent2
	for(int i = 0; i < point1; i++)
	{
		// adds genes
		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);
		// marks genes
		genes1[parent1[i]] = 1;
		genes2[parent2[i]] = 1;
	}
	// 	marks remaining genes
	for(int i = point2 + 1; i < graph->V; i++)
	{
		genes1[parent1[i]] = 1;
		genes2[parent2[i]] = 1;
	}

	// here is the substring inverted
	// child1 receives genes of the parent2 and
	// child2 receives genes of the parent1
	for(int i = point2; i >= point1; i--)
	{
		if(genes1[parent2[i]] == 0) // if the gene is not used
		{
			child1.push_back(parent2[i]);
			genes1[parent2[i]] = 1; // marks the gene
		}
		else
		{
			// if the gene already is used, chooses gene that is not used
			for(map<int, int>::iterator it = genes1.begin(); it != genes1.end(); ++it)
			{
				if(it->second == 0) // checks if is not used
				{
					child1.push_back(it->first);
					genes1[it->first] = 1; // marks as used
					break; // left the loop
				}
			}
		}
		if(genes2[parent1[i]] == 0) // if the gene is not used
		{
			child2.push_back(parent1[i]);
			genes2[parent1[i]] = 1; // marks the gene
		}
		else
		{
			// if the gene already is used, chooses gene that is not used
			for(map<int, int>::iterator it = genes2.begin(); it != genes2.end(); ++it)
			{
				if(it->second == 0) // checks if is not used
				{
					child2.push_back(it->first);
					genes2[it->first] = 1; // marks as used
					break; // left the loop
				}
			}
		}
	}
	// ramaining genes: child1 receives genes of the parent1
	// and child2 receives genes of the parent2
	for(int i = point2 + 1; i < graph->V; i++)
	{
		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);
	}
	// mutation
	int mutation = rand() % 100 + 1; // random number in [1,100]
	if(mutation <= mutation_rate) // checks if the random number <= mutation rate
	{
		// makes a mutation: change of two genes
		int index_gene1, index_gene2;
		index_gene1 = rand() % (graph->V - 1) + 1;
		index_gene2 = rand() % (graph->V - 1) + 1;

		// makes for child1
		int aux = child1[index_gene1];
		child1[index_gene1] = child1[index_gene2];
		child1[index_gene2] = aux;

		// makes for child2
		aux = child2[index_gene1];
		child2[index_gene1] = child2[index_gene2];
		child2[index_gene2] = aux;
	}
	int total_cost_child1 = isValidSolution(child1);
	int total_cost_child2 = isValidSolution(child2);

	// cout<<__LINE__<<" In crossover till before adding the solution to the lazy list"<<endl;
	// checks if is a valid solution and not exists in the population
	if(total_cost_child1 != -1 && !list_contains(the_list,make_pair(child1,total_cost_child1)))
	{
		// add child in the population
		// cout<<__LINE__<<" Adding child1"<<endl;
		if(list_add(the_list,make_pair(child1,total_cost_child1)))
			real_size_population++; // increments the real_size_population
		//insertBinarySearch(child1, total_cost_child1, thread_id); // uses binary search to insert
	}

	// cout<<__LINE__<<" After adding child1 to the lazy list"<<endl;
	// checks again...
	if(total_cost_child2 != -1 && !list_contains(the_list,make_pair(child2,total_cost_child2)))
	{
		// add child in the population
		// cout<<__LINE__<<" Adding child2"<<endl;
		if(list_add(the_list,make_pair(child2,total_cost_child2)))
			real_size_population++; // increments the real_size_population
		// insertBinarySearch(child2, total_cost_child2, thread_id); // uses binary search to insert
	}

	// cout<<__LINE__<<" After adding child2 to the lazy list"<<endl;
	// showPopulation(thread_id);
}


// runs the genetic algorithm (Multi-threaded)
void Genetic::run()
{

	// cout<<"Thead number "<<thread_id<<" is running rn!!\n";
	generations = (generations * (num_proc-1)) / num_proc;
	// auto n_generation = generations;
	// cout<<"Hello1: "<<n_generation;
	// cout<<__LINE__<<" In run()"<<endl;
	// int my_max_size_population = (1.5 * size_population)/num_proc; // <==== see best value for this 
	// cout<<__LINE__<<" Total population size = "<<real_size_population<<endl;

	if(real_size_population == 0)
		return;

	// cout<<__LINE__<<" Total population size = "<<real_size_population<<endl;
	for(int i = 0; i < generations; i++)
	{
		// cout<<__LINE__<<" In generation number: "<<i<<endl;
		int  old_size_population = real_size_population;

		/* selects two parents (if exists) who will participate
			of the reproduction process */
		if(real_size_population >= 2)
		{
			if(real_size_population == 2)
			{
				// applying crossover in the parents
				/*forward_list<my_pair>::iterator it = population2.begin();
				auto first_elem = it;
				auto second_elem = it++;*/
				auto first_elem = choose(the_list,0);
				auto second_elem = choose(the_list,1);
				crossOver(first_elem, second_elem);
			}
			else
			{
				// real_size_population[thread_id] > 2
				int parent1, parent2;
				do
				{
					// select two random parents
					parent1 = rand() % real_size_population;
					parent2 = rand() % real_size_population;
				}while(parent1 == parent2);
				// applying crossover in the two parents
				/*auto parent1_elem  =  population2.begin().first;
				auto parent2_elem  =  population2.begin().first;
				int maximum_parent = (parent1 > parent2) ? parent1 : parent2;
				forward_list<my_pair>::iterator it = population2.begin();
				for(int count = 0; count <= maximum_parent; count++, ++it)
				{
					if(count == parent1)
					{
						parent1_elem = it.first;
					}
					if(count == parent2)
					{
						parent2_elem = it.first;
					}
				}*/

				auto parent1_elem = choose(the_list,parent1);
				auto parent2_elem = choose(the_list,parent2);
				// cout<<__LINE__<<" Before crossing over the two parents chosen"<<endl;
				crossOver(parent1_elem, parent2_elem);
			}

			// gets difference to check if the th_population[thread_id] grew
			int diff_population = real_size_population - old_size_population;
			if(diff_population == 2)
			{
				if(real_size_population > size_population)
				{
					// removes the two worst parents of the th_population[thread_id]
					// This helps in converging of values
					// population2.pop_front();
					// population2.pop_front();
					//population3.
					// decrements the real_size_population[thread_id] in 2 units
					real_size_population -= 2;
				}
			}
			else if(diff_population == 1)
			{
				if(real_size_population > size_population)
				{
					//population2.pop_front(); // removes the worst parent of the th_population[thread_id]
					real_size_population--; // decrements the real_size_population[thread_id] in the unit
				}
			}
		}
		else // th_population[thread_id] contains only 1 parent
		{
			// applying crossover in the parent
			crossOver(choose(the_list,0), choose(the_list,0));
			if(real_size_population > size_population)
			{
				//population2.pop_front(); // removes the worst parent of the th_population[thread_id]
				real_size_population--; // decrements the real_size_population[thread_id] in the unit
			}
		}
	}
	// cout<<__LINE__<<" Out of run()"<<endl;
	// if(show_population == true)
	// showPopulation(thread_id); // shows the th_population[thread_id]
}

// Gets each threads result and print minimum
void Genetic::getResult(){
	//sort(result.begin(), result.end(), sort_pred());
	node_t * result = get_head(the_list);
	result = result->next;
	cout << "\nBest solution: ";
	const vector<int>& vec = result->data.first;

	// for(int i = 0; i < graph->V; i++)
	// 	cout << vec[i]<<" ";
	// cout<<endl;


	for(int i = 0; i < graph->V; i++){
		cout << vec[i];
		if(i != graph->V-1)
	    {
	    	cout << graph->inbetween_vert[make_pair(vec[i], vec[i+1])] << " ";
		    // cout << vec[i+1] << " ";
		}
	    else
	    {
	    	cout << graph->inbetween_vert[make_pair(vec[i], vec[0])] << " ";
	        // cout << vec[0] << " ";
	    }
	}
	// cout << graph->initial_vertex;
	cout << " | Cost: " << result->data.second;

	ofstream file;
	file.open("Multi-threaded_1.csv",ios::out | ios::app);
	file<<"\n"<<N<<","<<SEED<<","<<POP<<","<<C<<","<<GEN<<","<<result->data.second<<",";
	for(int i = 0; i < graph->V; i++)
		file << vec[i] << " ";

	file.close();
}


// Get best cost for given graph
int Genetic::getCostBestSolution()
{
	if(real_size_population > 0)
		return population[0].second;
	return -1;
}


//__________________________________________________________________________________________________
// Extra functions added here

void Graph::floydWarshall()
{
	int i, j, k;
	// cout<<__LINE__<<" Is it till here?"<<endl;
	int ** dist = new int * [V];
	for(int l = 0; l<V; l++){
		dist[l] = new int [V];
	}
	// cout<<__LINE__<<" Is it till here?"<<endl;
	for(i = 0; i < V; i++)
	{
		for(j = 0; j < V; j++)
		{
			if(existsEdge(i,j) != 0)
			{
				// cout<<dist[i][j]<<" ";
				dist[i][j] = map_edges[make_pair(i, j)];
			}
			else
			{
				if(i == j)
				{
					dist[i][j] = 0;
				}
				else
					dist[i][j] = INF; // INFINITY ACTUALLY
			}

			// cout <<  __LINE__ << " " << i << " " << j << " " << dist[i][j] << endl;
			inbetween_vert[make_pair(i, j)] = "";
		}
	}
	for(k = 0; k < V; k++)
	{
		for(i = 0; i < V; i++)
		{
			for(j = 0; j < V; j++)
			{
				if(dist[i][k] + dist[k][j] < dist[i][j])
				{
					inbetween_vert[make_pair(i, j)] += " " + std::to_string(k);
					// cout << inbetween_vert[make_pair(i, j)] << endl;
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
	}
	for(i = 0; i < V; i++)
	{
		for(j = 0; j < V; j++)
		{
			if(i != j) 
			{

				map_edges[make_pair(i, j)] = dist[i][j];

				// cout <<  __LINE__ << " " << i << " " << j << " " << dist[i][j] << endl;
			}

		}
	}

}


