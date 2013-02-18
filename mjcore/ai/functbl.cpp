#include "functbl.h"

#include <lua.hpp>
#include "../func.h"
#include "../yaku/yaku.h"
#include "../shanten.h"
#include "../random.h"
#include "../../common/version.h"
#include "../logging.h"
#include "../chat.h"

const char* aiscript::table::functable::tblname = "mihajong"; // table name
const char* aiscript::table::functable::gametbl::tblname = "gametbl"; // table name

void aiscript::table::functable::inittable(lua_State* const L, int playerID) {
	lua_newtable(L); // table 'mihajong'
	discardTileCode(L); // subtable 'DiscardType'
	meldCallCode(L); // subtable 'Call'
	meldTypeCode(L); // subtable 'MeldType'
	TileCode(L); // subtable 'Tile'
	agariTypeCode(L); // subtable 'AgariType'
	doraColorCode(L); // subtable 'DoraColor'
	gametbl::makeprototype(L, playerID); // subtable 'gametbl' (prototype)
	lua_pushcfunction(L, random); lua_setfield(L, -2, "random"); // function 'random'
	gametype(L); // string 'gametype'
	version(L); // subtable 'version'
	lua_pushcfunction(L, log); lua_setfield(L, -2, "log"); // function 'log'
	lua_pushcfunction(L, say); lua_setfield(L, -2, "say"); // function 'say'
	lockTable(L); // mark as read-only
	lua_setglobal(L, tblname); // global table
}

int aiscript::table::functable::tableLockedErr(lua_State* const L) { // ロックされたテーブルを書き換えようとした時
	return luaL_error(L, "Attempt to modify a read-only table");
}

void aiscript::table::functable::lockTable(lua_State* const L) { // テーブルをロックする
	lua_newtable(L); // メタテーブルの用意
	lua_pushcfunction(L, tableLockedErr); lua_setfield(L, -2, "__newindex"); // 書き換え禁止用
	lua_insert(L, -2); lua_setfield(L, -2, "__index"); // 本来のテーブル
	lua_newtable(L); lua_insert(L, -2); // プロキシテーブル
	lua_setmetatable(L, -2); // メタテーブル設定
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
	lockTable(L); lua_setfield(L, -2, "DiscardType");
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
	lockTable(L); lua_setfield(L, -2, "Chii");
	lockTable(L); lua_setfield(L, -2, "Call");
}

/* ドラの色コード */
inline void aiscript::table::functable::doraColorCode(lua_State* const L) {
	lua_newtable(L);
	TableAdd(L, "Normal", (int)Normal);
	TableAdd(L, "Red", (int)AkaDora);
	TableAdd(L, "Blue", (int)AoDora);
	lockTable(L); lua_setfield(L, -2, "DoraColor");
}

/* 鳴きの種別コード */
inline void aiscript::table::functable::meldTypeCode(lua_State* const L) {
	lua_newtable(L); // MeldType
	lua_newtable(L); // Sequence
	TableAdd(L, "Lower", (int)meldSequenceExposedLower);
	TableAdd(L, "Middle", (int)meldSequenceExposedMiddle);
	TableAdd(L, "Upper", (int)meldSequenceExposedUpper);
	lockTable(L); lua_setfield(L, -2, "Sequence");
	lua_newtable(L); // Triplet
	TableAdd(L, "Kamicha", (int)meldTripletExposedLeft);
	TableAdd(L, "Toimen", (int)meldTripletExposedCenter);
	TableAdd(L, "Shimocha", (int)meldTripletExposedRight);
	lockTable(L); lua_setfield(L, -2, "Triplet");
	lua_newtable(L); // Quad
	TableAdd(L, "Concealed", (int)meldQuadConcealed);
	lua_newtable(L); // Exposed
	TableAdd(L, "Kamicha", (int)meldQuadExposedLeft);
	TableAdd(L, "Toimen", (int)meldQuadExposedCenter);
	TableAdd(L, "Shimocha", (int)meldQuadExposedRight);
	lockTable(L); lua_setfield(L, -2, "Exposed");
	lua_newtable(L); // Added
	TableAdd(L, "Kamicha", (int)meldQuadAddedLeft);
	TableAdd(L, "Toimen", (int)meldQuadAddedCenter);
	TableAdd(L, "Shimocha", (int)meldQuadAddedRight);
	lockTable(L); lua_setfield(L, -2, "Added");
	lockTable(L); lua_setfield(L, -2, "Quad");
	lockTable(L); lua_setfield(L, -2, "MeldType");
}

