#ifndef YAKURSLT_H
#define YAKURSLT_H

#include "ykclass.h"
#include "../mjexport.h"
#include "../largenum.h"
#include "../gametbl.h"
#include "../except.h"

EXPORT_STRUCT yaku::YAKUSTAT {
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
	char yakuNameList[nameBufSize];
	char yakuValList[nameBufSize];
	char yakumanNameList[nameBufSize];
	char yakumanValList[nameBufSize];
	// -----------------------------------------------------------------
	MELD_BUF MianziDat; // 面子に分解したデータ
	// -----------------------------------------------------------------
	static int getSize(); // 構造体のサイズを返す
	static void Init(YAKUSTAT* const myInstance); // インスタンスを初期化する
	static void exportYakuPoint(const YAKUSTAT* const myInstance, int* const exportArray);
	static void setYakuInfo(YAKUSTAT* const myInstance, int index, int value);
	static void setYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value);
	static void addYakuInfo(YAKUSTAT* const myInstance, int index, int value);
	static void addYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value);
	static int getYakuInfo(const YAKUSTAT* const myInstance, int index);
	static void getYakuInfo(const YAKUSTAT* const myInstance, int index, char* const targetStr, int bufSize);
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<yaku::YAKUSTAT>::value, "YAKUSTAT is not POD");
#endif

#endif
