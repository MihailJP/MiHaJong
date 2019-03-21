#include "functbl.h"

#include "../func.h"
#include "../yaku/yaku.h"
#include "../ruletbl.h"

/* gameStatのアドレスを取得（暗黙の引数） */
GameTable* aiscript::table::functable::gametbl::getGameStatAddr(lua_State* const L) {
	lua_getfield(L, 1, "addr"); GameTable* addr = reinterpret_cast<GameTable*>(lua_touserdata(L, -1)); lua_pop(L, 1);
	return addr;
}

/* プレイヤー番号を取得 */
PlayerID aiscript::table::functable::gametbl::getPlayerID(lua_State* const L, int index) {
	PlayerID player; int n = lua_gettop(L);
	if ((index != 0)&&(n >= index)&&(!lua_isnil(L, index))) player = static_cast<Player_ID>(lua_tointeger(L, index));
	else {lua_getfield(L, 1, "playerid"); player = static_cast<Player_ID>(lua_tointeger(L, -1)); lua_pop(L, 1);}
	return player - 1;
}

/* 牌の種類ごとの表をスタックに積む */
constexpr std::array<TileCode, 35> aiscript::table::functable::gametbl::validTiles = {
	CharacterOne, CharacterTwo, CharacterThree, CharacterFour, CharacterFive,
	CharacterSix, CharacterSeven, CharacterEight, CharacterNine,
	CircleOne, CircleTwo, CircleThree, CircleFour, CircleFive,
	CircleSix, CircleSeven, CircleEight, CircleNine,
	BambooOne, BambooTwo, BambooThree, BambooFour, BambooFive,
	BambooSix, BambooSeven, BambooEight, BambooNine,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon,
	Flower,
};
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, Int8ByTile& tptr) {
	lua_newtable(L); // テーブル
	for (const auto& k : validTiles)
		TableAdd(L, static_cast<lua_Integer>(k), static_cast<lua_Integer>(tptr[k]));
}
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, UInt8ByTile& tptr) {
	lua_newtable(L); // テーブル
	for (const auto& k : validTiles)
		TableAdd(L, static_cast<lua_Integer>(k), static_cast<lua_Integer>(tptr[k]));
}
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, FlagByTile& tptr) {
	lua_newtable(L); // テーブル
	for (const auto& k : validTiles)
		TableAdd(L, static_cast<lua_Integer>(k), tptr[k]);
}
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, InfoByTile<MachihaiTileInfo>& tptr) {
	lua_newtable(L); // テーブル
	for (const auto& k : validTiles) {
		lua_pushinteger(L, static_cast<int>(k));
		lua_newtable(L);
		TableAdd(L, "flag", tptr[k].MachihaiFlag);
		if (tptr[k].MachihaiFlag) TableAdd(L, "count", static_cast<lua_Integer>(tptr[k].MachihaiCount));
		lua_settable(L, -3);
	}
}

/* 手牌を取得 */
void aiscript::table::functable::gametbl::setHand(lua_State* const L, GameTable* const tmpGameStat, int index) {
	int n = lua_gettop(L);
	PlayerID player = getPlayerID(L, 0);
	if ((index != 0)&&(n >= index)&&(!lua_isnil(L, index))) { // 牌姿を指定した場合
		for (int i = 0; i < NumOfTilesInHand; i++) {
			lua_pushnumber(L, i + 1); lua_gettable(L, index);
			if (lua_isnil(L, -1)) { // そこに牌はなかった
				tmpGameStat->Player[player].Hand[i] = Tile();
			} else if (lua_istable(L, -1)) { // 牌があった
				lua_getfield(L, -1, "tile");
				tmpGameStat->Player[player].Hand[i].tile = static_cast<TileCode>(lua_tointeger(L, -1));
				lua_pop(L, 1);
				lua_getfield(L, -1, "red");
				tmpGameStat->Player[player].Hand[i].red = static_cast<doraCol>(lua_tointeger(L, -1));
				lua_pop(L, 1);
			} // 変なことになっていたら無視
			lua_pop(L, 1);
		}
	}
}