/* 牌の番号 */
inline void aiscript::table::functable::TileCode(lua_State* const L) {
	const char suitname[3][16] = {"Character","Circle","Bamboo",};
	const char numeral[9][8] = {"One","Two","Three","Four","Five","Six","Seven","Eight","Nine",};
	lua_newtable(L);
	for (int suit = 0; suit < TileSuitHonors; suit += TileSuitStep) { // 数牌(ループ)
		lua_newtable(L);
		for (int num = 1; num <= 9; num++) {
			TableAdd(L, numeral[num - 1], suit + num);
			TableAdd(L, num, suit + num);
		}
		lockTable(L); lua_setfield(L, -2, suitname[suit / TileSuitStep]);
	}
	lua_newtable(L);
	TableAdd(L, "East", (int)EastWind); TableAdd(L, "South", (int)SouthWind);
	TableAdd(L, "West", (int)WestWind); TableAdd(L, "North", (int)NorthWind);
	lockTable(L); lua_setfield(L, -2, "Wind");
	lua_newtable(L);
	TableAdd(L, "White", (int)WhiteDragon); TableAdd(L, "Green", (int)GreenDragon);
	TableAdd(L, "Red", (int)RedDragon);
	lockTable(L); lua_setfield(L, -2, "Dragon");
	lua_newtable(L);
	TableAdd(L, "Spring", (int)Spring); TableAdd(L, "Summer", (int)Summer);
	TableAdd(L, "Autumn", (int)Autumn); TableAdd(L, "Winter", (int)Winter);
	TableAdd(L, "Plum", (int)Plum); TableAdd(L, "Orchid", (int)Orchid);
	TableAdd(L, "Chrysanthemum", (int)Chrysanthemum); TableAdd(L, "Bamboo", (int)Bamboo);
	TableAdd(L, "Fall", (int)Autumn); TableAdd(L, "Chrys", (int)Chrysanthemum);
	TableAdd(L, "Flower", (int)Flower);
	lockTable(L); lua_setfield(L, -2, "Flower");
	lockTable(L); lua_setfield(L, -2, "Tile");
}

/* 卓の種別 */
inline void aiscript::table::functable::gametype(lua_State* const L) {
	if (chkGameType(&GameStat, Yonma)) lua_pushstring(L, "yonma");
	else if (chkGameType(&GameStat, Sanma)) lua_pushstring(L, "sanma");
	else if (chkGameType(&GameStat, Sanma4)) lua_pushstring(L, "sanma_with_four_players");
	else if (chkGameType(&GameStat, SanmaS)) lua_pushstring(L, "sanma_without_honors");
	else lua_pushnil(L);
	lua_setfield(L, -2, "gametype");
}

/* バージョン番号 tostring */
int aiscript::table::functable::version_tostring(lua_State* const L) {
	lua_pushstring(L, MIHAJONG_VER);
	return 1;
}

/* バージョン番号 */
inline void aiscript::table::functable::version(lua_State* const L) {
	lua_newtable(L); // version
	TableAdd(L, "major", MIHAJONG_MAJOR_VER);
	TableAdd(L, "minor", MIHAJONG_MINOR_VER);
	TableAdd(L, "patch", MIHAJONG_PATCH_VER);
	lockTable(L);
	lua_getmetatable(L, -1);
	lua_pushcfunction(L, version_tostring); lua_setfield(L, -2, "__tostring");
	lua_setmetatable(L, -2);
	lua_setfield(L, -2, "version");
}

/* 引数の数を数える */
int aiscript::table::functable::chkargnum(lua_State* const L, int argmin, int argmax) {
	int n = lua_gettop(L);
	if ((n < argmin)||(n > argmax)) {luaL_error(L, "Invalid number of argument");}
	return n;
}

/* 乱数を返す */
int aiscript::table::functable::random(lua_State* const L) {
	int n = chkargnum(L, 0, 2);
	switch (n) {
	case 0:
		lua_pushnumber(L, RndNum::rnd()); break; // 連続一様乱数
	case 1:
		if (lua_isnil(L, 1)) {
			lua_pushnumber(L, RndNum::rnd()); break; // 連続一様乱数
		} else if (lua_isnumber(L, 1)) { // 離散一様乱数
			int i = lua_tointeger(L, 1);
			if (i >= 1) lua_pushinteger(L, RndNum::rnd(i - 1) + 1);
			else lua_pushnil(L);
		} else {lua_pushnil(L);}
		break;
	case 2:
		if ((lua_isnil(L, 1)) && (lua_isnil(L, 2))) {
			lua_pushnumber(L, RndNum::rnd()); break; // 連続一様乱数
		} else if ((lua_isnumber(L, 1)) && (lua_isnil(L, 2))) { // 離散一様乱数
			int i = lua_tointeger(L, 1);
			if (i >= 1) lua_pushinteger(L, RndNum::rnd(i - 1) + 1);
			else lua_pushnil(L);
		} else if ((lua_isnumber(L, 1)) && (lua_isnumber(L, 2))) {
			lua_Number arg1 = lua_tonumber(L, 1);
			lua_Number arg2 = lua_tonumber(L, 2);
			if (arg2 > 0) { // 正規乱数
				lua_pushnumber(L, RndNum::rnd(arg1, arg2));
			} else if ((arg1 >= 1) && (arg2 <= -1)) { // nDm形式乱数
				lua_Integer ans = 0;
				for (int i = 0; i < (int)arg1; i++)
					ans += RndNum::rnd((unsigned int)(-arg2) - 1u);
				lua_pushinteger(L, ans + (int)arg1);
			} else lua_pushnil(L);
		} else {lua_pushnil(L);}
		break;
	default:
		lua_pushnil(L); break; // このコードは実行されないはずである
	}
	return 1;
}

