#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpuUtilization_; }

string Process::Command() { return command_; }

string Process::Ram() { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return upTime_; }

bool Process::operator<(Process const& a) const { 
  if (cpuUtilization_ > a.cpuUtilization_)
    return true;
  else
    return false;
}

Process::Process(int pid) : pid_(pid) {
  calcCpuUtilization();
  detCommand();
  detRam();
  detUser();
  detUpTime();
}

void Process::calcCpuUtilization() {
  long upTime = LinuxParser::UpTime();
  std::vector<float> value = LinuxParser::CpuUtilization(Pid());
  if (value.size() == 5) {
    float totalTime = value[kUTime_] + value[kSTime_] + value[kCuTime_] + value[kCsTime_];
    float seconds = upTime - value[kStartTime_];
    cpuUtilization_ = totalTime / seconds;
  } else {
    cpuUtilization_ = 0;
  }
}

void Process::detCommand() { command_ = LinuxParser::Command(Pid()); }

void Process::detRam() {
  std::string value = LinuxParser::Ram(Pid());
  try {
    long conv = std::stol(value) / 1000;
    ram_ = std::to_string(conv);
  } catch (const std::invalid_argument& arg) {
    ram_ = "0";
  }
}

void Process::detUser() { user_ = LinuxParser::User(Pid()); } 

void Process::detUpTime() { upTime_ = LinuxParser::UpTime(Pid()); } 