#ifndef YAKU_H
#define YAKU_H

#include <cstdint>
#include <type_traits>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <Windows.h>
#include "ykclass.h"
#include "yk_tools.h"
#include "../largenum.h"
#include "../gametbl.h"
#include "../tileutil.h"
#include "../except.h"

class yaku::yakuCalculator {
private:
	enum hanUnit : uint8_t {Han, SemiMangan, Yakuman};
	class Yaku;

	class YakuCatalog { // ���̈ꗗ [singleton]
	private:
		YakuCatalog() {}
		YakuCatalog(const YakuCatalog&);
		YakuCatalog& operator= (const YakuCatalog&);
	public:
		static YakuCatalog* Instantiate(); // Singleton instance accessor
		std::list<Yaku> catalog;
	};

	enum MachiType : uint8_t { // �X�̎��
		machiInvalid, // ����
		machiRyanmen, // ����
		machiKanchan, // �ƒ�
		machiPenchan, // �Ӓ�
		machiShanpon, // �o�|��
		machiTanki    // �P�R
	};
	struct MENTSU_ANALYSIS { // �ʎq��͌���
		PLAYER_ID player;
		SHANTEN shanten[SHANTEN_PAGES];
		MELD_BUF MianziDat; // �ʎq�p�[�X����
		uint8_t BasePoint; // ��
		MachiType Machi; // �҂��̎��
		bool isPinfu; // ���a�ɂȂ��Ă邩�ǂ���
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
	typedef std::function<bool
		(const GameTable* const, const MENTSU_ANALYSIS* const)> YAKUFUNC;

	class CalculatorThread {
	public:
		static DWORD WINAPI calculator(LPVOID lpParam);
		int numOfRunningThreads(); // �����Ă���X���b�h�̐�
		static const int threadLimit = 4; // �����ɋN������ő�̃X���b�h��
		CalculatorThread(); // �f�t�H���g�R���X�g���N�^
		~CalculatorThread(); // �f�t�H���g�f�X�g���N�^
	private:
		void incThreadCount();
		void decThreadCount();
		int runningThreads;
		CRITICAL_SECTION cs;
		static void calcbasepoints(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis);
		DWORD WINAPI calculate(
			const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
			const ParseMode* const pMode, YAKUSTAT* const result);
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

	class Yaku {
	public:
		class YAKU_HAN {
		public:
			class HAN { // �|
			public:
				HAN();
				HAN(int8_t h);
				HAN(int8_t h, hanUnit u);
				int8_t getHan();
				hanUnit getUnit();
				static const HAN
					yv_1han, yv_2han, yv_3han, yv_4han, yv_5han, yv_6han, yv_7han, yv_8han, 
					yv_mangan, yv_haneman, yv_baiman, yv_3baiman, yv_yakuman, yv_double_yakuman;
			private:
				int8_t han; // ���l
				hanUnit unit; // �P��
			};
			HAN coreHan; // ����𖞂����|
			HAN bonusHan; // ����𖞂����Ȃ��|
			YAKU_HAN();
			YAKU_HAN(HAN han);
			YAKU_HAN(HAN han, HAN bonus);
		};
	private:
		YAKU_HAN han;
		std::string yakuName; // ���̖��O�i������j
		YAKUFUNC yakuProc; // ���̔�����@
		std::set<std::string> suppressionList; // ���ʖ��̃��X�g
		Yaku() {} // Default constructor
	public:
		bool checkYaku(const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu);
		std::string getName(); // ���̖��O���擾����
		YAKU_HAN getHan();
		std::set<std::string> getSuppression();
		// Constructor
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, std::string yk3, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, std::string yk8, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, std::string yk8, std::string yk9, YAKUFUNC f);
		Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, std::string yk8, std::string yk9, std::string yk10,
			YAKUFUNC f);
	};

	static void analysisNonLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
		SHANTEN* const shanten, YAKUSTAT* const yakuInfo);
	static void analysisLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
		SHANTEN* const shanten, YAKUSTAT* const yakuInfo);

public:
	static __declspec(dllexport) void init();

	static YAKUSTAT countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer);
	static __declspec(dllexport) void countyaku(const GameTable* const gameStat,
		YAKUSTAT* const yakuInfo, int targetPlayer);
};

#endif
