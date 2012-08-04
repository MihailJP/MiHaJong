#ifndef YAKU_H
#define YAKU_H

#include <cstdint>
#include <string>
#include <type_traits>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <array>
#include <vector>
#include <Windows.h>
#include "ykclass.h"
#include "yk_tools.h"
#include "../largenum.h"
#include "../gametbl.h"
#include "../tileutil.h"
#include "../except.h"
#include "../logging.h"

class yaku::yakuCalculator {
private:
	enum hanUnit : uint8_t {Han, SemiMangan, Yakuman};
	class Yaku;

	class YakuCatalog { // ���̈ꗗ [singleton]
		friend yaku::yakuCalculator;
	private:
		YakuCatalog() {}
		YakuCatalog(const YakuCatalog&);
		YakuCatalog& operator= (const YakuCatalog&);
		class catalogInit;
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
		MachihaiInfo MachiInfo; // �҂��v�̐��Ƃ�
		bool isPinfu; // ���a�ɂȂ��Ă邩�ǂ���
		Int8ByTile KeziCount; // ���q�E�Ȏq�̐�
		Int8ByTile AnKeziCount; // �Í��E�ÞȂ̐�
		Int8ByTile DuiziCount; // �Ύq�E���q�E�Ȏq�̐�
		Int8ByTile ShunziCount; // ���q�̐�
		Int8ByTile AnShunziCount; // ���Ă��Ȃ����q�̐�
		Int8ByTile KangziCount; // �Ȏq�̐�
		Int8ByTile AnKangziCount; // �ÞȂ̐�
		Int8ByTile KaKangziCount; // ���Ȃ̐�
		Int8ByTile SeenTiles; // �����Ă���v
		Int8ByTile TileCount; // �v�̐�
		uint8_t TotalKezi; // ���q�Ȏq���v
		uint8_t TotalAnKezi; // �Í��Þȍ��v
		uint8_t TotalShunzi; // ���q���v
		uint8_t TotalAnShunzi; // �Ï��q���v
		uint8_t TotalKangzi; // �Ȏq���v
		uint8_t TotalAnKangzi; // �Þȍ��v
		uint8_t TotalKaKangzi; // ���ȍ��v
		const GameTable* GameStat; // ����ւ̃|�C���^
		const PlayerTable* PlayerStat; // �v���C���[���ւ̃|�C���^
		const TILE* TsumoHai; // �a���v�ւ̃|�C���^
		const bool* MenzenFlag; // ��O���ǂ����̃t���O
		const bool* TsumoAgariFlag; // �c���A�K���ǂ����̃t���O
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<MENTSU_ANALYSIS>::value, "MENTSU_ANALYSIS is not POD");
#endif
	typedef std::function<bool (const MENTSU_ANALYSIS* const)> YAKUFUNC;

