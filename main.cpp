#include <stdlib.h>

#include "hn_statistics.h"

int main(int argc, char* argv[])
{
	int returned = 0;

	HnStatistics* stat = initSession();

	if (stat->parseCommandLine(argc, argv) != 0){
		return 1;
	}
	stat->run();

	return returned;
}


