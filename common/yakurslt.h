#pragma once

#include <type_traits>
#ifdef _WIN32
#include <tchar.h>
#endif /*_WIN32*/
#include "largenum.h"
#include "gametbl.h"
#include <vector>
#include "strcode.h"

namespace mihajong_structs {
	typedef std::vector<std::pair<CodeConv::tstring, CodeConv::tstring> > YakuListType;

struct YakuResult {
	static const int SemiMangan = 12500; // 半満貫
	static const int LimitMinus = 100; // マイナス翻の下限
	static const unsigned int nameBufSize = 1024; // 名前バッファのサイズ
	bool isValid; // 和了っているかどうか
	bool isYakuman; // 役満であるか（倍満設定の人和なども含む）
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
	int TotalHan() {return CoreHan + BonusHan;} // ドラ込みの翻
	int TotalSemiMangan() {return CoreSemiMangan + BonusSemiMangan;}
	LPCTSTR getYakuNameList() const {return isYakuman ? yakumanNameList : yakuNameList;}
	LPCTSTR getYakuValList() const {return isYakuman ? yakumanValList : yakuValList;}
	const auto yakuList(YakuListType* result = nullptr, bool alwaysWithYakuVal = false) const { // オブジェクト形式の役リスト
		thread_local YakuListType tmpResult;
		YakuListType *myResult = (result ? result : &tmpResult);
		std::vector<CodeConv::tstring> yakuNames(CodeConv::split(getYakuNameList(), _T('\n')));
		std::vector<CodeConv::tstring> yakuValues(CodeConv::split(getYakuValList(), _T('\n')));
		for (std::size_t i = 0; i < yakuNames.size(); ++i)
			myResult->push_back(std::make_pair(
				yakuNames.at(i),
				(!(isYakuman) || alwaysWithYakuVal) ? yakuValues.at(i) : CodeConv::tstring()
			));
		return myResult;
	}
	// -----------------------------------------------------------------
	YakuResult() { // インスタンスを初期化する
		isValid = isYakuman = false;
		BasePoints = 20;
		CoreHan = CoreSemiMangan =
			BonusHan = BonusSemiMangan =
			DoraQuantity = UraDoraQuantity =
			AkaDoraQuantity = AoDoraQuantity =
			AliceDora = FlowerQuantity = 0;
		AgariPoints = LargeNum::fromInt(0);
		memset(yakuNameList, 0, nameBufSize);
		memset(yakuValList, 0, nameBufSize);
		memset(yakumanNameList, 0, nameBufSize);
		memset(yakumanValList, 0, nameBufSize);
	}
	~YakuResult() = default;
};
static_assert(std::is_trivially_copyable<YakuResult>::value, "YakuResult is not trivially copyable");
static_assert(std::is_standard_layout<YakuResult>::value, "YakuResult is not standard layout");

static_assert(std::is_move_assignable<std::vector<std::pair<CodeConv::tstring, CodeConv::tstring>>>::value, "yakuList is not move assignable");
} /* namespace */
