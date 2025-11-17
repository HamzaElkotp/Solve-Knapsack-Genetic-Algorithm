#include <bits/stdc++.h>
using namespace std;


class Chromosome;
class Generation;
class Population;
bool validate(Chromosome solution);

int problem_size = 0;
int n = problem_size;
int weight_limit = 0;

struct item {
    string name;
    int weight;
    int value;
};
vector<item> items(problem_size);



class Chromosome{
public:
    vector<bool> chromosome_string;
    int fitness = 0;

    static function<int(const Chromosome&)> fitness_function;

    Chromosome() : chromosome_string(problem_size) {}

    void calc_chromosome_fitness(){
        if(!fitness_function)
            throw runtime_error("Fitness function must be provided!");
        fitness = fitness_function(*this);
    }

    bool operator==(const Chromosome& other) const {
        return this->chromosome_string == other.chromosome_string;
    }
};

class Generation{
public:
    vector<Chromosome> chromosomes;
    long long average_fitness = 0;

    void calc_average(){
        long long total = 0;
        for(auto & chromosome : chromosomes){
            total+= chromosome.fitness;
        }
        average_fitness = round(total/(long long)chromosomes.size());
    }

    void add_chromosome(Chromosome &solution){
        chromosomes.push_back(solution);
    }

    Generation() : chromosomes(0) {}
};

function<int(const Chromosome&)> Chromosome::fitness_function = nullptr; // in cpp

class Population{
public:
    vector<Generation> generations;

    void add_generation(Generation &solutions){
        generations.push_back(solutions);
    }

    Population() : generations(0) {}
};


int fitness(const Chromosome &solution){
    int weight = 0;
    int value = 0;
    for(int i=0; i<n; i++){
        if(solution.chromosome_string[i]){
            weight += items[i].weight;
            value += items[i].value;
        }
    }
    return weight;
}

bool validate(Chromosome &solution, Generation &gen){
    if(solution.fitness==0)
        return false;
    for(auto & chromosome : gen.chromosomes){
        if(solution == chromosome)
            return false;
    }
    // check redundunt;
    return true;
}





int main() {
    problem_size = 10;

    Chromosome::fitness_function = fitness;

    return 0;
}