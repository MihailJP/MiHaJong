#ifndef INI2MAP_H
#define INI2MAP_H

#include <map>
#include <string>
#include <cstring>

class INIParser {
public:
	typedef std::map<std::string, std::string> RECORD;
	typedef std::map<std::string, RECORD> IniMapMap;
	static void parseini(IniMapMap&, const char*);
};

#endif
