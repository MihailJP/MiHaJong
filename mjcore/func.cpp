#include "func.h"

/* �T�C�R���̏o�ڂ��擾 */
extern "C" inline uint8_t diceSum(const GameTable* const gameStat) {
	return (gameStat->Dice[0].Number + gameStat->Dice[1].Number);
}

/* �v���C���[�̎������ǂꂩ���ׂ� */
seatAbsolute inline playerwind(const GameTable* const gameStat, PLAYER_ID player, int currentRound) {
	if (chkGameType(gameStat, SanmaT))
		return (seatAbsolute)((player + 24 - (currentRound - ( currentRound / 4))) % 3);
	else return (seatAbsolute)((player + 32 - currentRound) % 4);
}
__declspec(dllexport) inline int playerwind(int player, int currentRound) {
	return (int)playerwind(&GameStat, (PLAYER_ID)player, (int)currentRound);
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
	if (RuleData::chkRule("sudden_death_type", "continue_into_white")) return 28;
	else return 16;
}

/* ���v���������R�v�̎c�薇�� */
__declspec(dllexport) inline int tilesLeft(const GameTable* const gameStat) {
	return ((int)gameStat->RinshanPointer -
		((int)gameStat->DeadTiles - 1) -
		(int)gameStat->TilePointer);
}

/* ���ʂ��v�Z���� */
PlayerRankList calcRank(const GameTable* const gameStat) {
	PlayerRankList rankList;
	memset(&rankList, 0, sizeof(rankList));
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		rankList[i] = 1;
		for (int j = 0; j < ACTUAL_PLAYERS; j++) {
			if ((LargeNum)gameStat->Player[j].PlayerScore > gameStat->Player[i].PlayerScore)
				rankList[i]++;
			if (((LargeNum)gameStat->Player[j].PlayerScore == gameStat->Player[i].PlayerScore) &&
				(i > j)) rankList[i]++; // �����̏ꍇ�͋N�Ƃɋ߂��������
		}
	}
	return rankList;
}
__declspec(dllexport) void calcRank(int* const Rank, const GameTable* const gameStat) {
	const PlayerRankList rankList = calcRank(gameStat);
	assert(rankList[0] > 0); assert(rankList[1] > 0);
	for (int i = 0; i < PLAYERS; i++) *(Rank + i) = rankList[i];
}

/* ��ǂ����̔��� */
bool isPao(const GameTable* const gameStat, PLAYER_ID agariPlayer, PLAYER_ID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if ((paoPlayer == gameStat->PaoFlag[i].paoPlayer) &&
			(agariPlayer == gameStat->PaoFlag[i].agariPlayer))
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isPao(const GameTable* const gameStat, int agariPlayer, int paoPlayer) {
	return (int)isPao(gameStat, (PLAYER_ID)agariPlayer, (PLAYER_ID)paoPlayer);
}

bool isPaoAgari(const GameTable* const gameStat, PLAYER_ID agariPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if (agariPlayer == gameStat->PaoFlag[i].agariPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isPaoAgari(const GameTable* const gameStat, int agariPlayer) {
	return (int)isPaoAgari(gameStat, (PLAYER_ID)agariPlayer);
}

bool isGotPao(const GameTable* const gameStat, PLAYER_ID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if (paoPlayer == gameStat->PaoFlag[i].paoPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isGotPao(const GameTable* const gameStat, int paoPlayer) {
	return (int)isGotPao(gameStat, (PLAYER_ID)paoPlayer);
}

PLAYER_ID getPaoPlayer(const GameTable* const gameStat, PLAYER_ID agariPlayer) {
	PLAYER_ID paoPlayer = -1;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if ((gameStat->PaoFlag[i].paoPlayer >= 0) &&
			(gameStat->PaoFlag[i].agariPlayer == agariPlayer))
			paoPlayer = gameStat->PaoFlag[i].paoPlayer;
	}
	return paoPlayer;
}
__declspec(dllexport) int getPaoPlayer(const GameTable* const gameStat, int agariPlayer) {
	return (int)getPaoPlayer(gameStat, (PLAYER_ID)agariPlayer);
}

/* ���������v���C���[�̐� */
__declspec(dllexport) int RonPlayers(const GameTable* const gameStat) {
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�ُ�Ȉ����ł�");
			return std::string("????"); break;
	}
}
__declspec(dllexport) void windName(char* str, int bufsz, int wind) {
	strcpy_s(str, bufsz, windName((seatAbsolute)wind).c_str());
}

/* �u�����ǁv�Ȃǂ̕������Ԃ� */
std::string inline roundName(int roundNum, const GameTable* const gameStat) {
	std::ostringstream roundNameTxt; roundNameTxt.str("");
	switch (roundNum / PLAYERS) {
		case 0: roundNameTxt << "��"; break;
		case 1: roundNameTxt << "��"; break;
		case 2: roundNameTxt << "��"; break;
		case 3: roundNameTxt << "�k"; break;
		case 4: roundNameTxt << "��"; break;
		case 5: roundNameTxt << "��"; break;
		case 6: roundNameTxt << "��"; break;
		default: 
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�ُ�Ȉ����ł��B�ꕗ����͂ł��܂���B");
			roundNameTxt << "??";
	}
	if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game")) {
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
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�ُ�Ȉ����ł��B������5�ǖڈȍ~�ł��B");
				roundNameTxt << "??��"; break;
		}
	}
	return std::string(roundNameTxt.str());
}
__declspec(dllexport) void roundName(char* str, int bufsz, int roundNum) {
	strcpy_s(str, bufsz, roundName(roundNum, &GameStat).c_str());
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
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�ُ�Ȉ����ł�");
			return std::string("????");
	}
}
__declspec(dllexport) void TileName(char* str, int bufsz, int tile) {
	strcpy_s(str, bufsz, TileName((tileCode)tile).c_str());
}

