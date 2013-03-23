#pragma once

#include <string>
#include <map>
#include <array>
#include "gametbl.h"

class RankVal {
private: /* Singleton class idiom */
	RankVal(); // INTERNAL USE ONLY
	RankVal(const RankVal&); // DO NOT USE
	RankVal& operator=(const RankVal&); // DO NOT USE
private:
	typedef std::array<std::array<int, Players - 1>, Players + 1> RankValSet;
	std::map<std::string, RankValSet> rankValueMap; // �E�}�ݒ�ꗗ
public:
	static RankVal* Instantiate(); // Singleton instance accessor
	int getRankVal(const GameTable* gameStat, const std::string& ruletag, unsigned playersAboveBase, unsigned rank) const;
};