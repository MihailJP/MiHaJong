#pragma once

#include <map>
#include "../strcode.h"

class INIParser {
public:
	typedef std::map<CodeConv::tstring, CodeConv::tstring> RECORD;
	typedef std::map<CodeConv::tstring, RECORD> IniMapMap;
	static void parseini(IniMapMap&, LPCTSTR);
};
