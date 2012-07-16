#ifndef YAKU_H
#define YAKU_H

#include <cstring>
#include <type_traits>
#include <functional>
#include <list>
#include <Windows.h>
#include "largenum.h"
#include "gametbl.h"
#include "tileutil.h"
#include "ykclass.h"

namespace yaku {
	EXPORT_STRUCT YAKUSTAT {
		bool isValid; // �a�����Ă��邩�ǂ���
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
	}

	// ---------------------------------------------------------------------

	namespace countingFacility {
		int countPairs(
			const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles);
		int countTileNumerals(const Int8ByTile tileCount);

		Int8ByTile countByMelds(
			const MELD_BUF MianziDat, int* const hits,
			std::function<bool (meldStat)> f);

		Int8ByTile countKez(const MELD_BUF MianziDat, int* const Kezi);
		Int8ByTile countAnKez(const MELD_BUF MianziDat, int* const Kezi);
		Int8ByTile countDuiz(const MELD_BUF MianziDat);

		Int8ByTile countShunz(const MELD_BUF MianziDat);
		Int8ByTile countAnShunz(const MELD_BUF MianziDat);

		Int8ByTile countKangz(const MELD_BUF MianziDat, int* const Kangzi);
		Int8ByTile countAnKangz(const MELD_BUF MianziDat, int* const Kangzi);
		Int8ByTile countKaKangz(const MELD_BUF MianziDat, int* const Kangzi);

		int countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
			const tileCode* const targetShunz, int numOfShunz, bool Mode);
		int countMentzNumerals(const MELD_BUF MianziDat);
	}

	// ---------------------------------------------------------------------

	namespace yakuCalculator {

		class YakuCatalog { // ���̈ꗗ [singleton]
		private:
			YakuCatalog() {}
			YakuCatalog(const YakuCatalog&);
			YakuCatalog& operator= (const YakuCatalog&);
		public:
			static YakuCatalog* Instantiate(); // Singleton instance accessor
			std::list<Yaku> catalog;
		};

		__declspec(dllexport) void init();

		struct MENTSU_ANALYSIS { // �ʎq��͌���
			PLAYER_ID player;
			SHANTEN shanten[SHANTEN_PAGES];
			MELD_BUF MianziDat; // �ʎq�p�[�X����
			Int8ByTile KeziCount; // ���q�E�Ȏq�̐�
			Int8ByTile AnKeziCount; // �Í��E�ÞȂ̐�
			Int8ByTile DuiziCount; // �Ύq�E���q�E�Ȏq�̐�
			Int8ByTile ShunziCount; // ���q�̐�
			Int8ByTile AnShunziCount; // ���Ă��Ȃ����q�̐�
			Int8ByTile KangziCount; // �Ȏq�̐�
			Int8ByTile AnKangziCount; // �ÞȂ̐�
			Int8ByTile KaKangziCount; // ���Ȃ̐�
		};
#ifdef MJCORE_EXPORTS
		static_assert(std::is_pod<MENTSU_ANALYSIS>::value, "MENTSU_ANALYSIS is not POD");
#endif

		class CalculatorThread {
		public:
			static DWORD WINAPI calculator(LPVOID lpParam);
			static int numOfRunningThreads(); // �����Ă���X���b�h�̐�
			static const int threadLimit = 4; // �����ɋN������ő�̃X���b�h��
			CalculatorThread(); // �f�t�H���g�R���X�g���N�^
			~CalculatorThread(); // �f�t�H���g�f�X�g���N�^
		private:
			static void incThreadCount();
			static void decThreadCount();
			static int runningThreads;
			static CRITICAL_SECTION cs;
			DWORD WINAPI calculate(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, const ParseMode* const pMode);
		};

		struct CalculatorParam {
			ParseMode pMode;
			CalculatorThread* instance;
			const GameTable* gameStat;
			MENTSU_ANALYSIS analysis;
			YAKUSTAT result;
		};
#ifdef MJCORE_EXPORTS
		static_assert(std::is_pod<CalculatorParam>::value, "CalculatorParam is not POD");
#endif

		YAKUSTAT countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer);
		__declspec(dllexport) void countyaku(const GameTable* const gameStat,
			YAKUSTAT* const yakuInfo, int targetPlayer);
	}
}

#endif
