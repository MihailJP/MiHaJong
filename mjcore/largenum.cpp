#include "largenum.h"

#include <cstdlib>
#include <memory>
#include "except.h"

// 青天ルール対策
// 一応21不可思議まで表現可能……

#define DIGIT_GROUPS 8

void LargeNum::fix() { // 正規形に直す
	for (int i = 0; i < (DIGIT_GROUPS - 1); i++) {
		if ((this->digitGroup[i] > 99999999)||(this->digitGroup[i] < -99999999)) {
			this->digitGroup[i + 1] += this->digitGroup[i] / 100000000;
			this->digitGroup[i] %= 100000000;
		}
	}
}

signed int LargeNum::compare(const LargeNum& cmp) const { // 比較用
	signed int ans = 0;
	for (int i = DIGIT_GROUPS - 1; i >= 0; i--) {
		if (this->digitGroup[i] > cmp.digitGroup[i]) {ans = 1; break;}
		else if (this->digitGroup[i] < cmp.digitGroup[i]) {ans = -1; break;}
	}
	return ans;
}

CodeConv::tstring LargeNum::bignumtotext(CodeConv::tstring plusSign, CodeConv::tstring minusSign) const {
	// 文字列表現に直す
	static const CodeConv::tstring unitname[] = {
		_T(""), _T("万"), _T("億"),_T("兆"), 
		_T("京"), _T("垓"), _T("杼"), _T("穣"),
		_T("溝"), _T("澗"), _T("正"), _T("載"),
		_T("極"), _T("恒河沙"), _T("阿僧祇"), _T("那由他"),
	};
	CodeConv::tostringstream o;
	// 符号
	if ((LargeNum)*this == fromInt(0, this->firstArg)) return _T("0");
	else if ((LargeNum)*this < fromInt(0, this->firstArg)) o << minusSign;
	else if ((LargeNum)*this > fromInt(0, this->firstArg)) o << plusSign;
	// 出力
	if (this->digitGroup[7] / 100000000)
		o << abs(this->digitGroup[7] / 100000000) << _T("不可思議");
	for (int i = 7; i >= 0; i--) {
		if (this->digitGroup[i] % 100000000 / 10000)
			o << abs(this->digitGroup[i] % 100000000 / 10000) << unitname[i * 2 + 1];
		if (this->digitGroup[i] % 10000)
			o << abs(this->digitGroup[i] % 10000) << unitname[i * 2];
	}
	return o.str();
}

double LargeNum::bignumtodbl() const {
	// 浮動小数点型に直す
	double ans = 0.0;
	for (int i = 7; i >= 0; i--) {
		ans *= 1e8; ans += this->digitGroup[i];
	}
	return ans;
}

LargeNum LargeNum::fromInt(int val) {
	LargeNum num;
	for (int i = 0; i < DIGIT_GROUPS; i++) num.digitGroup[i] = 0;
	num.digitGroup[0] = (val % 100000000);
	num.digitGroup[1] = (val / 100000000);
	num.firstArg = 100000000u;
	return num;
}
LargeNum LargeNum::fromInt(int val, unsigned int fArg) {
	LargeNum num;
	for (int i = 0; i < DIGIT_GROUPS; i++) num.digitGroup[i] = 0;
	num.digitGroup[0] = (val % 100000000);
	num.digitGroup[1] = (val / 100000000);
	num.firstArg = fArg;
	return num;
}
void LargeNum::ceilHundred() { // 100点単位に切り上げ
	if (this->digitGroup[0] % 100 != 0) {
		this->digitGroup[0] += 100 - this->digitGroup[0] % 100;
		this->fix();
	}
}

