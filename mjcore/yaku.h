#ifndef YAKU_H
#define YAKU_H

#include <cstring>
#include <type_traits>
#include "largenum.h"

namespace yaku {
	EXPORT_STRUCT YAKUSTAT {
		int BasePoints; // 符
		int CoreHan; // 翻
		int BonusHan; // 翻（縛りを満たさない）
		int DoraQuantity; // ドラの数
		int UraDoraQuantity; // 裏ドラの数
		int AkaDoraQuantity; // 赤ドラの数
		int AoDoraQuantity; // 青ドラの数
		int AliceDora; // アリスドラの数
		int FlowerQuantity; // 花牌の数
		LargeNum AgariPoints; // 合計和了点
		char yakuNameList[1024];
		char yakuValList[1024];
		char yakumanNameList[1024];
		char yakumanValList[1024];
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
	static_assert(std::is_pod<YAKUSTAT>::value, "YAKUSTAT is not POD");
#endif
}

#endif
