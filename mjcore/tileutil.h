#pragma once

#ifdef MJCORE_EXPORTS
#include <type_traits>
#endif
#include <cstdint>
#include "mjexport.h"
#include "gametbl.h"

// -------------------------------------------------------------------------

using mihajong_structs::InfoByTile;
using mihajong_structs::PlayerID;
using mihajong_structs::TileCode;
using mihajong_structs::Int8ByTile;

EXPORT_STRUCT MachihaiTileInfo {
	bool MachihaiFlag; // ë“ÇøîvÇ…Ç»Ç¡ÇƒÇ¢ÇÈÇ©ÇÃÉtÉâÉO
	int8_t MachihaiCount; // ë“ÇøîvÇÃécÇËñáêî
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<MachihaiTileInfo>::value, "MachihaiTileInfo is not POD");
#endif

EXPORT_TEMPLATE_STRUCT InfoByTile<MachihaiTileInfo>;
EXPORT_STRUCT MachihaiInfo { // ë“ÇøîvÇ∆Ç©ÇÃèÓïÒÇäiî[(chkFuritenä÷êîóp)
	bool FuritenFlag; // ÉtÉäÉeÉìÇ©Ç«Ç§Ç©ÇÃÉtÉâÉO
	InfoByTile<MachihaiTileInfo> Machihai; // ë“ÇøîvèÓïÒ
	int8_t MachihaiTotal; // ë“ÇøîvÇÃçáåvñáêî
	int8_t MachiMen; // âΩñ ë“ÇøÅH
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<MachihaiInfo>::value, "MachihaiInfo is not POD");
#endif

EXPORT_STRUCT TileStatus { // gettilestatusóp
	bool isExistent, canFormQuad,
		seqMidWait, seqDoubleSideWait, seqSingleSideWait,
		formsPair, formsSequence, formsTriplet;
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<TileStatus>::value, "TileStatus is not POD");
#endif

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
PlayerID* tobePlayed(const GameTable* const gameStat);
PlayerID tobePlayed(const GameTable* const gameStat, int id);

const TileCode* Honor_Major_Tiles();
TileCode Honor_Major_Tiles(int code);
bool isYaojiu(TileCode code);

void lipai(GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) void lipai(GameTable* const gameStat, int targetPlayer);
#endif

MJCORE Int8ByTile countseentiles(const GameTable* const gameStat);

MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PlayerID playerID);
#ifdef MJCORE_EXPORTS
__declspec(dllexport) void countTilesInHand(int* const tileCount, const GameTable* const gameStat, int playerID);
#endif

MJCORE Int8ByTile countRedTilesInHand(const GameTable* const gameStat, PlayerID playerID, int doraCol);

MJCORE TileStatus gettilestatus(
	const GameTable* const gameStat, PlayerID targetPlayer, int targetTile, bool CheckMode);

MJCORE MachihaiInfo chkFuriten(const GameTable* const gameStat, PlayerID targetPlayer);
#ifdef MJCORE_EXPORTS
__declspec(dllexport) void chkFuriten(
	int* const furitenFlag, int* const machihaiFlag, int* const machihaiCount,
	int* const machihaiTotal, int* const machiMen, const GameTable* const gameStat,
	int targetPlayer);

void chkOpenMachi(GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) void chkOpenMachi(GameTable* const gameStat, int targetPlayer);
#endif

MJCORE bool chkdaopaiability(const GameTable* const gameStat, PlayerID targetPlayer);
#ifdef MJCORE_EXPORTS
__declspec(dllexport) int chkdaopaiability(const GameTable* const gameStat, int targetPlayer);

__declspec(dllexport) void setdora(GameTable* const gameStat, int Mode);
#endif

MJCORE bool chkAnkanAbility(const GameTable* const gameStat, PlayerID targetPlayer);
#ifdef MJCORE_EXPORTS
__declspec(dllexport) int chkAnkanAbility(const GameTable* const gameStat, int targetPlayer);

__declspec(dllexport) void calcdoukasen(GameTable* const gameStat);

bool isTenpai(const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) int isTenpai(const GameTable* const gameStat, void *, int targetPlayer);

bool isNagashiMangan(const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) int isNagashiMangan(const GameTable* const gameStat, void *, int targetPlayer);
#endif
