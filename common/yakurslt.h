#pragma once

#include <type_traits>
#ifdef _WIN32
#include <tchar.h>
#endif /*_WIN32*/
#include "largenum.h"
#include "gametbl.h"

namespace mihajong_structs {

struct YakuResult {
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
	MeldBuf MianziDat; // �ʎq�ɕ��������f�[�^
	// -----------------------------------------------------------------
	static void Init(YakuResult* const myInstance) { // �C���X�^���X������������
		myInstance->isValid = false;
		myInstance->BasePoints = 20;
		myInstance->CoreHan = myInstance->CoreSemiMangan =
			myInstance->BonusHan = myInstance->BonusSemiMangan =
			myInstance->DoraQuantity = myInstance->UraDoraQuantity =
			myInstance->AkaDoraQuantity = myInstance->AoDoraQuantity =
			myInstance->AliceDora = myInstance->FlowerQuantity = 0;
		myInstance->AgariPoints = LargeNum::fromInt(0);
		memset(myInstance->yakuNameList, 0, nameBufSize);
		memset(myInstance->yakuValList, 0, nameBufSize);
		memset(myInstance->yakumanNameList, 0, nameBufSize);
		memset(myInstance->yakumanValList, 0, nameBufSize);
	}
	void Init() {Init(this);} // �C���X�^���X������������
};
static_assert(std::is_pod<YakuResult>::value, "YakuResult is not POD");

} /* namespace */
