#include <iostream>

#include "hn_statistics.h"

static HnStatistics * hn_stat = NULL;

HnStatistics* initSession()
{
	if (!hn_stat) {
		hn_stat = new HnStatistics();
	}
	return hn_stat;
}

HnStatistics::HnStatistics()
: _showHelp(false)
, _showDistinctQ(false)
, _showPopularQ(false)
, _pQ(0)
, _minTimeRange(0)
, _maxTimeRange(0)
{
	_dataMap.clear();
	_perf = new Performance();
}

HnStatistics::~HnStatistics()
{
	delete _perf;
}

void HnStatistics::displayHelp()
{
	std::cout << std::endl;
	std::cout << "USAGE                              : HackerNewsStatistics [OPTION] " << std::endl << std::endl;
	std::cout << "OPTIONS " << std::endl;
	std::cout << "  -d, --distinctQueries            : Display the number of distinct queries in a specified time range" << std::endl;
	std::cout << "  -p, --popularQueries <int>       : Display the top N(<int>) popular queries in a specified time range" << std::endl;
	std::cout << "  -t, --timeRange <min> <max>      : Specify the time range (Example: <hour:minute:day> <hour:minute:day>)" << std::endl;
	std::cout << "  -h, --help                       : Display the usage message " << std::endl << std::endl;
}

bool isOnlyDigit(const std::string& str)
{
	return str.find_first_not_of("0123456789:") == std::string::npos;
}

bool isTimeFormat(const std::string& str)
{
	if (!isOnlyDigit(str)) 
		return false;

	if (str.size() != 8) 
		return false;

	std::size_t found = str.find(":");
	if (found != std::string::npos && (found == 2 || found == 5))
	return true; else return false;

	return true;
}

int HnStatistics::parseCommandLine(int& argc, char* argv[])
{
	bool isRange = false;
	// check arguments number
	if (argc < 2) {
		displayHelp();
		return 1;
	}

	for (int i = 1; i < argc; ++i) {

		// display help
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 ) {
			displayHelp();
			_showHelp = true;
			return 0;
		} else {

			// handle arguments
			if  (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--distinctQueries") == 0 ) {
				_showDistinctQ = true;
			}

			if ( strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--popularQueries") == 0 ) {
				if (i + 1 == argc) {
					std::cerr << "### Too few arguments, please specify the number of queries to display after the --popularQueries option"<< std::endl; 
					return 1;
				}
				std::string s = argv[i+1];
				if (!isOnlyDigit(s)){
					std::cerr << "### The argument of --popularQueries is incorrect, please specify a correct number "<< std::endl;
					return 1;
				} else {
					_pQ = atoi(s.c_str());
					_showPopularQ = true;
				}
			}
		
			if ( strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--timeRange") == 0 ) {
				if (i + 1 == argc || i + 2 ==argc) {
					std::cerr << "### Too few arguments, please specify the time range <min> <max> after the --timeRange option (Example 00:00:00)"<< std::endl;
					return 1;
				}
				isRange = true;
				std::string min = argv[i+1];
				std::string max = argv[i+2];
				if (!isTimeFormat(min) || !isTimeFormat(max)){
					std::cerr << "### The arguments of --timeRange are incorrect, please specify correct numbers "<< std::endl;
					return 1;
				} else {
					_minTimeRange = convertToSeconds(min);
					_maxTimeRange = convertToSeconds(max);
				}
			}
		}
	}

	if ( (_showDistinctQ || _showPopularQ)  && !isRange ){
		std::cerr << "### Too few arguments, please specify a time range for displaying the queries"<< std::endl;
		return 1;
	} else if (  (!_showDistinctQ && !_showPopularQ) && isRange ) {
		std::cerr << "### Too few arguments, please specify which queries to display for the given time range"<< std::endl;
		return 1;
	} else if (_showDistinctQ && _showPopularQ) {
		std::cerr << "### --distinctQueries and --popularQueries are mutually exclusive options"<< std::endl;
		return 1;
	}

	return 0;
}

int HnStatistics::run()
{
	_perf->startClock();

	parseTsvFile();

	if (_showDistinctQ){
		displayDistinctQueries();
	} else if (_showPopularQ) {
		displayPopularQueries();
	}

	_perf->stopClock();
	_perf->computeCPU();
	_perf->computeMemory();

	return 0;
}

bool HnStatistics::parseTsvFile()
{
	std::string filename = "hn_logs.tsv";
	std::ifstream file(filename.c_str());
	if (!file) {
		std::cout << "Unable to load file " << filename.c_str() << std::endl;
		return false;
	} 

	std::string line;
	std::string date;
	std::string html;
	int logTime = 0;

	while (getline(file, line)) {
		size_t found = line.find('\t');
		if (found != std::string::npos) {
			date = line.substr(0, found);
			html = line.substr(found+1);

			logTime = convertToSeconds(date);
			if (_minTimeRange <= logTime && _maxTimeRange >= logTime) {
				_dataMap[html]++;
			}
		}
	}

	return true;
}

bool HnStatistics::displayDistinctQueries() 
{
	// Output the number of distinct queries that have been done during a specific time range
	std::cout << std::endl;
	std::cout << _dataMap.size() << " distinct queries registered between time range [" 
			<< _minTimeRange << " | " << _maxTimeRange << "] s"<< std::endl << std::endl;
}

bool HnStatistics::displayPopularQueries() 
{
	// Output the top N popular queries (one per line) that have been done during a specific time range
	std::cout << std::endl;
	std::cout << "  The " << _pQ << " most popular queries registered between time range ["
			<< _minTimeRange << " | " << _maxTimeRange << "] s:"<< std::endl;

	std::set<int> frequency;
	
	std::map<std::string, int>::iterator it = _dataMap.begin();
	for (; it != _dataMap.end(); ++it) {
		frequency.insert(it->second);
	}

	int check_pQ = 1;
	std::set<int>::reverse_iterator rit = frequency.rbegin();
	if (frequency.size() >=1 ) {
		for(; rit != frequency.rend(); ++rit) {
			if (check_pQ <= _pQ) {
				std::map<std::string, int>::iterator itdf = _dataMap.begin();
				for (; itdf != _dataMap.end(); ++itdf) {
					if (itdf->second == *rit) {
						std::cout << itdf->first << "   " << itdf->second << std::endl;
					}
				}
				++check_pQ;
			} 
		} 
	} else std::cout << "  Only distinct queries registered between this time range; Please try another time range" << std::endl;

}

time_t HnStatistics::convertToSeconds(const std::string& logTime)
{
	time_t hour = 0;
	time_t minute = 0;
	time_t second = 0;
	time_t ref_time = 0;

	size_t found = logTime.find_first_of(':');
	if (found != std::string::npos) {
		//get hour
		std::string h = logTime.substr(found-2, found);
		hour = (time_t)atoll(h.c_str());
		//get minute
		std::string m = logTime.substr(found+1, found+3);
		minute = (time_t)atoll(m.c_str());
		//get seconds
		std::string s = logTime.substr(found+4);
		second = (time_t)atoll(s.c_str());
	} else std::cerr << "### The format of time range is incorrect"<< std::endl;
	
	ref_time = hour*3600 + minute*60 + second;
	
	return ref_time;
}
