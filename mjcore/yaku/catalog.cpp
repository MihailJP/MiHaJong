#include "catalog.h"

/* シングルトン インスタンス アクセサ */
yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// 設定したルールに基づいて役インスタンスを初期化する
void yaku::yakuCalculator::YakuCatalog::catalogInit::init() {
	YakuCatalog::Instantiate()->catalog.clear(); // リセット
	info("役カタログをリセットしました。");
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
	info("役カタログの構築を完了しました。");
}
void yaku::yakuCalculator::init() {
	YakuCatalog::catalogInit::init();
}

// ルール文字列から飜を設定する用
yaku::yakuCalculator::Yaku::HANFUNC
	yaku::yakuCalculator::YakuCatalog::catalogInit::get_yaku_han
	(const std::string& RuleTag)
{
	std::string currcnf = RuleData::chkRule(RuleTag);
	if (currcnf == "1han") return yaku::yakuCalculator::Yaku::yval_1han;
	else if (currcnf == "2han") return yaku::yakuCalculator::Yaku::yval_2han;
	else if (currcnf == "3han") return yaku::yakuCalculator::Yaku::yval_3han;
	else if (currcnf == "4han") return yaku::yakuCalculator::Yaku::yval_4han;
	else if (currcnf == "5han") return yaku::yakuCalculator::Yaku::yval_5han;
	else if (currcnf == "6han") return yaku::yakuCalculator::Yaku::yval_6han;
	else if (currcnf == "8han")
		return yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_8han);
	else if (currcnf == "2han_kuisagari") return yaku::yakuCalculator::Yaku::yval_2han_kuisagari;
	else if (currcnf == "3han_kuisagari") return yaku::yakuCalculator::Yaku::yval_3han_kuisagari;
	else if (currcnf == "4han_kuisagari") return yaku::yakuCalculator::Yaku::yval_4han_kuisagari;
	else if (currcnf == "5han_kuisagari") return yaku::yakuCalculator::Yaku::yval_5han_kuisagari;
	else if (currcnf == "6han_kuisagari") return yaku::yakuCalculator::Yaku::yval_6han_kuisagari;
	else if (currcnf == "8han_kuisagari")
		return yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_8han);
	else if (currcnf == "1han_menzen") return yaku::yakuCalculator::Yaku::yval_1han_menzen;
	else if (currcnf == "2han_menzen") return yaku::yakuCalculator::Yaku::yval_2han_menzen;
	else if (currcnf == "3han_menzen") return yaku::yakuCalculator::Yaku::yval_3han_menzen;
	else if (currcnf == "4han_menzen") return yaku::yakuCalculator::Yaku::yval_4han_menzen;
	else if (currcnf == "5han_menzen") return yaku::yakuCalculator::Yaku::yval_5han_menzen;
	else if (currcnf == "6han_menzen") return yaku::yakuCalculator::Yaku::yval_6han_menzen;
	//else if (currcnf == "8han_menzen") return yaku::yakuCalculator::Yaku::yval_8han_menzen;
	else if (currcnf == "1han_50fu") return yaku::yakuCalculator::Yaku::yval_1han;
	else if (currcnf == "2han_25fu") return yaku::yakuCalculator::Yaku::yval_2han;
	else if (currcnf == "3han_30fu") return yaku::yakuCalculator::Yaku::yval_3han;
	else if (currcnf == "3han_40fu") return yaku::yakuCalculator::Yaku::yval_3han;
	else if (currcnf == "4han_40fu") return yaku::yakuCalculator::Yaku::yval_4han;
	else if (currcnf == "mangan") return yaku::yakuCalculator::Yaku::yval_mangan;
	//else if (currcnf == "haneman") return yaku::yakuCalculator::Yaku::yval_haneman;
	else if (currcnf == "baiman") return yaku::yakuCalculator::Yaku::yval_baiman;
	else if (currcnf == "yakuman") return yaku::yakuCalculator::Yaku::yval_yakuman;
	else if (currcnf == "double_yakuman") return yaku::yakuCalculator::Yaku::yval_double_yakuman;
	else if (currcnf == "triple_yakuman") return yaku::yakuCalculator::Yaku::yval_triple_yakuman;
	else if (currcnf == "quadruple_yakuman") return yaku::yakuCalculator::Yaku::yval_quad_yakuman;
	else if (currcnf == "mangan_menzen") return yaku::yakuCalculator::Yaku::yval_mangan_menzen;
	//else if (currcnf == "haneman_menzen") return yaku::yakuCalculator::Yaku::yval_haneman_menzen;
	else if (currcnf == "baiman_menzen") return yaku::yakuCalculator::Yaku::yval_baiman_menzen;
	else if (currcnf == "yakuman_menzen") return yaku::yakuCalculator::Yaku::yval_yakuman_menzen;
	else if (currcnf == "double_yakuman_menzen") return yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen;
	else if (currcnf == "yakuman_with_pao") return yaku::yakuCalculator::Yaku::yval_yakuman;
	else if (currcnf == "double_yakuman_with_pao") return yaku::yakuCalculator::Yaku::yval_double_yakuman;
	else if (currcnf == "1han_added") return yaku::yakuCalculator::Yaku::yval_1han_dependent;
	else if (currcnf == "2han_added") return yaku::yakuCalculator::Yaku::yval_2han_dependent;
	else if (currcnf == "4han_added") return yaku::yakuCalculator::Yaku::yval_4han_dependent;
	else if (currcnf == "yakuman_unbound") return yaku::yakuCalculator::Yaku::yval_yakuman_dependent;
	else return yaku::yakuCalculator::Yaku::HANFUNC();
}

/* 一色の判定 */
bool yaku::yakuCalculator::YakuCatalog::catalogInit::isshoku (const MENTSU_ANALYSIS* const analysis, bool chin_itsu) {
	if (analysis->shanten[ShantenAnalyzer::shantenAll] >= 0) return false; // 足切り要件：和了ってない
	const tileCode Tiles[][9] = {
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
	for (unsigned k = 0; k < 4; k++)
		for (unsigned i = 0; i < 9; i++)
			if (Tiles[k][i] != NoTile)
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
					count[k] += analysis->DuiziCount[Tiles[k][i]] + analysis->ShunziCount[Tiles[k][i]];
				else
					count[k] += analysis->TileCount[Tiles[k][i]];
	return ((count[0] ? 1 : 0) + (count[1] ? 1 : 0) + (count[2] ? 1 : 0) == 1) &&
		((!chin_itsu) || (count[3] ? false: true));
};
