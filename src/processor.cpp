#include "processor.h"
#include <process.h>
#include <system.h>

#include <vector>

#include <process.h>

using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<Process> processes{LinuxParser::RunningProcesses()};

    for (unsigned i; i < processes.size(); i++)
    {
        _utilization += processes[i].CpuUtilization();
    }

    return _utilization;
 }