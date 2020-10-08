#pragma once

#include "../common/strcode.h"
#include "../common/largenum.h"
#include "gametbl.h"
#include "func.h"
#include "discard.h"
#include "endtype.h"
#include <array>

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
	struct TileNameData : public std::array<CodeConv::tstring, TileFlowerMax> {
		using super = std::array<CodeConv::tstring, TileFlowerMax>;
		const CodeConv::tstring operator[] (const Tile& p) const { return super::operator[](static_cast<std::size_t>(p.tile) + static_cast<std::size_t>(p.red) * TileNonflowerMax); }
		reference operator[] (const Tile& p) { return super::operator[](static_cast<std::size_t>(p.tile) + static_cast<std::size_t>(p.red) * TileNonflowerMax); }
		const CodeConv::tstring operator[] (TileCode p) const { return super::operator[](static_cast<std::size_t>(p)); }
		reference operator[] (TileCode p) { return super::operator[](static_cast<std::size_t>(p)); }
		const CodeConv::tstring operator[] (std::size_t p) const { return super::operator[](p); }
		reference operator[] (std::size_t p) { return super::operator[](p); }
		TileNameData() = default;
		TileNameData(const TileNameData&) = default;
		TileNameData& operator = (const TileNameData&) = default;
		TileNameData(const std::initializer_list<LPCTSTR>& ini) {
			auto i = begin(); auto j = ini.begin();
			while (i != end() && j != ini.end())
				*(i++) = *(j++);
		};
	};
	static const TileNameData Xtilerefcode;

	static InfoByPlayer<LargeNum> origPoint;
	static CodeConv::tostringstream XMLhaifuBuffer, XhaifuBuffer, XhaifuBufferBody;
	static bool haifukanflag;

	class PlayerStream {
	public:
		CodeConv::tostringstream haipai, tsumo, tsumolabel, sutehai, sutehailabel, final;
		PlayerStream() {};
		PlayerStream(const PlayerStream&) = delete; // Delete unexpected copy constructor
		PlayerStream& operator= (const PlayerStream&) = delete; // Delete unexpected assign operator
	};
	class HaifuStreams {
	public:
		InfoByPlayer<PlayerStream> streamDat;
		CodeConv::tostringstream dora, uraDora, aliceDora, aliceDoraMax, resultDesc;
		HaifuStreams() {}
		HaifuStreams(const HaifuStreams&) = delete; // Delete unexpected copy constructor
		HaifuStreams& operator= (const HaifuStreams&) = delete; // Delete unexpected assign operator
	};
	static HaifuStreams XhaifuP;

	class tools {
	public:
		static const unsigned int cols = 40u;

		static void checkCycle(bool reset = false);

		static void haifuRecTime(CodeConv::tstring tagName);
		static void haifuskipX(PlayerID targetPlayer);
		static void haifuskip(PlayerID PassivePlayer, PlayerID ActivePlayer);
		static CodeConv::tstring haifudoraClassX(DoraCol Akadora);
		static void recordDoraStream(CodeConv::tostringstream* const x, TileCode tmpDora);
		static void recordTile_Inline(Tile tlCode, bool rotate);
		static void recordTile_Inline(Tile tlCode, DoraCol kakanCol);
		static void recordTile_Table(Tile tlCode, CodeConv::tstring tagName = _T("tile"), bool keepOpen = false);
		static void haifuwritetsumohai(PlayerID ActivePlayer, Tile tlCode, CodeConv::tstring XAttr);
		static void haifuskipall(PlayerID PassivePlayer);

		class kan_sub {
		public:
			static void recordKanOrFlower(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);
			static void inline recordChanKan(const GameTable* const gameStat, CodeConv::tstring XAttrA, CodeConv::tstring XAttrB);
			static void inline recordKan(const GameTable* const gameStat, CodeConv::tstring XAttrA, CodeConv::tstring XAttrB);
		public: /* Monostate class: cannot be instantiated */
			kan_sub() = delete;
			kan_sub(const kan_sub&) = delete;
			kan_sub& operator= (const kan_sub&) = delete;
			~kan_sub() = delete;
		};

		class hfwriter {
		private:
			class finalformWriter {
			private:
				class MeldWriter {
				public:
					static void hfChii(PlayerID player, MeldCode meld);
					static inline void hfPon1(PlayerID player, MeldCode meld);
					static void hfPon(PlayerID player, MeldCode meld);
				public: /* Monostate class: cannot be instantiated */
					MeldWriter() = delete;
					MeldWriter(const MeldWriter&) = delete;
					MeldWriter& operator= (const MeldWriter&) = delete;
					~MeldWriter() = delete;
				};
			public:
				static void hfFinalForm(const GameTable* const gameStat, PlayerID player, EndType RoundEndType);
				static void hfFlower(const GameTable* const gameStat, PlayerID player);
				static void hfExposedMeld(const GameTable* const gameStat, PlayerID player);
			public: /* Monostate class: cannot be instantiated */
				finalformWriter() = delete;
				finalformWriter(const finalformWriter&) = delete;
				finalformWriter& operator= (const finalformWriter&) = delete;
				~finalformWriter() = delete;
			};

			static void hfScoreWriteOut(const GameTable* const gameStat, PlayerID player, SeatAbsolute wind, EndType RoundEndType);

		public:
			static void hfWriteHead(const GameTable* const gameStat,
				int OrigTurn, int OrigHonba, bool tmpUraFlag, bool tmpAliceFlag,
				CodeConv::tstring ResultDesc, EndType RoundEndType);

			static void hfWriteFinalForms(const GameTable* const gameStat, int OrigTurn, EndType RoundEndType);
			static void hfWriteBottom();
		public: /* Monostate class: cannot be instantiated */
			hfwriter() = delete;
			hfwriter(const hfwriter&) = delete;
			hfwriter& operator= (const hfwriter&) = delete;
			~hfwriter() = delete;
		};
	public: /* Monostate class: cannot be instantiated */
		tools() = delete;
		tools(const tools&) = delete;
		tools& operator= (const tools&) = delete;
		~tools() = delete;
	};

public:
	static void haifubufinit();
	static void haifuinit();

	static void haifurechaipai(const GameTable* const gameStat);

	static void haifurecdora(TileCode tmpDora);
	static void haifurecuradora(TileCode tmpDora);

	static void haifurecalicedora(TileCode tmpDora);
	static void haifualicedoraupd();
	static void haifuresetalicedora();

	static void haifurecmota(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);

	static void haifurecfurikomi(const GameTable* const gameStat);

	static void haifurecchi(const GameTable* const gameStat);
	static void haifurecpon(const GameTable* const gameStat);
	static void haifurectsumo(const GameTable* const gameStat);
	static void haifurecminkan(const GameTable* const gameStat);

	static void haifurecnorth(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);
	static void haifurecchanpei(const GameTable* const gameStat);
	static void haifurecnorthproc(const GameTable* const gameStat);

	static void haifurecankan(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);
	static void haifurecchankan(const GameTable* const gameStat);
	static void haifureckanproc(const GameTable* const gameStat);

	static void haifurecflower(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);

	static void haifuwritebuffer(
		const GameTable* const gameStat,
		int OrigTurn, int OrigHonba, bool tmpUraFlag, bool tmpAliceFlag,
		LPCTSTR ResultDesc, EndType RoundEndType);

	static void haifusave(const GameTable* const gameStat);

public: /* Monostate class: cannot be instantiated */
	haifu() = delete;
	haifu(const haifu&) = delete;
	haifu& operator= (const haifu&) = delete;
	~haifu() = delete;
};
