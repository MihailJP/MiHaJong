#ifndef YAKURSLT_H
#define YAKURSLT_H

#include "ykclass.h"
#include "../mjexport.h"
#include "../largenum.h"
#include "../gametbl.h"
#include "../except.h"

EXPORT_STRUCT yaku::YAKUSTAT {
	static const int SemiMangan = 12500; // ������
	static const int LimitMinus = 100; // �}�C�i�X�|�̉���
	static const unsigned int nameBufSize = 1024; // ���O�o�b�t�@�̃T�C�Y
	bool isValid; // �a�����Ă��邩�ǂ���
	int BasePoints; // ��
	int CoreHan, CoreSemiMangan; // �|
	int BonusHan, BonusSemiMangan; // �|�i����𖞂����Ȃ��j
	int DoraQuantity; // �h���̐�
	int UraDoraQuantity; // ���h���̐�
	int AkaDoraQuantity; // �ԃh���̐�
	int AoDoraQuantity; // �h���̐�
	int AliceDora; // �A���X�h���̐�
	int FlowerQuantity; // �Ԕv�̐�
	LargeNum AgariPoints; // ���v�a���_
	char yakuNameList[nameBufSize];
	char yakuValList[nameBufSize];
	char yakumanNameList[nameBufSize];
	char yakumanValList[nameBufSize];
	// -----------------------------------------------------------------
	MELD_BUF MianziDat; // �ʎq�ɕ��������f�[�^
	// -----------------------------------------------------------------
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
static_assert(std::is_pod<yaku::YAKUSTAT>::value, "YAKUSTAT is not POD");
#endif

#endif
