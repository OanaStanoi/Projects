#ifndef HNSTATISTICS_HH
#define HNSTATISTICS_HH

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include "performance.h"

struct Queries
{
Queries(int time, const std::string& queryName)
:_time(time)
,_queryName(queryName)
{}

std::string _queryName;
int _time;
//int occurenceQuery;
};

class HnStatistics
{

public:
  HnStatistics();
  ~HnStatistics();

  int parseCommandLine(int&, char* []);
  int run();

private:
  void displayHelp();
  bool parseTsvFile();
  bool displayDistinctQueries();
  bool displayPopularQueries();
  time_t convertToSeconds(const std::string&);

  bool _showHelp;
  bool _showDistinctQ;
  bool _showPopularQ;

  int _pQ;
  time_t _minTimeRange;
  time_t _maxTimeRange;

  std::map<std::string, int> _dataMap;

  Performance* _perf;

};

extern HnStatistics* initSession();

#endif
