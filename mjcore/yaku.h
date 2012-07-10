#ifndef YAKU_H
#define YAKU_H

#include <cstring>
#include <type_traits>
#include "largenum.h"

namespace yaku {
	EXPORT_STRUCT YAKUSTAT {
		int BasePoints; // ��
		int CoreHan; // �|
		int BonusHan; // �|�i����𖞂����Ȃ��j
		int DoraQuantity; // �h���̐�
		int UraDoraQuantity; // ���h���̐�
		int AkaDoraQuantity; // �ԃh���̐�
		int AoDoraQuantity; // �h���̐�
		int AliceDora; // �A���X�h���̐�
		int FlowerQuantity; // �Ԕv�̐�
		LargeNum AgariPoints; // ���v�a���_
		char yakuNameList[1024];
		char yakuValList[1024];
		char yakumanNameList[1024];
		char yakumanValList[1024];
		static int getSize(); // �\���̂̃T�C�Y��Ԃ�
		static void Init(YAKUSTAT* const myInstance); // �C���X�^���X������������
		static void exportYakuPoint(const YAKUSTAT* const myInstance, int* const exportArray);
		static void setYakuInfo(YAKUSTAT* const myInstance, int index, int value);
		static void setYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value);
		static void addYakuInfo(YAKUSTAT* const myInstance, int index, int value);
		static void addYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value);
		static int getYakuInfo(const YAKUSTAT* const myInstance, int index);
		static void getYakuInfo(const YAKUSTAT* const myInstance, int index, char* const targetStr, int bufSize);
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<YAKUSTAT>::value, "YAKUSTAT is not POD");
#endif
}

#endif
