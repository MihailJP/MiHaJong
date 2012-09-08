#ifndef FUNC_H
#define FUNC_H

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <memory>
#include <cstring>
#include <sstream>
#include <Windows.h>
#include <Shlwapi.h>
#include <direct.h>
#endif
#include "mjexport.h"
#include "gametbl.h"
#include "except.h"

// -------------------------------------------------------------------------

enum seatAbsolute : uint8_t { sEast, sSouth, sWest, sNorth };
enum seatRelative : uint8_t { sSelf, sRight, sOpposite, sLeft };
typedef uint8_t PLAYER_RANK;
EXPORT_TEMPLATE_STRUCT InfoByPlayer<PLAYER_RANK>;
typedef InfoByPlayer<PLAYER_RANK> PlayerRankList;
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<PlayerRankList>::value, "PlayerRankList is not POD");
#endif

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
extern "C" inline uint8_t diceSum(const GameTable* const gameStat);

seatAbsolute inline playerwind(const GameTable* const gameStat, PLAYER_ID player, int currentRound);
__declspec(dllexport) inline int playerwind(int player, int currentRound);

seatRelative inline playerRelative(PLAYER_ID targetPlayer, PLAYER_ID basePlayer);
__declspec(dllexport) inline int playerRelative(int targetPlayer, int basePlayer);

PLAYER_ID inline RelativePositionOf(PLAYER_ID targetPlayer, seatRelative relative);
__declspec(dllexport) inline int RelativePositionOf(int targetPlayer, int relative);

__declspec(dllexport) inline int roundLoopRate();

__declspec(dllexport) inline int tilesLeft(const GameTable* const gameStat);

PlayerRankList calcRank(const GameTable* const gameStat);
__declspec(dllexport) void calcRank(int* Rank, const GameTable* const gameStat);

bool isPao(const GameTable* const gameStat, PLAYER_ID agariPlayer, PLAYER_ID paoPlayer);
__declspec(dllexport) int isPao(const GameTable* const gameStat, int agariPlayer, int paoPlayer);

bool isPaoAgari(const GameTable* const gameStat, PLAYER_ID agariPlayer);
__declspec(dllexport) int isPaoAgari(const GameTable* const gameStat, int agariPlayer);

bool isGotPao(const GameTable* const gameStat, PLAYER_ID paoPlayer);
__declspec(dllexport) int isGotPao(const GameTable* const gameStat, int paoPlayer);

PLAYER_ID getPaoPlayer(const GameTable* const gameStat, PLAYER_ID agariPlayer);
__declspec(dllexport) int getPaoPlayer(const GameTable* const gameStat, int agariPlayer);

__declspec(dllexport) int RonPlayers(const GameTable* const gameStat);

std::string inline windName(seatAbsolute wind);
__declspec(dllexport) void windName(char* str, int bufsz, int wind);

std::string inline roundName(int roundNum, const GameTable* const gameStat);
__declspec(dllexport) void roundName(char* str, int bufsz, int roundNum);

std::string inline TileName(tileCode tile);
__declspec(dllexport) void TileName(char* str, int bufsz, int tile);

__declspec(dllexport) inline int WindMax();
tileCode inline Wind2Tile(uint8_t wind);
__declspec(dllexport) int Wind2Tile(int wind);

int BasePoint();
__declspec(dllexport) int BasePointHSP();

bool isAboveBase(const GameTable* const gameStat, PLAYER_ID player);
__declspec(dllexport) int isAboveBase(const GameTable* const gameStat, int player);

std::string intstr(int val);

namespace confpath {
	bool isVista();

	std::string confPath();
	__declspec(dllexport) void confPath(char* path, int bufsz);
}

#endif

#endif