	class CalculatorThread {
	public:
		static DWORD WINAPI calculator(LPVOID lpParam);
		int numOfRunningThreads(); // �����Ă���X���b�h�̐�
		static const int threadLimit = 4; // �����ɋN������ő�̃X���b�h��
		int numOfStartedThreads(); // �J�n�����X���b�h�̐�
		void sync(int threads); // �X���b�h�𓯊�����
		CalculatorThread(); // �f�t�H���g�R���X�g���N�^
		~CalculatorThread(); // �f�t�H���g�f�X�g���N�^
	private:
		void incThreadCount();
		void decThreadCount();
		int runningThreads;
		CRITICAL_SECTION cs;
		int startedThreads;
		static void calcbasepoints(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis);
		DWORD WINAPI calculate(
			const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
			const ParseMode* const pMode, YAKUSTAT* const result);
		static void countDora(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result);
		static void doraText(YAKUSTAT* const result, const char* const label, int quantity);
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
				int8_t getHan() const;
				hanUnit getUnit() const;
				static const HAN
					yv_null, yv_1han, yv_2han, yv_3han, yv_4han, yv_5han, yv_6han, yv_7han, yv_8han, 
					yv_mangan, yv_haneman, yv_baiman, yv_3baiman, yv_yakuman, yv_double_yakuman,
					yv_triple_yakuman, yv_quad_yakuman;
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
	public:
		class HANFUNC {
		protected:
			std::function<YAKU_HAN (const MENTSU_ANALYSIS* const)> hFunc;
		public:
			YAKU_HAN operator() (const MENTSU_ANALYSIS* const);
			HANFUNC ();
			HANFUNC (std::function<YAKU_HAN (const MENTSU_ANALYSIS* const)>);
		};
		class FixedHan : public HANFUNC {
		public:
			FixedHan () : HANFUNC () {}
			FixedHan (YAKU_HAN bHan);
			FixedHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
		class MenzenHan : public HANFUNC {
		public:
			MenzenHan () : HANFUNC () {}
			MenzenHan (YAKU_HAN bHan);
			MenzenHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
		class KuisagariHan : public HANFUNC {
		public:
			KuisagariHan () : HANFUNC () {}
			KuisagariHan (YAKU_HAN bHan);
			KuisagariHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
		static const FixedHan yval_none, yval_1han, yval_2han, yval_3han, yval_4han, yval_5han, yval_6han,
			yval_mangan, yval_baiman, yval_yakuman, yval_double_yakuman,
			yval_triple_yakuman, yval_quad_yakuman,
			yval_1han_dependent, yval_2han_dependent, yval_4han_dependent, yval_yakuman_dependent;
		static const MenzenHan yval_1han_menzen, yval_2han_menzen, yval_3han_menzen,
			yval_4han_menzen, yval_5han_menzen, yval_6han_menzen,
			yval_mangan_menzen, yval_baiman_menzen, yval_yakuman_menzen, yval_double_yakuman_menzen,
			yval_1han_menzen_dependent, yval_2han_menzen_dependent, yval_yakuman_menzen_dependent;
		static const KuisagariHan yval_1han_kuisagari, yval_2han_kuisagari, yval_3han_kuisagari,
			yval_4han_kuisagari, yval_5han_kuisagari, yval_6han_kuisagari;
	private:
		HANFUNC han;
		std::string yakuName; // ���̖��O�i������j
		YAKUFUNC yakuProc; // ���̔�����@
		std::set<std::string> suppressionList; // ���ʖ��̃��X�g
		Yaku() {} // Default constructor
	public:
		bool checkYaku(const MENTSU_ANALYSIS* const mentsu);
		std::string getName(); // ���̖��O���擾����
		HANFUNC getHan();
		YAKU_HAN getHan(const MENTSU_ANALYSIS* const mentsu);
		std::set<std::string> getSuppression();
		// Constructor
		Yaku(std::string name, HANFUNC hanVal, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, std::string yk8, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, std::string yk8, std::string yk9, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, std::string yk8, std::string yk9, std::string yk10,
			YAKUFUNC f);
	};

	static void doubling(yaku::YAKUSTAT* const yStat);
	static void calculateScore(yaku::YAKUSTAT* const yStat);

	static void analysisNonLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
		SHANTEN* const shanten, YAKUSTAT* const yakuInfo);
	static void analysisLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
		SHANTEN* const shanten, YAKUSTAT* const yakuInfo);

public:
	static __declspec(dllexport) void init();

	static YAKUSTAT countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer);
	static __declspec(dllexport) void countyaku(const GameTable* const gameStat,
		YAKUSTAT* const yakuInfo, int targetPlayer);
	static bool chkShisanBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer);
	static __declspec(dllexport) int chkShisanBuDa(const GameTable* const gameStat, int targetPlayer);
	static bool chkShisiBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer);
	static __declspec(dllexport) int chkShisiBuDa(const GameTable* const gameStat, int targetPlayer);
};

#endif
