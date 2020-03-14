#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include <iostream>
#include <iomanip>


using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() { 
  float utilization{0};
  float memtotal{0};
  float memfree{0};

  string name, size, unit;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> name >> size >> unit)
      {
        if (name == "MemTotal:")
        {
          if (unit == "kB")
          {
             memtotal = std::stof(size) / (1024 * 1024);
          }
          
        }
        
        if (name == "MemFree:")
        {
          if (unit == "kB")
          {
            memfree = std::stof(size) / (1024 * 1024);
          }
        }
      }

    }
  }

  utilization = 1 - (memfree / (memtotal + memfree));

  return utilization; 
  
}


long LinuxParser::SystemUpTime() { 
  long uptime{0};
  string suptime, nuptime;
  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> suptime ;
  }

  uptime = std::stol(suptime, nullptr, 10);

  return uptime; 
}


long LinuxParser::Jiffies() { 
  return 0;
}


long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }


long LinuxParser::ActiveJiffies() { return 0; }


long LinuxParser::IdleJiffies() { return 0; }


float LinuxParser::CpuUtilization() { 
  string  cpu, user, nice, system, idle, iowait, irq, softirq, steal;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    if (cpu == "cpu")
    {
      float fnonidle = std::stof(user) + std::stof(nice) + std::stof(system) + std::stof(irq) + std::stof(softirq) + std::stof(steal);
      float fidle = std::stof(idle) + std::stof(iowait);
      float total = fnonidle + fidle;

      float cpu_uti = (total -fidle) / total;

      return cpu_uti;

    }

  }
  return 0; 
}


int LinuxParser::TotalProcesses() {

  string name, sprocesses;
  string line;
  int processes{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> name >> sprocesses)
      {
        if (name == "processes")
        {
          processes = stoi(sprocesses);
          return processes;
        }
      }
    }
  }
  
   return 0;
   
}


int LinuxParser::RunningProcesses()
 {
  string name, s_running_processes;
  string line;
  int running_processes{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> name >> s_running_processes)
      {
        if (name == "procs_running")
        {
          running_processes = stoi(s_running_processes);
          return running_processes;
        }
      }
    }
  }
  
   return 0;
 }


string LinuxParser::Command(int pid) 
{
  string command;
  string line;

  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/comm");
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command;
  
  }
   return command; 
   
}


string LinuxParser::Ram(int pid)
 { 
  string param, value;
  string line;
  
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/status");
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> param >> value)
      {
        if (param == "VmSize:")
        {
          long ram = stol(value, nullptr, 10);
          ram = ram / 1000;
          value = to_string(ram);
          
          return value;
        }
      }
    }
  }

  return string(); 
 }


string LinuxParser::Uid(int pid) 
{  
  string param, value;
  string line;
  
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/status");
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> param >> value)
      {
        if (param == "Uid:")
        {
          return value;
        }
      }
    }
  }

  return string(); 
}


string LinuxParser::User(int pid)
{ 
  string line;
  string name, x, key;
  string uid;
  uid = LinuxParser::Uid(pid);

  
  std::ifstream filestream("/etc/passwd");
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> key)
       {
        if (key == uid) 
        {
          return name;
        }
      }
    }
  }
  return string();
  
}


long LinuxParser::ProcessUpTime(int pid)
{
  long uptime{0};
  string param, line;
  int counter{1};
 
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/stat");
 
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> param)
    {
      if (counter == 22)
      {
        uptime = std::stol(param, nullptr, 10);
        break;
      }

      counter ++; 
    }
   
  }
    long hertz = sysconf(_SC_CLK_TCK);
    if (hertz == 0)
    {
      hertz = 1;
    }
    
    long seconds = LinuxParser::SystemUpTime() - (uptime / hertz);
    return seconds; 

}

float LinuxParser::CpuUtilization(int pid)
{
  int counter{1};
  std::vector<long> times;
  string param, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + "/stat");
  if(filestream.is_open())
  {
      std::getline(filestream, line);
    
      std::istringstream linestream(line);

      while(linestream >> param)
      {
        if (counter == 14 || counter == 15 || counter == 16 || counter == 17 || counter == 22)
        {
          times.push_back(stol(param, nullptr, 10));
         
        }
        counter ++; 
      }

    
  }

  if (!times.empty())
  {
    long hertz = sysconf(_SC_CLK_TCK);
    long total_time = (times[0] + times[1] + times[2] + times[3]) / hertz;
    long uptime = LinuxParser::SystemUpTime() - (times[4] / hertz);
    if (uptime == 0)
    {
      uptime = 1;
    }
    float output = float(total_time) / float(uptime);
    return  output; 
  }

  return 0;

  
}