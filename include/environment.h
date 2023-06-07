#include <vector>
#include <fstream>
#include <sstream>

#include "vmachine.h"
#include "task.h"

static std::vector<VMachine> load_VMachines(const std::string &filename)
{
    std::vector<VMachine> machines;

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

static std::vector<Task> load_Tasks(const std::string &filename)
{
    std::vector<Task> tasks;
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