#ifndef FUNCTBL_H
#define FUNCTBL_H

#include <lua.hpp>
#include "class.h"
#include "../func.h"

enum MeldCallID {
	meldNone, meldRon, meldKan, meldPon, meldChiiLower, meldChiiMiddle, meldChiiUpper
};

class aiscript::table::functable {
private:
	static const char* tblname;
	static inline void discardTileCode(lua_State* const L);
	static inline void meldCallCode(lua_State* const L);
	static inline void tileCode(lua_State* const L);
	class gametbl;
public:
	static void inittable(lua_State* const L);
};

class aiscript::table::functable::gametbl {
private:
	static GameTable* getGameStatAddr(lua_State* const L);
	static PLAYER_ID getPlayerID(lua_State* const L, int index);
	static int gametbl_getactiveplayer(lua_State* const L);
	static int gametbl_getdeckleft(lua_State* const L);
	static int gametbl_getdiscard(lua_State* const L);
	static int gametbl_getdoukasen(lua_State* const L);
	static int gametbl_gethand(lua_State* const L);
	static int gametbl_getrule(lua_State* const L);
	static int gametbl_getwareme(lua_State* const L);
public:
	static inline void makeprototype(lua_State* const L);
};

#endif
