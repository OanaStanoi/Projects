
About tool:
Name:  HackerNewsStatistics

USAGE                              : HackerNewsStatistics [OPTION]
OPTIONS
  -d, --distinctQueries            : Display the number of distinct queries in a specified time range
  -p, --popularQueries <int>       : Display the top N(<int>) popular queries in a specified time range
  -t, --timeRange <min> <max>      : Specify the time range (Example: <hour:minute:day> <hour:minute:day>)
  -h, --help                       : Display the usage message

1. A makefile is provided with the source files (tool name is created via Makefile)
2. Please make sure that the log file hn_logs.tsv is placed under the same directory where the tool is launched. The name of the file is hardcoded inside the tool.
	(I agree that another command line option can be implemented to specify the log file name)

#########################################################################################################

About command line options:

1. The options "-d" and "-p" are mutually exclusive
2. The option "-t" has no effect when used alone
3. Usage example:
	HackerNewsStatistics -p 15 -t 00:00:00 23:59:59
	HackerNewsStatistics -d -t 00:00:00 23:59:59
4. Time range must be specified in format <hour:minute:second>, with 6 digits and separator ":" (8 characters total)
	In order to have an "easy to type" command line for "-t" option, I chosed to consider as time range only the "hour, minute, second" from the log provided. 
	So the search is done for those hours in each days/months/years. 
	The application can be extened to y:m:d in the same manner as it is done for h:m:s.

#########################################################################################################

About algorithm:

1. I filtered the log time with the user provided range at parsing time, so that there is no need of storing the log time. 
2. I choosed std::map to store and process the log information, storing the query as KEY and its occurence as VALUE. By doing this the map is easily usable for both requests, distinct and popular queries.

 