/* 牌の移動 */
int aiscript::table::functable::gametbl::luafunc::movetile(lua_State* const L) {
	int n = chkargnum(L, 2, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 0);
	int index1 = static_cast<int>(lua_tointeger(L, -2)) - 1, index2 = static_cast<int>(lua_tointeger(L, -1)) - 1;
	lua_pop(L, 2);
	if ((index1 >= 0) && (index1 < TsumohaiIndex) && (index2 >= 0) && (index2 < TsumohaiIndex)) {
		MoveTile::moveTile(gameStat, player, false, index1);
		MoveTile::moveTile(gameStat, player, true, index2);
		MoveTile::enqueue(player, index1, index2);
	}
	return 0;
}

/* 手を評価する */
int aiscript::table::functable::gametbl::luafunc::evaluate(lua_State* const L) {
	int n = chkargnum(L, 2, 3);
	const GameTable* gameStat = getGameStatAddr(L); GameTable tmpGameStat = *gameStat;
	tmpGameStat.TsumoAgariFlag = lua_toboolean(L, 2);
	PlayerID player = getPlayerID(L, 0);
	setHand(L, &tmpGameStat, 3);
	yaku::YAKUSTAT evaluation = yaku::yakuCalculator::countyaku(&tmpGameStat, player);
	lua_newtable(L); // 返り値を格納
	TableAdd(L, "ismahjong", evaluation.isValid); // あがっていればtrue
	if (evaluation.isValid) {
		TableAdd(L, "isvalid", yaku::yakuCalculator::checkShibari(&tmpGameStat, &evaluation)); // 縛りを満たしているか
		TableAdd(L, "fu", static_cast<lua_Integer>(evaluation.BasePoints)); // 符
		TableAdd(L, "han", static_cast<lua_Integer>(evaluation.TotalHan())); // 飜
		TableAdd(L, "mangan", static_cast<double>(evaluation.TotalSemiMangan()) / 2.0); // 満貫
		TableAdd(L, "points", static_cast<double>(evaluation.AgariPoints)); // 点数
	}
	return 1;
}

/* ツモ番のプレイヤー番号 */
int aiscript::table::functable::gametbl::luafunc::getactiveplayer(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushinteger(L, static_cast<int>(getGameStatAddr(L)->CurrentPlayer.Active) + 1);
	return 1;
}

/* 場風牌 */
int aiscript::table::functable::gametbl::luafunc::getbakaze(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 0);
	lua_pushinteger(L, static_cast<int>(Wind2Tile(static_cast<uint8_t>(gameStat->GameRound / 4))));
	return 1;
}

/* 原点(返し点) */
int aiscript::table::functable::gametbl::luafunc::getbasepoint(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L); // dummy
	lua_pushinteger(L, static_cast<lua_Integer>(BasePoint()));
	return 1;
}

/* チップ取得 */
int aiscript::table::functable::gametbl::luafunc::getchip(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("chip")) lua_pushnil(L); // チップ無しルールならnil
	else lua_pushinteger(L, gameStat->Player[player].playerChip); // チップの収支をスタックに積む
	return 1;
}

/* 現在の捨牌 */
int aiscript::table::functable::gametbl::luafunc::getcurrentdiscard(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	lua_newtable(L); // 戻り値を格納するテーブル
	TableAdd(L, "tile", static_cast<lua_Integer>(gameStat->CurrentDiscard.tile));
	TableAdd(L, "red", static_cast<lua_Integer>(gameStat->CurrentDiscard.red));
	return 1;
}

/* 山牌の残り枚数 */
int aiscript::table::functable::gametbl::luafunc::getdeckleft(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushinteger(L, getGameStatAddr(L)->tilesLeft());
	return 1;
}

/* 供託点棒の数 */
int aiscript::table::functable::gametbl::luafunc::getdeposit(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushinteger(L, getGameStatAddr(L)->Deposit);
	return 1;
}

/* 捨牌テーブル */
int aiscript::table::functable::gametbl::luafunc::getdiscard(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	lua_newtable(L); // 戻り値を格納するテーブル
	for (uint8_t i = 1; i <= gameStat->Player[player].DiscardPointer; i++) {
		lua_pushnumber(L, i);
		lua_newtable(L);
		TableAdd(L, "tile", static_cast<lua_Integer>(gameStat->Player[player].Discard[i].tcode.tile));
		TableAdd(L, "red", static_cast<lua_Integer>(gameStat->Player[player].Discard[i].tcode.red));
		TableAdd(L, "through", gameStat->Player[player].Discard[i].isDiscardThrough);
		TableAdd(L, "riichi", (gameStat->Player[player].Discard[i].dstat == discardRiichi) ||
			(gameStat->Player[player].Discard[i].dstat == discardRiichiTaken));
		TableAdd(L, "taken", (gameStat->Player[player].Discard[i].dstat == discardTaken) ||
			(gameStat->Player[player].Discard[i].dstat == discardRiichiTaken));
		lua_settable(L, -3);
	}
	return 1;
}

