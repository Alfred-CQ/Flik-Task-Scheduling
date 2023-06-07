#ifndef __FLIK__
#define __FLIK__

#include <vector>

#include <environment.h>

class Flick
{
public:
    Flick(const std::vector<VMachine> &vms, const std::vector<Task> &tasks);

    // Methods

private:
    std::vector<VMachine> vms;
    std::vector<Task> tasks;
};

#endif // !__FLIK__
