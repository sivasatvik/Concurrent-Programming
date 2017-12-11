#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>
#include <sys/time.h>

// Nnumber of threads
#define num_proc N


using namespace std;

// Matrix representation of graph ( graph[i][j] = cost to go from i to j [ i.e distance[i to j]])
int graph[V][V];
// global array where each thread store the minimum cost it  found
int mins[num_proc];
// Second vertex number ranges from [1 to V-1] ( assume init vertex = 0)
std::atomic_int vert_no;



// Print Generated Graph
void printGraph(){
	for (int i = 0; i < V; ++i){
        for (int j = 0; j < V; ++j){
            cout<<graph[i][j]<<" ";
        }
        cout<<endl;
    }
}


// Creates Random Graph with V number of vertex
void generatesGraph()
{
    vector<int> vec;

    // creates the vector
    for(int i = 0; i < V; i++){
        vec.push_back(i);
        // cout<<vec[i]<<" ";
    }
    // cout<<endl;

    // generates a random permutation
    random_shuffle(vec.begin(), vec.end());

    // gives random weight to vertices
    int i, weight;

    // This for loop connect all the edges
    for(i = 0; i <= V; i++)
    {
        weight = rand() % V + 1; // random weight in range [1,V]
        // cout<<vec[i]<<" "<<vec[i+1]<<" "<<weight<<endl;

        if(i + 1 < V){
            graph[vec[i]][vec[i+1]] = weight;
        }
        else
        {
            // add last edge
            graph[vec[i]][vec[0]] = weight;
            break;
        }
    }

    int limit_edges = V * (V - 1); // calculates the limit of edges
    int size_edges = rand() % (2 * limit_edges) + limit_edges;

    // This for loop adds some extra edges to graph
    for(int i = 0; i < size_edges; i++)
    {
        int src = rand() % V; // random source
        int dest = rand() % V; // random destination
        weight = rand() % V + 1; // random weight in range [1,V]
        if(src != dest)
        {
            graph[vec[src]][vec[dest]] = weight;
            graph[vec[dest]][vec[src]] = weight;
        }
    }
}


// implementation of traveling Salesman Problem
void travllingSalesmanProblem(int id)
{
    int min_path = 0;
    do
    {
        // store all vertex apart from source vertex and initial vertex i.e 0
        int sec_vert = vert_no++;   
        vector<int> vertex;
        for (int i = 1; i < V; i++)
            if (i != sec_vert)
                vertex.push_back(i);

        // store minimum weight Hamiltonian Cycle.
        min_path = 999999;
        do {     

            // store current Path weight(cost)
            int current_pathweight = graph[0][sec_vert]; // initial vertex(1) to second vertex pathlength add
             
            // compute current path weight
            int k = sec_vert; // Starting from second vertex, calc dist between adjacent vertices
            for (int i = 0; i < vertex.size(); i++) {
                current_pathweight += graph[k][vertex[i]];
    			k = vertex[i];
            }
            // Add distance to get back to initial vertex
            current_pathweight += graph[k][0];

     
            // update minimum
            min_path = min(min_path, current_pathweight);
                
        } while (next_permutation(vertex.begin(), vertex.end()));
    }while(vert_no < V);
    mins[id] = min_path; // Store in Global array
}
 
// Main function
int main()
{	
    // Seed to make sure same random cities are generated when testing sequencial and parallel code

	srand(SEED);
    vert_no = 1;

    generatesGraph();
	// int graph[][V] = { { 0, 10, 15, 20 },
    //                    { 10, 0, 35, 25 },
    //                    { 15, 35, 0, 30 },
    //                    { 20, 25, 30, 0 }
    //                   };
    printGraph();
    cout << "\n\n\n\n";

    struct timeval start;
	gettimeofday(&start, 0);
	vector<thread> threads;
    // Start num_proc threads
	for(int i = 0; i<num_proc; i++){
		threads.push_back(std::thread(travllingSalesmanProblem, i));
	}
    // wait for threads to finish
	for(auto &th : threads)
        th.join();
    struct timeval end;
	gettimeofday(&end, 0);

    // find min of the minimum value each thread generated
    int cur_min = mins[0];
    for(int i=1;i<num_proc;i++)
    {
        if(mins[i] < cur_min)
        {
            cur_min = mins[i];
        }
    }
    cout << cur_min <<endl;
	long long duration = (end.tv_sec-start.tv_sec)*1000000LL + end.tv_usec-start.tv_usec;
	float time_val = (float)duration/1000000;
	cout << "\n\nTime for to run this algorithm: " << time_val << " seconds.\n\n"; // shows time in seconds
	cout<<"----------------------------------------------------------------------------------------------\n";

    return 0;
}