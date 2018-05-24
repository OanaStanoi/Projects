#include <iostream>
#include <unistd.h> // sleep

#include "performance.h"


Performance::Performance()
: _start_c(0)
, _end_c(0)
{
}

Performance::~Performance()
{
}

void Performance::startClock()
{
	_start_c = time(0);
}

void Performance::stopClock()
{
	_end_c = time(0);
}

void Performance::computeCPU()
{
	time_t cpu = _end_c - _start_c;
	std::cout << std::endl;
	std::cout << "CPU time used   : " << cpu << " s" << std::endl;
}

int parseLine(char* line){
    //line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

void Performance::computeMemory(){ //Note: this value is in KB
    FILE* file = fopen("/proc/self/status", "r");
    int virtual_result = -1;
	int physical_result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            virtual_result = parseLine(line);
        }
        if (strncmp(line, "VmRSS:", 6) == 0){
            physical_result = parseLine(line);
        }
    }
    fclose(file);
	
	std::cout << "Virtual memory  : " << virtual_result << " KB" << std::endl;
	std::cout << "Physical memory : " << physical_result << " KB" << std::endl;
}
