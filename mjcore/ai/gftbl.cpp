#include "functbl.h"

/* gameStat�̃A�h���X���擾�i�Öق̈����j */
GameTable* aiscript::table::functable::gametbl::getGameStatAddr(lua_State* const L) {
	lua_getfield(L, 1, "addr"); GameTable* addr = (GameTable*)lua_touserdata(L, -1); lua_pop(L, 1);
	return addr;
}

/* �v���C���[�ԍ����擾 */
PLAYER_ID aiscript::table::functable::gametbl::getPlayerID(lua_State* const L, int index) {
	PLAYER_ID player; int n = lua_gettop(L);
	if ((index != 0)&&(n >= index)&&(!lua_isnil(L, index))) player = lua_tointeger(L, index);
	else {lua_getfield(L, 1, "playerid"); player = lua_tointeger(L, -1); lua_pop(L, 1);}
	return player - 1;
}

/* �v�̎�ނ��Ƃ̕\���X�^�b�N�ɐς� */
const std::array<tileCode, 35> aiscript::table::functable::gametbl::validTiles = {
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
	lua_newtable(L); // �e�[�u��
	for (auto k = validTiles.begin(); k != validTiles.end(); k++)
		TableAdd(L, (int)*k, (int)(tptr[*k]));
}
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, UInt8ByTile& tptr) {
	lua_newtable(L); // �e�[�u��
	for (auto k = validTiles.begin(); k != validTiles.end(); k++)
		TableAdd(L, (int)*k, (int)(tptr[*k]));
}
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, FlagByTile& tptr) {
	lua_newtable(L); // �e�[�u��
	for (auto k = validTiles.begin(); k != validTiles.end(); k++)
		TableAdd(L, (int)*k, tptr[*k]);
}
void aiscript::table::functable::gametbl::pushTileTable(lua_State* const L, InfoByTile<MachihaiTileInfo>& tptr) {
	lua_newtable(L); // �e�[�u��
	for (auto k = validTiles.begin(); k != validTiles.end(); k++) {
		lua_pushinteger(L, (int)*k);
		lua_newtable(L);
		TableAdd(L, "flag", tptr[*k].MachihaiFlag);
		if (tptr[*k].MachihaiFlag) TableAdd(L, "count", tptr[*k].MachihaiCount);
		lua_settable(L, -3);
	}
}

/* ��v���擾 */
void aiscript::table::functable::gametbl::setHand(lua_State* const L, GameTable* const tmpGameStat, int index) {
	int n = lua_gettop(L);
	PLAYER_ID player = getPlayerID(L, 0);
	if ((index != 0)&&(n >= index)&&(!lua_isnil(L, index))) { // �v�p���w�肵���ꍇ
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			lua_pushnumber(L, i + 1); lua_gettable(L, n);
			if (lua_isnil(L, -1)) { // �����ɔv�͂Ȃ�����
				tmpGameStat->Player[player].Hand[i].tile = NoTile;
				tmpGameStat->Player[player].Hand[i].red = Normal;
			} else if (lua_istable(L, -1)) { // �v��������
				lua_pushstring(L, "tile"); lua_gettable(L, -2);
				tmpGameStat->Player[player].Hand[i].tile = (::tileCode)lua_tointeger(L, -1);
				lua_pop(L, -1);
				lua_pushstring(L, "red"); lua_gettable(L, -2);
				tmpGameStat->Player[player].Hand[i].red = (doraCol)lua_tointeger(L, -1);
				lua_pop(L, -1);
			} // �ςȂ��ƂɂȂ��Ă����疳��
			lua_pop(L, 1);
		}
	}
}

/* ���]������ */
int aiscript::table::functable::gametbl::luafunc::evaluate(lua_State* const L) {
	int n = chkargnum(L, 2, 3);
	const GameTable* gameStat = getGameStatAddr(L); GameTable tmpGameStat = *gameStat;
	tmpGameStat.TsumoAgariFlag = lua_toboolean(L, 2);
	PLAYER_ID player = getPlayerID(L, 0);
	setHand(L, &tmpGameStat, 3);
	yaku::YAKUSTAT evaluation = yaku::yakuCalculator::countyaku(gameStat, player);
	lua_newtable(L); // �Ԃ�l���i�[
	TableAdd(L, "ismahjong", evaluation.isValid); // �������Ă����true
	if (evaluation.isValid) {
		TableAdd(L, "isvalid", yaku::yakuCalculator::checkShibari(gameStat, &evaluation)); // ����𖞂����Ă��邩
		TableAdd(L, "fu", evaluation.BasePoints); // ��
		TableAdd(L, "han", evaluation.CoreHan + evaluation.BonusHan); // ��
		TableAdd(L, "mangan", (double)(evaluation.CoreSemiMangan + evaluation.BonusSemiMangan) / 2.0); // ����
		TableAdd(L, "points", evaluation.AgariPoints.bignumtodbl()); // �_��
	}
	return 1;
}

