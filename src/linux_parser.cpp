#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// Read and return the system memory utilization: the fraction of used memory
float LinuxParser::MemoryUtilization() {
  float total_memory{0.0f}, free_memory{0.0f};
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "MemTotal:") {
          total_memory = std::stof(value);
        } else if (key == "MemFree:") {
          free_memory = std::stof(value);
          return (total_memory - free_memory) / total_memory;
        }
      }
    }
  }
  return 0.0;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return uptime;
  }
  return 0;
}

// Read and return all the jiffies for the system in a struct
void LinuxParser::SystemJiffiesReader(SystemJiffies& jiffies) {
  // long user, nice, system, idle, iowait, irq, softirq, steal, guest,
  // guest_nice;
  string line, a;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> a >> jiffies.user >> jiffies.nice >> jiffies.system >>
        jiffies.idle >> jiffies.iowait >> jiffies.irq >> jiffies.softirq >>
        jiffies.steal >> jiffies.guest >> jiffies.guest_nice;
  }
}

// Read and return the number of active jiffies (in clock ticks) for a PID
long LinuxParser::ActiveJiffies(int pid) {
  // From Stack Overflow post:
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  // active jiffies = utime + stime + cutime + cstime
  const int indx = 13;
  string line, buffer;
  long utime, stime, cutime, cstime;  // expressed in clock ticks
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < indx; i++) {
      linestream >> buffer;
    }
    linestream >> utime >> stime >> cutime >> cstime;
    return utime + stime + cutime + cstime;
  }
  return 0;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  // from Stack Overflow post: NonIdle = user + nice + system + irq + softirq +
  // steal
  SystemJiffies jiffies{};
  SystemJiffiesReader(jiffies);
  return jiffies.user + jiffies.nice + jiffies.system + jiffies.irq +
         jiffies.softirq + jiffies.steal;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  // from Stack Overflow post: Idle = idle + iowait
  SystemJiffies jiffies{};
  SystemJiffiesReader(jiffies);
  return jiffies.idle + jiffies.iowait;
}

// // Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int processes;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = std::stoi(value);
          return processes;
        }
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int processes;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          processes = std::stoi(value);
          return processes;
        }
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// Read and return the memory used by a process in kB
string LinuxParser::Ram(int pid) {
  string line, key, ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> ram) {
        if (key == "VmSize:") {
          return ram;
        }
      }
    }
  }
  // TODO: figure out why this line is reached
  // std::cout << "Don't print" << std::endl;

  return 0;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid:") {
          return uid;
        }
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, uid, username;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> uid) {
        if (uid == Uid(pid)) {
          return username;
        }
      }
    }
  }
  return 0;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  const int indx = 22;
  string line;
  string starttime;  // expressed in clock ticks
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < indx; i++) {
      linestream >> starttime;
    }
    long proc_starttime = std::stof(starttime) / sysconf(_SC_CLK_TCK);
    long system_uptime = LinuxParser::UpTime();
    return system_uptime - proc_starttime;
  }
  return 0;
}