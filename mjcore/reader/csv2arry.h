#pragma once

#include <vector>
#include "../strcode.h"

class CSVReader {
public:
	typedef std::vector<CodeConv::tstring> RECORD;
	typedef std::vector<RECORD> CsvVecVec;
	static void parsecsv (CsvVecVec&, LPCTSTR csv);
};
