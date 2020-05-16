#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>
// #include <algorithm> 

#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  auto pids = LinuxParser::Pids();
  // Clear processes in processes_ that have an id which does not exist in pids
  processes_.erase(std::remove_if(processes_.begin(), processes_.end(),
                                  [pids](const Process& proc) {
                                    return std::find(pids.begin(), pids.end(),
                                                     proc.Pid()) == pids.end();
                                  }),
                   processes_.end());
  // Only add processes that do not already exist
  for (int pid : pids) {
    if (std::find_if(
            processes_.begin(), processes_.end(),
            [pid](const Process& proc) -> bool { return proc.Pid() == pid; }) == processes_.end()) {
      // If the process with id pid does not exist, add it to the list of
      // processes
      processes_.push_back(Process(pid));
    }
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }