#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <process.h>
class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float _utilization;
};

#endif