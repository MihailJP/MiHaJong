#ifndef AISCRIPT_H
#define AISCRIPT_H

#include <lua.hpp>
#include <sstream>
#include "gametbl.h"
#include "logging.h"
#include "discard.h"

class aiscript {
private:
	static const char* const fncname_discard;
	static const char* const fncname_call;
	static lua_State* lsMJCore;
	static bool scriptLoaded;
	class table;
	static const DiscardTileNum DiscardThrough;
	static void inittbl(lua_State* const L);
	static void readfile(lua_State* const L, const char* const filename);
public:
	__declspec(dllexport) static void initscript();
	static void GameStatToLuaTable(lua_State *L, const GameTable* const gameStat);
	__declspec(dllexport) static int compdahai(const GameTable* const gameStat);
	static DiscardTileNum determine_discard(const GameTable* const gameStat);
};

class aiscript::table {
private:
	static inline void TableAdd(lua_State *L, std::string key, lua_Integer val);
	static inline void TableAdd(lua_State *L, std::string key, lua_Number val);
	static inline void TableAdd(lua_State *L, std::string key, bool val);
	static inline void TableAdd(lua_State *L, std::string key, const TILE val);
	static inline void TableAdd(lua_State *L, std::string key, const meldCode val);
	class pltable;
public:
	static inline void PlayerTbl(lua_State *L, const PlayerTable* const plStat, PLAYER_ID player);
};

class aiscript::table::pltable {
public:
	static inline void PlayerHand(lua_State *L, const HAND_TILES* const plHand);
	static inline void PlayerDiscard(lua_State *L, const DISCARD_BUF* const plDiscard, uint8_t DiscardPointer);
	static inline void PlayerMeld(lua_State *L, const MELD_BUF* const plMeld, uint8_t MeldPointer);
	static inline void PlayerRichiStat(lua_State *L, const RichiStat* const plstat);
	static inline void PlayerFlower(lua_State *L, const FLOWERS* const plstat);
	static inline void PlayerDeclFlag(lua_State *L, const DECLFLAG* const plstat);
};
#endif