/* ドラ情報の配列 */
int aiscript::table::functable::gametbl::luafunc::getdorainfo(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	pushTileTable(L, gameStat->DoraFlag.Omote);
	return 1;
}

/* 導火線 */
int aiscript::table::functable::gametbl::luafunc::getdoukasen(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (getGameStatAddr(L)->DoukasenPlayer == -1)
		lua_pushnil(L); // 導火線なしの時はnil
	else lua_pushinteger(L, static_cast<int>(getGameStatAddr(L)->DoukasenPlayer + 1));
	return 1;
}

/* 花牌の数 */
int aiscript::table::functable::gametbl::luafunc::getflower(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	if (gameStat->chkGameType(Yonma)) { // 四麻は花牌
		int k = 0;
		if (gameStat->Player[player].FlowerFlag.Spring) ++k;
		if (gameStat->Player[player].FlowerFlag.Summer) ++k;
		if (gameStat->Player[player].FlowerFlag.Autumn) ++k;
		if (gameStat->Player[player].FlowerFlag.Winter) ++k;
		if (gameStat->Player[player].FlowerFlag.Plum) ++k;
		if (gameStat->Player[player].FlowerFlag.Orchid) ++k;
		if (gameStat->Player[player].FlowerFlag.Chrys) ++k;
		if (gameStat->Player[player].FlowerFlag.Bamboo) ++k;
		lua_pushinteger(L, k);
	} else { // 三麻は抜き北
		lua_pushinteger(L, static_cast<int>(gameStat->Player[player].NorthFlag));
	}
	return 1;
}

/* 純手牌テーブル */
int aiscript::table::functable::gametbl::luafunc::gethand(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	lua_newtable(L); // 戻り値を格納するテーブル
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (gameStat->Player[player].Hand[i]) {
			/* 牌があったらサプテーブルに変換、なかったらnilのまま放置 */
			lua_pushnumber(L, i + 1);
			lua_newtable(L);
			TableAdd(L, "tile", static_cast<lua_Integer>(gameStat->Player[player].Hand[i].tile));
			TableAdd(L, "red", static_cast<lua_Integer>(gameStat->Player[player].Hand[i].red));
			lua_settable(L, -3);
		}
	}
	return 1;
}

/* 場風牌 */
int aiscript::table::functable::gametbl::luafunc::getjikaze(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 0);
	lua_pushinteger(L, static_cast<int>(Wind2Tile(static_cast<uint8_t>(gameStat->playerwind(player)))));
	return 1;
}

/* 副露面子 */
int aiscript::table::functable::gametbl::luafunc::getmeld(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	lua_newtable(L); // 戻り値を格納するテーブル
	for (uint8_t i = 1; i <= gameStat->Player[player].MeldPointer; i++) {
		lua_pushinteger(L, i); lua_newtable(L);
		TableAdd(L, "tile", static_cast<lua_Integer>(gameStat->Player[player].Meld[i].tile));
		lua_newtable(L);
		for (int k = 0; k < (gameStat->Player[player].Meld[i].mstat >= meldQuadConcealed ? 4 : 3); k++)
			TableAdd(L, k + 1, static_cast<lua_Integer>(gameStat->Player[player].Meld[i].red[k]));
		lua_setfield(L, -2, "red");
		TableAdd(L, "type", static_cast<lua_Integer>(gameStat->Player[player].Meld[i].mstat));
		lua_settable(L, -3);
	}
	return 1;
}

/* オープンリーチの待ち牌情報 */
int aiscript::table::functable::gametbl::luafunc::getopenwait(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	pushTileTable(L, gameStat->OpenRichiWait);
	return 1;
}

/* 直前の捨牌情報(食い変え防止用) */
int aiscript::table::functable::gametbl::luafunc::getpreviousdiscard(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	if (gameStat->PreviousMeld.Discard != NoTile) lua_pushinteger(L, gameStat->PreviousMeld.Discard); else lua_pushnil(L);
	if (gameStat->PreviousMeld.Stepped != NoTile) lua_pushinteger(L, gameStat->PreviousMeld.Stepped); else lua_pushnil(L);
	return 2;
}

