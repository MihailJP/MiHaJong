#include "functbl.h"

const char* aiscript::table::functable::tblname = "mihajong"; // table name

void aiscript::table::functable::inittable(lua_State* const L) {
	lua_newtable(L); // table 'mihajong'
	discardTileCode(L); // subtable 'DiscardType'
	meldCallCode(L); // subtable 'Call'
	tileCode(L); // subtable 'Tile'
	lua_setglobal(L, tblname); // global table
}

/* 捨牌番号の付帯情報 */
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

/* 鳴きの種別コード */
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

/* 牌の番号 */
inline void aiscript::table::functable::tileCode(lua_State* const L) {
	const char suitname[3][16] = {"Character","Circle","Bamboo",};
	const char numeral[9][8] = {"One","Two","Three","Four","Five","Six","Seven","Eight","Nine",};
	lua_newtable(L);
	for (int suit = 0; suit < TILE_SUIT_HONORS; suit += TILE_SUIT_STEP) { // 数牌(ループ)
		lua_newtable(L);
		for (int num = 1; num <= 9; num++) {
			TableAdd(L, numeral[num - 1], suit + num);
			TableAdd(L, num, suit + num);
		}
		lua_setfield(L, -2, suitname[suit / TILE_SUIT_HONORS]);
	}
	lua_newtable(L);
	TableAdd(L, "East", (int)EastWind); TableAdd(L, "South", (int)SouthWind);
	TableAdd(L, "West", (int)WestWind); TableAdd(L, "North", (int)NorthWind);
	lua_setfield(L, -2, "Wind");
	lua_newtable(L);
	TableAdd(L, "White", (int)WhiteDragon); TableAdd(L, "Green", (int)GreenDragon);
	TableAdd(L, "Red", (int)RedDragon);
	lua_setfield(L, -2, "Dragon");
	lua_newtable(L);
	TableAdd(L, "Spring", (int)Spring); TableAdd(L, "Summer", (int)Summer);
	TableAdd(L, "Autumn", (int)Autumn); TableAdd(L, "Winter", (int)Winter);
	TableAdd(L, "Plum", (int)Plum); TableAdd(L, "Orchid", (int)Orchid);
	TableAdd(L, "Chrysanthemum", (int)Chrysanthemum); TableAdd(L, "Bamboo", (int)Bamboo);
	TableAdd(L, "Fall", (int)Autumn); TableAdd(L, "Chrys", (int)Chrysanthemum);
	lua_setfield(L, -2, "Flower");
	lua_setfield(L, -2, "Tile");
}

inline void aiscript::table::functable::gametbl::makeprototype(lua_State* const L) {
	lua_newtable(L);
	lua_pushlightuserdata(L, NULL); lua_setfield(L, -2, "addr"); // pointer to C++ struct
	/* ここにメソッドを書く */
	lua_pushcfunction(L, gametbl_getactiveplayer); lua_setfield(L, -2, "getactiveplayer");
	lua_pushcfunction(L, gametbl_getdeckleft); lua_setfield(L, -2, "getdeckleft");
	lua_pushcfunction(L, gametbl_getdoukasen); lua_setfield(L, -2, "getdoukasen");
	lua_pushcfunction(L, gametbl_getrule); lua_setfield(L, -2, "getrule");
	lua_pushcfunction(L, gametbl_getwareme); lua_setfield(L, -2, "getwareme");
	/* メソッド定義ここまで */
	lua_setfield(L, -2, "gametbl");
}

/* gameStatのアドレスを取得（暗黙の引数） */
GameTable* aiscript::table::functable::gametbl::getGameStatAddr(lua_State* const L) {
	lua_getfield(L, 1, "addr"); GameTable* addr = (GameTable*)lua_touserdata(L, -1); lua_pop(L, 1);
	return addr;
}

/* ツモ番のプレイヤー番号 */
int aiscript::table::functable::gametbl::gametbl_getactiveplayer(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	lua_pushinteger(L, (int)getGameStatAddr(L)->CurrentPlayer.Active + 1);
	return 1;
}

/* 山牌の残り枚数 */
int aiscript::table::functable::gametbl::gametbl_getdeckleft(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	lua_pushinteger(L, tilesLeft(getGameStatAddr(L)));
	return 1;
}

/* 導火線 */
int aiscript::table::functable::gametbl::gametbl_getdoukasen(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	if (getGameStatAddr(L)->DoukasenPlayer == -1)
		lua_pushnil(L); // 導火線なしの時はnil
	else lua_pushinteger(L, (int)getGameStatAddr(L)->DoukasenPlayer + 1);
	return 1;
}

/* ルール番号取得 */
int aiscript::table::functable::gametbl::gametbl_getrule(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 2) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	const char* fieldname = lua_tostring(L, 2);
	lua_pushinteger(L, (int)RuleData::getRule(fieldname));
	return 1;
}

/* 割れ目 */
int aiscript::table::functable::gametbl::gametbl_getwareme(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "引数が正しくありません"); lua_error(L);}
	if (getGameStatAddr(L)->WaremePlayer == -1)
		lua_pushnil(L); // 割れ目なしの時はnil
	else lua_pushinteger(L, (int)getGameStatAddr(L)->WaremePlayer + 1);
	return 1;
}
