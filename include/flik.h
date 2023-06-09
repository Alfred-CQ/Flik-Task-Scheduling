#ifndef __FLIK__
#define __FLIK__

#include <vector>

#include <environment.h>

class Flik
{
public:
    Flik(const VMS &vms, const TASKS &tasks, int num_groups);

    // Methods
    // Main
    void colony_Launch(int numb_epochs);

    // HAGA
    void encoder();
    void fitness();
    void selection();
    void crossover();
    void mutation();

    // GA
    void shuffle_Tasks();
    void make_Population();

    CHROMOSOME *roulette(std::vector<float> circular_disk);
    void fisherYates(CHROMOSOME *&incomplete);
    // Getters

private:
    VMS vms;
    TASKS tasks;

    POPULATION population;
    POPULATION survivors;

    FITNESS_VALUES fit_vals;
    float sum_fitness;

    float sum_pheromone;
    std::vector<float> group_pheromones;

    int num_groups;
};

#endif // !__FLIK__
