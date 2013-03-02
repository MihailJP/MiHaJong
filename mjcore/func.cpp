#include "func.h"

#include <cassert>
#include <memory>
#include <cstring>
#include <sstream>
#include <Windows.h>
#include <ImageHlp.h>
#include <direct.h>
#include "../common/strcode.h"
#include "../sound/sound.h"
#include "../socket/socket.h"
#include "socknum.h"
#include "except.h"
#include "ruletbl.h"
#include "largenum.h"

/* サイコロの出目を取得 */
extern "C" inline uint8_t diceSum(const GameTable* const gameStat) {
	return (gameStat->Dice[0].Number + gameStat->Dice[1].Number);
}

/* プレイヤーの自風がどれか調べる */
seatAbsolute inline playerwind(const GameTable* const gameStat, PlayerID player, int currentRound) {
	if (chkGameType(gameStat, SanmaT))
		return (seatAbsolute)((player + 24 - (currentRound - ( currentRound / 4))) % 3);
	else return (seatAbsolute)((player + 32 - currentRound) % 4);
}
__declspec(dllexport) inline int playerwind(int player, int currentRound) {
	return (int)playerwind(&GameStat, (PlayerID)player, (int)currentRound);
}

/* あるプレイヤーに対して指定したプレイヤーがどこ(下家、対面、上家)にいるか調べる */
seatRelative inline playerRelative(PlayerID targetPlayer, PlayerID basePlayer) {
	return (seatRelative)((Players + targetPlayer - basePlayer) % Players);
}
__declspec(dllexport) inline int playerRelative(int targetPlayer, int basePlayer) {
	return (int)playerRelative((PlayerID)targetPlayer, (PlayerID)basePlayer);
}

/* あるプレイヤーの(下家、対面、上家)を調べる */
PlayerID inline RelativePositionOf(PlayerID targetPlayer, seatRelative relative) {
	return (PlayerID)(((int)targetPlayer + (int)relative) % Players);
}
__declspec(dllexport) inline int RelativePositionOf(int targetPlayer, int relative) {
	return (int)RelativePositionOf((PlayerID)targetPlayer, (seatRelative)relative);
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
			if ((LNum)gameStat->Player[j].PlayerScore > gameStat->Player[i].PlayerScore)
				rankList[i]++;
			if (((LNum)gameStat->Player[j].PlayerScore == gameStat->Player[i].PlayerScore) &&
				(i > j)) rankList[i]++; // 同着の場合は起家に近い側が上位
		}
	}
	return rankList;
}
__declspec(dllexport) void calcRank(int* const Rank, const GameTable* const gameStat) {
	const PlayerRankList rankList = calcRank(gameStat);
	assert(rankList[0] > 0); assert(rankList[1] > 0);
	for (int i = 0; i < Players; i++) *(Rank + i) = rankList[i];
}

