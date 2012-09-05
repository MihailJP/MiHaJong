#ifndef FUNCTBL_H
#define FUNCTBL_H

#include <lua.hpp>
#include "class.h"
#include "../func.h"
#include "../tileutil.h"
#include "../yaku/yaku.h"
#include "../shanten.h"

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
	static inline void meldTypeCode(lua_State* const L);
	static inline void tileCode(lua_State* const L);
	class gametbl;
public:
	static inline const char* getGTblName();
	static inline const char* getTblName() {return tblname;}
	static void inittable(lua_State* const L, int playerID);
};

class aiscript::table::functable::gametbl {
private:
	static const char* tblname;
	static const std::array<::tileCode, 35> validTiles;
	static GameTable* getGameStatAddr(lua_State* const L);
	static PLAYER_ID getPlayerID(lua_State* const L, int index);
	static void pushTileTable(lua_State* const L, Int8ByTile& tptr);
	static void pushTileTable(lua_State* const L, UInt8ByTile& tptr);
	static void pushTileTable(lua_State* const L, FlagByTile& tptr);
	static void pushTileTable(lua_State* const L, InfoByTile<MachihaiTileInfo>& tptr);
	static void setHand(lua_State* const L, GameTable* const tmpGameStat, int index);
	static int chkargnum(lua_State* const L, int argmin, int argmax);
	class luafunc;
public:
	static inline const char* getTblName() {return tblname;}
	static inline void makeprototype(lua_State* const L, int playerID);
};

inline const char* aiscript::table::functable::getGTblName() {return gametbl::getTblName();}

class aiscript::table::functable::gametbl::luafunc {
private:
	class riskchk;
public:
	static int evaluate(lua_State* const L);
	static int getactiveplayer(lua_State* const L);
	static int getbakaze(lua_State* const L);
	static int getbasepoint(lua_State* const L);
	static int getchip(lua_State* const L);
	static int getdeckleft(lua_State* const L);
	static int getdeposit(lua_State* const L);
	static int getdiscard(lua_State* const L);
	static int getdorainfo(lua_State* const L);
	static int getdoukasen(lua_State* const L);
	static int getflower(lua_State* const L);
	static int gethand(lua_State* const L);
	static int getjikaze(lua_State* const L);
	static int getmeld(lua_State* const L);
	static int getopenwait(lua_State* const L);
	static int getpreviousdiscard(lua_State* const L);
	static int getrank(lua_State* const L);
	static int getround(lua_State* const L);
	static int getrule(lua_State* const L);
	static int getscore(lua_State* const L);
	static int getseentiles(lua_State* const L);
	static int getshanten(lua_State* const L);
	static int gettenpaistat(lua_State* const L);
	static int gettilecontext(lua_State* const L);
	static int gettilerisk(lua_State* const L);
	static int gettilesinhand(lua_State* const L);
	static int gettsumibou(lua_State* const L);
	static int getwareme(lua_State* const L);
	static int getyakuhaiwind(lua_State* const L);
	static int isabovebase(lua_State* const L);
	static int isankanallowed(lua_State* const L);
	static int isfinalround(lua_State* const L);
	static int isfirstdraw(lua_State* const L);
	static int isippatsu(lua_State* const L);
	static int iskyuushu(lua_State* const L);
	static int ismenzen(lua_State* const L);
	static int isopenriichideclared(lua_State* const L);
	static int ispenultimateround(lua_State* const L);
	static int isriichideclared(lua_State* const L);
	static int isshisanbuda(lua_State* const L);
	static int isshisibuda(lua_State* const L);
	static int issumaroallowed(lua_State* const L);
	static int isyakitori(lua_State* const L);
};

class aiscript::table::functable::gametbl::luafunc::riskchk {
public:
	static bool issameasprevious(const GameTable* const gameStat, PLAYER_ID player, int index);
	static bool isdora(const GameTable* const gameStat, PLAYER_ID player, int index);
	static bool isdorasuji(const GameTable* const gameStat, PLAYER_ID player, int index);
	static bool isdorasoba(const GameTable* const gameStat, PLAYER_ID player, int index);
	static bool isnochance(const GameTable* const gameStat, PLAYER_ID player, int index);
	static bool isonechance(const GameTable* const gameStat, PLAYER_ID player, int index);
	static bool isneverdiscarded(const GameTable* const gameStat, PLAYER_ID player, int index);
	static bool isseenfour(const GameTable* const gameStat, PLAYER_ID player, int index);
};

#endif
