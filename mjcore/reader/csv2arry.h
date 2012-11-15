#ifndef CSV2ARRY_H
#define CSV2ARRY_H

#include <vector>
#include <string>
#include <cstring>
#include "../strcode.h"

class CSVReader {
public:
	typedef std::vector<CodeConv::tstring> RECORD;
	typedef std::vector<RECORD> CsvVecVec;
	static void parsecsv (CsvVecVec&, LPCTSTR csv);
};

#endif
