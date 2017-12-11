#include <bits/stdc++.h>
#include <vector>
#include <random>
#include <cstdlib>
#include <algorithm>
#include <sys/time.h>


using namespace std;

int graph[V][V];



void printGraph(){
	for (int i = 0; i < V; ++i){
        for (int j = 0; j < V; ++j){
            cout<<graph[i][j]<<" ";
        }
        cout<<endl;
    }
}

void generatesGraph()
{
    vector<int> vec;

    // creates the vector
    for(int i = 0; i < V; i++){
        vec.push_back(i);
        // cout<<vec[i]<<" ";
    }
    cout<<endl;

    // generates a random permutation
    random_shuffle(vec.begin(), vec.end());

    //initial_vertex = vec[0]; // updates initial vertex

    int i, weight;
    for(i = 0; i <= V; i++)
    {
        weight = rand() % V + 1; // random weight in range [1,V]
        // cout<<vec[i]<<" "<<vec[i+1]<<" "<<weight<<endl;

        if(i + 1 < V){
            graph[vec[i]][vec[i+1]] = weight;
            // cost[i] = weight;
            // cout<<ary[vec[i]][vec[i+1]]<<endl;
        }
            // addEdge(vec[i], vec[i + 1], weight);
        else
        {
            // add last edge
            graph[vec[i]][vec[0]] = weight;
            // cost[i] = weight;
            // addEdge(vec[i], vec[0], weight);
            break;
        }
    }

    int limit_edges = V * (V - 1); // calculates the limit of edges
    int size_edges = rand() % (2 * limit_edges) + limit_edges;

    // add others edges randomly
    for(int i = 0; i < size_edges; i++)
    {
        int src = rand() % V; // random source
        int dest = rand() % V; // random destination
        weight = rand() % V + 1; // random weight in range [1,V]
        if(src != dest)
        {
            graph[vec[src]][vec[dest]] = weight;
            graph[vec[dest]][vec[src]] = weight;
            // addEdge(vec[src], vec[dest], weight);
            // addEdge(vec[dest], vec[src], weight);
        }
    }
}


// implementation of traveling Salesman Problem
int travllingSalesmanProblem(int graph[][V], int s)
{
    // store all vertex apart from source vertex
    vector<int> vertex;
    for (int i = 0; i < V; i++)
        if (i != s)
            vertex.push_back(i);
 
    // store minimum weight Hamiltonian Cycle.
    int min_path = 999999;
    do {
 
        // store current Path weight(cost)
        int current_pathweight = 0;
         
        // compute current path weight
        int k = s;
        for (int i = 0; i < vertex.size(); i++) {
            current_pathweight += graph[k][vertex[i]];
            // cout<<current_pathweight<<endl;
			k = vertex[i];
        }
        current_pathweight += graph[k][s];

 
        // update minimum
        min_path = min(min_path, current_pathweight);
        
    } while (next_permutation(vertex.begin(), vertex.end()));
 
    return min_path;
}
 
// driver program to test above function
int main()
{	
	srand(SEED);
    // matrix representation of graph
    generatesGraph();
	// int graph[][V] = { { 0, 10, 15, 20 },
 //                       { 10, 0, 35, 25 },
 //                       { 15, 35, 0, 30 },
 //                       { 20, 25, 30, 0 } };
    printGraph();

    struct timeval start;
    gettimeofday(&start, 0);
    
    int s = 0;

    cout << travllingSalesmanProblem(graph, s) << endl;

    struct timeval end;
    gettimeofday(&end, 0);
    long long duration = (end.tv_sec-start.tv_sec)*1000000LL + end.tv_usec-start.tv_usec;
    float time_val = (float)duration/1000000;

    cout << "\n\nTime for to run the genetic algorithm: " << time_val/*float(clock() - begin_time)/CLOCKS_PER_SEC*/ << " seconds.\n\n"; // shows time in seconds
    cout<<"----------------------------------------------------------------------------------------------\n";

    return 0;
}