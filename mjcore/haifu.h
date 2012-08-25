#ifndef HAIFU_H
#define HAIFU_H

#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "gametbl.h"
#include "largenum.h"
#include "func.h"
#include "envtbl.h"
#include "version.h"
#include "discard.h"

// 今のところここでしか使ってないから仮置き。
// 移植が進んで別のところでも使うようなら移動します

/* 局終了コード */
enum EndType : int { // 局の終了理由
	Ryuukyoku,       // 普通の流局
	Agari,           // 誰かの和了
	Chonbo,          // 錯和で終了
	KyuushuKyuuhai,  // 九種九牌
	Suukaikan,       // 四開槓
	TripleRon,       // 三家和
	SuufonRenda,     // 四風連打
	SuuchaRiichi,    // 四人立直
	NagashiMangan,   // 流し満貫
	Disconnect,      // 通信途絶(ネットワーク対戦時のみ)
	Uukaikan,        // 五回目の槓で流局した場合(誰かが四槓子聴牌の場合のみ)
};

// 牌譜関係のコードはクラスに隔離しておきましょうか。
class haifu {
private:
	/* 雀牌の名前データ */
	static const std::string tilecodelabel, HTtilecodelabel1, HTtilecodelabel2;

	static InfoByPlayer<LargeNum> origPoint;
	static std::ostringstream haifuBuffer, HThaifuBuffer;
	static bool haifukanflag;

	class PlayerStream {
	public:
		static std::ostringstream haipai, tsumo, tsumolabel, sutehai, sutehailabel, final;
	};
	class HaifuStreams {
	public:
		static InfoByPlayer<PlayerStream> streamDat;
		static std::ostringstream dora, uraDora, aliceDora, aliceDoraMax, resultDesc;
	};
	static HaifuStreams haifuP, HThaifuP;

	class tools {
	public:
		static const unsigned int cols = 40u;

		static void haifuskip(
			HaifuStreams* haifuP, HaifuStreams* HThaifuP,
			PLAYER_ID PassivePlayer, PLAYER_ID ActivePlayer
			);
		static std::string haifudoraClass(doraCol Akadora);
		static void recordDoraStream(std::ostringstream* const p, std::ostringstream* const h, tileCode tmpDora);
		static void recordTile_Inline(std::ostringstream* const p, std::ostringstream* const h, TILE tlCode, bool rotate);
		static void recordTile_Inline(std::ostringstream* const p, std::ostringstream* const h, TILE tlCode, doraCol kakanCol);
		static void recordTile_Table(std::ostringstream* const p, std::ostringstream* const h, TILE tlCode);
		static void recordBlank_Table(std::ostringstream* const p, std::ostringstream* const h);
		static void haifuwritetsumohai(
			HaifuStreams* haifuP, HaifuStreams* HThaifuP,
			PLAYER_ID ActivePlayer, TILE tlCode,
			std::string PText, std::string HTText
			);
		static void haifuskipall(HaifuStreams* haifuP, HaifuStreams* HThaifuP, PLAYER_ID PassivePlayer);

		class kan_sub {
		public:
			static void recordKanOrFlower(
				const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex,
				HaifuStreams* haifuP, HaifuStreams* HThaifuP
				);
			static void inline recordChanKan(const GameTable* const gameStat, std::string pTxt, std::string hTxt);
			static void inline recordKan(const GameTable* const gameStat, std::string pTxt, std::string hTxt);
		};

		class hfwriter {
		private:
			class finalformWriter {
			private:
				class MeldWriter {
				public:
					static void hfChii(PLAYER_ID player, meldCode meld);
					static inline void hfPon1(PLAYER_ID player, meldCode meld);
					static void hfPon(PLAYER_ID player, meldCode meld);
				};
			public:
				static void hfFinalForm(const GameTable* const gameStat, PLAYER_ID player, EndType RoundEndType);
				static void hfFlower(const GameTable* const gameStat, PLAYER_ID player);
				static void hfExposedMeld(const GameTable* const gameStat, PLAYER_ID player);
			};

			static void hfScoreWriteOut(const GameTable* const gameStat, PLAYER_ID player, seatAbsolute wind);
			static void hfWriteOut(const GameTable* const gameStat, PLAYER_ID player);

		public:
			static void hfWriteHead(const GameTable* const gameStat,
				int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
				std::string ResultDesc, EndType RoundEndType);

			static void hfWriteFinalForms(const GameTable* const gameStat, int OrigTurn, EndType RoundEndType);
			static void hfWriteBottom();
		};
	};

public:
	static __declspec(dllexport) void haifubufinit();
	static __declspec(dllexport) void haifuinit();

	static __declspec(dllexport) void haifurechaipai(const GameTable* const gameStat);

	static void haifurecdora(tileCode tmpDora);
	static __declspec(dllexport) void haifurecdora(int tmpDora);

	static void haifurecuradora(tileCode tmpDora);
	static __declspec(dllexport) void haifurecuradora(int tmpDora);

	static void haifurecalicedora(tileCode tmpDora);
	static __declspec(dllexport) void haifurecalicedora(int tmpDora);

	static __declspec(dllexport) void haifualicedoraupd();

	static __declspec(dllexport) void haifuresetalicedora();

	static __declspec(dllexport) void haifurecdorap();

	static __declspec(dllexport) void haifurecmota(const GameTable* const gameStat, int DiscardTileIndex);
	static void haifurecmota(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);

	static __declspec(dllexport) void haifurecfurikomi(const GameTable* const gameStat);

	static __declspec(dllexport) void haifurecchi(const GameTable* const gameStat);
	static __declspec(dllexport) void haifurecpon(const GameTable* const gameStat);
	static __declspec(dllexport) void haifurectsumo(const GameTable* const gameStat);
	static __declspec(dllexport) void haifurecminkan(const GameTable* const gameStat);

	static __declspec(dllexport) void haifurecnorth(const GameTable* const gameStat, int DiscardTileIndex);
	static void haifurecnorth(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);
	static __declspec(dllexport) void haifurecchanpei(const GameTable* const gameStat);
	static __declspec(dllexport) void haifurecnorthproc(const GameTable* const gameStat);

	static __declspec(dllexport) void haifurecankan(const GameTable* const gameStat, int DiscardTileIndex);
	static void haifurecankan(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);
	static __declspec(dllexport) void haifurecchankan(const GameTable* const gameStat);
	static __declspec(dllexport) void haifureckanproc(const GameTable* const gameStat);

	static __declspec(dllexport) void haifurecflower(const GameTable* const gameStat, int DiscardTileIndex);
	static void haifurecflower(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);

	static __declspec(dllexport) void haifuwritebuffer(
		const GameTable* const gameStat, void *,
		int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
		const char* ResultDesc, EndType RoundEndType);

	static void haifusave(const GameTable* const gameStat);
	static __declspec(dllexport) void haifusave();
};

#endif