/* ここから演算子をオーバーロード */
const LargeNum LargeNum::operator+(const LargeNum& addend) const {
	LargeNum ans;
	for (int i = 0; i < DIGIT_GROUPS; i++)
		ans.digitGroup[i] = digitGroup[i] + addend.digitGroup[i];
	ans.fix();
	return ans;
}
const LargeNum LargeNum::operator+(const int32_t addend) const {
	LargeNum ans;
	ans.digitGroup[0] = digitGroup[0] + addend;
	for (int i = 1; i < DIGIT_GROUPS; i++)
		ans.digitGroup[i] = digitGroup[i];
	ans.fix();
	return ans;
}
LargeNum& LargeNum::operator+=(const LargeNum& addend) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		digitGroup[i] += addend.digitGroup[i];
	fix();
	return *this;
}
LargeNum& LargeNum::operator+=(const int32_t addend) {
	digitGroup[0] += addend;
	fix();
	return *this;
}
const LargeNum LargeNum::operator-(const LargeNum& subtrahend) const {
	LargeNum ans;
	for (int i = 0; i < DIGIT_GROUPS; i++)
		ans.digitGroup[i] = digitGroup[i] - subtrahend.digitGroup[i];
	ans.fix();
	return ans;
}
const LargeNum LargeNum::operator-(const int32_t subtrahend) const {
	LargeNum ans;
	ans.digitGroup[0] = digitGroup[0] - subtrahend;
	for (int i = 1; i < DIGIT_GROUPS; i++)
		ans.digitGroup[i] = digitGroup[i];
	ans.fix();
	return ans;
}
LargeNum& LargeNum::operator-=(const LargeNum& subtrahend) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		digitGroup[i] += subtrahend.digitGroup[i];
	fix();
	return *this;
}
LargeNum& LargeNum::operator-=(const int32_t subtrahend) {
	digitGroup[0] += subtrahend;
	fix();
	return *this;
}
const LargeNum LargeNum::operator*(const int32_t multiplier) const { // めんどくさいので32bit整数倍だけ……
	LargeNum ans = LargeNum();
	for (int i = 0; i < DIGIT_GROUPS; i++) {
		int64_t tmpdigit = digitGroup[i] * multiplier;
		if ((i == (DIGIT_GROUPS - 1))
			&& ((tmpdigit > INT_MAX) || (tmpdigit < INT_MIN))) {
				Raise(EXCEPTION_MJCORE_OVERFLOW, _T("オーバーフローしました"));
		}
		else if (i == (DIGIT_GROUPS - 1)) ans.digitGroup[i] += (int32_t)tmpdigit;
		else {
			ans.digitGroup[i] += (int32_t)(tmpdigit % 100000000L);
			ans.digitGroup[i + 1] += (int32_t)(tmpdigit / 100000000L);
		}
	}
	ans.fix();
	return ans;
}
LargeNum& LargeNum::operator*=(const int32_t multiplier) {
	LargeNum ans = LargeNum();
	for (int i = 0; i < DIGIT_GROUPS; i++) {
		int64_t tmpdigit = digitGroup[i] * multiplier;
		if ((i == (DIGIT_GROUPS - 1))
			&& ((tmpdigit > INT_MAX) || (tmpdigit < INT_MIN))) {
				Raise(EXCEPTION_MJCORE_OVERFLOW, _T("オーバーフローしました"));
		}
		else if (i == (DIGIT_GROUPS - 1)) ans.digitGroup[i] += (int32_t)tmpdigit;
		else {
			ans.digitGroup[i] += (int32_t)(tmpdigit % 100000000L);
			ans.digitGroup[i + 1] += (int32_t)(tmpdigit / 100000000L);
		}
	}
	for (int i = 0; i < DIGIT_GROUPS; i++) digitGroup[i] = ans.digitGroup[i];
	fix();
	return *this;
}
const LargeNum LargeNum::operator/(const int32_t divisor) const {
	LargeNum ans = LargeNum();
	int64_t tmpdigit[DIGIT_GROUPS];
	for (int i = 0; i < DIGIT_GROUPS; i++) tmpdigit[i] = digitGroup[i];
	for (int i = DIGIT_GROUPS - 1; i >= 0; i--) {
		if (i > 0) tmpdigit[i - 1] += (tmpdigit[i] % (int64_t)divisor) * 100000000L;
		tmpdigit[i] /= (int64_t)divisor;
	}
	for (int i = 0; i < DIGIT_GROUPS; i++) ans.digitGroup[i] = tmpdigit[i];
	ans.fix();
	return ans;
}
LargeNum& LargeNum::operator/=(const int32_t divisor) {
	LargeNum ans = LargeNum();
	int64_t tmpdigit[DIGIT_GROUPS];
	for (int i = 0; i < DIGIT_GROUPS; i++) tmpdigit[i] = digitGroup[i];
	for (int i = DIGIT_GROUPS - 1; i >= 0; i--) {
		if (i > 0) tmpdigit[i - 1] += (tmpdigit[i] % (int64_t)divisor) * 100000000L;
		tmpdigit[i] /= (int64_t)divisor;
	}
	for (int i = 0; i < DIGIT_GROUPS; i++) digitGroup[i] = tmpdigit[i];
	fix();
	return *this;
}
const bool LargeNum::operator==(const LargeNum& cmp) const { return (this->compare(cmp) == 0); }
const bool LargeNum::operator!=(const LargeNum& cmp) const { return (this->compare(cmp) != 0); }
const bool LargeNum::operator<(const LargeNum& cmp) const { return (this->compare(cmp) < 0); }
const bool LargeNum::operator>(const LargeNum& cmp) const { return (this->compare(cmp) > 0); }
const bool LargeNum::operator<=(const LargeNum& cmp) const { return (this->compare(cmp) <= 0); }
const bool LargeNum::operator>=(const LargeNum& cmp) const { return (this->compare(cmp) >= 0); }

