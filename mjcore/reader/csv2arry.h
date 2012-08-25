#ifndef CSV2ARRY_H
#define CSV2ARRY_H

#include <vector>
#include <string>
#include <cstring>

class CSVReader {
public:
	typedef std::vector<std::string> RECORD;
	typedef std::vector<RECORD> CsvVecVec;
	static void parsecsv (CsvVecVec&, const char* csv);
};

#endif
