#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

 private:
  long prevUser_;
  long prevNice_;
  long prevSystem_;
  long prevIdle_;
  long prevIOwait_;
  long prevIRQ_;
  long prevSoftIRQ_;
  long prevSteal_;
  long prevGuest_;
  long prevGuestNice_;
};

#endif