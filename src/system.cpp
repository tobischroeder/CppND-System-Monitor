#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "processor.h"
#include "system.h"
#include "process.h"


using std::set;
using std::size_t;
using std::string;
using std::vector;


Processor& System::Cpu() { return this->cpu_; }


vector<Process>& System::Processes() { 

    vector<int> pids{LinuxParser::Pids()};

    set<int> extant_pids;
    for (Process & process : processes_)
    {
        extant_pids.insert(process.Pid());
    }
    
    for (int pid : pids)
    {
        if (extant_pids.find(pid) == extant_pids.end())
        {
            processes_.emplace_back(pid);
        }

    }

    std::sort(processes_.begin(), processes_.end());
    
    
    return this->processes_; 
}


std::string System::Kernel() { return LinuxParser::Kernel(); }


float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }


std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }


int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }


int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }


long int System::UpTime() { return LinuxParser::SystemUpTime(); }