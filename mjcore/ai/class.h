#ifndef AISCRIPT_CLASS_H
#define AISCRIPT_CLASS_H

#include <lua.hpp>
#include "../discard.h"

class aiscript {
private:
	static const char* const fncname_discard;
	static const char* const fncname_call;
	static lua_State* lsMJCore;
	static bool scriptLoaded;
	class table;
	static const DiscardTileNum DiscardThrough;
	static void readfile(lua_State* const L, const char* const filename);
public:
	__declspec(dllexport) static void initscript();
	static void GameStatToLuaTable(lua_State* const L, const GameTable* const gameStat);
	__declspec(dllexport) static int compdahai(const GameTable* const gameStat);
	static DiscardTileNum determine_discard(const GameTable* const gameStat);
};

class aiscript::table {
private:
	static inline void TableAdd(lua_State* const L, const char* const key, lua_Integer val) {
		lua_pushnumber(L, val); lua_setfield(L, -2, key);
	}
	static inline void TableAdd(lua_State* const L, const char* const key, lua_Number val) {
		lua_pushnumber(L, val); lua_setfield(L, -2, key);
	}
	static inline void TableAdd(lua_State* const L, const char* const key, bool val) {
		lua_pushboolean(L, val); lua_setfield(L, -2, key);
	}
	static inline void TableAdd(lua_State* const L, lua_Integer key, lua_Integer val) {
		lua_pushnumber(L, key); lua_pushnumber(L, val); lua_settable(L, -3);
	}
	static inline void TableAdd(lua_State* const L, lua_Integer key, lua_Number val) {
		lua_pushnumber(L, key); lua_pushnumber(L, val); lua_settable(L, -3);
	}
	static inline void TableAdd(lua_State* const L, lua_Integer key, bool val) {
		lua_pushnumber(L, key); lua_pushboolean(L, val); lua_settable(L, -3);
	}
	static inline void TableAdd(lua_State* const L, const char* const, const TILE val);
	static inline void TableAdd(lua_State* const L, const char* const, const meldCode val);
public:
	class playertable;
	class functable;
};

#endif
