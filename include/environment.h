#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip>

#include "vmachine.h"
#include "task.h"

#define N 100
#define Y 10
#define TG (N / Y)

#define SELECTION_RATE 0.6
#define CROSSOVER_RATE 0.6
#define MUTATION_RATE 0.4

typedef std::vector<Task> TASKS;
typedef std::vector<VMachine> VMS;

typedef std::pair<Task *, VMachine *> GENOME;
typedef std::vector<GENOME *> CHROMOSOME;
typedef std::vector<CHROMOSOME *> POPULATION;

typedef std::vector<float> FITNESS_VALUES;

static VMS load_VMachines(const std::string &filename)
{
    VMS machines;

    std::ifstream file(filename);

    if (!file.is_open())
        return machines;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string vmId, processor_Name;
        int mips, bandwidth, ram, storage;
        float clock_Frequency, compute_Unit;

        if (std::getline(iss, vmId, ',') &&
            std::getline(iss, processor_Name, ',') &&
            (iss >> mips).ignore(1) &&
            (iss >> clock_Frequency).ignore(1) &&
            (iss >> bandwidth).ignore(1) &&
            (iss >> ram).ignore(1) &&
            (iss >> storage).ignore(1) &&
            (iss >> compute_Unit))
        {
            machines.emplace_back(vmId, processor_Name, mips, clock_Frequency, bandwidth, ram, storage, compute_Unit);
        }
        else
        {
            std::cout << "Parsing Error\n";
        }
    }

    file.close();

    return machines;
}

static TASKS load_Tasks(const std::string &filename)
{
    TASKS tasks;
    std::ifstream file(filename);

    if (!file.is_open())
        return tasks;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string task_Id, mips_Str, ram_Str, bandwidth_Str, duration_Str;

        std::getline(ss, task_Id, ',');
        std::getline(ss, mips_Str, ',');
        std::getline(ss, ram_Str, ',');
        std::getline(ss, bandwidth_Str, ',');
        std::getline(ss, duration_Str, ',');

        int mips = std::stoi(mips_Str);
        int ram = std::stoi(ram_Str);
        int bandwidth = std::stoi(bandwidth_Str);
        float duration = std::stof(duration_Str);

        tasks.emplace_back(task_Id, mips, ram, bandwidth, duration);
    }

    file.close();

    return tasks;
}

static float get_RunningTime(const GENOME &genome)
{
    return ((genome.first)->get_MIPS() / (genome.second)->get_ComputeUnit() / 5);
}

static void print_Population(const POPULATION &population)
{
    float totalMachineMIPS = 0.0;
    float totalTaskMIPS = 0.0;
    float totalEstimatedTime = 0.0;
    float totalRealTime = 0.0;

    int chromosomeCount = 1;

    for (const CHROMOSOME *chromosome : population)
    {
        std::cout << "No.  | Task ID | Machine ID |   CU   | Machine MIPS | Task MIPS | Estimated Time | Real Time\n";
        std::cout << "--------------------------------------------------------------------------------------------\n";

        for (const GENOME *genome : *chromosome)
        {

            float realTime = get_RunningTime(*genome);

            std::cout << std::setw(4) << chromosomeCount << " | "
                      << std::setw(7) << genome->first->get_TaskId() << " | "
                      << std::setw(10) << genome->second->get_VirtualMachineId() << " | "
                      << std::setw(6) << genome->second->get_ComputeUnit() << " | "
                      << std::setw(12) << genome->second->get_Mips() << " | "
                      << std::setw(9) << genome->first->get_MIPS() << " | "
                      << std::setw(14) << genome->first->get_Duration() << " | "
                      << std::setw(9) << realTime << '\n';

            totalMachineMIPS += genome->second->get_Mips();
            totalTaskMIPS += genome->first->get_MIPS();
            totalEstimatedTime += genome->first->get_Duration();
            totalRealTime += realTime;
            chromosomeCount++;
        }
        std::cout << "--------------------------------------------------------------------------------------------\n";
        std::cout << "Total Machine MIPS: " << totalMachineMIPS << '\n';
        std::cout << "Total Task MIPS: " << totalTaskMIPS << '\n';
        std::cout << "Total Estimated Time: " << totalEstimatedTime << '\n';
        std::cout << "Total Real Time: " << totalRealTime << '\n';

        if (totalMachineMIPS > 0.0)
        {
            float machineUsagePercentage = (totalTaskMIPS / totalMachineMIPS) * 100.0;
            std::cout << "Machine MIPS Usage Percentage: " << machineUsagePercentage << "%\n";
        }
        std::cout << "\n\n";
        totalMachineMIPS = totalTaskMIPS = totalEstimatedTime = totalRealTime = 0;
    }
}