/* �c���Ԃ̃v���C���[�ԍ� */
int aiscript::table::functable::gametbl::luafunc::getactiveplayer(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushinteger(L, (int)getGameStatAddr(L)->CurrentPlayer.Active + 1);
	return 1;
}

/* �ꕗ�v */
int aiscript::table::functable::gametbl::luafunc::getbakaze(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 0);
	lua_pushinteger(L, (int)Wind2Tile((uint8_t)(gameStat->GameRound / 4)));
	return 1;
}

/* ���_(�Ԃ��_) */
int aiscript::table::functable::gametbl::luafunc::getbasepoint(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L); // dummy
	lua_pushinteger(L, BasePoint());
	return 1;
}

/* �`�b�v�擾 */
int aiscript::table::functable::gametbl::luafunc::getchip(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("chip")) lua_pushnil(L); // �`�b�v�������[���Ȃ�nil
	else lua_pushinteger(L, gameStat->Player[player].playerChip); // �`�b�v�̎��x���X�^�b�N�ɐς�
	return 1;
}

/* ���݂̎̔v */
int aiscript::table::functable::gametbl::luafunc::getcurrentdiscard(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	lua_newtable(L); // �߂�l���i�[����e�[�u��
	TableAdd(L, "tile", (int)gameStat->CurrentDiscard.tile);
	TableAdd(L, "red", (int)gameStat->CurrentDiscard.red);
	return 1;
}

/* �R�v�̎c�薇�� */
int aiscript::table::functable::gametbl::luafunc::getdeckleft(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushinteger(L, tilesLeft(getGameStatAddr(L)));
	return 1;
}

/* �����_�_�̐� */
int aiscript::table::functable::gametbl::luafunc::getdeposit(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushinteger(L, getGameStatAddr(L)->Deposit);
	return 1;
}