/* non-POD wrapper */
LNum::LNum() {
	myVal = LargeNum::fromInt(0);
}
LNum::LNum(int32_t val) {
	myVal = LargeNum::fromInt(val);
}
LNum::LNum(int32_t val, uint32_t fArg) {
	myVal = LargeNum::fromInt(val, fArg);
}
LNum::LNum(const LargeNum& val) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		myVal.digitGroup[i] = val.digitGroup[i];
	myVal.firstArg = val.firstArg;
}
LNum::LNum(const LNum& val) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		myVal.digitGroup[i] = LargeNum(val).digitGroup[i];
	myVal.firstArg = LargeNum(val).firstArg;
}
LNum& LNum::operator=(int32_t val) {
	myVal = LargeNum::fromInt(val);
	return *this;
}
LNum& LNum::operator=(const LargeNum& val) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		myVal.digitGroup[i] = val.digitGroup[i];
	myVal.firstArg = val.firstArg;
	return *this;
}
LNum& LNum::operator=(const LNum& val) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		myVal.digitGroup[i] = LargeNum(val).digitGroup[i];
	myVal.firstArg = LargeNum(val).firstArg;
	return *this;
}
LNum::operator LargeNum() const {return myVal;}
LNum::operator double() const {return myVal.bignumtodbl();}
LNum::operator CodeConv::tstring() const {return myVal.bignumtotext(_T(""), _T("-"));}
CodeConv::tstring LNum::to_str(CodeConv::tstring plusSign, CodeConv::tstring minusSign) const {
	return myVal.bignumtotext(plusSign, minusSign);
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

const bool LNum::operator==(const LNum& cmp) const {return this->myVal == LargeNum(cmp);}
const bool LNum::operator!=(const LNum& cmp) const {return this->myVal != LargeNum(cmp);}
const bool LNum::operator<(const LNum& cmp) const {return this->myVal < LargeNum(cmp);}
const bool LNum::operator>(const LNum& cmp) const {return this->myVal > LargeNum(cmp);}
const bool LNum::operator<=(const LNum& cmp) const {return this->myVal <= LargeNum(cmp);}
const bool LNum::operator>=(const LNum& cmp) const {return this->myVal >= LargeNum(cmp);}
