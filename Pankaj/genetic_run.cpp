void Genetic::run(int thread_id)
{
	initialPopulation(); // gets initial my_population
	auto my_population = th_population[thread_id]
	auto my_size_population = my_population.size()
	auto n_generation = generations/num_proc;
	auto my_max_size_population = (2*size_population)/num_proc; // <==== see best value for this 
	if(my_size_population == 0)
		return;

	for(int i = 0; i < n_generation; i++)
	{
		int  old_size_population = my_size_population;

		/* selects two parents (if exists) who will participate
			of the reproduction process */
		if(my_size_population >= 2)
		{
			if(my_size_population == 2)
			{
				// applying crossover in the parents
				crossOver(my_population[0].first, my_population[1].first);
			}
			else
			{
				// my_size_population > 2
				int parent1, parent2;
				do
				{
					// select two random parents
					parent1 = rand() % my_size_population;
					parent2 = rand() % my_size_population;
				}while(parent1 == parent2);

				// applying crossover in the two parents
				crossOver(my_population[parent1].first, my_population[parent2].first);
			}

			// gets difference to check if the my_population grew
			int diff_population = my_size_population - old_size_population;

			if(diff_population == 2)
			{
				if(my_size_population > my_max_size_population)
				{
					// removes the two worst parents of the my_population
					// This helps in converging of values
					my_population.pop_back();
					my_population.pop_back();

					// decrements the my_size_population in 2 units
					my_size_population -= 2;
				}
			}
			else if(diff_population == 1)
			{
				if(my_size_population > my_max_size_population)
				{
					my_population.pop_back(); // removes the worst parent of the my_population
					my_size_population--; // decrements the my_size_population in the unit
				}
			}
		}
		else // my_population contains only 1 parent
		{
			// applying crossover in the parent
			crossOver(my_population[0].first, my_population[0].first);
			if(my_size_population > my_max_size_population)
			{
				my_population.pop_back(); // removes the worst parent of the my_population
				my_size_population--; // decrements the my_size_population in the unit
			}
		}
	}

	if(show_population == true)
		showPopulation(); // shows the my_population

	return my_population[0];
	// cout << "\nBest solution: ";
	// const vector<int>& vec = my_population[0].first;
	// for(int i = 0; i < graph->V; i++)
	// 	cout << vec[i] << " ";
	// cout << graph->initial_vertex;
	// cout << " | Cost: " << my_population[0].second;
}

