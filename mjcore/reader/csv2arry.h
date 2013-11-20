#pragma once

#include <vector>
#include "../../common/strcode.h"

class CSVReader {
public:
	typedef std::vector<CodeConv::tstring> RECORD;
	typedef std::vector<RECORD> CsvVecVec;
	static void parsecsv (CsvVecVec&, LPCTSTR csv);
public: /* Monostate class: cannot be instantiated */
	CSVReader() = delete;
	CSVReader(const CSVReader&) = delete;
	CSVReader& operator= (const CSVReader&) = delete;
	~CSVReader() = delete;
};
