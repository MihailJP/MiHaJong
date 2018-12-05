#pragma once

#include <type_traits>
#ifdef _WIN32
#include <tchar.h>
#endif /*_WIN32*/
#include "largenum.h"
#include "gametbl.h"

namespace mihajong_structs {

struct YakuResult {
	static const int SemiMangan = 12500; // 半満貫
	static const int LimitMinus = 100; // マイナス翻の下限
	static const unsigned int nameBufSize = 1024; // 名前バッファのサイズ
	bool isValid; // 和了っているかどうか
	int BasePoints; // 符
	int CoreHan, CoreSemiMangan; // 翻
	int BonusHan, BonusSemiMangan; // 翻（縛りを満たさない）
	int DoraQuantity; // ドラの数
	int UraDoraQuantity; // 裏ドラの数
	int AkaDoraQuantity; // 赤ドラの数
	int AoDoraQuantity; // 青ドラの数
	int AliceDora; // アリスドラの数
	int FlowerQuantity; // 花牌の数
	LargeNum AgariPoints; // 合計和了点
	TCHAR yakuNameList[nameBufSize];
	TCHAR yakuValList[nameBufSize];
	TCHAR yakumanNameList[nameBufSize];
	TCHAR yakumanValList[nameBufSize];
	// -----------------------------------------------------------------
	MeldBuf MianziDat; // 面子に分解したデータ
	// -----------------------------------------------------------------
	static void Init(YakuResult* const myInstance) { // インスタンスを初期化する
		myInstance->isValid = false;
		myInstance->BasePoints = 20;
		myInstance->CoreHan = myInstance->CoreSemiMangan =
			myInstance->BonusHan = myInstance->BonusSemiMangan =
			myInstance->DoraQuantity = myInstance->UraDoraQuantity =
			myInstance->AkaDoraQuantity = myInstance->AoDoraQuantity =
			myInstance->AliceDora = myInstance->FlowerQuantity = 0;
		myInstance->AgariPoints = LargeNum::fromInt(0);
		memset(myInstance->yakuNameList, 0, nameBufSize);
		memset(myInstance->yakuValList, 0, nameBufSize);
		memset(myInstance->yakumanNameList, 0, nameBufSize);
		memset(myInstance->yakumanValList, 0, nameBufSize);
	}
	void Init() {Init(this);} // インスタンスを初期化する
};
static_assert(std::is_trivially_copyable<YakuResult>::value, "YakuResult is not trivially copyable");
static_assert(std::is_standard_layout<YakuResult>::value, "YakuResult is not standard layout");

} /* namespace */
