#include "func.h"

#include <cassert>
#include <memory>
#include <cstring>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#include <imagehlp.h>
#include <direct.h>
#else /*_WIN32*/
#include <sys/stat.h>
#endif /*_WIN32*/
#include "../common/strcode.h"
#include "../sound/sound.h"
#include "../socket/socket.h"
#include "socknum.h"
#include "except.h"
#include "ruletbl.h"
#include "largenum.h"

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
bool isPaoAgari(const GameTable* const gameStat, PlayerID agariPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PaoYakuPages; i++) {
		if (agariPlayer == gameStat->PaoFlag[i].agariPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
bool isGotPao(const GameTable* const gameStat, PlayerID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PaoYakuPages; i++) {
		if (paoPlayer == gameStat->PaoFlag[i].paoPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
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

/* ロンしたプレイヤーの数 */
int RonPlayers(const GameTable* const gameStat) {
	int qualified = 0;
	for (int i = 0; i < Players; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron)
			qualified++;
	return qualified;
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
	if (GameStat.chkGameType(SanmaT)) {
		const char rulestat[6][16] = {
			"35000pts_oka15", "40000pts_oka0", "45000pts_oka-15", "50000pts_oka-30", "25000pts_oka45", "30000pts_oka30",
		};
		for (int i = 0; i < 6; i++)
			if (RuleData::chkRule("starting_point", rulestat[i]))
				return 40000;
	}
	return 30000;
}

/* 浮いているか判定する関数 */
bool isAboveBase(const GameTable* const gameStat, PlayerID player) {
	return gameStat->Player[player].PlayerScore >= (LNum)BasePoint();
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
#ifdef _WIN32
	bool isVista() {
		OSVERSIONINFO versionInfo;
		versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&versionInfo) == 0) return false;
		return ((versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)&&(versionInfo.dwMajorVersion >= 6));
	}
#else /*_WIN32*/
	bool isVista() { /* Windowsじゃない */
		return false;
	}
#endif /*_WIN32*/

	/* コンフィグのパスを自動設定 */
	/* Vista以降でRoamingに準備できていなければ作る */
	std::string confPath() {
		std::string configpath = "";
#ifdef _WIN32
		if (isVista()) {
			char* cur = new char[1024];
			GetCurrentDirectoryA(1024, cur);
			char* progfiles = new char[1024];
			char* appdata = new char[1024];
#if defined(_MSC_VER)
			size_t* sz = new size_t;
			getenv_s(sz, progfiles, 1024, "ProgramFiles");
			getenv_s(sz, appdata, 1024, "APPDATA");
#else
			strncpy(progfiles, getenv("ProgramFiles"), 1023);
			strncpy(appdata,   getenv("APPDATA"),      1023);
			progfiles[1023] = appdata[1023] = '\0';
#endif

			if (strstr(cur, progfiles) == cur) {
				// MakeSureDirectoryPathExistsがワイド文字対応してないので仕方なくANSI文字版
				MakeSureDirectoryPathExists((std::string(appdata) + std::string("\\MiHaJong\\haifu\\")).c_str());
				CopyFileA(".\\haifu\\haifu.css",
					(std::string(appdata) + std::string("\\MiHaJong\\haifu\\haifu.css")).c_str(),
					TRUE);
				CopyFileA(".\\haifu\\haifu.dtd",
					(std::string(appdata) + std::string("\\MiHaJong\\haifu\\haifu.dtd")).c_str(),
					TRUE);
				CopyFileA(".\\haifu\\haifu.xsd",
					(std::string(appdata) + std::string("\\MiHaJong\\haifu\\haifu.xsd")).c_str(),
					TRUE);
				CopyFileA(".\\haifu\\haifu.xsl",
					(std::string(appdata) + std::string("\\MiHaJong\\haifu\\haifu.xsl")).c_str(),
					TRUE);
				MakeSureDirectoryPathExists((std::string(appdata) + std::string("\\MiHaJong\\ai\\")).c_str());
				CopyFileA(".\\ai\\default.lua",
					(std::string(appdata) + std::string("\\MiHaJong\\ai\\default.lua")).c_str(),
					TRUE);
				configpath = std::string(appdata) + std::string("\\MiHaJong\\");
			}
			
			delete[] cur; delete[] appdata; delete[] progfiles;
#if defined(_MSC_VER)
			delete sz;
#endif
		}
#else /*_WIN32*/
		/* TODO: 未実装箇所 */
		/* ~/.mihajong あたりが妥当か。環境変数を参照せよ */
		std::string homedir(getenv("HOME"));
		configpath = homedir + std::string("/.mihajong");
		mkdir(configpath.c_str(), 0755);
		configpath += std::string("/");
#endif /*_WIN32*/
		return configpath;
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
