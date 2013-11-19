#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "../../common/thread.h"
#include <lua.hpp>
#include "../discard.h"
#include "../gametbl.h"

class aiscript {
private:
	struct ScriptStates;
	static const char fncname_discard[8];
	static const char fncname_call[3][12];
	static ScriptStates status[Players];
	class table;
	class FileSelector;
	static void calcDiscard_threaded(DiscardTileNum& answer, const GameTable* gameStat);
	static void calcCall_threaded(GameTable* gameStat);
	static const DiscardTileNum DiscardThrough;
	static void readfile(aiscript::ScriptStates* const L, const char* const filename);
	static bool callFunc(const GameTable* const gameStat, PlayerID PlayerID, const char* const function_name, bool is_mandatory);
public:
	static void initscript();
	static void initephemeral();
	static void initcall(const GameTable* const gameStat, PlayerID player);
	static void closescript();
	static void GameStatToLuaTable(lua_State* const L, const GameTable* const gameStat);
	static DiscardTileNum compdahai(const GameTable* const gameStat);
	static DiscardTileNum determine_discard(const GameTable* const gameStat);
	static void compfuuro(GameTable* const gameStat);
	static void determine_meld(GameTable* const gameStat);
public: /* Monostate class: cannot be instantiated */
	aiscript() = delete;
	aiscript(const aiscript&) = delete;
	aiscript& operator= (const aiscript&) = delete;
	~aiscript() = delete;
};

struct aiscript::ScriptStates {
	lua_State* state;
	bool scriptLoaded;
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
	static inline void TableAdd(lua_State* const L, const char* const, const Tile val);
	static inline void TableAdd(lua_State* const L, const char* const, const MeldCode val);
public:
	class playertable;
	class functable;
public: /* Monostate class: cannot be instantiated */
	table() = delete;
	table(const table&) = delete;
	table& operator= (const table&) = delete;
	~table() = delete;
};
