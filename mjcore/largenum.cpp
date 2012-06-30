#include "largenum.h"

// �V���[���΍�
// �ꉞ21�s�v�c�܂ŕ\���\�c�c

#define DIGIT_GROUPS 8

/* private�֐� */

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

/* �R���X�g���N�^ */
LargeNum::LargeNum() { // �}21�s�v�c�܂ŕ\���\�Ȑ��̃N���X
	this->digitGroup.fill(0);
}
LargeNum::LargeNum(int val) { // �}21�s�v�c�܂ŕ\���\�Ȑ��̃N���X
	this->digitGroup.fill(0);
	this->digitGroup[0] = (val % 100000000);
	this->digitGroup[1] = (val / 100000000);
	this->firstArg = 100000000u;
}
LargeNum::LargeNum(int val, unsigned int fArg) { // �}21�s�v�c�܂ŕ\���\�Ȑ��̃N���X
	this->digitGroup.fill(0);
	this->digitGroup[0] = (val % 100000000);
	this->digitGroup[1] = (val / 100000000);
	this->firstArg = fArg;
}

/* public�֐� */
unsigned int LargeNum::getFirstArg() {return this->firstArg;}

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
			throw std::overflow_error("�I�[�o�[�t���[���܂���");
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
const int32_t& LargeNum::operator[] (const int i) const { // const �z��A�N�Z�X�c�c
	return digitGroup[i];
}
int32_t& LargeNum::operator[] (const int i) { // non-const �z��A�N�Z�X�c�c
	return digitGroup[i];
}
