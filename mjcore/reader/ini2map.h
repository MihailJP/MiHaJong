#ifndef INI2MAP_H
#define INI2MAP_H

#include <map>
#include <string>
#include <cstring>

class INIParser {
public:
	static void parseini(std::map<std::string, std::map<std::string, std::string> > &, const char*);
};

#endif
