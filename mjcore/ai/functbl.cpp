#include "functbl.h"

const char* aiscript::table::functable::tblname = "mihajong"; // table name

void aiscript::table::functable::inittable(lua_State* const L) {
	lua_newtable(L); // table 'mihajong'
	discardTileCode(L); // subtable 'DiscardType'
	meldCallCode(L); // subtable 'Call'
	tileCode(L); // subtable 'Tile'
	lua_setglobal(L, tblname); // global table
}

/* �̔v�ԍ��̕t�я�� */
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

/* ���̎�ʃR�[�h */
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

/* �v�̔ԍ� */
inline void aiscript::table::functable::tileCode(lua_State* const L) {
	const char suitname[3][16] = {"Character","Circle","Bamboo",};
	const char numeral[9][8] = {"One","Two","Three","Four","Five","Six","Seven","Eight","Nine",};
	lua_newtable(L);
	for (int suit = 0; suit < TILE_SUIT_HONORS; suit += TILE_SUIT_STEP) { // ���v(���[�v)
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
	/* �����Ƀ��\�b�h������ */
	lua_pushcfunction(L, gametbl_getactiveplayer); lua_setfield(L, -2, "getactiveplayer");
	lua_pushcfunction(L, gametbl_getdeckleft); lua_setfield(L, -2, "getdeckleft");
	lua_pushcfunction(L, gametbl_getdiscard); lua_setfield(L, -2, "getdiscard");
	lua_pushcfunction(L, gametbl_getdoukasen); lua_setfield(L, -2, "getdoukasen");
	lua_pushcfunction(L, gametbl_gethand); lua_setfield(L, -2, "gethand");
	lua_pushcfunction(L, gametbl_getrule); lua_setfield(L, -2, "getrule");
	lua_pushcfunction(L, gametbl_getwareme); lua_setfield(L, -2, "getwareme");
	/* ���\�b�h��`�����܂� */
	lua_setfield(L, -2, "gametbl");
}

/* gameStat�̃A�h���X���擾�i�Öق̈����j */
GameTable* aiscript::table::functable::gametbl::getGameStatAddr(lua_State* const L) {
	lua_getfield(L, 1, "addr"); GameTable* addr = (GameTable*)lua_touserdata(L, -1); lua_pop(L, 1);
	return addr;
}

/* �v���C���[�ԍ����擾�i�Öق̈����j */
PLAYER_ID aiscript::table::functable::gametbl::getPlayerID(lua_State* const L, int index) {
	PLAYER_ID player; int n = lua_gettop(L);
	if ((n >= index)&&(!lua_isnil(L, index))) player = lua_tointeger(L, index);
	else {lua_getfield(L, 1, "playerid"); player = lua_tointeger(L, -1); lua_pop(L, 1);}
	return player;
}

/* �c���Ԃ̃v���C���[�ԍ� */
int aiscript::table::functable::gametbl::gametbl_getactiveplayer(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "����������������܂���"); lua_error(L);}
	lua_pushinteger(L, (int)getGameStatAddr(L)->CurrentPlayer.Active + 1);
	return 1;
}

/* �R�v�̎c�薇�� */
int aiscript::table::functable::gametbl::gametbl_getdeckleft(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "����������������܂���"); lua_error(L);}
	lua_pushinteger(L, tilesLeft(getGameStatAddr(L)));
	return 1;
}

/* �̔v�e�[�u�� */
int aiscript::table::functable::gametbl::gametbl_getdiscard(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "����������������܂���"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_newtable(L); // �߂�l���i�[����e�[�u��
	for (uint8_t i = 1; i <= gameStat->Player[player].DiscardPointer; i++) {
		lua_pushnumber(L, i);
		lua_newtable(L);
		TableAdd(L, "tile", (int)gameStat->Player[player].Discard[i].tcode.tile);
		TableAdd(L, "red", (int)gameStat->Player[player].Discard[i].tcode.red);
		TableAdd(L, "through", gameStat->Player[player].Discard[i].isDiscardThrough);
		TableAdd(L, "riichi", (gameStat->Player[player].Discard[i].dstat == discardRiichi) ||
			(gameStat->Player[player].Discard[i].dstat == discardRiichiTaken));
		TableAdd(L, "taken", (gameStat->Player[player].Discard[i].dstat == discardTaken) ||
			(gameStat->Player[player].Discard[i].dstat == discardRiichiTaken));
		lua_settable(L, -3);
	}
	return 1;
}

/* ���ΐ� */
int aiscript::table::functable::gametbl::gametbl_getdoukasen(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "����������������܂���"); lua_error(L);}
	if (getGameStatAddr(L)->DoukasenPlayer == -1)
		lua_pushnil(L); // ���ΐ��Ȃ��̎���nil
	else lua_pushinteger(L, (int)getGameStatAddr(L)->DoukasenPlayer + 1);
	return 1;
}

/* ����v�e�[�u�� */
int aiscript::table::functable::gametbl::gametbl_gethand(lua_State* const L) {
	int n = lua_gettop(L);
	if ((n < 1)||(n > 2)) {lua_pushstring(L, "����������������܂���"); lua_error(L);}
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_newtable(L); // �߂�l���i�[����e�[�u��
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[player].Hand[i].tile != NoTile) {
			/* �v����������T�v�e�[�u���ɕϊ��A�Ȃ�������nil�̂܂ܕ��u */
			lua_pushnumber(L, i + 1);
			lua_newtable(L);
			TableAdd(L, "tile", (int)gameStat->Player[player].Hand[i].tile);
			TableAdd(L, "red", (int)gameStat->Player[player].Hand[i].red);
			lua_settable(L, -3);
		}
	}
	return 1;
}

/* ���I�ʎq */
/*inline void aiscript::table::playertable::pltable::PlayerMeld(lua_State* const L, const MELD_BUF* const plMeld, uint8_t MeldPointer) {
	lua_newtable(L);
	for (uint8_t i = 1; i <= MeldPointer; i++) {
		std::ostringstream o; o << (int)i;
		TableAdd(L, o.str().c_str(), plMeld[i]);
	}
	lua_setfield(L, -2, "Meld");
}*/

/* ���[���ԍ��擾 */
int aiscript::table::functable::gametbl::gametbl_getrule(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 2) {lua_pushstring(L, "����������������܂���"); lua_error(L);}
	const char* fieldname = lua_tostring(L, 2);
	lua_pushinteger(L, (int)RuleData::getRule(fieldname));
	return 1;
}

/* ����� */
int aiscript::table::functable::gametbl::gametbl_getwareme(lua_State* const L) {
	int n = lua_gettop(L);
	if (n != 1) {lua_pushstring(L, "����������������܂���"); lua_error(L);}
	if (getGameStatAddr(L)->WaremePlayer == -1)
		lua_pushnil(L); // ����ڂȂ��̎���nil
	else lua_pushinteger(L, (int)getGameStatAddr(L)->WaremePlayer + 1);
	return 1;
}
