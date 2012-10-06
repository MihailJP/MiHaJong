#include "func.h"

/* サイコロの出目を取得 */
extern "C" inline uint8_t diceSum(const GameTable* const gameStat) {
	return (gameStat->Dice[0].Number + gameStat->Dice[1].Number);
}

/* プレイヤーの自風がどれか調べる */
seatAbsolute inline playerwind(const GameTable* const gameStat, PLAYER_ID player, int currentRound) {
	if (chkGameType(gameStat, SanmaT))
		return (seatAbsolute)((player + 24 - (currentRound - ( currentRound / 4))) % 3);
	else return (seatAbsolute)((player + 32 - currentRound) % 4);
}
__declspec(dllexport) inline int playerwind(int player, int currentRound) {
	return (int)playerwind(&GameStat, (PLAYER_ID)player, (int)currentRound);
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
	if (RuleData::chkRule("sudden_death_type", "continue_into_white")) return 28;
	else return 16;
}

/* 王牌を除いた山牌の残り枚数 */
__declspec(dllexport) inline int tilesLeft(const GameTable* const gameStat) {
	return ((int)gameStat->RinshanPointer -
		((int)gameStat->DeadTiles - 1) -
		(int)gameStat->TilePointer);
}

/* 順位を計算する */
PlayerRankList calcRank(const GameTable* const gameStat) {
	PlayerRankList rankList;
	memset(&rankList, 0, sizeof(rankList));
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		rankList[i] = 1;
		for (int j = 0; j < ACTUAL_PLAYERS; j++) {
			if ((LargeNum)gameStat->Player[j].PlayerScore > gameStat->Player[i].PlayerScore)
				rankList[i]++;
			if (((LargeNum)gameStat->Player[j].PlayerScore == gameStat->Player[i].PlayerScore) &&
				(i > j)) rankList[i]++; // 同着の場合は起家に近い側が上位
		}
	}
	return rankList;
}
__declspec(dllexport) void calcRank(int* const Rank, const GameTable* const gameStat) {
	const PlayerRankList rankList = calcRank(gameStat);
	assert(rankList[0] > 0); assert(rankList[1] > 0);
	for (int i = 0; i < PLAYERS; i++) *(Rank + i) = rankList[i];
}

/* 包かどうかの判定 */
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

/* ロンしたプレイヤーの数 */
__declspec(dllexport) int RonPlayers(const GameTable* const gameStat) {
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "異常な引数です");
			return std::string("????"); break;
	}
}
__declspec(dllexport) void windName(char* str, int bufsz, int wind) {
	strcpy_s(str, bufsz, windName((seatAbsolute)wind).c_str());
}

/* 「東○局」などの文字列を返す */
std::string inline roundName(int roundNum, const GameTable* const gameStat) {
	std::ostringstream roundNameTxt; roundNameTxt.str("");
	switch (roundNum / PLAYERS) {
		case 0: roundNameTxt << "東"; break;
		case 1: roundNameTxt << "南"; break;
		case 2: roundNameTxt << "西"; break;
		case 3: roundNameTxt << "北"; break;
		case 4: roundNameTxt << "白"; break;
		case 5: roundNameTxt << "発"; break;
		case 6: roundNameTxt << "中"; break;
		default: 
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "異常な引数です。場風を解析できません。");
			roundNameTxt << "??";
	}
	if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game")) {
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
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "異常な引数です。同一場の5局目以降です。");
				roundNameTxt << "??局"; break;
		}
	}
	return std::string(roundNameTxt.str());
}
__declspec(dllexport) void roundName(char* str, int bufsz, int roundNum) {
	strcpy_s(str, bufsz, roundName(roundNum, &GameStat).c_str());
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
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "異常な引数です");
			return std::string("????");
	}
}
__declspec(dllexport) void TileName(char* str, int bufsz, int tile) {
	strcpy_s(str, bufsz, TileName((tileCode)tile).c_str());
}

/* 場風牌のリスト */
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "異常な引数です");
			return NoTile;
	}
}

/* 原点(返し点) */
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

/* 浮いているか判定する関数 */
bool isAboveBase(const GameTable* const gameStat, PLAYER_ID player) {
	return gameStat->Player[player].PlayerScore >= LargeNum::fromInt(BasePoint());
}
__declspec(dllexport) int isAboveBase(const GameTable* const gameStat, int player) {
	return isAboveBase(gameStat, (PLAYER_ID)player) ? 1 : 0;
}

/* 非負整数1桁なら全角・それ以外は半角 */
std::string intstr(int val) {
	const char* hanstr = "０\0１\0２\0３\0４\0５\0６\0７\0８\0９\0";
	std::ostringstream o;
	if ((val <= 9) && (val >= 0)) o << &(hanstr[val * 3]);
	else o << val;
	return o.str();
}

namespace confpath {
	using std::string;

	/* Vista/7を使っているかどうか */
	bool isVista() {
		OSVERSIONINFO versionInfo;
		versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&versionInfo) == 0) return false;
		return ((versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)&&(versionInfo.dwMajorVersion >= 6));
	}

	/* コンフィグのパスを自動設定 */
	/* Vista以降でRoamingに準備できていなければ作る */
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

/* リーチするのに持ち点が足りているかどうか */
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
