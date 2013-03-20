#pragma once

#include <cstdint>
#include "mjexport.h"
#include "../common/strcode.h"
#include "../common/largenum.h"

// 青天ルール対策
// 一応21不可思議まで表現可能……

#ifdef MJCORE_EXPORTS

#define DIGIT_GROUPS mihajong_structs::DigitGroups
using mihajong_structs::LargeNum;

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
	CodeConv::tstring to_str_plain() const;
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
