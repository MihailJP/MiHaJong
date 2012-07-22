#include "catalog.h"

// 設定したルールに基づいて役インスタンスを初期化する
__declspec(dllexport) void yaku::yakuCalculator::init() {
	YakuCatalog::Instantiate()->catalog.clear(); // リセット
	info("役カタログをリセットしました。");
	/*
	 * ここにコンストラクタを並べる
	 */
	YakuCatalog::Instantiate()->catalog.push_back(Yaku( // テスト用のダミーの役
		"ダミー", yaku::yakuCalculator::Yaku::yval_1han,
		[](const GameTable* const, const MENTSU_ANALYSIS* const) {
			return true;
		}
	));
	info("役カタログの構築を完了しました。");
}

