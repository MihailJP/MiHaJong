#ifndef YKCLASS_H
#define YKCLASS_H

#include <string>
#include <functional>

class Yaku {
private:
	int yakuHan;
	std::string yakuName;
	std::function<bool ()> yakuProc;
	Yaku() {} // Default constructor
public:
	bool checkYaku() {return this->yakuProc();}
	// Constructor
	Yaku(std::string name, int han, std::function<bool ()> f) {
	}
};

#endif
