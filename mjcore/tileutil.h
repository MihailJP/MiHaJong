#pragma once

#ifdef MJCORE_EXPORTS
#include <type_traits>
#endif
#include <cstdint>
#include <functional>
#include "mjexport.h"
#include "gametbl.h"
#include "../common/machihai.h"

// -------------------------------------------------------------------------

using mihajong_structs::InfoByTile;
using mihajong_structs::PlayerID;
using mihajong_structs::TileCode;
using mihajong_structs::Int8ByTile;

using mihajong_structs::MachihaiTileInfo;
using mihajong_structs::MachihaiInfo;
using mihajong_structs::TileStatus;

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
PlayerID* tobePlayed(const GameTable* const gameStat);
PlayerID tobePlayed(const GameTable* const gameStat, int id);

constexpr std::array<TileCode, 13> Honor_Major_Tiles = {
	CharacterOne, CharacterNine, CircleOne, CircleNine, BambooOne, BambooNine,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
};
bool isYaojiu(TileCode code);

void lipai(GameTable* const gameStat, PlayerID targetPlayer);
namespace MoveTile {
void moveTile(GameTable* const gameStat, PlayerID targetPlayer, bool execute, int tileIndex);
void enqueue(PlayerID targetPlayer, int from, int to);
std::pair<int, int> dequeue(PlayerID targetPlayer);
void for_each(const std::function<void(std::pair<int, int>)> & f, PlayerID targetPlayer, bool preserve);
void apply(GameTable* const gameStat, PlayerID targetPlayer, bool preserve = false);
}
#endif

MJCORE Int8ByTile countseentiles(const GameTable* const gameStat);

MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PlayerID playerID);

MJCORE Int8ByTile countRedTilesInHand(const GameTable* const gameStat, PlayerID playerID, int doraCol);

MJCORE TileStatus gettilestatus(
	const GameTable* const gameStat, PlayerID targetPlayer, int targetTile, bool CheckMode);

MJCORE MachihaiInfo chkFuriten(const GameTable* const gameStat, PlayerID targetPlayer);

#ifdef MJCORE_EXPORTS
void chkOpenMachi(GameTable* const gameStat, PlayerID targetPlayer);
#endif

MJCORE bool chkdaopaiability(const GameTable* const gameStat, PlayerID targetPlayer);

#ifdef MJCORE_EXPORTS
void setdora(GameTable* const gameStat, int Mode);
#endif

MJCORE bool chkAnkanAbility(const GameTable* const gameStat, PlayerID targetPlayer);

#ifdef MJCORE_EXPORTS
void calcdoukasen(GameTable* const gameStat);

bool isTenpai(const GameTable* const gameStat, PlayerID targetPlayer);

bool isNagashiMangan(const GameTable* const gameStat, PlayerID targetPlayer);

bool isKataagari(const GameTable* gameStat, PlayerID targetPlayer);
#endif