/* 順位取得 */
int aiscript::table::functable::gametbl::luafunc::getrank(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	lua_pushinteger(L, calcRank(gameStat)[player]); // 順位をスタックに積む
	return 1;
}

/* 現在の局番号 */
int aiscript::table::functable::gametbl::luafunc::getround(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game"))
		// 東場しかないルール
		lua_pushinteger(L, gameStat->LoopRound * 4 + gameStat->GameRound + 1);
	else // 普通のルール
		lua_pushinteger(L, gameStat->LoopRound * roundLoopRate() + gameStat->GameRound + 1);
	return 1;
}

/* ルール設定取得 */
int aiscript::table::functable::gametbl::luafunc::getrule(lua_State* const L) {
	int n = chkargnum(L, 2, 2);
	const char* fieldname = lua_tostring(L, 2);
	if (RuleData::chkRule(fieldname, "N/A")) lua_pushnil(L);
	else lua_pushstring(L, RuleData::chkRule(fieldname));
	return 1;
}

/* 持ち点取得 */
int aiscript::table::functable::gametbl::luafunc::getscore(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	lua_pushnumber(L, static_cast<double>(gameStat->Player[player].PlayerScore)); // 持ち点をdoubleにしてスタックに積む
	return 1;
}

/* 見えている牌の数の配列 */
int aiscript::table::functable::gametbl::luafunc::getseentiles(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	Int8ByTile tiles(countseentiles(gameStat));
	pushTileTable(L, tiles);
	return 1;
}

/* 向聴数を取得 */
int aiscript::table::functable::gametbl::luafunc::getshanten(lua_State* const L) {
	int n = chkargnum(L, 1, 3);
	const GameTable* gameStat = getGameStatAddr(L); GameTable tmpGameStat = *gameStat;
	PlayerID player = getPlayerID(L, 0);
	setHand(L, &tmpGameStat, 2);
	ShantenType type = shantenAll;
	if ((n >= 3) && (lua_isnumber(L, 3))) type = static_cast<ShantenType>(lua_tointeger(L, 3));
	Shanten s = ShantenAnalyzer::calcShanten(&tmpGameStat, player, type);
	if (s == ShantenImpossible) lua_pushnil(L);
	else lua_pushinteger(L, ShantenAnalyzer::calcShanten(&tmpGameStat, player, type));
	return 1;
}

/* 聴牌に関する情報 */
int aiscript::table::functable::gametbl::luafunc::gettenpaistat(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	const GameTable* gameStat = getGameStatAddr(L); GameTable tmpGameStat = *gameStat;
	PlayerID player = getPlayerID(L, 0);
	setHand(L, &tmpGameStat, 2); MachihaiInfo status = chkFuriten(&tmpGameStat, player);
	// ---------------------------------------------------------------------
	lua_newtable(L);
	TableAdd(L, "isfuriten", status.FuritenFlag);
	TableAdd(L, "total", static_cast<lua_Integer>(status.MachihaiTotal));
	TableAdd(L, "kinds", static_cast<lua_Integer>(status.MachiMen));
	pushTileTable(L, status.Machihai); lua_setfield(L, -2, "bytile");
	return 1;
}

/* 牌についての情報 */
int aiscript::table::functable::gametbl::luafunc::gettilecontext(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	lua_newtable(L); // 戻り値を格納するテーブル
	for (int i = 0; i < NumOfTilesInHand; i++) {
		lua_pushnumber(L, i + 1);
		lua_newtable(L);
		TileStatus tileStat = gettilestatus(gameStat, player, i, false);
		TableAdd(L, "isexistent", tileStat.isExistent);
		if (tileStat.isExistent) {
			TableAdd(L, "formstriplet", tileStat.formsTriplet);
			TableAdd(L, "formssequence", tileStat.formsSequence);
			TableAdd(L, "canformquad", tileStat.canFormQuad);
			TableAdd(L, "formspair", tileStat.formsPair);
			TableAdd(L, "formsryanmen", tileStat.seqDoubleSideWait);
			TableAdd(L, "formskanchan", tileStat.seqMidWait);
			TableAdd(L, "formspenchan", tileStat.seqSingleSideWait);
		}
		lua_settable(L, -3);
	}
	return 1;
}

