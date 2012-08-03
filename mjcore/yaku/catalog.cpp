#include "catalog.h"

/* シングルトン インスタンス アクセサ */
yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// 設定したルールに基づいて役インスタンスを初期化する
void yaku::yakuCalculator::init() {
	YakuCatalog::Instantiate()->catalog.clear(); // リセット
	info("役カタログをリセットしました。");
	YakuCatalog::catalogInit::yakulst_contextual();
	YakuCatalog::catalogInit::yakulst_pinhu();
	YakuCatalog::catalogInit::yakulst_suit();
	YakuCatalog::catalogInit::yakulst_yaojiu();
	YakuCatalog::catalogInit::yakulst_triplet_1();
	/* TODO: 槓子系の役 */
	YakuCatalog::catalogInit::yakulst_sequence();
	/* TODO: その他の役 */
	YakuCatalog::catalogInit::yakulst_dora();
	info("役カタログの構築を完了しました。");
}
