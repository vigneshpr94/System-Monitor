#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                      
  long int UpTime();                       
  bool operator<(Process const& a) const;  
  
  Process(int pid);
  
 private:
  int pid_;
  std::string user_;
  std::string command_;
  float cpuUtilization_;
  std::string ram_;
  long int upTime_;
  
  void calcCpuUtilization();
  void detCommand();
  void detRam();
  void detUser();
  void detUpTime();
  
  enum CPUStatesProcess {
    kUTime_ = 0,
    kSTime_,
    kCuTime_,
    kCsTime_, 
    kStartTime_ 
  };
};

#endif