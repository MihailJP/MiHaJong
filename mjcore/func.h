#ifndef FUNC_H
#define FUNC_H

#include <array>
#include <cstdint>
#include <memory>
#include <exception>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include "gametbl.h"

typedef std::array<uint8_t, TILE_CODE_MAXIMUM> tileCountTbl;

class TileCount { // éËîvÇ…Ç†ÇÈéÌóﬁï ÇÃîvÇÃêî
private:
	tileCountTbl count;
public:
	TileCount() {
		this->count.fill(0);
	}
	const uint8_t& operator[] (const int i) const {
		return count[i];
	}
	uint8_t& operator[] (const int i) {
		return count[i];
	}
};

// -------------------------------------------------------------------------

enum seatAbsolute { sEast, sSouth, sWest, sNorth };
enum seatRelative { sSelf, sRight, sOpposite, sLeft };
typedef uint8_t PLAYER_RANK;
typedef std::array<PLAYER_RANK, PLAYERS> PlayerRankList;

// -------------------------------------------------------------------------

TileCount countTilesInHand(GameTable* gameStat, PLAYER_ID playerID);

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

PlayerRankList calcRank(GameTable* gameStat);
__declspec(dllexport) void calcRank(int* Rank, GameTable* gameStat);

bool isPao(GameTable* gameStat, PLAYER_ID agariPlayer, PLAYER_ID paoPlayer);
__declspec(dllexport) int isPao(GameTable* gameStat, int agariPlayer, int paoPlayer);

bool isPaoAgari(GameTable* gameStat, PLAYER_ID agariPlayer);
__declspec(dllexport) int isPaoAgari(GameTable* gameStat, int agariPlayer);

bool isGotPao(GameTable* gameStat, PLAYER_ID paoPlayer);
__declspec(dllexport) int isGotPao(GameTable* gameStat, int paoPlayer);

PLAYER_ID getPaoPlayer(GameTable* gameStat, PLAYER_ID agariPlayer);
__declspec(dllexport) int getPaoPlayer(GameTable* gameStat, int agariPlayer);

__declspec(dllexport) int RonPlayers(GameTable* gameStat);

std::string inline windName(seatAbsolute wind);
__declspec(dllexport) void windName(char* str, int wind);

std::string inline roundName(int roundNum, GameTable* gameStat);
__declspec(dllexport) void roundName(char* str, int roundNum);

std::string inline TileName(tileCode tile);
__declspec(dllexport) void TileName(char* str, int tile);

__declspec(dllexport) inline int WindMax();
tileCode inline Wind2Tile(uint8_t wind);
__declspec(dllexport) int Wind2Tile(int wind);

#endif
