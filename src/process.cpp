#include "process.h"

#include <unistd.h>

#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization as a fraction
float Process::CpuUtilization() const {
  // From Stack overflow post:
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  // seconds = uptime - (starttime / Hertz)
  // cpu_usage = ((total_time / Hertz) / seconds)
  long seconds = UpTime();
  float cpuutilization =
      (LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK)) / static_cast<float>(seconds);
  return cpuutilization;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() {
  float kBmemory = std::stof(LinuxParser::Ram(pid_));
  int mBmemory = static_cast<int>(kBmemory / std::pow(10, 3));
  return to_string(mBmemory);
}

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  if (a.CpuUtilization() < this->CpuUtilization()) {
    return true;
  }
  return false;
}
