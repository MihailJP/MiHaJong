#include "catalog.h"

#include "../../common/strcode.h"
#include <cassert>

/* シングルトン インスタンス アクセサ */
yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// 設定したルールに基づいて役インスタンスを初期化する
void yaku::yakuCalculator::YakuCatalog::catalogInit::init() {
	YakuCatalog::Instantiate()->catalog.clear(); // リセット
	info(_T("役カタログをリセットしました。"));
	yakulst_contextual();
	yakulst_irregular();
	yakulst_pinhu();
	yakulst_suit();
	yakulst_yaojiu();
	yakulst_triplet_1(); yakulst_triplet_2(); yakulst_triplet_3();
	yakulst_quad();
	yakulst_sequence();
	yakulst_misc();
	yakulst_dora();
	info(_T("役カタログの構築を完了しました。"));
}
void yaku::yakuCalculator::init() {
	YakuCatalog::catalogInit::init();
}

// ルール文字列から飜を設定する用
#ifndef GUOBIAO
yaku::yakuCalculator::Yaku::HANFUNC
	yaku::yakuCalculator::YakuCatalog::catalogInit::get_yaku_han
	(const std::string& RuleTag)
{
	std::string currcnf = RuleData::chkRule(RuleTag);
	if (currcnf == "1han") return 1_hanF;
	else if (currcnf == "2han") return 2_hanF;
	else if (currcnf == "3han") return 3_hanF;
	else if (currcnf == "4han") return 4_hanF;
	else if (currcnf == "5han") return 5_hanF;
	else if (currcnf == "6han") return 6_hanF;
	else if (currcnf == "8han") return 8_hanF;
	else if (currcnf == "2han_kuisagari") return 2_hanK;
	else if (currcnf == "3han_kuisagari") return 3_hanK;
	else if (currcnf == "4han_kuisagari") return 4_hanK;
	else if (currcnf == "5han_kuisagari") return 5_hanK;
	else if (currcnf == "6han_kuisagari") return 6_hanK;
	else if (currcnf == "8han_kuisagari") return 8_hanK;
	else if (currcnf == "1han_menzen") return 1_hanM;
	else if (currcnf == "2han_menzen") return 2_hanM;
	else if (currcnf == "3han_menzen") return 3_hanM;
	else if (currcnf == "4han_menzen") return 4_hanM;
	else if (currcnf == "5han_menzen") return 5_hanM;
	else if (currcnf == "6han_menzen") return 6_hanM;
	//else if (currcnf == "8han_menzen") return 8_hanM;
	else if (currcnf == "1han_50fu") return 1_hanF;
	else if (currcnf == "2han_25fu") return 2_hanF;
	else if (currcnf == "3han_30fu") return 3_hanF;
	else if (currcnf == "3han_40fu") return 3_hanF;
	else if (currcnf == "4han_40fu") return 4_hanF;
	else if (currcnf == "mangan") return 1_manganF;
	//else if (currcnf == "haneman") return 1.5_manganF;
	else if (currcnf == "baiman") return 2_manganF;
	else if (currcnf == "yakuman") return 1_yakumanF;
	else if (currcnf == "double_yakuman") return 2_yakumanF;
	else if (currcnf == "triple_yakuman") return 3_yakumanF;
	else if (currcnf == "quadruple_yakuman") return 4_yakumanF;
	else if (currcnf == "mangan_menzen") return 1_manganM;
	//else if (currcnf == "haneman_menzen") return 1.5_manganM;
	else if (currcnf == "baiman_menzen") return 2_manganM;
	else if (currcnf == "yakuman_menzen") return 1_yakumanM;
	else if (currcnf == "double_yakuman_menzen") return 2_yakumanM;
	else if (currcnf == "yakuman_with_pao") return 1_yakumanF;
	else if (currcnf == "double_yakuman_with_pao") return 2_yakumanF;
	else if (currcnf == "1han_added") return 1_hanD;
	else if (currcnf == "2han_added") return 2_hanD;
	else if (currcnf == "4han_added") return 4_hanD;
	else if (currcnf == "yakuman_unbound") return 1_yakumanD;
	else if (currcnf == "-4han")
		return yaku::yakuCalculator::Yaku::FixedHan(
		1_han,
#ifdef _MSC_VER
		yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(-5)
#else
		yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(-5)
#endif
		);
	else return 0_hanF;
}
#endif /* GUOBIAO */

/* 一色の判定 */
bool yaku::yakuCalculator::YakuCatalog::catalogInit::isshoku (const MENTSU_ANALYSIS* const analysis, bool chin_itsu) {
	if (analysis->shanten[ShantenType::all] >= 0) return false; // 足切り要件：和了ってない
	constexpr TileCode Tiles[][9] = {
		{
			CharacterOne, CharacterTwo,   CharacterThree, CharacterFour, CharacterFive,
			CharacterSix, CharacterSeven, CharacterEight, CharacterNine,
		}, {
			CircleOne,    CircleTwo,      CircleThree,    CircleFour,    CircleFive,
			CircleSix,    CircleSeven,    CircleEight,    CircleNine,
		}, {
			BambooOne,    BambooTwo,      BambooThree,    BambooFour,    BambooFive,
			BambooSix,    BambooSeven,    BambooEight,    BambooNine,
		}, {
			EastWind,     SouthWind,      WestWind,       NorthWind,
			WhiteDragon,  GreenDragon,    RedDragon,      NoTile,        NoTile,
		}
	};
	unsigned count[4] = {0};
	for (unsigned k = 0; k < 4; k++) {
		for (unsigned i = 0; i < 9; i++) {
			if (Tiles[k][i] != NoTile) {
				if (analysis->shanten[ShantenType::regular] == -1)
					count[k] += analysis->DuiziCount[Tiles[k][i]] + analysis->ShunziCount[Tiles[k][i]];
				else
					count[k] += analysis->TileCount[Tiles[k][i]];
			}
		}
	}
	if (analysis->shanten[ShantenType::regular] == -1) {
		assert(count[0] + count[1] + count[2] + count[3] == SizeOfMeldBuffer);
	} else {
		assert(count[0] + count[1] + count[2] + count[3] == NumOfTilesInHand);
	}
	return ((count[0] ? 1 : 0) + (count[1] ? 1 : 0) + (count[2] ? 1 : 0) == 1) &&
		((!chin_itsu) || (count[3] ? false: true));
};
