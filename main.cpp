#include <bits/stdc++.h>
using namespace std;


class Chromosome;
class Generation;
class Population;
bool validate(Chromosome solution);

int n = 0;
int weight_limit = 0;

struct item {
    string name;
    int weight;
    int value;
};
vector<item> items;



class Chromosome{
private:

protected:

public:
    int fitness = 0;
    vector<bool> chromosome_string;

    [[nodiscard]] const std::vector<bool>& get_chromosome_string() const noexcept {
        return chromosome_string;
    }

    bool operator==(const Chromosome& other) const {
        return this->chromosome_string == other.chromosome_string;
    }

    Chromosome(int sz) : chromosome_string(sz) {}
    Chromosome() : chromosome_string(0) {}
};

class Generation{
private:
    bool is_generation_sorted = false;
    bool is_worst_set = false;
    bool is_average_set = false;
    bool is_best_set = false;
public:
    vector<Chromosome> chromosomes;
    long long average_fitness = 0;
    long long worst_fitness = 0;
    long long best_fitness = 0;
    long long generation_size = 0;
    Chromosome best_chromosome;
    Chromosome worst_chromosome;

    void sort_generation() {
        sort(chromosomes.begin(), chromosomes.end(),
             [](const Chromosome& a, const Chromosome& b){
                      return a.fitness > b.fitness; // descending
                  });
        is_generation_sorted = true;
    }

    void set_best_chromosome(){
        if(!is_generation_sorted)
            sort_generation();
        best_chromosome = chromosomes.front();
        best_fitness = best_chromosome.fitness;
        is_best_set = true;
    }

    void set_worst_chromosome(){
        if(!is_generation_sorted)
            sort_generation();
        worst_chromosome = chromosomes.back();
        worst_fitness = worst_chromosome.fitness;
        is_worst_set = true;
    }

    void set_generation_size(){
        generation_size = (long long)chromosomes.size();
    }

    long long set_average(){
        if(generation_size==0)
            set_generation_size();

        long long total = 0;
        for(auto & chromosome : chromosomes){
            total+= chromosome.fitness;
        }
        average_fitness = llround((double)total / (double )generation_size);
        is_average_set = true;
        return average_fitness;
    }

    void add_chromosome(const Chromosome &solution){
        chromosomes.push_back(solution);
    }

    void destroy_generation_memory() {
        if(!is_average_set) set_average();
        if(!is_worst_set) set_worst_chromosome();
        if(!is_best_set) set_best_chromosome();
        vector<Chromosome>().swap(chromosomes);
    }
};


class Population{
private:
    int chromosome_size = 0;
    int generations_cntr = 0;
    int start_population_size = 0;
    int max_attempts = 0;

public:
    vector<Generation> generations;

    void set_chromosome_size(int num){
        chromosome_size = num;
    }

    void set_start_population_size(int num){
        start_population_size = num;
    }

    void set_max_attempts(int num){
        max_attempts = num;
    }

    Chromosome new_chromosome(){
        if (chromosome_size <= 0) throw runtime_error("chromosome_size not set");
        return Chromosome(chromosome_size);
    }

    vector<Chromosome> do_elitism(Generation &old_generation, Generation &new_generation){
        throw runtime_error("Elitism function must be overridden!");
        return vector<Chromosome>(0);
    }

    vector<Chromosome> do_mutation(Generation &old_generation, Generation &new_generation){
        throw runtime_error("Mutation function must be overridden!");
        return vector<Chromosome>(0);
    }

    vector<pair<Chromosome, Chromosome>> do_selection(Generation &old_generation){
        throw runtime_error("selection function must be overridden!");
        return vector<pair<Chromosome, Chromosome>>(0);
    }

    vector<Chromosome> do_crossover(vector<pair<Chromosome, Chromosome>> &pairs){
        throw runtime_error("Crossover function must be overridden!");
        return vector<Chromosome>(0);
    }

