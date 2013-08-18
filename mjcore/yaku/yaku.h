#pragma once

#include <cstdint>
#include <type_traits>
#include <functional>
#include <list>
#include <set>
#include <map>
#include <vector>
#include "ykclass.h"
#include "yk_tools.h"
#include "../gametbl.h"
#include "../tileutil.h"
#include "../shanten.h"
#include "../../common/mutex.h"

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
		PlayerID player;
		Shanten shanten[SHANTEN_PAGES];
		MeldBuf MianziDat; // �ʎq�p�[�X����
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
		const Tile* TsumoHai; // �a���v�ւ̃|�C���^
		const bool* MenzenFlag; // ��O���ǂ����̃t���O
		const bool* TsumoAgariFlag; // �c���A�K���ǂ����̃t���O
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<MENTSU_ANALYSIS>::value, "MENTSU_ANALYSIS is not POD");
#endif
	typedef std::function<bool (const MENTSU_ANALYSIS* const)> YAKUFUNC;

	class CalculatorThread; // �錾����
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
#ifdef GUOBIAO
				static const HAN
					yv_1, yv_2, yv_4, yv_6, yv_8, yv_12, yv_16, yv_24, yv_32, yv_48, yv_64, yv_88;
#else /* GUOBIAO */
				static const HAN
					yv_null, yv_1han, yv_2han, yv_3han, yv_4han, yv_5han, yv_6han, yv_7han, yv_8han, 
					yv_mangan, yv_haneman, yv_baiman, yv_3baiman, yv_yakuman, yv_double_yakuman,
					yv_triple_yakuman, yv_quad_yakuman;
#endif /* GUOBIAO */
			private:
				int8_t han; // ���l
#ifndef GUOBIAO
				hanUnit unit; // �P��
#endif /* GUOBIAO */
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
			YAKU_HAN operator() (const MENTSU_ANALYSIS* const) const;
			HANFUNC ();
			HANFUNC (std::function<YAKU_HAN (const MENTSU_ANALYSIS* const)>);
		};
		class FixedHan : public HANFUNC {
		public:
			FixedHan () : HANFUNC () {}
			FixedHan (YAKU_HAN bHan);
			FixedHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
#ifdef GUOBIAO
		static const FixedHan
			yval_1, yval_2, yval_4, yval_6, yval_8, yval_12, yval_16,
			yval_24, yval_32, yval_48, yval_64, yval_88;
#else /* GUOBIAO */
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
#endif /* GUOBIAO */
	private:
		HANFUNC han;
		CodeConv::tstring yakuName; // ���̖��O�i������j
		YAKUFUNC yakuProc; // ���̔�����@
		std::set<CodeConv::tstring> suppressionList; // ���ʖ��̃��X�g
		Yaku() {} // Default constructor
	public:
		bool checkYaku(const MENTSU_ANALYSIS* const mentsu);
		CodeConv::tstring getName(); // ���̖��O���擾����
		HANFUNC getHan();
		YAKU_HAN getHan(const MENTSU_ANALYSIS* const mentsu);
		std::set<CodeConv::tstring> getSuppression();
		// Constructor
		Yaku(CodeConv::tstring name, HANFUNC hanVal, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, CodeConv::tstring yk7, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, CodeConv::tstring yk9, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, CodeConv::tstring yk9, CodeConv::tstring yk10,
			YAKUFUNC f);
	};

	class CalculatorThread {
	public:
#ifdef _WIN32
		static DWORD WINAPI calculator(LPVOID lpParam);
#else /*_WIN32*/
		static void* calculator(void* lpParam);
#endif /*_WIN32*/
		int numOfFinishedThreads(); // �I������X���b�h�̐�
		static const int threadLimit = 4; // �����ɋN������ő�̃X���b�h��
		int numOfStartedThreads(); // �J�n�����X���b�h�̐�
		void sync(int threads); // �X���b�h�𓯊�����
		CalculatorThread(); // �f�t�H���g�R���X�g���N�^
		~CalculatorThread(); // �f�t�H���g�f�X�g���N�^
	private:
		void recordThreadStart();
		void recordThreadFinish();
		int finishedThreads;
		MHJMutex cs;
		int startedThreads;
#ifndef GUOBIAO
		static void calcbasepoints(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis);
#endif /* GUOBIAO */
#ifdef _WIN32
		DWORD WINAPI calculate(
#else /*_WIN32*/
		void* calculate(
#endif /*_WIN32*/
			const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
			const ParseMode* const pMode, YAKUSTAT* const result);
		static void countDora(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result);
		static void checkPostponedYaku(
			const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result,
			std::map<CodeConv::tstring, Yaku::YAKU_HAN> &yakuHan, std::set<CodeConv::tstring> &suppression,
			std::vector<CodeConv::tstring> &yakuOrd);
		static void hanSummation(
			int& totalHan, int& totalSemiMangan, int& totalBonusHan, int& totalBonusSemiMangan,
			std::map<CodeConv::tstring, Yaku::YAKU_HAN> &yakuHan, std::vector<CodeConv::tstring> &yakuOrd, YAKUSTAT* const result);
	};

#ifndef GUOBIAO
	static void doubling(yaku::YAKUSTAT* const yStat);
#endif /* GUOBIAO */
	static void calculateScore(yaku::YAKUSTAT* const yStat);

	static void analysisNonLoop(const GameTable* const gameStat, PlayerID targetPlayer,
		Shanten* const shanten, YAKUSTAT* const yakuInfo);
	static void analysisLoop(const GameTable* const gameStat, PlayerID targetPlayer,
		Shanten* const shanten, YAKUSTAT* const yakuInfo);

	static void countDora(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
		YAKUSTAT* const result, PlayerID targetPlayer);

public:
	static void init();

	static YAKUSTAT countyaku(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool chkShisanBuDa(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool chkShisiBuDa(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool checkShibari(const GameTable* const gameStat, const YAKUSTAT* const yakuStat);
};
