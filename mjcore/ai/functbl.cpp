#include "functbl.h"

const char* aiscript::table::functable::tblname = "mihajong"; // table name

void aiscript::table::functable::inittable(lua_State* const L) {
	lua_newtable(L); // table 'mihajong'
	discardTileCode(L); // subtable 'DiscardType'
	meldCallCode(L); // subtable 'Call'
	lua_setglobal(L, tblname); // global table
}
inline void aiscript::table::functable::discardTileCode(lua_State* const L) {
	lua_newtable(L);
	TableAdd(L, "Normal", (int)DiscardTileNum::Normal);
	TableAdd(L, "Ankan", (int)DiscardTileNum::Ankan);
	TableAdd(L, "Kakan", (int)DiscardTileNum::Kakan);
	TableAdd(L, "Riichi", (int)DiscardTileNum::Riichi);
	TableAdd(L, "Flower", (int)DiscardTileNum::Flower);
	TableAdd(L, "OpenRiichi", (int)DiscardTileNum::OpenRiichi);
	TableAdd(L, "Agari", (int)DiscardTileNum::Agari);
	TableAdd(L, "Kyuushu", (int)DiscardTileNum::Kyuushu);
	lua_setfield(L, -2, "DiscardType");
}
inline void aiscript::table::functable::meldCallCode(lua_State* const L) {
	lua_newtable(L);
	TableAdd(L, "None", meldNone);
	TableAdd(L, "Ron", meldRon);
	TableAdd(L, "Kan", meldKan);
	TableAdd(L, "Pon", meldPon);
	lua_newtable(L);
	TableAdd(L, "Lower", meldChiiLower);
	TableAdd(L, "Middle", meldChiiMiddle);
	TableAdd(L, "Upper", meldChiiUpper);
	lua_setfield(L, -2, "Chii");
	lua_setfield(L, -2, "DiscardType");
}
