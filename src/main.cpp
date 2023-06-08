#include <flik.h>

#define N 100
#define Y 5
#define TG (N / Y)

int main()
{
    VMS vmachines = load_VMachines("dataset/VMachine_Flik.csv");
    TASKS tasks = load_Tasks("dataset/Task_Flik.csv");

    Flik flik(vmachines, tasks, Y);

    return 0;
}
