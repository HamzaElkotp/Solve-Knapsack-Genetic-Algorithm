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
    int fitness = 0;
    bool fitness_calculated = false;

protected:
    vector<bool> chromosome_string;

public:

    static function<int(const Chromosome&)> fitness_function;

    static int chromosome_size;

    Chromosome() : chromosome_string(chromosome_size) {
        if(!chromosome_size)
            throw runtime_error("Chromosome size must be provided!");
    }

    int calc_chromosome_fitness(){
        if(!fitness_function)
            throw runtime_error("Fitness function must be provided!");
        fitness = fitness_function(*this);
        fitness_calculated = true;
        return fitness;
    }

    int get_chromosome_fitness(){
        if(!fitness_calculated)
            calc_chromosome_fitness();
        return fitness;
    }

    [[nodiscard]] vector<bool> get_chromosome_string() const {
        return chromosome_string;
    }

    bool operator==(const Chromosome& other) const {
        return this->chromosome_string == other.chromosome_string;
    }
};

class Generation{
    bool is_generation_sorted = false;
    bool is_worst_stored = false;
    bool is_average_stored = false;
    bool is_worst_best = false;
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
             [](Chromosome& a, Chromosome& b){
                      return a.get_chromosome_fitness() > b.get_chromosome_fitness(); // descending
                  });
        is_generation_sorted = true;
    }

    void set_best_chromosome(){
        if(!is_generation_sorted)
            sort_generation();
        best_chromosome = chromosomes.front();
        best_fitness = best_chromosome.get_chromosome_fitness();
    }

    void set_worst_chromosome(){
        if(!is_generation_sorted)
            sort_generation();
        worst_chromosome = chromosomes.back();
        worst_fitness = worst_chromosome.get_chromosome_fitness();
    }

    void set_generation_size(){
        generation_size = (long long)chromosomes.size();
    }

    long long set_average(){
        if(generation_size==0)
            set_generation_size();

        long long total = 0;
        for(auto & chromosome : chromosomes){
            total+= chromosome.get_chromosome_fitness();
        }
        average_fitness = round(total/generation_size);
        return average_fitness;
    }

    void add_chromosome(Chromosome &solution){
        chromosomes.push_back(solution);
    }

    // Crossover function override
    // Mutation function override
    // validation function override


    void destroy_generation_memory() {
        if(!is_average_stored) set_average();
        if(!is_worst_stored) set_worst_chromosome();
        if(!is_worst_best) set_best_chromosome();
        vector<Chromosome>().swap(chromosomes);
    }

    Generation() : chromosomes(0) {}
};

function<int(const Chromosome&)> Chromosome::fitness_function = nullptr; // in cpp
int Chromosome::chromosome_size = 0; // in cpp

class Population{
public:
    vector<Generation> generations;

    void add_generation(Generation &solutions){
        generations.push_back(solutions);
    }

    void new_generation(){
        Generation new_generation;
        // Get best x percent
        // push best x percent

        // get worst y percent
        // mutate worst y percent
        // validate
        // push mutated worst y percent

        // do crossover
        // vaidate
        // push crossover

        new_generation.set_generation_size();
        new_generation.set_average();
        new_generation.sort_generation();
        new_generation.set_worst_chromosome();
        new_generation.set_best_chromosome();

        generations.back().destroy_generation_memory(); // clear previous generation

        generations.push_back(new_generation);
    }

    Population() : generations(0) {}
};


int fitness(const Chromosome &solution){
    int weight = 0;
    int value = 0;
    for(int i=0; i<n; i++){
        if(solution.get_chromosome_string()[i]){
            weight += items[i].weight;
            value += items[i].value;
        }
    }
    return weight;
}

bool validate(Chromosome &solution, Generation &gen){
    if(solution.get_chromosome_fitness()==0)
        return false;
    for(auto & chromosome : gen.chromosomes){
        if(solution == chromosome)
            return false;
    }
    return true;
}





int main() {
    int problem_size = 10;
    items.resize(problem_size);
    n = problem_size;

    Chromosome::fitness_function = fitness;
    Chromosome::chromosome_size = problem_size;

    Chromosome *x;

    return 0;
}