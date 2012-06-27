#ifndef FUNC_H
#define FUNC_H

#include <array>
#include <cstdint>
#include <memory>
#include "gametbl.h"

typedef std::array<uint8_t, TILE_CODE_MAXIMUM> tileCountTbl;

class TileCount { // éËîvÇ…Ç†ÇÈéÌóﬁï ÇÃîvÇÃêî
public:
	tileCountTbl count;
	TileCount() {
		this->count.fill(0);
	}
};

// -------------------------------------------------------------------------

enum seatAbsolute { sEast, sSouth, sWest, sNorth };
enum seatRelative { sSelf, sRight, sOpposite, sLeft };
typedef uint8_t PLAYER_RANK;
typedef std::array<PLAYER_RANK, PLAYERS> PlayerRankList;

// -------------------------------------------------------------------------

TileCount countTilesInHand(GameTable gameStat, PLAYER_ID playerID);

/* -------------------------------------------------------------------------
 *  function.hsp Ç©ÇÁà⁄êA
 * -------------------------------------------------------------------------
 */

seatAbsolute inline playerwind(GameTable* gameStat, PLAYER_ID player, PLAYER_ID currentRound);
__declspec(dllexport) inline int playerwind(int player, int currentRound);

seatRelative inline playerRelative(PLAYER_ID targetPlayer, PLAYER_ID basePlayer);
__declspec(dllexport) inline int playerRelative(int targetPlayer, int basePlayer);

PLAYER_ID inline RelativePositionOf(PLAYER_ID targetPlayer, seatRelative relative);
__declspec(dllexport) inline int RelativePositionOf(int targetPlayer, int relative);

__declspec(dllexport) inline int roundLoopRate();

__declspec(dllexport) inline int tilesLeft(GameTable* gameStat);

PlayerRankList calcRanc(GameTable* gameStat);
__declspec(dllexport) void calcRanc(int* Rank, GameTable* gameStat);

bool isPao(GameTable* gameStat, PLAYER_ID agariPlayer, PLAYER_ID paoPlayer);
__declspec(dllexport) int isPao(GameTable* gameStat, int agariPlayer, int paoPlayer);

bool isPaoAgari(GameTable* gameStat, PLAYER_ID agariPlayer);
__declspec(dllexport) int isPaoAgari(GameTable* gameStat, int agariPlayer);

bool isGotPao(GameTable* gameStat, PLAYER_ID paoPlayer);
__declspec(dllexport) int isGotPao(GameTable* gameStat, int paoPlayer);

PLAYER_ID getPaoPlayer(GameTable* gameStat, PLAYER_ID agariPlayer);
__declspec(dllexport) int getPaoPlayer(GameTable* gameStat, int agariPlayer);

__declspec(dllexport) int RonPlayers(GameTable* gameStat);

#endif
