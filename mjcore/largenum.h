#ifndef LARGENUM_H
#define LARGENUM_H

#include <array>
#include <cstdint>

// �V���[���΍�
// �ꉞ21�s�v�c�܂ŕ\���\�c�c

#define DIGIT_GROUPS 8

class LargeNum { // �}21�s�v�c�܂ŕ\���\�Ȑ��̃N���X
private:
	std::array<int32_t, DIGIT_GROUPS> digitGroup;
	unsigned int firstArg; // �݊��p�B
	void fix();
	signed int compare(const LargeNum& cmp);
public:
	LargeNum();
	LargeNum(int val);
	LargeNum(int val, unsigned int fArg);
	unsigned int getFirstArg();
	/* �������牉�Z�q���I�[�o�[���[�h */
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
	const int32_t& operator[] (const int i) const;
	int32_t& operator[] (const int i);
};

#endif
