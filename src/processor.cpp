#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() { 
  std::vector<std::string> Utilization = LinuxParser::CpuUtilization();
  long user = std::stol(Utilization[0]);
  long nice = std::stol(Utilization[1]);
  long system = std::stol(Utilization[2]);
  long idle = std::stol(Utilization[3]);
  long iowait = std::stol(Utilization[4]);
  long irq = std::stol(Utilization[5]);
  long softirq = std::stol(Utilization[6]);
  long steal = std::stol(Utilization[7]);
  
  long PrevIdle = prevIdle_ + prevIOwait_;
  long Idle = idle + iowait;
  
  long PrevNonIdle = prevUser_ + prevNice_ + prevSystem_ + prevIRQ_ + prevSoftIRQ_ + prevSteal_;
  long NonIdle = user + nice + system + irq + softirq + steal;
  
  long PrevTotal = PrevIdle + PrevNonIdle;
  long Total = Idle + NonIdle;
  
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;
  
  prevUser_ = user;
  prevNice_ = nice;
  prevSystem_ = system;
  prevIdle_ = idle;
  prevIOwait_ = iowait;
  prevIRQ_ = irq;
  prevSoftIRQ_ = softirq;
  prevSteal_ = steal;
  
  return (totald - idled) / totald; 
}