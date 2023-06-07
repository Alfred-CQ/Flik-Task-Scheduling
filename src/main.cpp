#include <flik.h>

int main()
{
    std::vector<VMachine> vmachines = load_VMachines("dataset/VMachine_Flik.csv");
    std::vector<Task> tasks = load_Tasks("dataset/Task_Flik.csv");

    Flick flick(vmachines, tasks);

    return 0;
}
