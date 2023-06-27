#include "flik.h"

Flik::Flik(const VMS &vms,
           const TASKS &tasks, int num_groups)
    : vms(vms), tasks(tasks), num_groups(num_groups)
{
    sum_pheromone = 1.0;

    for (int i = 0; i < Y; ++i)
    {
        fit_vals.push_back(0.0);
        survivors.push_back(nullptr);
        group_pheromones.push_back(1.0);
    }
}

void Flik::colony_Launch(int numb_epochs)
{
    std::cout << "Flik Task Scheduling [ Launch ] \n";

    encoder();

    print_VMachines(vms);
    print_Population(population);

    int iterations, count_crossover, count_mutations;
    for (int idx = 0; idx < population.size(); ++idx)
    {
        std::cout << " ***************" << idx << "***************\n";

        iterations = count_crossover = count_mutations = 0;

        if (idx > 0)
        {
            int i = 0;
            VMachine *unique = (*population[0])[0]->second;
            for (const CHROMOSOME *chromosome : population)
            {
                for (const GENOME *genome : *chromosome)
                {
                    if (genome->second != unique)
                    {
                        genome->second->pheromone -= (genome->second->pheromone / TG);
                        unique = genome->second;
                    }
                }
                ++i;
            }
        }

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
            {
                crossover();
                count_crossover++;
            }

            if (mut_rate < MUTATION_RATE)
            {
                mutation();
                count_mutations++;
            }
            iterations++;
        }

        for (CHROMOSOME *chromosome : population)
            std::sort(chromosome->begin(), chromosome->end(), compareGenomeByPheromone);

        int i = 0;

        for (const CHROMOSOME *chromosome : population)
        {
            // float group_pheromone = 0.0;
            VMachine *unique = (*chromosome)[0]->second;
            sum_pheromone = unique->pheromone;
            for (const GENOME *genome : *chromosome)
            {
                if (genome->second != unique)
                {
                    sum_pheromone += genome->second->pheromone;
                    unique = genome->second;
                }
                genome->second->pheromone += get_RunningTime(*genome);
            }
            group_pheromones[i] = sum_pheromone;
            ++i;
        }

        std::cout << "[ CROSSOVERS ]: " << count_crossover << "  "
                  << "[ MUTATIONS ]: " << count_mutations << " "
                  << "[ GLOBAL_P ]: " << sum_pheromone << "\n";
    }

    print_VMachines(vms);
    print_Population(population);
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
        float overload = 0.0;

        for (const GENOME *genome : (*population[i]))
        {
            overload += (*genome).second->pheromone;
            realTime += get_RunningTime(*genome);
        }

        realTime -= (((overload / group_pheromones[i]) / realTime));

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
    /*
        std::cout << "No.  |  Fit_Val  |    Fi \n"
                  << "------------------------------\n";

        for (int i = 0; i < fit_vals.size(); ++i)
        {
            std::cout << std::setw(4) << i + 1 << " | "
                      << std::setw(9) << fit_vals[i] << " | "
                      << std::setw(9) << circular_disk[i] << " | \n";
        }
    */
    int numb_survivors = Y * SELECTION_RATE;

    for (int i = 0; i < numb_survivors; ++i)
        survivors[i] = roulette(circular_disk);
}

void Flik::crossover()
{
    std::random_device seed;
    std::mt19937 generator(seed());
    std::uniform_real_distribution<> dis(0, TG - 2);

    int numb_survivors = Y * SELECTION_RATE;

    for (int i = 0; i < numb_survivors; i += 2)
    {
        int cutPoint1 = dis(generator);

        std::swap((*survivors[i])[cutPoint1], (*survivors[i + 1])[cutPoint1 + 1]);
    }
}

void Flik::mutation()
{
    int numb_survivors = Y * SELECTION_RATE;

    for (int i = 0; i < numb_survivors; ++i)
    {
        std::random_device seed;
        std::mt19937 generator(seed());
        std::uniform_real_distribution<> dis(0, TG - 1);

        int start = dis(generator);
        int end = dis(generator);

        if (start > end)
            std::swap(start, end);

        while (start < end)
        {
            std::swap((*survivors[i])[start]->second, (*survivors[i])[end]->second);
            start++;
            end--;
        }
    }
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

    int i{0}, j{0}, idx_task{vms.size() - 1};

    CHROMOSOME *chromosome = nullptr;

    fisherYates(chromosome);

    for (i = chromosome->size(); i < TG; ++i)
    {
        std::cout << idx_task << "\n";
        int vmIndex = dist(generator);
        GENOME *genome = new GENOME(&tasks[idx_task], &vms[vmIndex]);
        chromosome->push_back(genome);

        vms[vmIndex].status = true;
        idx_task++;
    }

    population.push_back(chromosome);

    int remaining_groups{Y - population.size()};

    for (i = 0; i < remaining_groups; ++i)
    {
        chromosome = new CHROMOSOME;

        for (j = 0; j < TG; ++j)
        {
            std::cout << idx_task << "\n";
            int vmIndex = dist(generator);
            GENOME *genome = new GENOME(&tasks[idx_task], &vms[vmIndex]);
            chromosome->push_back(genome);

            vms[vmIndex].status = true;
            idx_task++;
        }

        population.push_back(chromosome);
    }

    std::cout << "\nN [ VAL ]: " << tasks.size() << " vs N [ PARAM ]: " << N
              << "\nY [ VAL ]: " << population.size() << " vs Y [ PARAM ]: " << Y
              << "\nTG [ VAL ]: " << population[population.size() - 1]->size() << " vs TG [ PARAM ]: " << TG
              << "\nVMS available: " << vms.size() << "\n";
}

void Flik::fisherYates(CHROMOSOME *&incomplete)
{
    srand(time(NULL));

    int i, idx_task{0}, n{vms.size()}, tg_counter{1};

    CHROMOSOME *chromosome = new CHROMOSOME;

    for (i = n - 1; i > -1; --i)
    {
        int vmIndex = rand() % (i + 1);

        if (tg_counter % (TG + 1))
        {
            GENOME *genome = new GENOME(&tasks[idx_task], &vms[vmIndex]);
            chromosome->push_back(genome);
            idx_task++;
        }
        else
        {
            tg_counter = 1;
            population.push_back(chromosome);
            chromosome = new CHROMOSOME;
        }

        vms[vmIndex].status = true;
        std::swap(vms[i], vms[vmIndex]);

        tg_counter++;
    }

    incomplete = chromosome;

    std::cout << "[ FY ] Last Chromosome Size: " << chromosome->size() << "\n";
    std::cout << "[ FY ] Population Size: " << population.size() << "\n";
}

CHROMOSOME *Flik::roulette(std::vector<float> circular_disk)
{
    std::random_device seed;
    std::mt19937 generator(seed());
    std::uniform_real_distribution<> dis(0.0, sum_fitness);

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
