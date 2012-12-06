#pragma once

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
