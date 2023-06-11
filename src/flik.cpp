#include "flik.h"

Flik::Flik(const VMS &vms,
           const TASKS &tasks, int num_groups)
    : vms(vms), tasks(tasks), num_groups(num_groups)
{
    for (int i = 0; i < Y; ++i)
    {
        fit_vals.push_back(0.0);
        survivors.push_back(nullptr);
    }
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
    sum_fitness = 0.0;

    for (int i = 0; i < population.size(); ++i)
    {
        float realTime = 0.0;

        for (const GENOME *genome : (*population[i]))
            realTime += get_RunningTime(*genome);

        sum_fitness += realTime;
        fit_vals[i] = realTime;
    }
}

void Flik::selection()
{
    std::vector<float> circular_disk(fit_vals.size());
    circular_disk[0] = fit_vals[0] / sum_fitness;

    for (int i = 1; i < circular_disk.size(); ++i)
        circular_disk[i] = circular_disk[i - 1] + (fit_vals[i] / sum_fitness);

    std::cout << "No.  |  Task ID  | Machine ID \n"
              << "------------------------------\n";

    for (int i = 0; i < fit_vals.size(); ++i)
    {
        std::cout << std::setw(4) << i + 1 << " | "
                  << std::setw(9) << fit_vals[i] << " | "
                  << std::setw(9) << circular_disk[i] << " | \n";
    }

    int numb_survivors = Y * SELECTION_RATE;

    for (int i = 0; i < numb_survivors; ++i)
        survivors[i] = roulette(circular_disk);
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

CHROMOSOME *Flik::roulette(std::vector<float> circular_disk)
{
    std::random_device seed;
    std::mt19937 generator(seed());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    CHROMOSOME *selected_chromosome = nullptr;

    do
    {
        float random_num1 = dis(generator);

        for (int i = 0; i < circular_disk.size(); i++)
        {
            if (random_num1 <= circular_disk[i] && selected_chromosome == nullptr)
                selected_chromosome = population[i];
        }
    } while (selected_chromosome == nullptr);

    return selected_chromosome;
}