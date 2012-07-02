#ifndef LARGENUM_H
#define LARGENUM_H

#include <cstdint>
#include <memory>
#include "mjexport.h"
#include "except.h"

// 青天ルール対策
// 一応21不可思議まで表現可能……

#define DIGIT_GROUPS 8

EXPORT_STRUCT LargeNum { // ±21不可思議まで表現可能な数のクラス
	int32_t digitGroup[DIGIT_GROUPS];
	unsigned int firstArg; // 互換用。
	void fix();
	signed int compare(const LargeNum& cmp);
	static std::unique_ptr<LargeNum> fromInt(int val);
	static std::unique_ptr<LargeNum> fromInt(int val, unsigned int fArg);
	unsigned int getFirstArg();
	/* ここから演算子をオーバーロード */
	const LargeNum operator+(const LargeNum& addend);
	const LargeNum operator+(const int32_t addend);
	LargeNum& operator+=(const LargeNum& addend);
	LargeNum& operator+=(const int32_t addend);
	const LargeNum operator-(const LargeNum& subtrahend);
	const LargeNum operator-(const int32_t subtrahend);
	const LargeNum operator*(const int32_t multiplier);
	const bool operator==(const LargeNum& cmp);
	const bool operator!=(const LargeNum& cmp);
	const bool operator<(const LargeNum& cmp);
	const bool operator>(const LargeNum& cmp);
	const bool operator<=(const LargeNum& cmp);
	const bool operator>=(const LargeNum& cmp);
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<LargeNum>::value, "LargeNum is not POD");
#endif

#endif
