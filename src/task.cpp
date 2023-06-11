#include "task.h"

Task::Task(std::string task_Id,
           int mips,
           int ram,
           int bandwidth,
           float duration)
    : task_Id(task_Id), mips(mips),
      ram(ram), bandwidth(bandwidth),
      duration(duration) {}

std::string Task::get_TaskId() const
{
    return task_Id;
}

int Task::get_MIPS() const
{
    return mips;
}

int Task::get_RAM() const
{
    return ram;
}

int Task::get_Bandwidth() const
{
    return bandwidth;
}

float Task::get_Duration() const
{
    return duration;
}