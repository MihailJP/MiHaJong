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
	bool flag[TILE_SUIT_HONORS / TILE_SUIT_STEP]; bool yakuFlag = true;
	for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++) flag[i] = true;
	if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) {
		for (int k = 1; k < (chin_itsu ? TILE_NONFLOWER_MAX : TILE_SUIT_HONORS); k++)
			if (analysis->TileCount[k] > 0)
				for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
					if ((k / TILE_SUIT_STEP) != i) flag[i] = false;
	} else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1) {
		for (int k = 1; k < (chin_itsu ? TILE_NONFLOWER_MAX : TILE_SUIT_HONORS); k++)
			if (analysis->TileCount[k] > 0)
				for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
					if ((k / TILE_SUIT_STEP) != i) flag[i] = false;
	}
	else for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++) flag[i] = false;
	for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
		yakuFlag &= flag[i];
	return yakuFlag;
};