/* �̔v�e�[�u�� */
int aiscript::table::functable::gametbl::luafunc::getdiscard(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
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

/* �h�����̔z�� */
int aiscript::table::functable::gametbl::luafunc::getdorainfo(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	pushTileTable(L, gameStat->DoraFlag.Omote);
	return 1;
}

/* ���ΐ� */
int aiscript::table::functable::gametbl::luafunc::getdoukasen(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (getGameStatAddr(L)->DoukasenPlayer == -1)
		lua_pushnil(L); // ���ΐ��Ȃ��̎���nil
	else lua_pushinteger(L, (int)getGameStatAddr(L)->DoukasenPlayer + 1);
	return 1;
}

/* �Ԕv�̐� */
int aiscript::table::functable::gametbl::luafunc::getflower(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	if (chkGameType(gameStat, Yonma)) { // �l���͉Ԕv
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
	} else { // �O���͔����k
		lua_pushinteger(L, (int)gameStat->Player[player].NorthFlag);
	}
	return 1;
}

/* ����v�e�[�u�� */
int aiscript::table::functable::gametbl::luafunc::gethand(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
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

/* �ꕗ�v */
int aiscript::table::functable::gametbl::luafunc::getjikaze(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 0);
	lua_pushinteger(L, (int)Wind2Tile((uint8_t)playerwind(gameStat, player, gameStat->GameRound)));
	return 1;
}

/* ���I�ʎq */
int aiscript::table::functable::gametbl::luafunc::getmeld(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_newtable(L); // �߂�l���i�[����e�[�u��
	for (uint8_t i = 1; i <= gameStat->Player[player].MeldPointer; i++) {
		lua_pushinteger(L, i); lua_newtable(L);
		TableAdd(L, "tile", (int)gameStat->Player[player].Meld[i].tile);
		lua_newtable(L);
		for (int k = 0; k < (gameStat->Player[player].Meld[i].mstat >= meldQuadConcealed ? 4 : 3); k++)
			TableAdd(L, k + 1, (int)gameStat->Player[player].Meld[i].red[k]);
		lua_setfield(L, -2, "red");
		TableAdd(L, "type", (int)gameStat->Player[player].Meld[i].mstat);
		lua_settable(L, -3);
	}
	return 1;
}

/* �I�[�v�����[�`�̑҂��v��� */
int aiscript::table::functable::gametbl::luafunc::getopenwait(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	pushTileTable(L, gameStat->OpenRichiWait);
	return 1;
}

/* ���O�̎̔v���(�H���ς��h�~�p) */
int aiscript::table::functable::gametbl::luafunc::getpreviousdiscard(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	if (gameStat->PreviousMeld.Discard != NoTile) lua_pushinteger(L, gameStat->PreviousMeld.Discard); else lua_pushnil(L);
	if (gameStat->PreviousMeld.Stepped != NoTile) lua_pushinteger(L, gameStat->PreviousMeld.Stepped); else lua_pushnil(L);
	return 2;
}

/* ���ʎ擾 */
int aiscript::table::functable::gametbl::luafunc::getrank(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushinteger(L, calcRank(gameStat)[player]); // ���ʂ��X�^�b�N�ɐς�
	return 1;
}

/* ���݂̋ǔԍ� */
int aiscript::table::functable::gametbl::luafunc::getround(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game"))
		// ���ꂵ���Ȃ����[��
		lua_pushinteger(L, gameStat->LoopRound * 4 + gameStat->GameRound + 1);
	else // ���ʂ̃��[��
		lua_pushinteger(L, gameStat->LoopRound * roundLoopRate() + gameStat->GameRound + 1);
	return 1;
}

/* ���[���ݒ�擾 */
int aiscript::table::functable::gametbl::luafunc::getrule(lua_State* const L) {
	int n = chkargnum(L, 2, 2);
	const char* fieldname = lua_tostring(L, 2);
	lua_pushstring(L, RuleData::chkRule(fieldname).c_str());
	return 1;
}

/* �����_�擾 */
int aiscript::table::functable::gametbl::luafunc::getscore(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushnumber(L, gameStat->Player[player].PlayerScore.bignumtodbl()); // �����_��double�ɂ��ăX�^�b�N�ɐς�
	return 1;
}

/* �����Ă���v�̐��̔z�� */
int aiscript::table::functable::gametbl::luafunc::getseentiles(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	pushTileTable(L, countseentiles(gameStat));
	return 1;
}

/* ���������擾 */
int aiscript::table::functable::gametbl::luafunc::getshanten(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	const GameTable* gameStat = getGameStatAddr(L); GameTable tmpGameStat = *gameStat;
	PLAYER_ID player = getPlayerID(L, 0);
	setHand(L, &tmpGameStat, 2);
	lua_pushinteger(L, ShantenAnalyzer::calcShanten(&tmpGameStat, player, ShantenAnalyzer::shantenAll));
	return 1;
}

/* ���v�Ɋւ����� */
int aiscript::table::functable::gametbl::luafunc::gettenpaistat(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	const GameTable* gameStat = getGameStatAddr(L); GameTable tmpGameStat = *gameStat;
	PLAYER_ID player = getPlayerID(L, 0);
	setHand(L, &tmpGameStat, 2); MachihaiInfo status = chkFuriten(&tmpGameStat, player);
	// ---------------------------------------------------------------------
	lua_newtable(L);
	TableAdd(L, "isfuriten", status.FuritenFlag);
	TableAdd(L, "total", (int)status.MachihaiTotal);
	TableAdd(L, "kinds", (int)status.MachiMen);
	pushTileTable(L, status.Machihai); lua_setfield(L, -2, "bytile");
	return 1;
}

/* �v�ɂ��Ă̏�� */
int aiscript::table::functable::gametbl::luafunc::gettilecontext(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_newtable(L); // �߂�l���i�[����e�[�u��
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
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

/* ���v���ǂ������f����肪���� */
int aiscript::table::functable::gametbl::luafunc::gettilerisk(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 0);
	lua_newtable(L); // �߂�l���i�[����e�[�u��
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[player].Hand[i].tile != NoTile) {
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
				const PLAYER_ID targetP[3] = {
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

/* ��v�Ɏ����Ă��閇���̔z�� */
int aiscript::table::functable::gametbl::luafunc::gettilesinhand(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	pushTileTable(L, countTilesInHand(gameStat, player));
	return 1;
}

/* �ςݖ_ */
int aiscript::table::functable::gametbl::luafunc::gettsumibou(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushinteger(L, getGameStatAddr(L)->Honba);
	return 1;
}

/* ����� */
int aiscript::table::functable::gametbl::luafunc::getwareme(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (getGameStatAddr(L)->WaremePlayer == -1)
		lua_pushnil(L); // ����ڂȂ��̎���nil
	else lua_pushinteger(L, (int)getGameStatAddr(L)->WaremePlayer + 1);
	return 1;
}

/* ��v���ǂ����̃��X�g */
int aiscript::table::functable::gametbl::luafunc::getyakuhaiwind(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 0);
	lua_newtable(L); // �Ԃ�l���i�[
	const ::tileCode windtiles[4] = {EastWind, SouthWind, WestWind, NorthWind,};
	const char windname[4][8] = {"East", "South", "West", "North",};
	for (int i = 0; i < 4; i++) {
		bool flag = false;
		if (windtiles[i] ==
			Wind2Tile((uint8_t)(gameStat->GameRound / 4))) // �ꕗ�v
			flag = true;
		else if (windtiles[i] ==
			playerwind(gameStat, player, gameStat->GameRound)) // �����v
			flag = true;
		else if ((RuleData::chkRuleApplied("kaimenkaze")) && (windtiles[i] == // �J�啗�v
			playerwind(gameStat, gameStat->WaremePlayer, gameStat->GameRound)))
			flag = true;
		else if ((RuleData::chkRuleApplied("urakaze")) && (windtiles[i] == // �����v
			playerwind(gameStat, player + 2, gameStat->GameRound)))
			flag = true;
		TableAdd(L, windname[i], flag); // ���ʂ��i�[
	}
	return 1;
}

/* �����Ă��邩�H */
int aiscript::table::functable::gametbl::luafunc::isabovebase(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	const GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushboolean(L, isAboveBase(gameStat, player));
	return 1;
}

/* ���[�`��ÞȂł��邩�H */
int aiscript::table::functable::gametbl::luafunc::isankanallowed(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	const GameTable* gameStat = getGameStatAddr(L);
	PLAYER_ID player = getPlayerID(L, 0);
	lua_pushboolean(L, chkAnkanAbility(gameStat, player));
	return 1;
}

/* �I�[���X���H */
int aiscript::table::functable::gametbl::luafunc::isfinalround(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	const GameTable* gameStat = getGameStatAddr(L);
	lua_pushboolean(L, (gameStat->LoopRound * 16 + gameStat->GameRound) >= gameStat->GameLength);
	return 1;
}

/* ��ꎩ�̂��H */
int aiscript::table::functable::gametbl::luafunc::isfirstdraw(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].FirstDrawFlag);
	return 1;
}

