#include "init.h"

// 設定したルールに基づいて役インスタンスを初期化する
__declspec(dllexport) std::list<Yaku>* initYaku() {
	static std::list<Yaku> yakuCatalog; // 初期化する
	/*
	 * ここにコンストラクタを並べる
	 */
	return &yakuCatalog;
}
