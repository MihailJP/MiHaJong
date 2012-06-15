#include "init.h"

// 設定したルールに基づいて役インスタンスを初期化する
void createYakuCatalog(std::list<Yaku> yakuCatalog) {
	/*
	 * ここにコンストラクタを並べる
	 */
	return;
}

__declspec(dllexport) void* initYaku() {
	static std::list<Yaku> yakuCatalog; // 初期化する
	/*
	 * ここにコンストラクタを並べる
	 */
	return &yakuCatalog;
}
