#include "func.h"

TileCount countTilesInHand(GameTable* gameStat, PLAYER_ID playerID) {
	// ��v�ɑ��݂���v����ޕʂɃJ�E���g����i���ʎq�E�ÞȂ͏����j
	TileCount count = TileCount(); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = tileCode(gameStat->Player[playerID].Hand[i].tile)) != tileCode::NoTile)
			count[tmpTC]++;
	}
	return TileCount(count);
}

/* -------------------------------------------------------------------------
 *  function.hsp ����ڐA
 * -------------------------------------------------------------------------
 */

/* �v���C���[�̎������ǂꂩ���ׂ� */
seatAbsolute inline playerwind(GameTable* gameStat, PLAYER_ID player, PLAYER_ID currentRound) {
	if (chkGameType(gameStat, SanmaT))
		return (seatAbsolute)((player + 24 - (currentRound - ( currentRound / 4))) % 3);
	else return (seatAbsolute)((player + 32 - currentRound) % 4);
}
__declspec(dllexport) inline int playerwind(int player, int currentRound) {
	return (int)playerwind(&GameStat, (PLAYER_ID)player, (PLAYER_ID)currentRound);
}

/* ����v���C���[�ɑ΂��Ďw�肵���v���C���[���ǂ�(���ƁA�ΖʁA���)�ɂ��邩���ׂ� */
seatRelative inline playerRelative(PLAYER_ID targetPlayer, PLAYER_ID basePlayer) {
	return (seatRelative)((PLAYERS + targetPlayer - basePlayer) % PLAYERS);
}
__declspec(dllexport) inline int playerRelative(int targetPlayer, int basePlayer) {
	return (int)playerRelative((PLAYER_ID)targetPlayer, (PLAYER_ID)basePlayer);
}

/* ����v���C���[��(���ƁA�ΖʁA���)�𒲂ׂ� */
PLAYER_ID inline RelativePositionOf(PLAYER_ID targetPlayer, seatRelative relative) {
	return (PLAYER_ID)(((int)targetPlayer + (int)relative) % PLAYERS);
}
__declspec(dllexport) inline int RelativePositionOf(int targetPlayer, int relative) {
	return (int)RelativePositionOf((PLAYER_ID)targetPlayer, (seatRelative)relative);
}

/* �������܂łɕK�v�ȏ�̐� */
__declspec(dllexport) inline int roundLoopRate() {
	if (getRule(RULE_SUDDEN_DEATH_TYPE) == 1) return 28;
	else return 16;
}

/* ���v���������R�v�̎c�薇�� */
__declspec(dllexport) inline int tilesLeft(GameTable* gameStat) {
	return ((int)gameStat->RinshanPointer -
		((int)gameStat->DeadTiles - 1) -
		(int)gameStat->TilePointer);
}

/* ���ʂ��v�Z���� */
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

/* ��ǂ����̔��� */
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

/* ���������v���C���[�̐� */
__declspec(dllexport) int RonPlayers(GameTable* gameStat) {
	int qualified = 0;
	for (int i = 0; i < PLAYERS; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron)
			qualified++;
	return qualified;
}

/* �u���Ɓv�u��Ɓv�u���Ɓv�u�k�Ɓv�̕������Ԃ� */
std::string inline windName(seatAbsolute wind) {
	switch (wind) {
		case sEast: return std::string("����");
		case sSouth: return std::string("���");
		case sWest: return std::string("����");
		case sNorth: return std::string("�k��");
		default:
			throw std::domain_error("windName(): �ُ�Ȉ����ł�");
	}
}
__declspec(dllexport) void windName(char* str, int wind) {
	std::strcpy(str, windName((seatAbsolute)wind).c_str());
}

