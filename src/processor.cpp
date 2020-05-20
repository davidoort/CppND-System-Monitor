#include "processor.h"

#include <iostream>

// Return the aggregate CPU utilization. float has less precision than double. Keep this in mind.
float Processor::Utilization() {
  long idle = LinuxParser::IdleJiffies();
//   std::cout << "Idle: " << idle << std::endl;
  long nonidle = LinuxParser::ActiveJiffies();

  long total = idle + nonidle;

  long totald = total - prevtotal;
  long idled = idle - previdle;

  prevtotal = total;
  previdle = idle;

//   std::cout << "Totald: " << totald << std::endl;

  return totald == 0 ? 0 : (totald - idled) / static_cast<float>(totald);
}