#include "Individual.h"

int Individual::static_count = 0;

Individual::Individual(const std::shared_ptr<PopulationSettings>& population_settings) : 
    m_id(static_count), 
    population_settings(population_settings),
    m_fitness(0)
{
    static_count++;
};

Individual::Individual(const std::shared_ptr<PopulationSettings>& population_settings, const std::string& str_data) : 
    m_id(static_count), 
    population_settings(population_settings),
    m_data(str_data),
    m_fitness(0)
{
    static_count++;
};

const int Individual::get_id() const
{
    return m_id;
}

void Individual::create_rand_data()
{
    m_data = gen_random_str(population_settings->data_settings.str_length, population_settings->data_settings.alphanum_only);
}

const std::string& Individual::get_data() const
{
    return m_data;
}

void Individual::set_data(const std::string& data)
{
    m_data = data;
}

void Individual::process_fitness(const std::string& target_data)
{
    float raw_fit = 0;

    if(m_data.size() != target_data.size()){
        std::cerr << "ERROR: Comparing strings of different sizes" << std::endl;
        throw std::invalid_argument("ERROR: Invalid target data");
    };

    for(int i=0; i < m_data.size(); i++)
    {
        if(m_data.at(i) == target_data.at(i)){
            raw_fit++;
        }
    }

    m_fitness = raw_fit/m_data.size();
}

float Individual::get_fitness() const
{
    return m_fitness;
}

const std::shared_ptr<PopulationSettings>& Individual::get_population_settings() const
{
    return population_settings;
}

Individual Individual::make_child(const Individual& ind_a, const Individual& ind_b)
{
    // settings validation

    if(ind_a.get_population_settings() == nullptr || ind_b.get_population_settings() == nullptr)
    {
        std::cerr << "ERROR: Tried to breed " << ind_a.get_id() << " and " << ind_b.get_id() << " but they have uncomplete population settings! Aborting" << std::endl;
        throw std::invalid_argument("ERROR: Invalid population settings");
    }

    std::cout << ind_a.get_population_settings() << std::endl;
    std::cout << ind_b.get_population_settings() << std::endl;


    if(ind_a.get_population_settings().get() != ind_b.get_population_settings().get()){
        std::cerr << "ERROR: Tried to breed " << ind_a.get_id() << " and " << ind_b.get_id() << " but they have different population settings! Aborting" << std::endl;
        throw std::invalid_argument("ERROR: Different population settings");
    }

    // data validation

    std::string a_data = ind_a.get_data();
    std::string b_data = ind_b.get_data();

    if(a_data.size() != b_data.size())
    {
        std::cerr << "ERROR: Tried to breed " << ind_a.get_id() << " and " << ind_b.get_id() << " but they have different genome size! Aborting" << std::endl;
        throw std::invalid_argument("ERROR: Different genome sizes");
    }

    if(a_data.size() <= 0 || b_data.size() <= 0)
    {
        std::cerr << "ERROR: Tried to breed " << ind_a.get_id() << " and " << ind_b.get_id() << " but one have empty genome! Aborting" << std::endl;
        throw std::invalid_argument("ERROR: Invalid genome size");
    }

    // breeding

    std::string child_data = a_data;

    // at this point ind_a & ind_b are sure to have the same genetic and data settings
    std::shared_ptr<PopulationSettings> child_population_settings = ind_a.get_population_settings();

    static std::random_device rd;
    static std::mt19937 gen(rd());

    for(int i=0; i < a_data.size(); i++)
    {
        std::uniform_int_distribution<> dis(0, 1);
        int rand_bit = dis(gen);
        child_data[i] = (rand_bit == 0) ? a_data[i] : b_data[i];
    }

    // mutation
    float& mutation_factor = child_population_settings->genetic_settings.mutation_factor;

    if(mutation_factor < 0 || mutation_factor > 1)
    {
        std::cerr << "WARNING: Invalid mutation factor, resetting to zero" << std::endl;
        mutation_factor = 0;
    }

    int mutation_steps = child_data.size() * mutation_factor;

    for(int i=0; i<mutation_steps; i++)
    {
        std::uniform_int_distribution<> dis(0, child_data.size()-1);
        // use range instead of jumping to a random char?
        int mutation_index = dis(gen);

        char c = gen_random_char(child_population_settings->data_settings.alphanum_only);
        child_data[mutation_index] = c;
    }

    return Individual(child_population_settings, child_data);
}

std::string Individual::gen_random_str(const int length, const bool alphanum_only)
{
    std::string tmp_str;

    for (int i = 0; i < length; i++) {
        char c = gen_random_char(alphanum_only);
        tmp_str += c;
    }
    
    return tmp_str;
}

char Individual::gen_random_char(const bool alphanum_only)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::string alphanum =
        "0123456789 "
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    char c; 

    if(alphanum_only)
    {
        std::uniform_int_distribution<> dis(0, alphanum.size() - 1);
        c = alphanum.at(dis(gen));
    }
    else
    {
        // ASCI-II 32 to 126 are printable characters
        int lower_bound = 32; 
        int upper_bound = 126;
        std::uniform_int_distribution<> dis(lower_bound, upper_bound);

        c = static_cast<char>(dis(gen));
    }

    return c;
}