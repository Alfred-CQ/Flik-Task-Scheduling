#include <flik.h>

int main()
{
    VMS vmachines = load_VMachines("dataset/VMachine_Flik.csv");
    TASKS tasks = load_Tasks("dataset/Task_Flik.csv");

    Flik flik(vmachines, tasks, Y);

    flik.colony_Launch(100);

    return 0;
}
