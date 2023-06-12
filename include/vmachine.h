#ifndef __VMACHINE_H__
#define __VMACHINE_H__

#include <iostream>
#include <string>

class VMachine
{
public:
    // Constructor
    VMachine(std::string vm_Id,
             std::string processor_Name,
             int mips, float clock_Frequency,
             int bandwidth,
             int ram,
             int storage,
             float compute_Unit);

    float pheromone = 0.0;
    bool status = false;

    // Getters
    std::string get_VirtualMachineId() const;
    std::string get_ProcessorName() const;
    int get_Mips() const;
    float get_ClockFrequency() const;
    int get_Bandwidth() const;
    int get_RAM() const;
    int get_Storage() const;
    float get_ComputeUnit() const;

private:
    std::string virtual_MachineId;
    std::string processor_Name;
    int mips;
    float clock_Frequency;
    int bandwidth;
    int ram;
    int storage;
    float compute_Unit;
};

#endif // !__VMACHINE_H__
