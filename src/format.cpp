#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
  string time = "00:00:00";
  long calc = 0;
  if (seconds > 0) {
    calc = seconds / 3600;
    time = TimeToString(calc) + ":";
    calc = (seconds / 60) % 60;
    time += TimeToString(calc) + ":";
    calc = seconds % 60;
    time += TimeToString(calc);
    return time;
  }
  return time; 
}

std::string Format::TimeToString(long time) {
  if (time < 10) {
    return "0" + std::to_string(time);
  } else {
    return std::to_string(time);
  }
}