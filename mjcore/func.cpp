#include "func.h"

TileCount countTilesInHand(GameTable* gameStat, PLAYER_ID playerID) {
	// 手牌に存在する牌を種類別にカウントする（鳴き面子・暗槓は除く）
	TileCount count = TileCount(); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = tileCode(gameStat->Player[playerID].Hand[i].tile)) != tileCode::NoTile)
			count[tmpTC]++;
	}
	return TileCount(count);
}

/* -------------------------------------------------------------------------
 *  function.hsp から移植
 * -------------------------------------------------------------------------
 */

/* プレイヤーの自風がどれか調べる */
seatAbsolute inline playerwind(GameTable* gameStat, PLAYER_ID player, PLAYER_ID currentRound) {
	if (chkGameType(gameStat, SanmaT))
		return (seatAbsolute)((player + 24 - (currentRound - ( currentRound / 4))) % 3);
	else return (seatAbsolute)((player + 32 - currentRound) % 4);
}
__declspec(dllexport) inline int playerwind(int player, int currentRound) {
	return (int)playerwind(&GameStat, (PLAYER_ID)player, (PLAYER_ID)currentRound);
}

/* あるプレイヤーに対して指定したプレイヤーがどこ(下家、対面、上家)にいるか調べる */
seatRelative inline playerRelative(PLAYER_ID targetPlayer, PLAYER_ID basePlayer) {
	return (seatRelative)((PLAYERS + targetPlayer - basePlayer) % PLAYERS);
}
__declspec(dllexport) inline int playerRelative(int targetPlayer, int basePlayer) {
	return (int)playerRelative((PLAYER_ID)targetPlayer, (PLAYER_ID)basePlayer);
}

/* あるプレイヤーの(下家、対面、上家)を調べる */
PLAYER_ID inline RelativePositionOf(PLAYER_ID targetPlayer, seatRelative relative) {
	return (PLAYER_ID)(((int)targetPlayer + (int)relative) % PLAYERS);
}
__declspec(dllexport) inline int RelativePositionOf(int targetPlayer, int relative) {
	return (int)RelativePositionOf((PLAYER_ID)targetPlayer, (seatRelative)relative);
}

/* 一周するまでに必要な場の数 */
__declspec(dllexport) inline int roundLoopRate() {
	if (getRule(RULE_SUDDEN_DEATH_TYPE) == 1) return 28;
	else return 16;
}

/* 王牌を除いた山牌の残り枚数 */
__declspec(dllexport) inline int tilesLeft(GameTable* gameStat) {
	return ((int)gameStat->RinshanPointer -
		((int)gameStat->DeadTiles - 1) -
		(int)gameStat->TilePointer);
}

/* 順位を計算する */
PlayerRankList calcRank(GameTable* gameStat) {
	PlayerRankList rankList;
	rankList.fill(0);
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		rankList[i] = 1;
		for (int j = 0; j < ACTUAL_PLAYERS; j++) {
			if (gameStat->Player[j].PlayerScore > gameStat->Player[i].PlayerScore)
				rankList[i]++;
		}
	}
	return rankList;
}
__declspec(dllexport) void calcRank(int* Rank, GameTable* gameStat) {
	const PlayerRankList rankList = calcRank(gameStat);
	assert(rankList[0] > 0); assert(rankList[1] > 0);
	for (int i = 0; i < PLAYERS; i++) *(Rank + i) = rankList[i];
}

