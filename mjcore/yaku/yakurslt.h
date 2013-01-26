#pragma once

#include "ykclass.h"
#include "../mjexport.h"
#include "../largenum.h"
#include "../gametbl.h"

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
	TCHAR yakuNameList[nameBufSize];
	TCHAR yakuValList[nameBufSize];
	TCHAR yakumanNameList[nameBufSize];
	TCHAR yakumanValList[nameBufSize];
	// -----------------------------------------------------------------
	MELD_BUF MianziDat; // �ʎq�ɕ��������f�[�^
	// -----------------------------------------------------------------
	static void Init(YAKUSTAT* const myInstance); // �C���X�^���X������������
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<yaku::YAKUSTAT>::value, "YAKUSTAT is not POD");
#endif
