#ifndef PERFORMANCE_HH
#define PERFORMANCE_HH

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <time.h>
#include <string>
#include <string.h>

class Performance
{

public:
  Performance();
  ~Performance();

void startClock();
void stopClock();
void computeCPU();
void computeMemory();
private:

time_t _start_c;
time_t _end_c;

};


#endif
