#ifndef LARGENUM_H
#define LARGENUM_H

#include <array>
#include <cstdint>

// 青天ルール対策
// 一応21不可思議まで表現可能……

#define DIGIT_GROUPS 8

class LargeNum { // ±21不可思議まで表現可能な数のクラス
private:
	std::array<int32_t, DIGIT_GROUPS> digitGroup;
	unsigned int firstArg; // 互換用。
	void fix();
	signed int compare(const LargeNum& cmp);
public:
	LargeNum();
	LargeNum(int val);
	LargeNum(int val, unsigned int fArg);
	unsigned int getFirstArg();
	/* ここから演算子をオーバーロード */
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
