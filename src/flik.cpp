#include "flik.h"

Flick::Flick(const std::vector<VMachine> &vms,
             const std::vector<Task> &tasks)
    : vms(vms), tasks(tasks) {}
