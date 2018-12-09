﻿#pragma once

#include <type_traits>
#include <cstdint>
#include <climits>
#include <iomanip>
#include "strcode.h"

// 青天ルール対策
// 一応21不可思議まで表現可能……

#ifdef MJCORE_EXPORTS
#include "../mjcore/except.h"
#endif

#ifdef max
#undef max
#endif

namespace mihajong_structs {

const unsigned int DigitGroups = 8;

struct LargeNum { // ±21不可思議まで表現可能な数のクラス
	int32_t digitGroup[DigitGroups];

	LargeNum() {
		for (int i = 0; i < DigitGroups; ++i)
			digitGroup[i] = 0;
	}

	void fix() { // 正規形に直す
		for (int i = 0; i < (DigitGroups - 1); i++) {
			if ((this->digitGroup[i] > 99999999)||(this->digitGroup[i] < -99999999)) {
				this->digitGroup[i + 1] += this->digitGroup[i] / 100000000;
				this->digitGroup[i] %= 100000000;
			}
		}
		int sign = 0;
		for (int i = (DigitGroups - 1); i >= 0; --i) {
			if (sign == 0) {
				if      (digitGroup[i] > 0) sign = +1;
				else if (digitGroup[i] < 0) sign = -1;
			}
			if ((sign == +1) && (digitGroup[i] < 0)) {
				this->digitGroup[i + 1] -= 1;
				this->digitGroup[i] += 100000000;
				i = DigitGroups; sign = 0; continue;
			}
			if ((sign == -1) && (digitGroup[i] > 0)) {
				this->digitGroup[i + 1] += 1;
				this->digitGroup[i] -= 100000000;
				i = DigitGroups; sign = 0; continue;
			}
		}
	}
	signed int compare(const LargeNum& cmp) const { // 比較用
		signed int ans = 0;
		for (int i = DigitGroups - 1; i >= 0; i--) {
			if (this->digitGroup[i] > cmp.digitGroup[i]) {ans = 1; break;}
			else if (this->digitGroup[i] < cmp.digitGroup[i]) {ans = -1; break;}
		}
		return ans;
	}
	CodeConv::tstring bignumtotext(CodeConv::tstring plusSign, CodeConv::tstring minusSign) const {
		// 文字列表現に直す
		static const CodeConv::tstring unitname[] = {
			_T(""), _T("万"), _T("億"),_T("兆"), 
			_T("京"), _T("垓"), _T("杼"), _T("穣"),
			_T("溝"), _T("澗"), _T("正"), _T("載"),
			_T("極"), _T("恒河沙"), _T("阿僧祇"), _T("那由他"),
		};
		CodeConv::tostringstream o;
		// 符号
		if ((LargeNum)*this == 0) return _T("0");
		else if ((LargeNum)*this < 0) o << minusSign;
		else if ((LargeNum)*this > 0) o << plusSign;
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
	CodeConv::tstring bignumtoplaintext(CodeConv::tstring plusSign = _T(""), CodeConv::tstring minusSign = _T("-")) const {
		// 文字列表現に直す
		CodeConv::tostringstream o;
		// 符号
		if ((LargeNum)*this == 0) return _T("0");
		else if ((LargeNum)*this < 0) o << minusSign;
		else if ((LargeNum)*this > 0) o << plusSign;
		// 出力
		bool valFlag = false;
		for (int i = DigitGroups - 1; i >= 0; i--) {
			if (valFlag) {
				o << std::setw(8) << std::setfill(_T('0')) << abs(this->digitGroup[i]);
			} else if (this->digitGroup[i]) {
				o << abs(this->digitGroup[i]); valFlag = true;
			}
		}
		return o.str();
	}
	explicit operator double() const {
		// 浮動小数点型に直す
		double ans = 0.0;
		for (int i = 7; i >= 0; i--) {
			ans *= 1e8; ans += this->digitGroup[i];
		}
		return ans;
	}

	explicit operator int32_t() const {
		if ((digitGroup[7] == 0) && (digitGroup[6] == 0) && (digitGroup[5] == 0) && (digitGroup[4] == 0)
			&& (digitGroup[3] == 0) && (digitGroup[2] == 0)
			&& ((abs(digitGroup[1]) < 21) || ((abs(digitGroup[1]) == 21)
				&& (abs(digitGroup[0]) <= 47'483'647))))
		{
			return digitGroup[1] * 100'000'000 + digitGroup[0];
		} else { // OVERFLOW
			return (*this >= 0 ? 1 : -1) * std::numeric_limits<int32_t>::max();
		}
	}
	static_assert(std::numeric_limits<int32_t>::max() == 2'147'483'647, "Maximum of int32_t is not 2,147,483,647");
	explicit operator uint32_t() const {
		if (*this < 0) {
			// NEGATIVE
			return 0u;
		} else if ((digitGroup[7] == 0) && (digitGroup[6] == 0) && (digitGroup[5] == 0) && (digitGroup[4] == 0)
			&& (digitGroup[3] == 0) && (digitGroup[2] == 0)
			&& ((digitGroup[1] < 42) || ((digitGroup[1] == 42)
				&& (digitGroup[0] <= 94'967'295))))
		{
			return static_cast<uint32_t>(digitGroup[1]) * 100'000'000u + static_cast<uint32_t>(digitGroup[0]);
		} else { // OVERFLOW
			return std::numeric_limits<uint32_t>::max();
		}
	}
	static_assert(std::numeric_limits<uint32_t>::max() == 4'294'967'295, "Maximum of uint32_t is not 4,294,967,295");

	explicit operator int64_t() const {
		if ((digitGroup[7] == 0) && (digitGroup[6] == 0) && (digitGroup[5] == 0) && (digitGroup[4] == 0) && (digitGroup[3] == 0)
			&& ((abs(digitGroup[2]) < 922) || ((abs(digitGroup[2]) == 922)
				&& (abs(digitGroup[1]) <= 33'720'368)) || ((abs(digitGroup[1]) == 33'720'368)
					&& (abs(digitGroup[0]) <= 54'775'807))))
		{
			return static_cast<int64_t>(digitGroup[2]) * 1'000'000'000'000'000LL + digitGroup[1] * 100'000'000 + digitGroup[0];
		} else { // OVERFLOW
			return (*this >= 0 ? 1 : -1) * std::numeric_limits<int64_t>::max();
		}
	}
	static_assert(std::numeric_limits<int64_t>::max() == 9'223'372'036'854'775'807LL, "Maximum of int64_t is not 9,223,372,036,854,775,807");
	explicit operator uint64_t() const {
		if (*this < 0) {
			// NEGATIVE
			return 0uLL;
		} else if ((digitGroup[7] == 0) && (digitGroup[6] == 0) && (digitGroup[5] == 0) && (digitGroup[4] == 0) && (digitGroup[3] == 0)
			&& ((abs(digitGroup[2]) < 922) || ((abs(digitGroup[2]) == 922)
				&& (abs(digitGroup[1]) <= 33'720'368)) || ((abs(digitGroup[1]) == 33'720'368)
					&& (abs(digitGroup[0]) <= 54'775'807))))
		{
			return static_cast<uint64_t>(digitGroup[2]) * 1'000'000'000'000'000uLL
				+ static_cast<uint64_t>(digitGroup[1]) * 100'000'000uLL + static_cast<uint64_t>(digitGroup[0]);
		} else { // OVERFLOW
			return std::numeric_limits<uint64_t>::max();
		}
	}
	static_assert(std::numeric_limits<uint64_t>::max() == 18'446'744'073'709'551'615uLL, "Maximum of uint64_t is not 18,446,744,073,709,551,615");

	LargeNum(int32_t val) : LargeNum() {
		digitGroup[0] = (val % 100'000'000);
		digitGroup[1] = (val / 100'000'000);
	}
	LargeNum(uint32_t val) : LargeNum() {
		digitGroup[0] = (val % 100'000'000);
		digitGroup[1] = (val / 100'000'000);
	}
	LargeNum(int64_t val) : LargeNum() {
		digitGroup[0] = static_cast<int32_t>(val % 100'000'000uLL);
		digitGroup[1] = static_cast<int32_t>(val % 1'000'000'000'000'000uLL / 100'000'000uLL);
		digitGroup[2] = static_cast<int32_t>(val / 1'000'000'000'000'000uLL);
	}
	LargeNum(uint64_t val) : LargeNum() {
		digitGroup[0] = static_cast<int32_t>(val % 100'000'000uLL);
		digitGroup[1] = static_cast<int32_t>(val % 1'000'000'000'000'000uLL / 100'000'000uLL);
		digitGroup[2] = static_cast<int32_t>(val / 1'000'000'000'000'000uLL);
	}
	void ceilHundred() { // 100点単位に切り上げ
		if (this->digitGroup[0] % 100 != 0) {
			this->digitGroup[0] += 100 - this->digitGroup[0] % 100;
			this->fix();
		}
	}

	/* ここから演算子をオーバーロード */

	const LargeNum operator+(const LargeNum& addend) const {
		LargeNum ans;
		for (int i = 0; i < DigitGroups; i++)
			ans.digitGroup[i] = digitGroup[i] + addend.digitGroup[i];
		ans.fix();
		return ans;
	}
	const LargeNum operator+(const int32_t addend) const {
		LargeNum ans;
		ans.digitGroup[0] = digitGroup[0] + addend;
		for (int i = 1; i < DigitGroups; i++)
			ans.digitGroup[i] = digitGroup[i];
		ans.fix();
		return ans;
	}
	LargeNum& operator+=(const LargeNum& addend) {
		for (int i = 0; i < DigitGroups; i++)
			digitGroup[i] += addend.digitGroup[i];
		fix();
		return *this;
	}
	LargeNum& operator+=(const int32_t addend) {
		digitGroup[0] += addend;
		fix();
		return *this;
	}

	const LargeNum operator-(const LargeNum& subtrahend) const {
		LargeNum ans;
		for (int i = 0; i < DigitGroups; i++)
			ans.digitGroup[i] = digitGroup[i] - subtrahend.digitGroup[i];
		ans.fix();
		return ans;
	}
	const LargeNum operator-(const int32_t subtrahend) const {
		LargeNum ans;
		ans.digitGroup[0] = digitGroup[0] - subtrahend;
		for (int i = 1; i < DigitGroups; i++)
			ans.digitGroup[i] = digitGroup[i];
		ans.fix();
		return ans;
	}
	LargeNum& operator-=(const LargeNum& subtrahend) {
		for (int i = 0; i < DigitGroups; i++)
			digitGroup[i] -= subtrahend.digitGroup[i];
		fix();
		return *this;
	}
	LargeNum& operator-=(const int32_t subtrahend) {
		digitGroup[0] -= subtrahend;
		fix();
		return *this;
	}

	const LargeNum operator*(const int32_t multiplier) const { // めんどくさいので32bit整数倍だけ……
		LargeNum ans = LargeNum();
		for (int i = 0; i < DigitGroups; i++) {
			int64_t tmpdigit = static_cast<int64_t>(digitGroup[i]) * static_cast<int64_t>(multiplier);
			if ((i == (DigitGroups - 1))
				&& ((tmpdigit > INT_MAX) || (tmpdigit < INT_MIN))) {
#ifdef MJCORE_EXPORTS
					Raise(EXCEPTION_MJCORE_OVERFLOW, _T("オーバーフローしました"));
#else
					abort(); // ABEND by overflow
#endif
			}
			else if (i == (DigitGroups - 1)) ans.digitGroup[i] += (int32_t)tmpdigit;
			else {
				ans.digitGroup[i] += (int32_t)(tmpdigit % 100000000L);
				ans.digitGroup[i + 1] += (int32_t)(tmpdigit / 100000000L);
			}
		}
		ans.fix();
		return ans;
	}
	LargeNum& operator*=(const int32_t multiplier) {
		LargeNum ans = LargeNum();
		for (int i = 0; i < DigitGroups; i++) {
			int64_t tmpdigit = static_cast<int64_t>(digitGroup[i]) * static_cast<int64_t>(multiplier);
			if ((i == (DigitGroups - 1))
				&& ((tmpdigit > INT_MAX) || (tmpdigit < INT_MIN))) {
#ifdef MJCORE_EXPORTS
					Raise(EXCEPTION_MJCORE_OVERFLOW, _T("オーバーフローしました"));
#else
					abort(); // ABEND by overflow
#endif
			}
			else if (i == (DigitGroups - 1)) ans.digitGroup[i] += (int32_t)tmpdigit;
			else {
				ans.digitGroup[i] += (int32_t)(tmpdigit % 100000000L);
				ans.digitGroup[i + 1] += (int32_t)(tmpdigit / 100000000L);
			}
		}
		for (int i = 0; i < DigitGroups; i++) digitGroup[i] = ans.digitGroup[i];
		fix();
		return *this;
	}
	const LargeNum operator/(const int32_t divisor) const {
		LargeNum ans = LargeNum();
		int64_t tmpdigit[DigitGroups];
		for (int i = 0; i < DigitGroups; i++) tmpdigit[i] = digitGroup[i];
		for (int i = DigitGroups - 1; i >= 0; i--) {
			if (i > 0) tmpdigit[i - 1] += (tmpdigit[i] % (int64_t)divisor) * 100000000L;
			tmpdigit[i] /= (int64_t)divisor;
		}
		for (int i = 0; i < DigitGroups; i++) ans.digitGroup[i] = (int32_t)(tmpdigit[i] & 0xffffffffull);
		ans.fix();
		return ans;
	}
	LargeNum& operator/=(const int32_t divisor) {
		LargeNum ans = LargeNum();
		int64_t tmpdigit[DigitGroups];
		for (int i = 0; i < DigitGroups; i++) tmpdigit[i] = digitGroup[i];
		for (int i = DigitGroups - 1; i >= 0; i--) {
			if (i > 0) tmpdigit[i - 1] += (tmpdigit[i] % (int64_t)divisor) * 100000000L;
			tmpdigit[i] /= (int64_t)divisor;
		}
		for (int i = 0; i < DigitGroups; i++) digitGroup[i] = (int32_t)(tmpdigit[i] & 0xffffffffull);
		fix();
		return *this;
	}

	const LargeNum operator+() const { return *this; }
	const LargeNum operator-() const { return (*this) * (-1); }

	const bool operator==(const LargeNum& cmp) const { return (this->compare(cmp) == 0); }
	const bool operator!=(const LargeNum& cmp) const { return (this->compare(cmp) != 0); }
	const bool operator<(const LargeNum& cmp) const { return (this->compare(cmp) < 0); }
	const bool operator>(const LargeNum& cmp) const { return (this->compare(cmp) > 0); }
	const bool operator<=(const LargeNum& cmp) const { return (this->compare(cmp) <= 0); }
	const bool operator>=(const LargeNum& cmp) const { return (this->compare(cmp) >= 0); }
};
static_assert(std::is_trivially_copyable<LargeNum>::value, "LargeNum is not trivially copyable");
static_assert(std::is_standard_layout<LargeNum>::value, "LargeNum is not standard layout");

} /* namespace */
