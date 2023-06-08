#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip>

#include "vmachine.h"
#include "task.h"

typedef std::vector<Task> TASKS;
typedef std::vector<VMachine> VMS;

typedef std::vector<std::vector<Task>> CONTAINER;
typedef std::pair<VMachine, Task> GENOME;
typedef std::vector<GENOME> CHROMOSOME;

static VMS load_VMachines(const std::string &filename)
{
    VMS machines;

    std::ifstream file(filename);

    if (!file.is_open())
        return machines;

    std::string line;
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

static CONTAINER get_Container(int num_groups, const TASKS &tasks)
{
    TASKS shuffled_Tasks = tasks;
    std::shuffle(shuffled_Tasks.begin(), shuffled_Tasks.end(), std::random_device{});

    CONTAINER container(num_groups);

    int groupIdx = 0;

    for (Task &task : shuffled_Tasks)
    {
        container[groupIdx].push_back(task);
        groupIdx = (groupIdx + 1) % num_groups;
    }

    return container;
}

static float get_RunningTime(GENOME genome)
{
    return (genome.second.get_MIPS() / (genome.first.get_Mips() * genome.first.get_ClockFrequency())) +
           (genome.second.get_MIPS() / genome.first.get_Bandwidth()) +
           (genome.second.get_MIPS() / (genome.first.get_RAM() * 1024)) +
           (genome.second.get_MIPS() / (genome.first.get_Storage()));
}

static void print_Chromosomes(const std::vector<CHROMOSOME> &chromosomes)
{
    float totalMachineMIPS = 0.0;
    float totalTaskMIPS = 0.0;
    float totalEstimatedTime = 0.0;
    float totalRealTime = 0.0;

    std::cout << "No.  | Machine ID | Task ID |   CU   | Machine MIPS | Task MIPS | Estimated Time | Real Time\n";
    std::cout << "--------------------------------------------------------------------------------------------\n";

    int chromosomeCount = 1;
    for (const CHROMOSOME &chromosome : chromosomes)
    {
        for (const GENOME &genome : chromosome)
        {
            float realTime = get_RunningTime(genome);
            std::cout << std::setw(4) << chromosomeCount << " | "
                      << std::setw(10) << genome.first.get_VirtualMachineId() << " | "
                      << std::setw(7) << genome.second.get_TaskId() << " | "
                      << std::setw(6) << genome.first.get_ComputeUnit() << " | "
                      << std::setw(12) << genome.first.get_Mips() << " | "
                      << std::setw(9) << genome.second.get_MIPS() << " | "
                      << std::setw(14) << genome.second.get_Duration() << " | "
                      << std::setw(9) << realTime << '\n';

            totalMachineMIPS += genome.first.get_Mips();
            totalTaskMIPS += genome.second.get_MIPS();
            totalEstimatedTime += genome.second.get_Duration();
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