/* 包かどうかの判定 */
bool isPao(const GameTable* const gameStat, PlayerID agariPlayer, PlayerID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PaoYakuPages; i++) {
		if ((paoPlayer == gameStat->PaoFlag[i].paoPlayer) &&
			(agariPlayer == gameStat->PaoFlag[i].agariPlayer))
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isPao(const GameTable* const gameStat, int agariPlayer, int paoPlayer) {
	return (int)isPao(gameStat, (PlayerID)agariPlayer, (PlayerID)paoPlayer);
}

bool isPaoAgari(const GameTable* const gameStat, PlayerID agariPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PaoYakuPages; i++) {
		if (agariPlayer == gameStat->PaoFlag[i].agariPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isPaoAgari(const GameTable* const gameStat, int agariPlayer) {
	return (int)isPaoAgari(gameStat, (PlayerID)agariPlayer);
}

bool isGotPao(const GameTable* const gameStat, PlayerID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PaoYakuPages; i++) {
		if (paoPlayer == gameStat->PaoFlag[i].paoPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isGotPao(const GameTable* const gameStat, int paoPlayer) {
	return (int)isGotPao(gameStat, (PlayerID)paoPlayer);
}

PlayerID getPaoPlayer(const GameTable* const gameStat, PlayerID agariPlayer) {
	PlayerID paoPlayer = -1;
	for (int i = 0; i < PaoYakuPages; i++) {
		if ((gameStat->PaoFlag[i].paoPlayer >= 0) &&
			(gameStat->PaoFlag[i].agariPlayer == agariPlayer))
			paoPlayer = gameStat->PaoFlag[i].paoPlayer;
	}
	return paoPlayer;
}
__declspec(dllexport) int getPaoPlayer(const GameTable* const gameStat, int agariPlayer) {
	return (int)getPaoPlayer(gameStat, (PlayerID)agariPlayer);
}

/* ロンしたプレイヤーの数 */
__declspec(dllexport) int RonPlayers(const GameTable* const gameStat) {
	int qualified = 0;
	for (int i = 0; i < Players; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron)
			qualified++;
	return qualified;
}

/* 「東家」「南家」「西家」「北家」の文字列を返す */
CodeConv::tstring inline windName(seatAbsolute wind) {
	switch (wind) {
		case sEast: return CodeConv::tstring(_T("東家"));
		case sSouth: return CodeConv::tstring(_T("南家"));
		case sWest: return CodeConv::tstring(_T("西家"));
		case sNorth: return CodeConv::tstring(_T("北家"));
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です"));
			return CodeConv::tstring(_T("????")); break;
	}
}
__declspec(dllexport) void windName(LPTSTR str, int bufsz, int wind) {
	_tcscpy_s(str, bufsz, windName((seatAbsolute)wind).c_str());
}

/* 「東○局」などの文字列を返す */
CodeConv::tstring inline roundName(int roundNum, const GameTable* const gameStat) {
	CodeConv::tostringstream roundNameTxt; roundNameTxt.str(_T(""));
	switch (roundNum / Players) {
		case 0: roundNameTxt << _T("東"); break;
		case 1: roundNameTxt << _T("南"); break;
		case 2: roundNameTxt << _T("西"); break;
		case 3: roundNameTxt << _T("北"); break;
		case 4: roundNameTxt << _T("白"); break;
		case 5: roundNameTxt << _T("発"); break;
		case 6: roundNameTxt << _T("中"); break;
		default: 
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です。場風を解析できません。"));
			roundNameTxt << _T("??");
	}
	if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game")) {
		switch (int k = (gameStat->LoopRound * ACTUAL_PLAYERS + roundNum % Players)) {
			case 0: roundNameTxt << _T("一局"); break;
			case 1: roundNameTxt << _T("二局"); break;
			case 2: roundNameTxt << _T("三局"); break;
			case 3: roundNameTxt << _T("四局"); break;
			case 4: roundNameTxt << _T("五局"); break;
			case 5: roundNameTxt << _T("六局"); break;
			case 6: roundNameTxt << _T("七局"); break;
			case 7: roundNameTxt << _T("八局"); break;
			case 8: roundNameTxt << _T("九局"); break;
			case 9: roundNameTxt << _T("十局"); break;
			default: roundNameTxt << (k+1) << _T("局"); break;
		}
	} else {
		switch (roundNum % Players) {
			case 0: roundNameTxt << _T("一局"); break;
			case 1: roundNameTxt << _T("二局"); break;
			case 2: roundNameTxt << _T("三局"); break;
			case 3: roundNameTxt << _T("四局"); break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です。同一場の5局目以降です。"));
				roundNameTxt << _T("??局"); break;
		}
	}
	return CodeConv::tstring(roundNameTxt.str());
}
__declspec(dllexport) void roundName(LPTSTR str, int bufsz, int roundNum) {
	_tcscpy_s(str, bufsz, roundName(roundNum, &GameStat).c_str());
}

/* 牌の名前の文字列を返す */
CodeConv::tstring inline TileName(TileCode tile) {
	switch (tile) {
		case CharacterOne:   return CodeConv::tstring(_T("一萬"));
		case CharacterTwo:   return CodeConv::tstring(_T("二萬"));
		case CharacterThree: return CodeConv::tstring(_T("三萬"));
		case CharacterFour:  return CodeConv::tstring(_T("四萬"));
		case CharacterFive:  return CodeConv::tstring(_T("五萬"));
		case CharacterSix:   return CodeConv::tstring(_T("六萬"));
		case CharacterSeven: return CodeConv::tstring(_T("七萬"));
		case CharacterEight: return CodeConv::tstring(_T("八萬"));
		case CharacterNine:  return CodeConv::tstring(_T("九萬"));
		case CircleOne:      return CodeConv::tstring(_T("一筒"));
		case CircleTwo:      return CodeConv::tstring(_T("二筒"));
		case CircleThree:    return CodeConv::tstring(_T("三筒"));
		case CircleFour:     return CodeConv::tstring(_T("四筒"));
		case CircleFive:     return CodeConv::tstring(_T("五筒"));
		case CircleSix:      return CodeConv::tstring(_T("六筒"));
		case CircleSeven:    return CodeConv::tstring(_T("七筒"));
		case CircleEight:    return CodeConv::tstring(_T("八筒"));
		case CircleNine:     return CodeConv::tstring(_T("九筒"));
		case BambooOne:      return CodeConv::tstring(_T("一索"));
		case BambooTwo:      return CodeConv::tstring(_T("二索"));
		case BambooThree:    return CodeConv::tstring(_T("三索"));
		case BambooFour:     return CodeConv::tstring(_T("四索"));
		case BambooFive:     return CodeConv::tstring(_T("五索"));
		case BambooSix:      return CodeConv::tstring(_T("六索"));
		case BambooSeven:    return CodeConv::tstring(_T("七索"));
		case BambooEight:    return CodeConv::tstring(_T("八索"));
		case BambooNine:     return CodeConv::tstring(_T("九索"));
		case EastWind:       return CodeConv::tstring(_T("東"));
		case SouthWind:      return CodeConv::tstring(_T("南"));
		case WestWind:       return CodeConv::tstring(_T("西"));
		case NorthWind:      return CodeConv::tstring(_T("北"));
		case WhiteDragon:    return CodeConv::tstring(_T("白"));
		case GreenDragon:    return CodeConv::tstring(_T("發"));
		case RedDragon:      return CodeConv::tstring(_T("中"));
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です"));
			return CodeConv::tstring(_T("????"));
	}
}
__declspec(dllexport) void TileName(LPTSTR str, int bufsz, int tile) {
	_tcscpy_s(str, bufsz, TileName((TileCode)tile).c_str());
}

/* 場風牌のリスト */
TileCode Wind2Tile(uint8_t wind) {
	switch (wind) {
		case 0: return EastWind;
		case 1: return SouthWind;
		case 2: return WestWind;
		case 3: return NorthWind;
		case 4: return WhiteDragon;
		case 5: return GreenDragon;
		case 6: return RedDragon;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です"));
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
bool isAboveBase(const GameTable* const gameStat, PlayerID player) {
	return gameStat->Player[player].PlayerScore >= (LNum)BasePoint();
}
__declspec(dllexport) int isAboveBase(const GameTable* const gameStat, int player) {
	return isAboveBase(gameStat, (PlayerID)player) ? 1 : 0;
}

/* 非負整数1桁なら全角・それ以外は半角 */
CodeConv::tstring intstr(int val) {
	LPCTSTR hanstr[10] = {
		_T("０"), _T("１"), _T("２"), _T("３"), _T("４"),
		_T("５"), _T("６"), _T("７"), _T("８"), _T("９"),
	};
	CodeConv::tostringstream o;
	if ((val <= 9) && (val >= 0)) o << hanstr[val];
	else o << val;
	return o.str();
}

namespace confpath {
	using CodeConv::tstring;

	/* Vista/7を使っているかどうか */
	bool isVista() {
		OSVERSIONINFO versionInfo;
		versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&versionInfo) == 0) return false;
		return ((versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)&&(versionInfo.dwMajorVersion >= 6));
	}

	/* コンフィグのパスを自動設定 */
	/* Vista以降でRoamingに準備できていなければ作る */
	std::string confPath() {
		std::string configpath = "";
		if (isVista()) {
			char* cur = new char[1024];
			GetCurrentDirectoryA(1024, cur);
			char* progfiles = new char[1024];
			char* appdata = new char[1024];
			size_t* sz = new size_t;
			getenv_s(sz, progfiles, 1024, "ProgramFiles");
			getenv_s(sz, appdata, 1024, "APPDATA");

			if (strstr(cur, progfiles) == cur) {
				// MakeSureDirectoryPathExistsがワイド文字対応してないので仕方なくANSI文字版
				MakeSureDirectoryPathExists((std::string(appdata) + std::string("\\MiHaJong\\haifu\\")).c_str());
				CopyFileA(".\\haifu\\haifu.css",
					(std::string(appdata) + std::string("\\MiHaJong\\haifu\\haifu.css")).c_str(),
					TRUE);
				MakeSureDirectoryPathExists((std::string(appdata) + std::string("\\MiHaJong\\ai\\")).c_str());
				CopyFileA(".\\ai\\default.lua",
					(std::string(appdata) + std::string("\\MiHaJong\\ai\\default.lua")).c_str(),
					TRUE);
				configpath = std::string(appdata) + std::string("\\MiHaJong\\");
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
bool isRichiReqSatisfied (const GameTable* const gameStat, PlayerID targetPlayer) {
	bool Flag = true;
	if (gameStat->Player[targetPlayer].PlayerScore < (LNum)1000) Flag = false;
	else if ((gameStat->Player[targetPlayer].PlayerScore == (LNum)1000) &&
		(RuleData::chkRule("riichi_requisite", "require_1100"))) Flag = false;
	if (RuleData::chkRule("riichi_requisite", "no")) Flag = true;
	if (RuleData::chkRule("buttobi_border", "no")) Flag = true;
	return Flag;
}
__declspec(dllexport) int isRichiReqSatisfied (const GameTable* const gameStat, int targetPlayer) {
	return isRichiReqSatisfied(gameStat, (PlayerID)targetPlayer) ? 1 : 0;
}

/* 飛びになっているかどうか */
bool isDobon (const GameTable* const gameStat, PlayerID targetPlayer) {
	if (!RuleData::chkRuleApplied("buttobi_border"))
		return false;
	else if (gameStat->Player[targetPlayer].PlayerScore < (LNum)0)
		return true;
	else if ((gameStat->Player[targetPlayer].PlayerScore == (LNum)0) &&
		RuleData::chkRule("buttobi_border", "nonpositive"))
		return true;
	else return false;
}
__declspec(dllexport) int isDobon (const GameTable* const gameStat, int targetPlayer) {
	return isDobon(gameStat, (PlayerID)targetPlayer) ? 1 : 0;
}

/* 天辺になっているかどうか */
bool isTeppen (const GameTable* const gameStat, PlayerID targetPlayer) {
	if (RuleData::chkRule("teppen", "50000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= (LNum)50000))
		return true;
	else if (RuleData::chkRule("teppen", "55000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= (LNum)55000))
		return true;
	else if (RuleData::chkRule("teppen", "60000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= (LNum)60000))
		return true;
	else if (RuleData::chkRule("teppen", "65000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= (LNum)65000))
		return true;
	else if (RuleData::chkRule("teppen", "70000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= (LNum)70000))
		return true;
	else return false;
}
__declspec(dllexport) int isTeppen (const GameTable* const gameStat, int targetPlayer) {
	return isTeppen(gameStat, (PlayerID)targetPlayer) ? 1 : 0;
}

MJCORE void cleanup() {
	sound::Cleanup();
	info(_T("サウンドDLLを解放しました。"));
	for (int i = 0; i < Players; i++) {
		mihajong_socket::hangup(SOCK_GAME + i);
		mihajong_socket::hangup(SOCK_CHAT + i);
	}
	mihajong_socket::bye();
	info(_T("ソケットDLLを解放しました。"));
}
