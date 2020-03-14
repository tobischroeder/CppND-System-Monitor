#include "processor.h"
#include <vector>
#include <linux_parser.h>
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

    return LinuxParser::CpuUtilization();

 }