/* �ꕗ�v�̃��X�g */
tileCode Wind2Tile(uint8_t wind) {
	switch (wind) {
		case 0: return EastWind;
		case 1: return SouthWind;
		case 2: return WestWind;
		case 3: return NorthWind;
		case 4: return WhiteDragon;
		case 5: return GreenDragon;
		case 6: return RedDragon;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�ُ�Ȉ����ł�");
			return NoTile;
	}
}

/* ���_(�Ԃ��_) */
int BasePoint() {
	if (chkGameType(&GameStat, SanmaT)) {
		const char rulestat[6][16] = {
			"35000pts_oka15", "40000pts_oka0", "45000pts_oka-15", "50000pts_oka-30", "25000pts_oka45", "30000pts_oka30",
		};
		for (int i = 0; i < 6; i++)
			if (RuleData::chkRule("starting_point", rulestat[i]))
				return 40000;
	}
	return 30000;
}
__declspec(dllexport) int BasePointHSP() {
	return BasePoint() / 100;
}

/* �����Ă��邩���肷��֐� */
bool isAboveBase(const GameTable* const gameStat, PLAYER_ID player) {
	return gameStat->Player[player].PlayerScore >= LargeNum::fromInt(BasePoint());
}
__declspec(dllexport) int isAboveBase(const GameTable* const gameStat, int player) {
	return isAboveBase(gameStat, (PLAYER_ID)player) ? 1 : 0;
}

/* �񕉐���1���Ȃ�S�p�E����ȊO�͔��p */
std::string intstr(int val) {
	const char* hanstr = "�O\0�P\0�Q\0�R\0�S\0�T\0�U\0�V\0�W\0�X\0";
	std::ostringstream o;
	if ((val <= 9) && (val >= 0)) o << &(hanstr[val * 3]);
	else o << val;
	return o.str();
}

namespace confpath {
	using std::string;

	/* Vista/7���g���Ă��邩�ǂ��� */
	bool isVista() {
		OSVERSIONINFO versionInfo;
		versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&versionInfo) == 0) return false;
		return ((versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)&&(versionInfo.dwMajorVersion >= 6));
	}

	/* �R���t�B�O�̃p�X�������ݒ� */
	/* Vista�ȍ~��Roaming�ɏ����ł��Ă��Ȃ���΍�� */
	string confPath() {
		string configpath = "";
		if (isVista()) {
			char* cur = new char[1024];
			GetCurrentDirectory(1024, cur);
			char* progfiles = new char[1024];
			char* appdata = new char[1024];
			size_t* sz = new size_t;
			getenv_s(sz, progfiles, 1024, "ProgramFiles");
			getenv_s(sz, appdata, 1024, "APPDATA");

			if (strstr(cur, progfiles) == cur) {
				MakeSureDirectoryPathExists(((string)appdata + (string)"\\MiHaJong\\haifu\\").c_str());
				CopyFile(".\\haifu\\haifu.css",
					((string)appdata + (string)"\\MiHaJong\\haifu\\haifu.css").c_str(),
					TRUE);
				MakeSureDirectoryPathExists(((string)appdata + (string)"\\MiHaJong\\ai\\").c_str());
				CopyFile(".\\ai\\default.lua",
					((string)appdata + (string)"\\MiHaJong\\ai\\default.lua").c_str(),
					TRUE);
				configpath = (string)appdata + (string)"\\MiHaJong\\";
			}
			
			delete[] cur; delete[] appdata; delete[] progfiles; delete sz;
		}
		return configpath;
	}
	__declspec(dllexport) void confPath(char* path, int bufsz) {
		strcpy_s(path, bufsz, confPath().c_str());
	}

}

/* ���[�`����̂Ɏ����_������Ă��邩�ǂ��� */
bool isRichiReqSatisfied (const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	bool Flag = true;
	if (gameStat->Player[targetPlayer].PlayerScore < LargeNum::fromInt(1000)) Flag = false;
	else if ((gameStat->Player[targetPlayer].PlayerScore == LargeNum::fromInt(1000)) &&
		(RuleData::chkRule("riichi_requisite", "require_1100"))) Flag = false;
	if (RuleData::chkRule("riichi_requisite", "no")) Flag = true;
	if (RuleData::chkRule("buttobi_border", "no")) Flag = true;
	return Flag;
}
__declspec(dllexport) int isRichiReqSatisfied (const GameTable* const gameStat, int targetPlayer) {
	return isRichiReqSatisfied(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}
