#include "func.h"

#include <cassert>
#include <memory>
#include <cstring>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#include <imagehlp.h>
#include <direct.h>
#include <VersionHelpers.h>
#else /*_WIN32*/
#include <sys/stat.h>
#include <unistd.h>
#endif /*_WIN32*/
#include "../common/strcode.h"
#include "../common/largenum.h"
#include "../sound/sound.h"
#include "../socket/socket.h"
#include "socknum.h"
#include "except.h"
#include "ruletbl.h"
#include <regex>
#include "conffile/conffile.h"
#include "../graphic/graphic.h"

/* 順位を計算する */
PlayerRankList calcRank(const GameTable* const gameStat) {
	PlayerRankList rankList;
	memset(&rankList, 0, sizeof(rankList));
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		rankList[i] = 1;
		for (int j = 0; j < ACTUAL_PLAYERS; j++) {
			if (gameStat->Player[j].PlayerScore > gameStat->Player[i].PlayerScore)
				rankList[i]++;
			if ((gameStat->Player[j].PlayerScore == gameStat->Player[i].PlayerScore) &&
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
TileCode Wind2Tile(SeatAbsolute wind) {
	switch (wind) {
		case SeatAbsolute::east:  return TileCode::eastWind;
		case SeatAbsolute::south: return TileCode::southWind;
		case SeatAbsolute::west:  return TileCode::westWind;
		case SeatAbsolute::north: return TileCode::northWind;
		case SeatAbsolute::white: return TileCode::whiteDragon;
		case SeatAbsolute::green: return TileCode::greenDragon;
		case SeatAbsolute::red:   return TileCode::redDragon;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です"));
			return TileCode::noTile;
	}
}

/* 原点(返し点) */
LargeNum BasePoint() {
#ifdef GUOBIAO
	return 500;
#else /* GUOBIAO */
	if (RuleData::chkRule("starting_point", "custom")) {
		LargeNum basePoint = // 仮数部
		std::atoi(RuleData::chkRule("base_point_mantissa_tens")) * 10 +
		std::atoi(RuleData::chkRule("base_point_mantissa_ones"));
		/* 指数部の処理 */
		std::smatch matchDat; int exponent = 0;
		std::string expConf(RuleData::chkRule("base_point_exponent"));
		if (std::regex_match(expConf, matchDat, std::regex("exp_(\\d+)")))
			exponent = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
		for (int j = 0; j < exponent; ++j)
			basePoint *= 10;
		return basePoint;
	} else if (GameStat.chkGameType(GameTypeID::sanmaT)) {
		constexpr char rulestat[6][16] = {
			"35000pts_oka15", "40000pts_oka0", "45000pts_oka-15", "50000pts_oka-30", "25000pts_oka45", "30000pts_oka30",
		};
		for (int i = 0; i < 6; i++)
			if (RuleData::chkRule("starting_point", rulestat[i]))
				return 40000;
	}
	return 30000;
#endif /* GUOBIAO */
}

/* 浮いているか判定する関数 */
bool isAboveBase(const GameTable* const gameStat, PlayerID player) {
	return gameStat->Player[player].PlayerScore >= BasePoint();
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
		return IsWindowsVistaOrGreater();
	}
#else /*_WIN32*/
	bool isVista() { /* Windowsじゃない */
		return false;
	}
#endif /*_WIN32*/

	/* コンフィグのパスを自動設定 */
	/* Vista以降でRoamingに準備できていなければ作る */
	std::string confPath() {
		const std::string configpath(ConfigFile::ConfigFile::confPath());
#ifdef _WIN32
		if (isVista()) {
			constexpr size_t bufSize = 1024u;
			char* appdata = new char[bufSize] {};
#if defined(_MSC_VER) || defined(HAVE_GETENV_S)
			size_t sz = 0;
			getenv_s(&sz, appdata, bufSize, "APPDATA");
#else
			if (getenv("APPDATA"))
				strncpy(appdata, getenv("APPDATA"), bufSize - 1);
			appdata[bufSize - 1] = '\0';
#endif

			if (strstr(configpath.c_str(), appdata) != configpath.c_str()) {
				// MakeSureDirectoryPathExistsがワイド文字対応してないので仕方なくANSI文字版
				MakeSureDirectoryPathExists((configpath + std::string("\\haifu\\")).c_str());
				CopyFileA(".\\haifu\\haifu.css",
					(configpath + std::string("\\haifu\\haifu.css")).c_str(),
					TRUE);
				CopyFileA(".\\haifu\\haifu.dtd",
					(configpath + std::string("\\haifu\\haifu.dtd")).c_str(),
					TRUE);
				CopyFileA(".\\haifu\\haifu.xsd",
					(configpath + std::string("\\haifu\\haifu.xsd")).c_str(),
					TRUE);
				CopyFileA(".\\haifu\\haifu.xsl",
					(configpath + std::string("\\haifu\\haifu.xsl")).c_str(),
					TRUE);
				MakeSureDirectoryPathExists((configpath + std::string("\\ai\\")).c_str());
				CopyFileA(".\\ai\\default.lua",
					(configpath + std::string("\\ai\\default.lua")).c_str(),
					TRUE);
				MakeSureDirectoryPathExists((configpath + std::string("\\gbai\\")).c_str());
				CopyFileA(".\\gbai\\default.lua",
					(configpath + std::string("\\gbai\\default.lua")).c_str(),
					TRUE);
			}
			delete[] appdata;
		}
#else /*_WIN32*/
		/* Linux では、~/.mihajong に設定などを保存する */
		mkdir(configpath.c_str(), 0755);
		mkdir((configpath + std::string("/haifu")).c_str(), 0755);
		mkdir((configpath + std::string("/ai")).c_str(), 0755);
		mkdir((configpath + std::string("/gbai")).c_str(), 0755);

		symlink(PKGDATADIR "/haifu/haifu.css",
			(configpath + std::string("/haifu/haifu.css")).c_str());
		symlink(PKGDATADIR "/haifu/haifu.dtd",
			(configpath + std::string("/haifu/haifu.dtd")).c_str());
		symlink(PKGDATADIR "/haifu/haifu.xsd",
			(configpath + std::string("/haifu/haifu.xsd")).c_str());
		symlink(PKGDATADIR "/haifu/haifu.xsl",
			(configpath + std::string("/haifu/haifu.xsl")).c_str());
		symlink(PKGDATADIR "/ai/default.lua",
			(configpath + std::string("/ai/default.lua")).c_str());
		symlink(PKGDATADIR "/gbai/default.lua",
			(configpath + std::string("/gbai/default.lua")).c_str());
#endif /*_WIN32*/
		return configpath;
	}

}

/* リーチするのに持ち点が足りているかどうか */
bool isRichiReqSatisfied (const GameTable* const gameStat, PlayerID targetPlayer) {
#ifdef GUOBIAO
	return false;
#else /* GUOBIAO */
	bool Flag = true;
	if (gameStat->Player[targetPlayer].PlayerScore < 1000) Flag = false;
	else if ((gameStat->Player[targetPlayer].PlayerScore == 1000) &&
		(RuleData::chkRule("riichi_requisite", "require_1100"))) Flag = false;
	if (RuleData::chkRule("riichi_requisite", "no")) Flag = true;
	if (RuleData::chkRule("buttobi_border", "no")) Flag = true;
	return Flag;
#endif /* GUOBIAO */
}

/* 飛びになっているかどうか */
bool isDobon (const GameTable* const gameStat, PlayerID targetPlayer) {
#ifdef GUOBIAO
	return false;
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("buttobi_border"))
		return false;
	else if (gameStat->Player[targetPlayer].PlayerScore < 0)
		return true;
	else if ((gameStat->Player[targetPlayer].PlayerScore == 0) &&
		RuleData::chkRule("buttobi_border", "nonpositive"))
		return true;
	else return false;
#endif /* GUOBIAO */
}

/* 天辺になっているかどうか */
bool isTeppen (const GameTable* const gameStat, PlayerID targetPlayer) {
#ifdef GUOBIAO
	return false;
#else /* GUOBIAO */
	if (RuleData::chkRule("teppen", "50000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= 50000))
		return true;
	else if (RuleData::chkRule("teppen", "55000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= 55000))
		return true;
	else if (RuleData::chkRule("teppen", "60000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= 60000))
		return true;
	else if (RuleData::chkRule("teppen", "65000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= 65000))
		return true;
	else if (RuleData::chkRule("teppen", "70000pts") &&
		(gameStat->Player[targetPlayer].PlayerScore >= 70000))
		return true;
	else return false;
#endif /* GUOBIAO */
}

MJCORE void cleanup() {
	mihajong_graphic::Transit(mihajong_graphic::SceneID::null);
	info(_T("グラフィックDLLの後始末をしました。"));
	sound::Cleanup();
	info(_T("サウンドDLLを解放しました。"));
	for (int i = 0; i < Players; i++) {
		mihajong_socket::hangup(SOCK_GAME + i);
		mihajong_socket::hangup(SOCK_CHAT + i);
	}
	mihajong_socket::bye();
	info(_T("ソケットDLLを解放しました。"));
}