/* 安牌かどうか判断する手がかり */
int aiscript::table::functable::gametbl::luafunc::gettilerisk(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 0);
	lua_newtable(L); // 戻り値を格納するテーブル
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (gameStat->Player[player].Hand[i]) {
			lua_pushnumber(L, i + 1);
			lua_newtable(L);
			TableAdd(L, "issameasprevious", riskchk::issameasprevious(gameStat, player, i));
			TableAdd(L, "isdora", riskchk::isdora(gameStat, player, i));
			TableAdd(L, "isdorasuji", riskchk::isdorasuji(gameStat, player, i));
			TableAdd(L, "isdorasoba", riskchk::isdorasoba(gameStat, player, i));
			TableAdd(L, "isnochance", riskchk::isnochance(gameStat, player, i));
			TableAdd(L, "isonechance", riskchk::isonechance(gameStat, player, i));
			TableAdd(L, "isneverdiscarded", riskchk::isneverdiscarded(gameStat, player, i));
			TableAdd(L, "isseenfour", riskchk::isseenfour(gameStat, player, i));
			{
				static const char tblname[3][16] = {"kamicha", "toimen", "shimocha",};
				const PlayerID targetP[3] = {
					RelativePositionOf(player, sLeft),
					RelativePositionOf(player, sOpposite),
					RelativePositionOf(player, sRight),
				};
				for (int k = 0; k < 3; k++) {
					lua_newtable(L);
					bool ipsum = riskchk::isgembutsu(gameStat, player, targetP[k], i);
					TableAdd(L, "isgembutsu", ipsum);
					TableAdd(L, "issuji", ipsum ? false : riskchk::issuji(gameStat, player, targetP[k], i));
					TableAdd(L, "isurasuji", ipsum ? false : riskchk::isurasuji(gameStat, player, targetP[k], i));
					TableAdd(L, "ismatagisuji", ipsum ? false : riskchk::ismatagisuji(gameStat, player, targetP[k], i));
					TableAdd(L, "issenkisuji", ipsum ? false : riskchk::issenkisuji(gameStat, player, targetP[k], i));
					TableAdd(L, "isnamakurasuji", ipsum ? false : riskchk::isnamakurasuji(gameStat, player, targetP[k], i));
					TableAdd(L, "isnakasuji", ipsum ? false : riskchk::isnakasuji(gameStat, player, targetP[k], i));
					lua_setfield(L, -2, tblname[k]);
				}
			}
			lua_settable(L, -3);
		}
	}
	return 1;
}

/* 手牌に持っている枚数の配列 */
int aiscript::table::functable::gametbl::luafunc::gettilesinhand(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	Int8ByTile tiles(countTilesInHand(gameStat, player));
	pushTileTable(L, tiles);
	return 1;
}

/* 積み棒 */
int aiscript::table::functable::gametbl::luafunc::gettsumibou(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushinteger(L, getGameStatAddr(L)->Honba);
	return 1;
}

/* 割れ目 */
int aiscript::table::functable::gametbl::luafunc::getwareme(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (getGameStatAddr(L)->WaremePlayer == -1)
		lua_pushnil(L); // 割れ目なしの時はnil
	else lua_pushinteger(L, static_cast<int>(getGameStatAddr(L)->WaremePlayer) + 1);
	return 1;
}

/* 役牌かどうかのリスト */
int aiscript::table::functable::gametbl::luafunc::getyakuhaiwind(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 0);
	lua_newtable(L); // 返り値を格納
	constexpr TileCode windtiles[4] = {EastWind, SouthWind, WestWind, NorthWind,};
	constexpr char windname[4][8] = {"East", "South", "West", "North",};
	for (int i = 0; i < 4; i++) {
		bool flag = false;
		if (windtiles[i] ==
			Wind2Tile(static_cast<uint8_t>(gameStat->GameRound / 4))) // 場風牌
			flag = true;
		else if (windtiles[i] ==
			Wind2Tile(gameStat->playerwind(player))) // 自風牌
			flag = true;
		else if ((RuleData::chkRuleApplied("kaimenkaze")) && (windtiles[i] == // 開門風牌
			Wind2Tile(gameStat->playerwind(gameStat->WaremePlayer))))
			flag = true;
		else if ((RuleData::chkRuleApplied("urakaze")) && (windtiles[i] == // 裏風牌
			Wind2Tile(gameStat->playerwind(player + 2))))
			flag = true;
		TableAdd(L, windname[i], flag); // 結果を格納
	}
	return 1;
}