/* ロギング用 */
int aiscript::table::functable::log(lua_State* const L) {
	int n = lua_gettop(L); CodeConv::tstring logstr = _T(""); CodeConv::tstring linea = _T("");lua_Debug ar;
	if (n > 1) {
		for (int i = 2; i <= n; i++) {
			logstr += CodeConv::DecodeStr(luaL_tolstring(L, i, nullptr)); lua_pop(L, 1);
			if (i < n) logstr += _T(" ");
		}
	}
	if (lua_getstack(L, 1, &ar)) {
		lua_getinfo(L, "Sl", &ar);
		if (ar.currentline > 0)
			linea = logger::posPrefix(ar.short_src, ar.currentline, logstr.c_str());
	}
	if (linea.empty())
		linea = CodeConv::tstring(_T("(Unknown) ")) + logstr;
	CodeConv::tstring loglevel = CodeConv::DecodeStr(luaL_tolstring(L, 1, nullptr)); lua_pop(L, 1);
	if (loglevel == CodeConv::DecodeStr("fatal")) logger::fatal_msg(linea.c_str());
	else if (loglevel == CodeConv::DecodeStr("error")) logger::error_msg(linea.c_str());
	else if (loglevel == CodeConv::DecodeStr("warn")) logger::warn_msg(linea.c_str());
	else if (loglevel == CodeConv::DecodeStr("info")) logger::info_msg(linea.c_str());
	else if (loglevel == CodeConv::DecodeStr("debug")) logger::debug_msg(linea.c_str());
	else if (loglevel == CodeConv::DecodeStr("trace")) logger::trace_msg(linea.c_str());
	return 0;
}