    bool do_validation(const Chromosome &solution, const Generation &gen){
        throw runtime_error("Validation function must be overridden!");
        return false;
    }

    int calc_fitness(const Chromosome &solution){
        throw runtime_error("Fitness function must be overridden!");
        return 0;
    }

    void add_generation(Generation solutions){
        generations.push_back(std::move(solutions));
    }

    function<void(Chromosome&)> first_population_logic;

    void initiate_first_population(){
        if (!first_population_logic) throw runtime_error("first_population_logic not set");

        Generation first_generation;
        first_generation.chromosomes.reserve(start_population_size);
        int attempts = 0;
        int created = 0;

        while (created < start_population_size && attempts < max_attempts){
            Chromosome c(chromosome_size);

            // Let the user fill/update the chromosome
            first_population_logic(c);

            // Validate
            if (!do_validation(c, first_generation)) {
                attempts++;
                continue; // Try again
            }

            c.fitness = calc_fitness(c);
            first_generation.add_chromosome(c);
            created++;
        }

        if (created == 0)
            throw runtime_error("Could not create any chromosome. Validation too strict or user_logic incorrect.");

        generations_cntr++;

        first_generation.set_generation_size();
        first_generation.set_average();
        first_generation.sort_generation();
        first_generation.set_worst_chromosome();
        first_generation.set_best_chromosome();

        // Store first generation
        add_generation(std::move(first_generation));
    }

    void new_generation(){
        if (generations_cntr == 0)
            throw runtime_error("Initial Population should be generated first using initiate_first_population function");

        Generation new_generation;

        vector<Chromosome> elites = do_elitism(generations.back(), new_generation);
        for(auto &c : elites) new_generation.add_chromosome(c);

        vector<Chromosome> mutates = do_mutation(generations.back(), new_generation);
        for(auto &c : mutates) new_generation.add_chromosome(c);

        vector<pair<Chromosome, Chromosome>> pairs = do_selection(generations.back());
        vector<Chromosome> children = do_crossover(pairs);
        for(auto &ch:children){
            if(do_validation(ch, new_generation)) {
                ch.fitness = calc_fitness(ch);
                new_generation.add_chromosome(ch);
            }
        }

        new_generation.set_generation_size();
        new_generation.set_average();
        new_generation.sort_generation();
        new_generation.set_worst_chromosome();
        new_generation.set_best_chromosome();

        if(generations.size() > 1)
            generations.back().destroy_generation_memory(); // clear previous generation

        add_generation(new_generation); // generations.push_back(new_generation);
    }

    void run(int generations_count) {
        if(chromosome_size == 0)
            throw runtime_error("chromosome_size must be > 0, use set_chromosome_size function");
        if(start_population_size == 0)
            throw runtime_error("start_population_size must be > 0, use set_start_population_size function");
        if(max_attempts == 0)
            throw runtime_error("max_attempts must be > 0, use set_max_attempts function");

        initiate_first_population();

        for (int i = 0; i < generations_count; i++) {
            new_generation();
        }
    }

    Population() : generations(0) {}
};


//int fitness(const Chromosome &solution){
//    int weight = 0;
//    int value = 0;
//    for(int i=0; i<n; i++){
//        if(solution.get_chromosome_string()[i]){
//            weight += items[i].weight;
//            value += items[i].value;
//        }
//    }
//    return weight;
//}

//bool validate(Chromosome &solution, Generation &gen){
//    if(solution.get_chromosome_fitness()==0)
//        return false;
//    for(auto & chromosome : gen.chromosomes){
//        if(solution == chromosome)
//            return false;
//    }
//    return true;
//}





int main() {
    int problem_size = 10;
    items.resize(problem_size);
    n = problem_size;

//    Chromosome::fitness_function = fitness;
//    Chromosome::chromosome_size = problem_size;

    Chromosome *x;

    return 0;
}