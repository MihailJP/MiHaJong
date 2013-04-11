#include "func.h"

#include <cassert>
#include <memory>
#include <cstring>
#include <sstream>
#include <windows.h>
#include <imagehlp.h>
#include <direct.h>
#include "../common/strcode.h"
#include "../sound/sound.h"
#include "../socket/socket.h"
#include "socknum.h"
#include "except.h"
#include "ruletbl.h"
#include "largenum.h"

/* ���ʂ��v�Z���� */
PlayerRankList calcRank(const GameTable* const gameStat) {
	PlayerRankList rankList;
	memset(&rankList, 0, sizeof(rankList));
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		rankList[i] = 1;
		for (int j = 0; j < ACTUAL_PLAYERS; j++) {
			if ((LNum)gameStat->Player[j].PlayerScore > gameStat->Player[i].PlayerScore)
				rankList[i]++;
			if (((LNum)gameStat->Player[j].PlayerScore == gameStat->Player[i].PlayerScore) &&
				(i > j)) rankList[i]++; // �����̏ꍇ�͋N�Ƃɋ߂��������
		}
	}
	return rankList;
}
__declspec(dllexport) void calcRank(int* const Rank, const GameTable* const gameStat) {
	const PlayerRankList rankList = calcRank(gameStat);
	assert(rankList[0] > 0); assert(rankList[1] > 0);
	for (int i = 0; i < Players; i++) *(Rank + i) = rankList[i];
}

/* ��ǂ����̔��� */
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

/* ���������v���C���[�̐� */
__declspec(dllexport) int RonPlayers(const GameTable* const gameStat) {
	int qualified = 0;
	for (int i = 0; i < Players; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron)
			qualified++;
	return qualified;
}

/* �u���Ɓv�u��Ɓv�u���Ɓv�u�k�Ɓv�̕������Ԃ� */
__declspec(dllexport) void windName(LPTSTR str, int bufsz, int wind) {
#if defined(_MSC_VER)
	_tcscpy_s(str, bufsz, windName((seatAbsolute)wind).c_str());
#else
	_tcsncpy(str, windName((seatAbsolute)wind).c_str(), bufsz);
#endif
}

/* �u�����ǁv�Ȃǂ̕������Ԃ� */
__declspec(dllexport) void roundName(LPTSTR str, int bufsz, int roundNum) {
#if defined(_MSC_VER)
	_tcscpy_s(str, bufsz, roundName(roundNum, &GameStat).c_str());
#else
	_tcsncpy(str, roundName(roundNum, &GameStat).c_str(), bufsz);
#endif
}

/* �v�̖��O�̕������Ԃ� */
__declspec(dllexport) void TileName(LPTSTR str, int bufsz, int tile) {
#if defined(_MSC_VER)
	_tcscpy_s(str, bufsz, TileName((TileCode)tile).c_str());
#else
	_tcsncpy(str, TileName((TileCode)tile).c_str(), bufsz);
#endif
}

/* �ꕗ�v�̃��X�g */
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("�ُ�Ȉ����ł�"));
			return NoTile;
	}
}

/* ���_(�Ԃ��_) */
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
__declspec(dllexport) int BasePointHSP() {
	return BasePoint() / 100;
}

/* �����Ă��邩���肷��֐� */
bool isAboveBase(const GameTable* const gameStat, PlayerID player) {
	return gameStat->Player[player].PlayerScore >= (LNum)BasePoint();
}
__declspec(dllexport) int isAboveBase(const GameTable* const gameStat, int player) {
	return isAboveBase(gameStat, (PlayerID)player) ? 1 : 0;
}

/* �񕉐���1���Ȃ�S�p�E����ȊO�͔��p */
CodeConv::tstring intstr(int val) {
	LPCTSTR hanstr[10] = {
		_T("�O"), _T("�P"), _T("�Q"), _T("�R"), _T("�S"),
		_T("�T"), _T("�U"), _T("�V"), _T("�W"), _T("�X"),
	};
	CodeConv::tostringstream o;
	if ((val <= 9) && (val >= 0)) o << hanstr[val];
	else o << val;
	return o.str();
}

namespace confpath {
	using CodeConv::tstring;

	/* Vista/7���g���Ă��邩�ǂ��� */
	bool isVista() {
		OSVERSIONINFO versionInfo;
		versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&versionInfo) == 0) return false;
		return ((versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)&&(versionInfo.dwMajorVersion >= 6));
	}

	/* �R���t�B�O�̃p�X�������ݒ� */
	/* Vista�ȍ~��Roaming�ɏ����ł��Ă��Ȃ���΍�� */
	std::string confPath() {
		std::string configpath = "";
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
				// MakeSureDirectoryPathExists�����C�h�����Ή����ĂȂ��̂Ŏd���Ȃ�ANSI������
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
		return configpath;
	}
	__declspec(dllexport) void confPath(char* path, int bufsz) {
#if defined(_MSC_VER)
		strcpy_s(path, bufsz, confPath().c_str());
#else
		strncpy(path, confPath().c_str(), bufsz);
#endif
	}

}

/* ���[�`����̂Ɏ����_������Ă��邩�ǂ��� */
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

/* ��тɂȂ��Ă��邩�ǂ��� */
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

/* �V�ӂɂȂ��Ă��邩�ǂ��� */
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
	info(_T("�T�E���hDLL��������܂����B"));
	for (int i = 0; i < Players; i++) {
		mihajong_socket::hangup(SOCK_GAME + i);
		mihajong_socket::hangup(SOCK_CHAT + i);
	}
	mihajong_socket::bye();
	info(_T("�\�P�b�gDLL��������܂����B"));
}