/* 包かどうかの判定 */
bool isPao(GameTable* gameStat, PLAYER_ID agariPlayer, PLAYER_ID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if ((paoPlayer == gameStat->PaoFlag[i].paoPlayer) &&
			(agariPlayer == gameStat->PaoFlag[i].agariPlayer))
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isPao(GameTable* gameStat, int agariPlayer, int paoPlayer) {
	return (int)isPao(gameStat, (PLAYER_ID)agariPlayer, (PLAYER_ID)paoPlayer);
}

bool isPaoAgari(GameTable* gameStat, PLAYER_ID agariPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if (agariPlayer == gameStat->PaoFlag[i].agariPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isPaoAgari(GameTable* gameStat, int agariPlayer) {
	return (int)isPaoAgari(gameStat, (PLAYER_ID)agariPlayer);
}

bool isGotPao(GameTable* gameStat, PLAYER_ID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if (paoPlayer == gameStat->PaoFlag[i].paoPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isGotPao(GameTable* gameStat, int paoPlayer) {
	return (int)isGotPao(gameStat, (PLAYER_ID)paoPlayer);
}

PLAYER_ID getPaoPlayer(GameTable* gameStat, PLAYER_ID agariPlayer) {
	PLAYER_ID paoPlayer = -1;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if ((gameStat->PaoFlag[i].paoPlayer >= 0) &&
			(gameStat->PaoFlag[i].agariPlayer == agariPlayer))
			paoPlayer = gameStat->PaoFlag[i].paoPlayer;
	}
	return paoPlayer;
}
__declspec(dllexport) int getPaoPlayer(GameTable* gameStat, int agariPlayer) {
	return (int)getPaoPlayer(gameStat, (PLAYER_ID)agariPlayer);
}

/* ロンしたプレイヤーの数 */
__declspec(dllexport) int RonPlayers(GameTable* gameStat) {
	int qualified = 0;
	for (int i = 0; i < PLAYERS; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron)
			qualified++;
	return qualified;
}

/* 「東家」「南家」「西家」「北家」の文字列を返す */
std::string inline windName(seatAbsolute wind) {
	switch (wind) {
		case sEast: return std::string("東家");
		case sSouth: return std::string("南家");
		case sWest: return std::string("西家");
		case sNorth: return std::string("北家");
		default:
			throw std::domain_error("windName(): 異常な引数です");
	}
}
__declspec(dllexport) void windName(char* str, int wind) {
	std::strcpy(str, windName((seatAbsolute)wind).c_str());
}

/* 「東○局」などの文字列を返す */
std::string inline roundName(int roundNum, GameTable* gameStat) {
	std::ostringstream roundNameTxt; roundNameTxt.str("");
	switch (roundNum / PLAYERS) {
		case 0: roundNameTxt << "東"; break;
		case 1: roundNameTxt << "南"; break;
		case 2: roundNameTxt << "西"; break;
		case 3: roundNameTxt << "北"; break;
		case 4: roundNameTxt << "白"; break;
		case 5: roundNameTxt << "発"; break;
		case 6: roundNameTxt << "中"; break;
		default: throw std::domain_error("roundName(): 異常な引数です。場風を解析できません。");
	}
	if ((getRule(RULE_GAME_LENGTH) == 5)||(getRule(RULE_GAME_LENGTH) == 7)) {
		switch (int k = (gameStat->LoopRound * ACTUAL_PLAYERS + roundNum % PLAYERS)) {
			case 0: roundNameTxt << "一局"; break;
			case 1: roundNameTxt << "二局"; break;
			case 2: roundNameTxt << "三局"; break;
			case 3: roundNameTxt << "四局"; break;
			case 4: roundNameTxt << "五局"; break;
			case 5: roundNameTxt << "六局"; break;
			case 6: roundNameTxt << "七局"; break;
			case 7: roundNameTxt << "八局"; break;
			case 8: roundNameTxt << "九局"; break;
			case 9: roundNameTxt << "十局"; break;
			default: roundNameTxt << (k+1) << "局"; break;
		}
	} else {
		switch (roundNum % PLAYERS) {
			case 0: roundNameTxt << "一局"; break;
			case 1: roundNameTxt << "二局"; break;
			case 2: roundNameTxt << "三局"; break;
			case 3: roundNameTxt << "四局"; break;
			default: throw std::domain_error("roundName(): 異常な引数です。同一場の5局目以降です。");
		}
	}
	return std::string(roundNameTxt.str());
}
__declspec(dllexport) void roundName(char* str, int roundNum) {
	std::strcpy(str, roundName(roundNum, &GameStat).c_str());
}

/* 牌の名前の文字列を返す */
std::string inline TileName(tileCode tile) {
	switch (tile) {
		case CharacterOne:   return std::string("一萬");
		case CharacterTwo:   return std::string("二萬");
		case CharacterThree: return std::string("三萬");
		case CharacterFour:  return std::string("四萬");
		case CharacterFive:  return std::string("五萬");
		case CharacterSix:   return std::string("六萬");
		case CharacterSeven: return std::string("七萬");
		case CharacterEight: return std::string("八萬");
		case CharacterNine:  return std::string("九萬");
		case CircleOne:      return std::string("一筒");
		case CircleTwo:      return std::string("二筒");
		case CircleThree:    return std::string("三筒");
		case CircleFour:     return std::string("四筒");
		case CircleFive:     return std::string("五筒");
		case CircleSix:      return std::string("六筒");
		case CircleSeven:    return std::string("七筒");
		case CircleEight:    return std::string("八筒");
		case CircleNine:     return std::string("九筒");
		case BambooOne:      return std::string("一索");
		case BambooTwo:      return std::string("二索");
		case BambooThree:    return std::string("三索");
		case BambooFour:     return std::string("四索");
		case BambooFive:     return std::string("五索");
		case BambooSix:      return std::string("六索");
		case BambooSeven:    return std::string("七索");
		case BambooEight:    return std::string("八索");
		case BambooNine:     return std::string("九索");
		case EastWind:       return std::string("東");
		case SouthWind:      return std::string("南");
		case WestWind:       return std::string("西");
		case NorthWind:      return std::string("北");
		case WhiteDragon:    return std::string("白");
		case GreenDragon:    return std::string("發");
		case RedDragon:      return std::string("中");
		default: throw std::domain_error("TileName(): 異常な引数です");
	}
}
__declspec(dllexport) void TileName(char* str, int tile) {
	std::strcpy(str, TileName((tileCode)tile).c_str());
}

/* 場風牌のリスト */
__declspec(dllexport) inline int WindMax() {return 7;}
tileCode inline Wind2Tile(uint8_t wind) {
	switch (wind) {
		case 0: return EastWind;
		case 1: return SouthWind;
		case 2: return WestWind;
		case 3: return NorthWind;
		case 4: return WhiteDragon;
		case 5: return GreenDragon;
		case 6: return RedDragon;
		default: throw std::domain_error("Wind2Tile(): 異常な引数です");
	}
}
__declspec(dllexport) int Wind2Tile(int wind) {
	return Wind2Tile((uint8_t)wind);
}