/* �ꔭ�����H */
int aiscript::table::functable::gametbl::luafunc::isippatsu(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PLAYER_ID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("riichi_ippatsu")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].RichiFlag.IppatsuFlag);
	return 1;
}

/* ����v�H */
int aiscript::table::functable::gametbl::luafunc::iskyuushu(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (RuleData::chkRuleApplied("nine_terminals")) lua_pushnil(L);
	else lua_pushboolean(L, chkdaopaiability(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* ��O�H */
int aiscript::table::functable::gametbl::luafunc::ismenzen(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].MenzenFlag);
	return 1;
}

/* �I�[�v���������Ă���H */
int aiscript::table::functable::gametbl::luafunc::isopenriichideclared(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].RichiFlag.OpenFlag);
	return 1;
}

/* ���X�O���H */
int aiscript::table::functable::gametbl::luafunc::ispenultimateround(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	const GameTable* gameStat = getGameStatAddr(L);
	lua_pushboolean(L, (gameStat->LoopRound * 16 + gameStat->GameRound) == (gameStat->GameLength - 1));
	return 1;
}

/* �������Ă���H */
int aiscript::table::functable::gametbl::luafunc::isriichideclared(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PLAYER_ID player = getPlayerID(L, 2);
	lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].RichiFlag.RichiFlag);
	return 1;
}

/* �\�O�s���H */
int aiscript::table::functable::gametbl::luafunc::isshisanbuda(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (RuleData::chkRuleApplied("shiisan_puutaa")) lua_pushnil(L);
	else lua_pushboolean(L, yaku::yakuCalculator::chkShisanBuDa(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* �\�O���ρH */
int aiscript::table::functable::gametbl::luafunc::isshisibuda(lua_State* const L) {
	int n = chkargnum(L, 1, 1);
	if (RuleData::chkRuleApplied("shiisan_uushii")) lua_pushnil(L);
	else lua_pushboolean(L, yaku::yakuCalculator::chkShisiBuDa(getGameStatAddr(L), getPlayerID(L, 0)));
	return 1;
}

/* �l�n�H���ցH */
int aiscript::table::functable::gametbl::luafunc::issumaroallowed(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PLAYER_ID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("sumaro")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].SumaroFlag);
	return 1;
}

/* �Ă����H */
int aiscript::table::functable::gametbl::luafunc::isyakitori(lua_State* const L) {
	int n = chkargnum(L, 1, 2);
	PLAYER_ID player = getPlayerID(L, 2);
	if (RuleData::chkRuleApplied("yakitori")) lua_pushnil(L);
	else lua_pushboolean(L, getGameStatAddr(L)->Player[getPlayerID(L, 0)].YakitoriFlag);
	return 1;
}
