#pragma once

struct GeneticSettings
{
    float mutation_factor;
};

struct DataSettings
{
    int str_length;
    bool alphanum_only;
};

struct PopulationSettings
{
    GeneticSettings genetic_settings;
    DataSettings data_settings;
};