#include "init.h"

using namespace std;

// 設定したルールに基づいて役インスタンスを初期化する
__declspec(dllexport) list<Yaku>* initYaku() {
	static list<Yaku> yakuCatalog; // 初期化する
	/*
	 * ここにコンストラクタを並べる
	 */
	return &yakuCatalog;
}