/* �u�����ǁv�Ȃǂ̕������Ԃ� */
std::string inline roundName(int roundNum, GameTable* gameStat) {
	std::ostringstream roundNameTxt; roundNameTxt.str("");
	switch (roundNum / PLAYERS) {
		case 0: roundNameTxt << "��"; break;
		case 1: roundNameTxt << "��"; break;
		case 2: roundNameTxt << "��"; break;
		case 3: roundNameTxt << "�k"; break;
		case 4: roundNameTxt << "��"; break;
		case 5: roundNameTxt << "��"; break;
		case 6: roundNameTxt << "��"; break;
		default: throw std::domain_error("roundName(): �ُ�Ȉ����ł��B�ꕗ����͂ł��܂���B");
	}
	if ((getRule(RULE_GAME_LENGTH) == 5)||(getRule(RULE_GAME_LENGTH) == 7)) {
		switch (int k = (gameStat->LoopRound * ACTUAL_PLAYERS + roundNum % PLAYERS)) {
			case 0: roundNameTxt << "���"; break;
			case 1: roundNameTxt << "���"; break;
			case 2: roundNameTxt << "�O��"; break;
			case 3: roundNameTxt << "�l��"; break;
			case 4: roundNameTxt << "�܋�"; break;
			case 5: roundNameTxt << "�Z��"; break;
			case 6: roundNameTxt << "����"; break;
			case 7: roundNameTxt << "����"; break;
			case 8: roundNameTxt << "���"; break;
			case 9: roundNameTxt << "�\��"; break;
			default: roundNameTxt << (k+1) << "��"; break;
		}
	} else {
		switch (roundNum % PLAYERS) {
			case 0: roundNameTxt << "���"; break;
			case 1: roundNameTxt << "���"; break;
			case 2: roundNameTxt << "�O��"; break;
			case 3: roundNameTxt << "�l��"; break;
			default: throw std::domain_error("roundName(): �ُ�Ȉ����ł��B������5�ǖڈȍ~�ł��B");
		}
	}
	return std::string(roundNameTxt.str());
}
__declspec(dllexport) void roundName(char* str, int roundNum) {
	std::strcpy(str, roundName(roundNum, &GameStat).c_str());
}

/* �v�̖��O�̕������Ԃ� */
std::string inline TileName(tileCode tile) {
	switch (tile) {
		case CharacterOne:   return std::string("����");
		case CharacterTwo:   return std::string("����");
		case CharacterThree: return std::string("�O��");
		case CharacterFour:  return std::string("�l��");
		case CharacterFive:  return std::string("����");
		case CharacterSix:   return std::string("�Z��");
		case CharacterSeven: return std::string("����");
		case CharacterEight: return std::string("����");
		case CharacterNine:  return std::string("����");
		case CircleOne:      return std::string("�ꓛ");
		case CircleTwo:      return std::string("��");
		case CircleThree:    return std::string("�O��");
		case CircleFour:     return std::string("�l��");
		case CircleFive:     return std::string("�ܓ�");
		case CircleSix:      return std::string("�Z��");
		case CircleSeven:    return std::string("����");
		case CircleEight:    return std::string("����");
		case CircleNine:     return std::string("�㓛");
		case BambooOne:      return std::string("���");
		case BambooTwo:      return std::string("���");
		case BambooThree:    return std::string("�O��");
		case BambooFour:     return std::string("�l��");
		case BambooFive:     return std::string("�܍�");
		case BambooSix:      return std::string("�Z��");
		case BambooSeven:    return std::string("����");
		case BambooEight:    return std::string("����");
		case BambooNine:     return std::string("���");
		case EastWind:       return std::string("��");
		case SouthWind:      return std::string("��");
		case WestWind:       return std::string("��");
		case NorthWind:      return std::string("�k");
		case WhiteDragon:    return std::string("��");
		case GreenDragon:    return std::string("�");
		case RedDragon:      return std::string("��");
		default: throw std::domain_error("TileName(): �ُ�Ȉ����ł�");
	}
}
__declspec(dllexport) void TileName(char* str, int tile) {
	std::strcpy(str, TileName((tileCode)tile).c_str());
}

/* �ꕗ�v�̃��X�g */
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
		default: throw std::domain_error("Wind2Tile(): �ُ�Ȉ����ł�");
	}
}
__declspec(dllexport) int Wind2Tile(int wind) {
	return Wind2Tile((uint8_t)wind);
}