inline void aiscript::table::functable::gametbl::makeprototype(lua_State* const L, int playerID) {
	lua_newtable(L);
	lua_pushlightuserdata(L, nullptr); lua_setfield(L, -2, "addr"); // pointer to C++ struct
	lua_pushinteger(L, playerID + 1); lua_setfield(L, -2, "playerid"); // Player ID
	/* ここにメソッドを書く */
	lua_pushcfunction(L, luafunc::evaluate); lua_setfield(L, -2, "evaluate");
	lua_pushcfunction(L, luafunc::getactiveplayer); lua_setfield(L, -2, "getactiveplayer");
	lua_pushcfunction(L, luafunc::getbakaze); lua_setfield(L, -2, "getbakaze");
	lua_pushcfunction(L, luafunc::getbasepoint); lua_setfield(L, -2, "getbasepoint");
	lua_pushcfunction(L, luafunc::getchip); lua_setfield(L, -2, "getchip");
	lua_pushcfunction(L, luafunc::getcurrentdiscard); lua_setfield(L, -2, "getcurrentdiscard");
	lua_pushcfunction(L, luafunc::getdeckleft); lua_setfield(L, -2, "getdeckleft");
	lua_pushcfunction(L, luafunc::getdeposit); lua_setfield(L, -2, "getdeposit");
	lua_pushcfunction(L, luafunc::getdiscard); lua_setfield(L, -2, "getdiscard");
	lua_pushcfunction(L, luafunc::getdorainfo); lua_setfield(L, -2, "getdorainfo");
	lua_pushcfunction(L, luafunc::getdoukasen); lua_setfield(L, -2, "getdoukasen");
	lua_pushcfunction(L, luafunc::getflower); lua_setfield(L, -2, "getflower");
	lua_pushcfunction(L, luafunc::gethand); lua_setfield(L, -2, "gethand");
	lua_pushcfunction(L, luafunc::getjikaze); lua_setfield(L, -2, "getjikaze");
	lua_pushcfunction(L, luafunc::getmeld); lua_setfield(L, -2, "getmeld");
	lua_pushcfunction(L, luafunc::getopenwait); lua_setfield(L, -2, "getopenwait");
	lua_pushcfunction(L, luafunc::getpreviousdiscard); lua_setfield(L, -2, "getpreviousdiscard");
	lua_pushcfunction(L, luafunc::getrank); lua_setfield(L, -2, "getrank");
	lua_pushcfunction(L, luafunc::getround); lua_setfield(L, -2, "getround");
	lua_pushcfunction(L, luafunc::getrule); lua_setfield(L, -2, "getrule");
	lua_pushcfunction(L, luafunc::getscore); lua_setfield(L, -2, "getscore");
	lua_pushcfunction(L, luafunc::getseentiles); lua_setfield(L, -2, "getseentiles");
	lua_pushcfunction(L, luafunc::getshanten); lua_setfield(L, -2, "getshanten");
	lua_pushcfunction(L, luafunc::gettenpaistat); lua_setfield(L, -2, "gettenpaistat");
	lua_pushcfunction(L, luafunc::gettilecontext); lua_setfield(L, -2, "gettilecontext");
	lua_pushcfunction(L, luafunc::gettilerisk); lua_setfield(L, -2, "gettilerisk");
	lua_pushcfunction(L, luafunc::gettilesinhand); lua_setfield(L, -2, "gettilesinhand");
	lua_pushcfunction(L, luafunc::gettsumibou); lua_setfield(L, -2, "gettsumibou");
	lua_pushcfunction(L, luafunc::getwareme); lua_setfield(L, -2, "getwareme");
	lua_pushcfunction(L, luafunc::getyakuhaiwind); lua_setfield(L, -2, "getyakuhaiwind");
	lua_pushcfunction(L, luafunc::isankanallowed); lua_setfield(L, -2, "isankanallowed");
	lua_pushcfunction(L, luafunc::isdoujunfuriten); lua_setfield(L, -2, "isdoujunfuriten");
	lua_pushcfunction(L, luafunc::isfinalround); lua_setfield(L, -2, "isfinalround");
	lua_pushcfunction(L, luafunc::isfirstdraw); lua_setfield(L, -2, "isfirstdraw");
	lua_pushcfunction(L, luafunc::isippatsu); lua_setfield(L, -2, "isippatsu");
	lua_pushcfunction(L, luafunc::iskyuushu); lua_setfield(L, -2, "iskyuushu");
	lua_pushcfunction(L, luafunc::ismenzen); lua_setfield(L, -2, "ismenzen");
	lua_pushcfunction(L, luafunc::isopenriichideclared); lua_setfield(L, -2, "isopenriichideclared");
	lua_pushcfunction(L, luafunc::ispenultimateround); lua_setfield(L, -2, "ispenultimateround");
	lua_pushcfunction(L, luafunc::isriichideclared); lua_setfield(L, -2, "isriichideclared");
	lua_pushcfunction(L, luafunc::isshisanbuda); lua_setfield(L, -2, "isshisanbuda");
	lua_pushcfunction(L, luafunc::isshisibuda); lua_setfield(L, -2, "isshisibuda");
	lua_pushcfunction(L, luafunc::issumaroallowed); lua_setfield(L, -2, "issumaroallowed");
	lua_pushcfunction(L, luafunc::isyakitori); lua_setfield(L, -2, "isyakitori");
	/* メソッド定義ここまで */
	lockTable(L); lua_setfield(L, -2, tblname);
}

/* 鳴きの種別コード */
inline void aiscript::table::functable::agariTypeCode(lua_State* const L) {
	lua_newtable(L); // AgariType
	TableAdd(L, "All", (int)shantenAll);
	TableAdd(L, "Regular", (int)shantenRegular);
	TableAdd(L, "Pairs", (int)shantenPairs);
	TableAdd(L, "Orphans", (int)shantenOrphans);
	TableAdd(L, "Stellar", (int)shantenStellar);
	TableAdd(L, "CivilWar", (int)shantenCivilWar);
	TableAdd(L, "Syzygy", (int)shantenSyzygy);
	TableAdd(L, "Quanbukao", (int)shantenQuanbukao);
	TableAdd(L, "SevenUp", (int)shantenSevenup);
	TableAdd(L, "ZuheLong", (int)shantenZuhelong);
	lockTable(L); lua_setfield(L, -2, "AgariType");
}

/* プレイヤー番号を取得 */
PlayerID aiscript::table::functable::getPlayerID(lua_State* const L) {
	PlayerID player;
	lua_getglobal(L, tblname);
	lua_pushstring(L, "gametbl"); lua_gettable(L, -2);
	lua_pushstring(L, "playerid"); lua_gettable(L, -2);
	player = lua_tointeger(L, -1);
	lua_pop(L, 3);
	return player - 1;
}

/* チャット出力 */
int aiscript::table::functable::say(lua_State* const L) {
	int n = lua_gettop(L); CodeConv::tstring msgstr = _T("");
	if (n > 0) {
		for (int i = 1; i <= n; i++) {
			msgstr += CodeConv::DecodeStr(luaL_tolstring(L, i, nullptr)); lua_pop(L, 1);
			if (i < n) msgstr += _T(" ");
		}
		chat::sendchatx(getPlayerID(L), msgstr.c_str());
	}
	return 0;
}
