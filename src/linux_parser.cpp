#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  std::ifstream stream(kOSPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
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
  string os, version, kernel, line;
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

float LinuxParser::MemoryUtilization() { 
  float memTotal = 0.0;
  float memFree = 0.0;
  float memUsed = 0.0;
  string line, key, value;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memTotal = std::stof(value);
        }
        else if (key == "MemFree") {
          memFree = std::stof(value);
          break;
        }
      }
    }
  }
  memUsed = memTotal - memFree;
  return (memUsed / memTotal); 
}

long LinuxParser::UpTime() { 
  string line;
  long upTime = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }    
  return upTime; 
}

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuVal{};
  string line, key, val_user, val_nice, val_system, val_idle, val_iowait, val_irq, val_softirq, val_steal, val_guest, val_guestNice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> val_user >> val_nice >> val_system >> val_idle >> val_iowait >> val_irq >> val_softirq >> val_steal >> val_guest >> val_guestNice) {
        if (key == "cpu") {
          cpuVal.push_back(val_user);
          cpuVal.push_back(val_nice);
          cpuVal.push_back(val_system);
          cpuVal.push_back(val_idle);
          cpuVal.push_back(val_iowait);
          cpuVal.push_back(val_irq);
          cpuVal.push_back(val_softirq);
          cpuVal.push_back(val_steal);
          cpuVal.push_back(val_guest);
          cpuVal.push_back(val_guestNice);
          return cpuVal;
        }
      }
    }
  }
  return {}; 
}

int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    } 
  }
  return 0;
}


std::vector<float> LinuxParser::CpuUtilization(int pid) {
  vector<float> cpuVal{};
  string line, value;
  float time = 0;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i=1; i<=kStartTime_; i++) {
        linestream >> value;
        if (i == kUTime_ || i == kSTime_ || i == kCuTime_ || i == kCsTime_ || i == kStartTime_) {
          time = std::stol(value) / sysconf(_SC_CLK_TCK);
          cpuVal.push_back(time);
        }
      }
    }
  }
  return cpuVal;
}

string LinuxParser::Command(int pid) { 
  string value = "";
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, value);
    return value;
  }  
  return value; 
}

string LinuxParser::Ram(int pid) { 
  string line, key;
  string value = "";
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return value;
        }
      }
    }
  }
  return value; 
}

string LinuxParser::Uid(int pid) { 
  string line, key;
  string value = "";
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line, key, x;
  string value = "";
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> x >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return value; 
}

long LinuxParser::UpTime(int pid) { 
  string line, value;
  long upTime = 0;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      for (int i=1; i<=kStartTime_; i++) {
        linestream >> value;
        if (i == kStartTime_) {
          upTime = std::stol(value) / sysconf(_SC_CLK_TCK);
          return UpTime() - upTime;
        }
      }
    }
  }
  return upTime; 
}