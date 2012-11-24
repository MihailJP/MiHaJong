#ifndef AISCRIPT_CLASS_H
#define AISCRIPT_CLASS_H

#include <Windows.h>
#include <lua.hpp>
#include "../discard.h"
#include "../gametbl.h"

class aiscript {
private:
	struct ScriptStates;
	static const char fncname_discard[8];
	static const char fncname_call[3][12];
	static ScriptStates status[PLAYERS];
	class table;
	class FileSelector;
	class detDiscardThread;
	class detCallThread;
	static const DiscardTileNum DiscardThrough;
	static void readfile(aiscript::ScriptStates* const L, const char* const filename);
	static DiscardTileNum discard; static bool finished; static detDiscardThread* discard_worker;
	static detCallThread* meld_worker;
	static bool callFunc(const GameTable* const gameStat, PLAYER_ID player_id, const char* const function_name, bool is_mandatory);
public:
	__declspec(dllexport) static void initscript();
	__declspec(dllexport) static void initephemeral();
	static void initcall(const GameTable* const gameStat, PLAYER_ID player);
	__declspec(dllexport) static void initcall(const GameTable* const gameStat, int player);
	__declspec(dllexport) static void closescript();
	static void GameStatToLuaTable(lua_State* const L, const GameTable* const gameStat);
	__declspec(dllexport) static void compdahai_begin(const GameTable* const gameStat);
	__declspec(dllexport) static int compdahai_check();
	__declspec(dllexport) static int compdahai();
	static DiscardTileNum determine_discard(const GameTable* const gameStat);
	static void compfuuro(GameTable* const gameStat);
	static void determine_meld(GameTable* const gameStat);
};

class aiscript::detDiscardThread {
public:
	detDiscardThread();
	~detDiscardThread();
	void setprm(const GameTable* const gameStat, DiscardTileNum* const discard, bool* const finished);
	static DWORD WINAPI execute(LPVOID param);
private:
	const GameTable* i_gameStat;
	DiscardTileNum* i_discard;
	bool* i_finished;
	static DWORD WINAPI calculate(const GameTable* const gameStat, DiscardTileNum* const discard, bool* const finished);
};

class aiscript::detCallThread {
public:
	detCallThread();
	~detCallThread();
	void setprm(GameTable* const gameStat, bool* const finished);
	static DWORD WINAPI execute(LPVOID param);
private:
	GameTable* i_gameStat;
	bool* i_finished;
	static DWORD WINAPI calculate(GameTable* const gameStat, bool* const finished);
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
	static inline void TableAdd(lua_State* const L, const char* const, const TILE val);
	static inline void TableAdd(lua_State* const L, const char* const, const meldCode val);
public:
	class playertable;
	class functable;
};

#endif
