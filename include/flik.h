#ifndef __FLIK__
#define __FLIK__

#include <vector>

#include <environment.h>

class Flik
{
public:
    Flik(const VMS &vms, const TASKS &tasks, int num_groups);

    // Methods
    // Core
    void encoder();

    // GA
    void make_Chromosomes();

    // Getters

private:
    VMS vms;
    TASKS tasks;

    CONTAINER container;
    std::vector<CHROMOSOME> chromosomes;

    int num_groups;
};

#endif // !__FLIK__
