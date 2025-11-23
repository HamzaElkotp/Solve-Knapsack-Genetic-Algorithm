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

    [[nodiscard]] vector<bool> get_chromosome_string() const {
        return chromosome_string;
    }

    bool operator==(const Chromosome& other) const {
        return this->chromosome_string == other.chromosome_string;
    }

    Chromosome(int sz) : chromosome_string(sz) {}
    Chromosome() : chromosome_string(0) {}
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
                      return a.fitness > b.fitness; // descending
                  });
        is_generation_sorted = true;
    }

    void set_best_chromosome(){
        if(!is_generation_sorted)
            sort_generation();
        best_chromosome = chromosomes.front();
        best_fitness = best_chromosome.fitness;
    }

    void set_worst_chromosome(){
        if(!is_generation_sorted)
            sort_generation();
        worst_chromosome = chromosomes.back();
        worst_fitness = worst_chromosome.fitness;
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
        average_fitness = round(total/generation_size);
        return average_fitness;
    }

    void add_chromosome(Chromosome &solution){
        chromosomes.push_back(solution);
    }

    void destroy_generation_memory() {
        if(!is_average_stored) set_average();
        if(!is_worst_stored) set_worst_chromosome();
        if(!is_worst_best) set_best_chromosome();
        vector<Chromosome>().swap(chromosomes);
    }
};

//function<int(const Chromosome&)> Chromosome::fitness_function = nullptr; // in cpp
//int Chromosome::chromosome_size = 0; // in cpp

class Population{
private:
//    int elitism_percent;
//    int mutation_percent;
    int chromosome_size;

public:
    vector<Generation> generations;

    void set_chromosome_size(int num){
        chromosome_size = num;
    }

    Chromosome new_chromosome(){
        return Chromosome(chromosome_size);
    }

//    void set_elitism_percent(int num){
//        if(num > 100 || num<0)
//            throw runtime_error("Elitism percent must be between 0 - 100!");
//        elitism_percent = num;
//    }
//
//    void set_mutation_percent(int num){
//        if(num > 100 || num<0)
//            throw runtime_error("Mutation percent must be between 0 - 100!");
//        mutation_percent = num;
//    }

//    void initiate_population(){
//
//    }

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

    bool do_validation(Chromosome &solution, Generation &gen){
        throw runtime_error("Validation function must be overridden!");
        return false;
    }

    int calc_fitness(Chromosome &solution){
        throw runtime_error("Fitness function must be overridden!");
        return 0;
    }

    void add_generation(Generation &solutions){
        generations.push_back(solutions);
    }

    void new_generation(){
        Generation new_generation;

        do_elitism(generations.back(), new_generation);

        do_mutation(generations.back(), new_generation);

        vector<pair<Chromosome, Chromosome>> pairs = do_selection(generations.back());
        vector<Chromosome> children = do_crossover(pairs);
        for(auto ch:children){
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

        generations.back().destroy_generation_memory(); // clear previous generation

        add_generation(new_generation); // generations.push_back(new_generation);
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