#include "largenum.h"

#include <cstdlib>
#include <memory>
#include "except.h"

// 青天ルール対策
// 一応21不可思議まで表現可能……

/* non-POD wrapper */
LNum::LNum() {
	myVal = LargeNum::fromInt(0);
}
LNum::LNum(int32_t val) {
	myVal = LargeNum::fromInt(val);
}
LNum::LNum(const LargeNum& val) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		myVal.digitGroup[i] = val.digitGroup[i];
}
LNum::LNum(const LNum& val) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		myVal.digitGroup[i] = LargeNum(val).digitGroup[i];
}
LNum& LNum::operator=(int32_t val) {
	myVal = LargeNum::fromInt(val);
	return *this;
}
LNum& LNum::operator=(const LargeNum& val) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		myVal.digitGroup[i] = val.digitGroup[i];
	return *this;
}
LNum& LNum::operator=(const LNum& val) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		myVal.digitGroup[i] = LargeNum(val).digitGroup[i];
	return *this;
}
LNum::operator LargeNum() const {return myVal;}
LNum::operator double() const {return myVal.bignumtodbl();}
LNum::operator CodeConv::tstring() const {return myVal.bignumtotext(_T(""), _T("-"));}
CodeConv::tstring LNum::to_str(CodeConv::tstring plusSign, CodeConv::tstring minusSign) const {
	return myVal.bignumtotext(plusSign, minusSign);
}
CodeConv::tstring LNum::to_str_plain() const {
	return myVal.bignumtoplaintext();
}

const LNum LNum::operator+(const LNum& addend) const {return LNum(this->myVal + LargeNum(addend));}
const LNum LNum::operator+(const int32_t addend) const {return LNum(this->myVal + addend);}
LNum& LNum::operator+=(const LNum& addend) {this->myVal += LargeNum(addend); return *this;}
LNum& LNum::operator+=(const int32_t addend) {this->myVal += addend; return *this;}
const LNum LNum::operator-(const LNum& subtrahend) const {return LNum(this->myVal - LargeNum(subtrahend));}
const LNum LNum::operator-(const int32_t subtrahend) const {return LNum(this->myVal - subtrahend);}
LNum& LNum::operator-=(const LNum& subtrahend) {this->myVal -= LargeNum(subtrahend); return *this;}
LNum& LNum::operator-=(const int32_t subtrahend) {this->myVal -= subtrahend; return *this;}
const LNum LNum::operator*(const int32_t multiplier) const {return LNum(this->myVal * multiplier);}
LNum& LNum::operator*=(const int32_t multiplier) {this->myVal *= multiplier; return *this;}
const LNum LNum::operator/(const int32_t divisor) const {return LNum(this->myVal / divisor);}
LNum& LNum::operator/=(const int32_t divisor) {this->myVal /= divisor; return *this;}

const LNum LNum::operator+() const {return LNum(this->myVal);}
const LNum LNum::operator-() const {return LNum(-(this->myVal));}

const bool LNum::operator==(const LNum& cmp) const {return this->myVal == LargeNum(cmp);}
const bool LNum::operator!=(const LNum& cmp) const {return this->myVal != LargeNum(cmp);}
const bool LNum::operator<(const LNum& cmp) const {return this->myVal < LargeNum(cmp);}
const bool LNum::operator>(const LNum& cmp) const {return this->myVal > LargeNum(cmp);}
const bool LNum::operator<=(const LNum& cmp) const {return this->myVal <= LargeNum(cmp);}
const bool LNum::operator>=(const LNum& cmp) const {return this->myVal >= LargeNum(cmp);}
