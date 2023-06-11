#include "flik.h"

Flik::Flik(const VMS &vms,
           const TASKS &tasks, int num_groups)
    : vms(vms), tasks(tasks), num_groups(num_groups)
{
}

void Flik::colony_Launch(int numb_epochs)
{
    encoder();
    print_Population(population);

    for (int idx = 0; idx < population.size(); ++idx)
    {
        std::cout << " ***************" << idx << "***************\n";

        int iterations = 0;
        while (iterations < numb_epochs)
        {
            fitness();
            selection();

            std::random_device seed;
            std::mt19937 generator(seed());
            std::uniform_int_distribution<size_t> dist(0.0, 1.0);

            float cross_rate = dist(generator);
            float mut_rate = dist(generator);

            if (cross_rate < CROSSOVER_RATE)
                crossover();

            if (mut_rate < MUTATION_RATE)
                mutation();

            iterations++;
        }
    }
}

void Flik::encoder()
{
    shuffle_Tasks();
    make_Population();
}

void Flik::fitness()
{
}

void Flik::selection()
{
}

void Flik::crossover()
{
}

void Flik::mutation()
{
}

void Flik::shuffle_Tasks()
{
    std::random_device seed;
    std::mt19937 generator(seed());
    std::shuffle(tasks.begin(), tasks.end(), generator);
}

void Flik::make_Population()
{
    std::random_device seed;
    std::mt19937 generator(seed());
    std::uniform_int_distribution<size_t> dist(0, vms.size() - 1);

    int i{0}, j{0}, idx_task{0};
    for (i = 0; i < Y; ++i)
    {
        CHROMOSOME *chromosome = new CHROMOSOME;

        for (j = 0; j < TG; ++j)
        {
            GENOME *genome = new GENOME(&tasks[idx_task], &vms[dist(generator)]);
            chromosome->push_back(genome);
            idx_task++;
        }

        population.push_back(chromosome);
    }

    std::cout << "\nN[ VAL ]: " << tasks.size() << " vs N[ PARAM ]: " << N
              << "\nY[ VAL ]: " << population.size() << " vs Y[ PARAM ]: " << Y
              << "\nTG[ VAL ]: " << population[0]->size() << " vs TG[ PARAM ]: " << TG << "\n";
}
