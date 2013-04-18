#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
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
	class detDiscardThread;
	class detCallThread;
	static const DiscardTileNum DiscardThrough;
	static void readfile(aiscript::ScriptStates* const L, const char* const filename);
	static DiscardTileNum discard; static bool finished; static detDiscardThread* discard_worker;
	static detCallThread* meld_worker;
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
};

class aiscript::detDiscardThread {
public:
	detDiscardThread();
	~detDiscardThread();
	void setprm(const GameTable* const gameStat, DiscardTileNum* const discard, bool* const finished);
#ifdef _WIN32
	static DWORD WINAPI execute(LPVOID param);
#else /*_WIN32*/
	/* TODO: ñ¢é¿ëïâ”èä */
#endif /*_WIN32*/
private:
	const GameTable* i_gameStat;
	DiscardTileNum* i_discard;
	bool* i_finished;
#ifdef _WIN32
	static DWORD WINAPI calculate(const GameTable* const gameStat, DiscardTileNum* const discard, bool* const finished);
#else /*_WIN32*/
	/* TODO: ñ¢é¿ëïâ”èä */
#endif /*_WIN32*/
};

class aiscript::detCallThread {
public:
	detCallThread();
	~detCallThread();
	void setprm(GameTable* const gameStat, bool* const finished);
#ifdef _WIN32
	static DWORD WINAPI execute(LPVOID param);
#else /*_WIN32*/
	/* TODO: ñ¢é¿ëïâ”èä */
#endif /*_WIN32*/
private:
	GameTable* i_gameStat;
	bool* i_finished;
#ifdef _WIN32
	static DWORD WINAPI calculate(GameTable* const gameStat, bool* const finished);
#else /*_WIN32*/
	/* TODO: ñ¢é¿ëïâ”èä */
#endif /*_WIN32*/
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
};
