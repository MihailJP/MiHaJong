#pragma once

#include <type_traits>
#include <cstdint>
#include "mjexport.h"
#include "strcode.h"

// 青天ルール対策
// 一応21不可思議まで表現可能……

#define DIGIT_GROUPS 8

EXPORT_STRUCT LargeNum { // ±21不可思議まで表現可能な数のクラス
	int32_t digitGroup[DIGIT_GROUPS];
	void fix();
	signed int compare(const LargeNum& cmp) const;
	CodeConv::tstring bignumtotext(CodeConv::tstring plusSign, CodeConv::tstring minusSign) const;
	double bignumtodbl() const;
	static LargeNum fromInt(int val);
	void ceilHundred();
	/* ここから演算子をオーバーロード */
	const LargeNum operator+(const LargeNum& addend) const;
	const LargeNum operator+(const int32_t addend) const;
	LargeNum& operator+=(const LargeNum& addend);
	LargeNum& operator+=(const int32_t addend);
	const LargeNum operator-(const LargeNum& subtrahend) const;
	const LargeNum operator-(const int32_t subtrahend) const;
	LargeNum& operator-=(const LargeNum& subtrahend);
	LargeNum& operator-=(const int32_t subtrahend);
	const LargeNum operator*(const int32_t multiplier) const;
	LargeNum& operator*=(const int32_t multiplier);
	const LargeNum operator/(const int32_t divisor) const;
	LargeNum& operator/=(const int32_t divisor);
	const bool operator==(const LargeNum& cmp) const;
	const bool operator!=(const LargeNum& cmp) const;
	const bool operator<(const LargeNum& cmp) const;
	const bool operator>(const LargeNum& cmp) const;
	const bool operator<=(const LargeNum& cmp) const;
	const bool operator>=(const LargeNum& cmp) const;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<LargeNum>::value, "LargeNum is not POD");

/* non-POD wrapper, for convenience */
class LNum {
private:
	LargeNum myVal;
public:
	/* Constructors */
	LNum();
	LNum(int32_t val);
	LNum(const LargeNum& val);
	LNum(const LNum& val);
	LNum& operator=(int32_t val);
	LNum& operator=(const LargeNum& val);
	LNum& operator=(const LNum& val);
	/* Casts */
	operator LargeNum() const;
	operator double() const;
	operator CodeConv::tstring() const;
	/* Other methods */
	CodeConv::tstring to_str(CodeConv::tstring plusSign, CodeConv::tstring minusSign) const;
	/* Arithmetic operators */
	const LNum operator+(const LNum& addend) const;
	const LNum operator+(const int32_t addend) const;
	LNum& operator+=(const LNum& addend);
	LNum& operator+=(const int32_t addend);
	const LNum operator-(const LNum& subtrahend) const;
	const LNum operator-(const int32_t subtrahend) const;
	LNum& operator-=(const LNum& subtrahend);
	LNum& operator-=(const int32_t subtrahend);
	const LNum operator*(const int32_t multiplier) const;
	LNum& operator*=(const int32_t multiplier);
	const LNum operator/(const int32_t divisor) const;
	LNum& operator/=(const int32_t divisor);
	/* Comparison operators */
	const bool operator==(const LNum& cmp) const;
	const bool operator!=(const LNum& cmp) const;
	const bool operator<(const LNum& cmp) const;
	const bool operator>(const LNum& cmp) const;
	const bool operator<=(const LNum& cmp) const;
	const bool operator>=(const LNum& cmp) const;
};
#endif
