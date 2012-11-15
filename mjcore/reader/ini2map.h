#ifndef INI2MAP_H
#define INI2MAP_H

#include <map>
#include <string>
#include <cstring>
#include "../strcode.h"

class INIParser {
public:
	typedef std::map<CodeConv::tstring, CodeConv::tstring> RECORD;
	typedef std::map<CodeConv::tstring, RECORD> IniMapMap;
	static void parseini(IniMapMap&, LPCTSTR);
};

#endif
