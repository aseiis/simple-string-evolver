#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <memory>
#include "PopulationSettings.h"

class Individual
{

// ATTRIBUTES //

private: // statics
    static int static_count;

private:
    std::string m_data;
    float m_fitness;
    int m_id;
    std::shared_ptr<PopulationSettings> population_settings;

// FUNCTIONS //

public:
    Individual(const std::shared_ptr<PopulationSettings>& population_settings);
    Individual(const std::shared_ptr<PopulationSettings>& population_settings, const std::string& data);
    const int get_id() const;
    void create_rand_data();
    const std::string& get_data() const;
    void set_data(const std::string& data);
    void process_fitness(const std::string& target_data);
    float get_fitness() const;
    const std::shared_ptr<PopulationSettings>& get_population_settings() const;
    static Individual make_child(const Individual& ind_a, const Individual& ind_b);

private:
    static std::string gen_random_str(const int length, const bool alphanum_only);
    static char gen_random_char(const bool alphanum_only);


};