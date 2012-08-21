#ifndef FUNCTBL_H
#define FUNCTBL_H

#include <lua.hpp>
#include "class.h"
#include "../func.h"
#include "../tileutil.h"
#include "../yaku/yaku.h"

enum MeldCallID {
	meldNone, meldRon, meldKan, meldPon, meldChiiLower, meldChiiMiddle, meldChiiUpper
};

class aiscript::table::functable {
private:
	static const char* tblname;
	static int tableLockedErr(lua_State* const L);
	static void lockTable(lua_State* const L);
	static inline void discardTileCode(lua_State* const L);
	static inline void meldCallCode(lua_State* const L);
	static inline void tileCode(lua_State* const L);
	class gametbl;
public:
	static void inittable(lua_State* const L);
};

class aiscript::table::functable::gametbl {
private:
	static const std::array<::tileCode, 35> validTiles;
	static GameTable* getGameStatAddr(lua_State* const L);
	static PLAYER_ID getPlayerID(lua_State* const L, int index);
	static void pushTileTable(lua_State* const L, Int8ByTile& tptr);
	static void pushTileTable(lua_State* const L, UInt8ByTile& tptr);
	static void pushTileTable(lua_State* const L, FlagByTile& tptr);
	static int gametbl_getactiveplayer(lua_State* const L);
	static int gametbl_getchip(lua_State* const L);
	static int gametbl_getdeckleft(lua_State* const L);
	static int gametbl_getdeposit(lua_State* const L);
	static int gametbl_getdiscard(lua_State* const L);
	static int gametbl_getdorainfo(lua_State* const L);
	static int gametbl_getdoukasen(lua_State* const L);
	static int gametbl_gethand(lua_State* const L);
	static int gametbl_getopenwait(lua_State* const L);
	static int gametbl_getpreviousdiscard(lua_State* const L);
	static int gametbl_getrank(lua_State* const L);
	static int gametbl_getround(lua_State* const L);
	static int gametbl_getrule(lua_State* const L);
	static int gametbl_getscore(lua_State* const L);
	static int gametbl_getwareme(lua_State* const L);
	static int gametbl_gettsumibou(lua_State* const L);
	static int gametbl_gettilecontext(lua_State* const L);
	static int gametbl_isfinalround(lua_State* const L);
	static int gametbl_isfirstdraw(lua_State* const L);
	static int gametbl_isippatsu(lua_State* const L);
	static int gametbl_iskyuushu(lua_State* const L);
	static int gametbl_ismenzen(lua_State* const L);
	static int gametbl_isopenriichideclared(lua_State* const L);
	static int gametbl_ispenultimateround(lua_State* const L);
	static int gametbl_isriichideclared(lua_State* const L);
	static int gametbl_isshisanbuda(lua_State* const L);
	static int gametbl_isshisibuda(lua_State* const L);
	static int gametbl_issumaroallowed(lua_State* const L);
	static int gametbl_isyakitori(lua_State* const L);
public:
	static inline void makeprototype(lua_State* const L);
};

#endif
