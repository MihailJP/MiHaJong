#pragma once

#include "../common/strcode.h"
#include "gametbl.h"
#include "func.h"
#include "discard.h"
#include "endtype.h"

// 牌譜関係のコードはクラスに隔離しておきましょうか。
class haifu {
private:
	static const unsigned StringElemSize =
#ifdef _UNICODE
		1u;
#else
		2u;
#endif

	/* 雀牌の名前データ */
	static const CodeConv::tstring tilecodelabel, HTtilecodelabel1, HTtilecodelabel2;

	static InfoByPlayer<LNum> origPoint;
	static CodeConv::tostringstream haifuBuffer, HThaifuBuffer;
	static bool haifukanflag;

	class PlayerStream {
	public:
		CodeConv::tostringstream haipai, tsumo, tsumolabel, sutehai, sutehailabel, final;
	};
	class HaifuStreams {
	public:
		InfoByPlayer<PlayerStream> streamDat;
		CodeConv::tostringstream dora, uraDora, aliceDora, aliceDoraMax, resultDesc;
	};
	static HaifuStreams haifuP, HThaifuP;

	class tools {
	public:
		static const unsigned int cols = 40u;

		static void haifuskip(
			HaifuStreams* haifuP, HaifuStreams* HThaifuP,
			PLAYER_ID PassivePlayer, PLAYER_ID ActivePlayer
			);
		static CodeConv::tstring haifudoraClass(doraCol Akadora);
		static void recordDoraStream(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, tileCode tmpDora);
		static void recordTile_Inline(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, TILE tlCode, bool rotate);
		static void recordTile_Inline(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, TILE tlCode, doraCol kakanCol);
		static void recordTile_Table(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, TILE tlCode);
		static void recordBlank_Table(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h);
		static void haifuwritetsumohai(
			HaifuStreams* haifuP, HaifuStreams* HThaifuP,
			PLAYER_ID ActivePlayer, TILE tlCode,
			CodeConv::tstring PText, CodeConv::tstring HTText
			);
		static void haifuskipall(HaifuStreams* haifuP, HaifuStreams* HThaifuP, PLAYER_ID PassivePlayer);

		class kan_sub {
		public:
			static void recordKanOrFlower(
				const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex,
				HaifuStreams* haifuP, HaifuStreams* HThaifuP
				);
			static void inline recordChanKan(const GameTable* const gameStat, CodeConv::tstring pTxt, CodeConv::tstring hTxt);
			static void inline recordKan(const GameTable* const gameStat, CodeConv::tstring pTxt, CodeConv::tstring hTxt);
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
				CodeConv::tstring ResultDesc, EndType RoundEndType);

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

	static void haifurecalicedora(tileCode tmpDora);

	static __declspec(dllexport) void haifualicedoraupd();

	static void haifuresetalicedora();

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
		LPCTSTR ResultDesc, EndType RoundEndType);

	static void haifusave(const GameTable* const gameStat);
	static __declspec(dllexport) void haifusave();
};
