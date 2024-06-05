#include "Population.h"

Population::Population(const int size, const PopulationSettings& population_settings) : m_generation(0)
{
    m_population_settings = std::make_shared<PopulationSettings>(population_settings);
    for(int i=0; i<size; i++)
    {
        Individual ind = Individual(m_population_settings);
        ind.create_rand_data();
        m_individuals.push_back(ind);
    }
}

const std::shared_ptr<PopulationSettings>& Population::get_population_settings() const
{
    return m_population_settings;
}

void Population::set_population_settings(const std::shared_ptr<PopulationSettings>& population_settings)
{
    m_population_settings = population_settings;
}

Individual& Population::get_individual(int id)
{
    for (Individual& ind : m_individuals)
    {
        if (ind.get_id() == id) {
            return ind;
        }
    }

    std::cerr << "ERROR: Couldn't get individual with id " << id << " (not found)" << std::endl;
    throw std::invalid_argument("ERROR: Couldn't get individual");
}

void Population::add_individual(const Individual& ind)
{
    m_individuals.push_back(ind);
}

void Population::remove_individual(int id)
{
    for (auto it = m_individuals.begin(); it != m_individuals.end(); it++)
    {
        if ((*it).get_id() == id)
        {
            m_individuals.erase(it);
        }
    }

    std::cerr << "ERROR: Couldn't remove individual with id " << id << " (not found)" << std::endl;
    throw std::invalid_argument("ERROR: Couldn't remove individual");
}

void Population::print_pop() const
{
    std::cout << "--- POPULATION GEN. " << m_generation << " (size: < " << m_individuals.size() << ") -- - " << std::endl;
    for(const Individual& ind : m_individuals)
    {
        std::cout << ind.get_data() << " | fit: " << ind.get_fitness() << std::endl;
    }
    std::cout << "--- --- ---" << std::endl;
}

void Population::refresh_fit(const std::string& target)
{
    for (Individual& ind : m_individuals)
    {
        ind.process_fitness(target);
    }
}

void Population::sort_by_fit()
{
    std::sort(m_individuals.begin(), m_individuals.end(), [](const Individual& lhs, const Individual& rhs)
    {
        return lhs.get_fitness() > rhs.get_fitness();
    });
}

void Population::halving_breed()
{
    int death_count = m_individuals.size() / 2;

    std::random_device device;
    std::mt19937 gen(device());
    // indices of the survivors
    std::uniform_int_distribution<int> init_distribution(0, m_individuals.size() - 1 - death_count);

    for (int i = 0; i < death_count; i++)
    {
        m_individuals.pop_back();
    }

    for (int i = 0; i < death_count; i++)
    {
        const Individual& parent_1 = m_individuals.at(init_distribution(gen));
        const Individual& parent_2 = m_individuals.at(init_distribution(gen));
        const Individual child = Individual::make_child(parent_1, parent_2);
        add_individual(child);
    }
}

void Population::next_gen(const std::string& target)
{
    halving_breed();
    refresh_fit(target);
    sort_by_fit();
    m_generation++;
}
