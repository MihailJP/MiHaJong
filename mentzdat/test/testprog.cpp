#include "../../mjcore/mjimport.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>

TileCode tc_translate(int tc) {
	switch (tc) {
		case  0: return CharacterOne;
		case  1: return CharacterTwo;
		case  2: return CharacterThree;
		case  3: return CharacterFour;
		case  4: return CharacterFive;
		case  5: return CharacterSix;
		case  6: return CharacterSeven;
		case  7: return CharacterEight;
		case  8: return CharacterNine;
		case  9: return CircleOne;
		case 10: return CircleTwo;
		case 11: return CircleThree;
		case 12: return CircleFour;
		case 13: return CircleFive;
		case 14: return CircleSix;
		case 15: return CircleSeven;
		case 16: return CircleEight;
		case 17: return CircleNine;
		case 18: return BambooOne;
		case 19: return BambooTwo;
		case 20: return BambooThree;
		case 21: return BambooFour;
		case 22: return BambooFive;
		case 23: return BambooSix;
		case 24: return BambooSeven;
		case 25: return BambooEight;
		case 26: return BambooNine;
		case 27: return EastWind;
		case 28: return SouthWind;
		case 29: return WestWind;
		case 30: return NorthWind;
		case 31: return WhiteDragon;
		case 32: return GreenDragon;
		case 33: return RedDragon;
		default: return NoTile;
	}
}

void memclear(volatile void* buf, size_t sz) {
	memset(const_cast<void*>(buf), 0, sz);
}

void check_val(const GameTable* testStat, ShantenType sType, int expected, unsigned* failures, unsigned i) {
	Shanten shanten = ShantenAnalyzer::calcShanten(testStat, 0, sType);
	if (shanten != expected) {
		std::cerr << "failed: type " << (int)sType <<
			", calculated: " << (int)shanten << ", expected: " << expected << std::endl;
		++(*failures);
	}
}

unsigned do_test(std::ifstream& tcFile) {
	int tileNum[14], shantenVal[3];
	unsigned failures = 0u;
	GameTable testStat; memclear(&testStat, sizeof testStat);

	for (unsigned i = 0u; i < 10000u; ++i) {
		std::cerr << "\rTest case #" << std::setw(5) << std::setfill(' ') << (i + 1) << " ";
		std::cerr.flush();
		tcFile >>
			tileNum[ 0] >> tileNum[ 1] >> tileNum[ 2] >>
			tileNum[ 3] >> tileNum[ 4] >> tileNum[ 5] >>
			tileNum[ 6] >> tileNum[ 7] >> tileNum[ 8] >>
			tileNum[ 9] >> tileNum[10] >> tileNum[11] >>
			tileNum[12] >> tileNum[13] >>
			shantenVal[0] >> shantenVal[1] >> shantenVal[2];
		for (unsigned k = 0u; k < 14u; ++k)
			testStat.Player[0].Hand[k].tile = tc_translate(tileNum[k]);
		check_val(&testStat, shantenRegular, shantenVal[0], &failures, i);
		check_val(&testStat, shantenPairs,   shantenVal[2], &failures, i);
		check_val(&testStat, shantenOrphans, shantenVal[1], &failures, i);
	}
	return failures;
}

bool test() {
	std::ifstream tcFile; tcFile.exceptions(std::ios::failbit);
	tcFile.open("p_normal_10000.txt");
	unsigned failures = do_test(tcFile);
	std::cerr << "done" << std::endl;
	std::cerr << failures << " failure(s)" << std::endl;
	return failures == 0u;
}

int main() {
	testInit();
	return test() ? 0 : 1;
}
