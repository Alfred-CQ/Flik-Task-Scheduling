#include "vmachine.h"

// Constructor
VMachine::VMachine(const std::string &vm_Id,
                   const std::string &processor_Name,
                   int mips, float clock_Frequency,
                   int bandwidth, int ram, int storage, float compute_Unit)
    : virtual_MachineId(vm_Id),
      processor_Name(processor_Name),
      mips(mips), clock_Frequency(clock_Frequency), bandwidth(bandwidth),
      ram(ram), storage(storage), compute_Unit(compute_Unit) {}

// Getters
std::string VMachine::get_VirtualMachineId() const
{
    return virtual_MachineId;
}

std::string VMachine::get_ProcessorName() const
{
    return processor_Name;
}

int VMachine::get_Mips() const
{
    return mips;
}

float VMachine::get_ClockFrequency() const
{
    return clock_Frequency;
}

int VMachine::get_Bandwidth() const
{
    return bandwidth;
}

int VMachine::get_RAM() const
{
    return ram;
}

int VMachine::get_Storage() const
{
    return storage;
}

float VMachine::get_ComputeUnit() const
{
    return compute_Unit;
}