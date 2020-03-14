#include "processor.h"
#include <vector>
#include <linux_parser.h>
using std::vector;


float Processor::Utilization() { 

    return LinuxParser::CpuUtilization();

 }