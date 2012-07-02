#ifndef LARGENUM_H
#define LARGENUM_H

#include <cstdint>
#include <memory>
#include "mjexport.h"
#include "except.h"

// �V���[���΍�
// �ꉞ21�s�v�c�܂ŕ\���\�c�c

#define DIGIT_GROUPS 8

EXPORT_STRUCT LargeNum { // �}21�s�v�c�܂ŕ\���\�Ȑ��̃N���X
	int32_t digitGroup[DIGIT_GROUPS];
	unsigned int firstArg; // �݊��p�B
	void fix();
	signed int compare(const LargeNum& cmp);
	static std::unique_ptr<LargeNum> fromInt(int val);
	static std::unique_ptr<LargeNum> fromInt(int val, unsigned int fArg);
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
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<LargeNum>::value, "LargeNum is not POD");
#endif

#endif