/* 浮いているか？ */
int aiscript::table::functable::gametbl::luafunc::isabovebase(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	const GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 2);
	lua_pushboolean(L, isAboveBase(gameStat, player));
	return 1;
}

/* リーチ後暗槓できるか？ */
int aiscript::table::functable::gametbl::luafunc::isankanallowed(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	const GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 0);
	lua_pushboolean(L, chkAnkanAbility(gameStat, player));
	return 1;
}

/* 同巡内フリテンか？ */
int aiscript::table::functable::gametbl::luafunc::isdoujunfuriten(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	const GameTable* gameStat = getGameStatAddr(L);
	PlayerID player = getPlayerID(L, 0);
	lua_pushboolean(L, gameStat->Player[player].DoujunFuriten);
	return 1;
}

/* オーラスか？ */
int aiscript::table::functable::gametbl::luafunc::isfinalround(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	const GameTable* gameStat = getGameStatAddr(L);
	lua_pushboolean(L, (gameStat->LoopRound * 16 + gameStat->GameRound) >= gameStat->GameLength);
	return 1;
}

/* 第一自摸か？ */
int aiscript::table::functable::gametbl::luafunc::isfirstdraw(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].FirstDrawFlag);
	return 1;
}

/* 一発圏内？ */
int aiscript::table::functable::gametbl::luafunc::isippatsu(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PlayerID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("riichi_ippatsu")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[player].RichiFlag.IppatsuFlag);
	return 1;
}

/* 槓三巡？ */
int aiscript::table::functable::gametbl::luafunc::iskansanjunqualified(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PlayerID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("kansanjun")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[player].kansanjunFlag);
	return 1;
}

/* 九種九牌？ */
int aiscript::table::functable::gametbl::luafunc::iskyuushu(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (RuleData::chkRuleApplied("nine_terminals")) lua_pushnil(L);
	else lua_pushboolean(L, chkdaopaiability(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* 門前？ */
int aiscript::table::functable::gametbl::luafunc::ismenzen(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PlayerID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[player].MenzenFlag);
	return 1;
}

/* オープン立直している？ */
int aiscript::table::functable::gametbl::luafunc::isopenriichideclared(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PlayerID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[player].RichiFlag.OpenFlag);
	return 1;
}

/* ラス前か？ */
int aiscript::table::functable::gametbl::luafunc::ispenultimateround(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	const GameTable* gameStat = getGameStatAddr(L);
	lua_pushboolean(L, (gameStat->LoopRound * 16 + gameStat->GameRound) == (gameStat->GameLength - 1));
	return 1;
}

/* 戻牌天和？ */
int aiscript::table::functable::gametbl::luafunc::isrenpaitenhohqualified(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	PlayerID player = getPlayerID(L, 0);
	if (RuleData::chkRuleApplied("renpai_tenhoh")) {
		lua_pushnil(L);
		lua_pushnil(L);
	} else {
		lua_pushboolean(L, getGameStatAddr(L)->Player[player].renpaiTenhohStat > 0);
		lua_pushboolean(L, getGameStatAddr(L)->Player[player].renpaiTenhohStat >= 0);
	}
	return 2;
}

/* 立直している？ */
int aiscript::table::functable::gametbl::luafunc::isriichideclared(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PlayerID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[player].RichiFlag.RichiFlag);
	return 1;
}
/* 十三不塔？ */
int aiscript::table::functable::gametbl::luafunc::isshisanbuda(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (RuleData::chkRuleApplied("shiisan_puutaa")) lua_pushnil(L);
	else lua_pushboolean(L, yaku::yakuCalculator::chkShisanBuDa(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* 十三無靠？ */
int aiscript::table::functable::gametbl::luafunc::isshisibuda(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (RuleData::chkRuleApplied("shiisan_uushii")) lua_pushnil(L);
	else lua_pushboolean(L, yaku::yakuCalculator::chkShisiBuDa(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* 初槓？ */
int aiscript::table::functable::gametbl::luafunc::isshokanqualified(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PlayerID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("shokan")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[player].shokanFlag);
	return 1;
}

/* 四馬路解禁？ */
int aiscript::table::functable::gametbl::luafunc::issumaroallowed(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PlayerID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("sumaro")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[player].SumaroFlag);
	return 1;
}

/* 焼き鳥？ */
int aiscript::table::functable::gametbl::luafunc::isyakitori(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PlayerID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("yakitori")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[player].YakitoriFlag);
	return 1;
}
