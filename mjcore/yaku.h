#ifndef YAKU_H
#define YAKU_H

#include <cstring>
#include <type_traits>
#include <cstdio>
#include "largenum.h"
#include "gametbl.h"
#include "tileutil.h"

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
	static_assert(std::is_pod<YAKUSTAT>::value, "YAKUSTAT is not POD");
#endif

	// ---------------------------------------------------------------------

	enum ParseOrder : uint8_t {
		Ke_Shun, Shun_Ke, Ke_Shun_Rev, Shun_Ke_Rev
	};
	struct ParseMode {
		tileCode AtamaCode;
		ParseOrder Order;
	};

#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<ParseMode>::value, "ParseMode is not POD");
#endif

	// ---------------------------------------------------------------------

	namespace mentsuParser { // �ʎq�p�[�T
		bool makementsu_shuntsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile); /* ���q�̏��� */
		void makementsu_koutsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile); /* ���q�̏��� */

		void makementsu(const GameTable* const gameStat,
			PLAYER_ID targetPlayer, ParseMode AtamaCode,
			int* const ProcessedMelds, MELD_BUF MianziDat); /* �ʎq�ɕ������� */
		__declspec(dllexport) void makementsu(const GameTable* const gameStat,
			int targetPlayer, int AtamaCode, int* const ProcessedMelds, void* const MianziDat);
	}

	// ---------------------------------------------------------------------

	namespace countingFacility {
		/* �w��̑Ύq�����邩������ */
		int countPairs(
			const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles);
		__declspec(dllexport) int countPairs(
			const int* const tileCount, const int* const targetTiles, int numOfTiles);
		int countTileNumerals(const Int8ByTile tileCount);
		__declspec(dllexport) int countTileNumerals(const int* const tileCount);
	}
}

#endif
