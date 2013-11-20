#pragma once

#include <map>
#include "../../common/strcode.h"

class INIParser {
public:
	typedef std::map<CodeConv::tstring, CodeConv::tstring> RECORD;
	typedef std::map<CodeConv::tstring, RECORD> IniMapMap;
	static void parseini(IniMapMap&, LPCTSTR);
public: /* Monostate class: cannot be instantiated */
	INIParser() = delete;
	INIParser(const INIParser&) = delete;
	INIParser& operator= (const INIParser&) = delete;
	~INIParser() = delete;
};
