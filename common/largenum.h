#pragma once

#include <type_traits>
#include <cstdint>
#include <climits>
#include <iomanip>
#include "strcode.h"

// �V���[���΍�
// �ꉞ21�s�v�c�܂ŕ\���\�c�c

#ifdef MJCORE_EXPORTS
#include "../mjcore/except.h"
#endif

namespace mihajong_structs {

const unsigned int DigitGroups = 8;

struct LargeNum { // �}21�s�v�c�܂ŕ\���\�Ȑ��̃N���X
	int32_t digitGroup[DigitGroups];

	void fix() { // ���K�`�ɒ���
		for (int i = 0; i < (DigitGroups - 1); i++) {
			if ((this->digitGroup[i] > 99999999)||(this->digitGroup[i] < -99999999)) {
				this->digitGroup[i + 1] += this->digitGroup[i] / 100000000;
				this->digitGroup[i] %= 100000000;
			}
		}
	}
	signed int compare(const LargeNum& cmp) const { // ��r�p
		signed int ans = 0;
		for (int i = DigitGroups - 1; i >= 0; i--) {
			if (this->digitGroup[i] > cmp.digitGroup[i]) {ans = 1; break;}
			else if (this->digitGroup[i] < cmp.digitGroup[i]) {ans = -1; break;}
		}
		return ans;
	}
	CodeConv::tstring bignumtotext(CodeConv::tstring plusSign, CodeConv::tstring minusSign) const {
		// ������\���ɒ���
		static const CodeConv::tstring unitname[] = {
			_T(""), _T("��"), _T("��"),_T("��"), 
			_T("��"), _T("��"), _T("�`"), _T("��"),
			_T("�a"), _T("��"), _T("��"), _T("��"),
			_T("��"), _T("�P�͍�"), _T("���m�_"), _T("�ߗR��"),
		};
		CodeConv::tostringstream o;
		// ����
		if ((LargeNum)*this == fromInt(0)) return _T("0");
		else if ((LargeNum)*this < fromInt(0)) o << minusSign;
		else if ((LargeNum)*this > fromInt(0)) o << plusSign;
		// �o��
		if (this->digitGroup[7] / 100000000)
			o << abs(this->digitGroup[7] / 100000000) << _T("�s�v�c");
		for (int i = 7; i >= 0; i--) {
			if (this->digitGroup[i] % 100000000 / 10000)
				o << abs(this->digitGroup[i] % 100000000 / 10000) << unitname[i * 2 + 1];
			if (this->digitGroup[i] % 10000)
				o << abs(this->digitGroup[i] % 10000) << unitname[i * 2];
		}
		return o.str();
	}
	CodeConv::tstring bignumtoplaintext(CodeConv::tstring plusSign = _T(""), CodeConv::tstring minusSign = _T("-")) const {
		// ������\���ɒ���
		CodeConv::tostringstream o;
		// ����
		if ((LargeNum)*this == fromInt(0)) return _T("0");
		else if ((LargeNum)*this < fromInt(0)) o << minusSign;
		else if ((LargeNum)*this > fromInt(0)) o << plusSign;
		// �o��
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
	double bignumtodbl() const {
		// ���������_�^�ɒ���
		double ans = 0.0;
		for (int i = 7; i >= 0; i--) {
			ans *= 1e8; ans += this->digitGroup[i];
		}
		return ans;
	}
	static LargeNum fromInt(int val) {
		LargeNum num;
		for (int i = 0; i < DigitGroups; i++) num.digitGroup[i] = 0;
		num.digitGroup[0] = (val % 100000000);
		num.digitGroup[1] = (val / 100000000);
		return num;
	}
	void ceilHundred() { // 100�_�P�ʂɐ؂�グ
		if (this->digitGroup[0] % 100 != 0) {
			this->digitGroup[0] += 100 - this->digitGroup[0] % 100;
			this->fix();
		}
	}

	/* �������牉�Z�q���I�[�o�[���[�h */

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

	const LargeNum operator*(const int32_t multiplier) const { // �߂�ǂ������̂�32bit�����{�����c�c
		LargeNum ans = LargeNum();
		for (int i = 0; i < DigitGroups; i++) {
			int64_t tmpdigit = static_cast<int64_t>(digitGroup[i]) * static_cast<int64_t>(multiplier);
			if ((i == (DigitGroups - 1))
				&& ((tmpdigit > INT_MAX) || (tmpdigit < INT_MIN))) {
#ifdef MJCORE_EXPORTS
					Raise(EXCEPTION_MJCORE_OVERFLOW, _T("�I�[�o�[�t���[���܂���"));
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
					Raise(EXCEPTION_MJCORE_OVERFLOW, _T("�I�[�o�[�t���[���܂���"));
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

	const bool operator==(const LargeNum& cmp) const { return (this->compare(cmp) == 0); }
	const bool operator!=(const LargeNum& cmp) const { return (this->compare(cmp) != 0); }
	const bool operator<(const LargeNum& cmp) const { return (this->compare(cmp) < 0); }
	const bool operator>(const LargeNum& cmp) const { return (this->compare(cmp) > 0); }
	const bool operator<=(const LargeNum& cmp) const { return (this->compare(cmp) <= 0); }
	const bool operator>=(const LargeNum& cmp) const { return (this->compare(cmp) >= 0); }
};
static_assert(std::is_pod<LargeNum>::value, "LargeNum is not POD");

} /* namespace */
