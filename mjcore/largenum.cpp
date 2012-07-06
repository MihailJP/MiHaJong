#include "largenum.h"

// �V���[���΍�
// �ꉞ21�s�v�c�܂ŕ\���\�c�c

#define DIGIT_GROUPS 8

void LargeNum::fix() { // ���K�`�ɒ���
	for (int i = 0; i < (DIGIT_GROUPS - 1); i++) {
		if ((this->digitGroup[i] > 99999999)||(this->digitGroup[i] < -99999999)) {
			this->digitGroup[i + 1] += this->digitGroup[i] / 100000000;
			this->digitGroup[i] %= 100000000;
		}
	}
}

signed int LargeNum::compare(const LargeNum& cmp) { // ��r�p
	signed int ans = 0;
	for (int i = DIGIT_GROUPS - 1; i >= 0; i--) {
		if (this->digitGroup[i] > cmp.digitGroup[i]) {ans = 1; break;}
		else if (this->digitGroup[i] < cmp.digitGroup[i]) {ans = -1; break;}
	}
	return ans;
}

std::string LargeNum::bignumtotext(std::string plusSign, std::string minusSign) const {
	static const std::string unitname[] = {
		"", "��", "��","��", 
		"��", "��", "�`", "��",
		"�a", "��", "��", "��",
		"��", "�P�͍�", "���m�_", "�ߗR��",
	};
	std::ostringstream o;
	// ����
	if ((LargeNum)*this == fromInt(0, this->firstArg)) return "0";
	else if ((LargeNum)*this < fromInt(0, this->firstArg)) o << minusSign;
	else if ((LargeNum)*this > fromInt(0, this->firstArg)) o << plusSign;
	// �o��
	if (this->digitGroup[7] / 100000000)
		o << abs(this->digitGroup[7] / 100000000) << "�s�v�c";
	for (int i = 7; i >= 0; i--) {
		if (this->digitGroup[i] % 100000000 / 10000)
			o << abs(this->digitGroup[i] % 100000000 / 10000) << unitname[i * 2 + 1];
		if (this->digitGroup[i] % 10000)
			o << abs(this->digitGroup[i] % 10000) << unitname[i * 2];
	}
	return o.str();
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

/* �������牉�Z�q���I�[�o�[���[�h */
const LargeNum LargeNum::operator+(const LargeNum& addend) {
	LargeNum ans;
	for (int i = 0; i < DIGIT_GROUPS; i++)
		ans.digitGroup[i] = digitGroup[i] + addend.digitGroup[i];
	ans.fix();
	return ans;
}
const LargeNum LargeNum::operator+(const int32_t addend) {
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
const LargeNum LargeNum::operator-(const LargeNum& subtrahend) {
	LargeNum ans;
	for (int i = 0; i < DIGIT_GROUPS; i++)
		ans.digitGroup[i] = digitGroup[i] - subtrahend.digitGroup[i];
	ans.fix();
	return ans;
}
const LargeNum LargeNum::operator-(const int32_t subtrahend) {
	LargeNum ans;
	ans.digitGroup[0] = digitGroup[0] - subtrahend;
	for (int i = 1; i < DIGIT_GROUPS; i++)
		ans.digitGroup[i] = digitGroup[i];
	ans.fix();
	return ans;
}
const LargeNum LargeNum::operator*(const int32_t multiplier) { // �߂�ǂ������̂�32bit�����{�����c�c
	LargeNum ans = LargeNum();
	for (int i = 0; i < DIGIT_GROUPS; i++) {
		int64_t tmpdigit = digitGroup[i] * multiplier;
		ans.digitGroup[i] = (int32_t)(tmpdigit % 100000000L);
		if ((i == (DIGIT_GROUPS - 1))
			&& ((tmpdigit > INT_MAX) || (tmpdigit < INT_MIN)))
			Raise(EXCEPTION_MJCORE_OVERFLOW, "�I�[�o�[�t���[���܂���");
		ans.digitGroup[i + 1] = (int32_t)(tmpdigit / 100000000L);
	}
	ans.fix();
	return ans;
}
const bool LargeNum::operator==(const LargeNum& cmp) { return (this->compare(cmp) == 0); }
const bool LargeNum::operator!=(const LargeNum& cmp) { return (this->compare(cmp) != 0); }
const bool LargeNum::operator<(const LargeNum& cmp) { return (this->compare(cmp) < 0); }
const bool LargeNum::operator>(const LargeNum& cmp) { return (this->compare(cmp) > 0); }
const bool LargeNum::operator<=(const LargeNum& cmp) { return (this->compare(cmp) <= 0); }
const bool LargeNum::operator>=(const LargeNum& cmp) { return (this->compare(cmp) >= 0); }
