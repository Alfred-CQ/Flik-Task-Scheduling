#ifndef __TASK_H__
#define __TASK_H__

#include <string>

class Task
{
public:
    Task(std::string task_Id,
         int mips,
         int ram,
         int bandwidth,
         float duration);

    // Getters
    std::string get_TaskId() const;
    int get_MIPS() const;
    int get_RAM() const;
    int get_Bandwidth() const;
    float get_Duration() const;

private:
    std::string task_Id;
    int mips;
    int ram;
    int bandwidth;
    float duration;
};

#endif // !__TASK_H__
