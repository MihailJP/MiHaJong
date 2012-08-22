#ifndef CSV2ARRY_H
#define CSV2ARRY_H

#include <vector>
#include <string>
#include <cstring>

class CSVReader {
public:
	static void parsecsv (std::vector<std::vector<std::string> > &, const char* csv);
};

#endif
