#pragma once

#include "PopulationSettings.h"
#include "Individual.h"
#include "vector"
#include "algorithm"

class Population
{
public:
    Population(const int size, const PopulationSettings& population_settings);
    const std::shared_ptr<PopulationSettings>& get_population_settings() const;
    void set_population_settings(const std::shared_ptr<PopulationSettings>& population_settings);
    Individual& get_individual(int id);
    void add_individual(const Individual& ind);
    void remove_individual(int id);
    void print_pop() const;
    void refresh_fit(const std::string& target);
    void sort_by_fit();
    void halving_breed();
    void next_gen(const std::string& target);

private:
    // todo: use a map instead of only vector for trackind id<->indices if better speed is needed
    std::vector<Individual> m_individuals;
    int m_generation;
    std::shared_ptr<PopulationSettings> m_population_settings;
};