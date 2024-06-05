#include <iostream>
#include <vector>
#include <string>
#include "Population.h"

void next(Population& pop, std::string& target);

int main()
{
    int pop_size = 32;

    bool alphanum_only = true;
    float mutation_factor = 0.1f;

    // turn alphanum_only to false if you use special characters!
    std::string glob_target = "Hello World";

    PopulationSettings population_settings = {
        GeneticSettings{mutation_factor},
        DataSettings{(int)glob_target.size(), alphanum_only}
    };

    Population population(pop_size, population_settings);
    population.print_pop();
    
    for (int i = 0; i < 2001; i++)
    {
        next(population, glob_target);
        if(i % 10 == 0)
            population.print_pop();
    }
    
}

void next(Population& pop, std::string& target)
{
    pop.next_gen(target);
}