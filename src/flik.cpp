#include "flik.h"

Flik::Flik(const VMS &vms,
           const TASKS &tasks, int num_groups)
    : vms(vms), tasks(tasks), num_groups(num_groups)
{
    encoder();
}

void Flik::encoder()
{
    container = get_Container(num_groups, tasks);
    make_Chromosomes();
    print_Chromosomes(chromosomes);
}

void Flik::make_Chromosomes()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    for (const TASKS &group : container)
    {
        CHROMOSOME chromosome;

        for (const Task &task : group)
        {
            std::uniform_int_distribution<size_t> dist(0, vms.size() - 1);
            size_t vmIndex = dist(gen);

            GENOME genome(vms[vmIndex], task);
            chromosome.push_back(genome);
        }

        chromosomes.push_back(chromosome);
    }
}
