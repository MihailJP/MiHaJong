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

/* �T�C�R���̏o�ڂ��擾 */
extern "C" inline uint8_t diceSum(const GameTable* const gameStat) {
	return (gameStat->Dice[0].Number + gameStat->Dice[1].Number);
}

/* �v���C���[�̎������ǂꂩ���ׂ� */
seatAbsolute inline playerwind(const GameTable* const gameStat, PlayerID player, int currentRound) {
	if (chkGameType(gameStat, SanmaT))
		return (seatAbsolute)((player + 24 - (currentRound - ( currentRound / 4))) % 3);
	else return (seatAbsolute)((player + 32 - currentRound) % 4);
}
__declspec(dllexport) inline int playerwind(int player, int currentRound) {
	return (int)playerwind(&GameStat, (PlayerID)player, (int)currentRound);
}

/* ����v���C���[�ɑ΂��Ďw�肵���v���C���[���ǂ�(���ƁA�ΖʁA���)�ɂ��邩���ׂ� */
seatRelative inline playerRelative(PlayerID targetPlayer, PlayerID basePlayer) {
	return (seatRelative)((Players + targetPlayer - basePlayer) % Players);
}
__declspec(dllexport) inline int playerRelative(int targetPlayer, int basePlayer) {
	return (int)playerRelative((PlayerID)targetPlayer, (PlayerID)basePlayer);
}

/* ����v���C���[��(���ƁA�ΖʁA���)�𒲂ׂ� */
PlayerID inline RelativePositionOf(PlayerID targetPlayer, seatRelative relative) {
	return (PlayerID)(((int)targetPlayer + (int)relative) % Players);
}
__declspec(dllexport) inline int RelativePositionOf(int targetPlayer, int relative) {
	return (int)RelativePositionOf((PlayerID)targetPlayer, (seatRelative)relative);
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
CodeConv::tstring inline windName(seatAbsolute wind) {
	switch (wind) {
		case sEast: return CodeConv::tstring(_T("����"));
		case sSouth: return CodeConv::tstring(_T("���"));
		case sWest: return CodeConv::tstring(_T("����"));
		case sNorth: return CodeConv::tstring(_T("�k��"));
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("�ُ�Ȉ����ł�"));
			return CodeConv::tstring(_T("????")); break;
	}
}
__declspec(dllexport) void windName(LPTSTR str, int bufsz, int wind) {
	_tcscpy_s(str, bufsz, windName((seatAbsolute)wind).c_str());
}

/* �u�����ǁv�Ȃǂ̕������Ԃ� */
CodeConv::tstring inline roundName(int roundNum, const GameTable* const gameStat) {
	CodeConv::tostringstream roundNameTxt; roundNameTxt.str(_T(""));
	switch (roundNum / Players) {
		case 0: roundNameTxt << _T("��"); break;
		case 1: roundNameTxt << _T("��"); break;
		case 2: roundNameTxt << _T("��"); break;
		case 3: roundNameTxt << _T("�k"); break;
		case 4: roundNameTxt << _T("��"); break;
		case 5: roundNameTxt << _T("��"); break;
		case 6: roundNameTxt << _T("��"); break;
		default: 
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("�ُ�Ȉ����ł��B�ꕗ����͂ł��܂���B"));
			roundNameTxt << _T("??");
	}
	if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game")) {
		switch (int k = (gameStat->LoopRound * ACTUAL_PLAYERS + roundNum % Players)) {
			case 0: roundNameTxt << _T("���"); break;
			case 1: roundNameTxt << _T("���"); break;
			case 2: roundNameTxt << _T("�O��"); break;
			case 3: roundNameTxt << _T("�l��"); break;
			case 4: roundNameTxt << _T("�܋�"); break;
			case 5: roundNameTxt << _T("�Z��"); break;
			case 6: roundNameTxt << _T("����"); break;
			case 7: roundNameTxt << _T("����"); break;
			case 8: roundNameTxt << _T("���"); break;
			case 9: roundNameTxt << _T("�\��"); break;
			default: roundNameTxt << (k+1) << _T("��"); break;
		}
	} else {
		switch (roundNum % Players) {
			case 0: roundNameTxt << _T("���"); break;
			case 1: roundNameTxt << _T("���"); break;
			case 2: roundNameTxt << _T("�O��"); break;
			case 3: roundNameTxt << _T("�l��"); break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("�ُ�Ȉ����ł��B������5�ǖڈȍ~�ł��B"));
				roundNameTxt << _T("??��"); break;
		}
	}
	return CodeConv::tstring(roundNameTxt.str());
}
__declspec(dllexport) void roundName(LPTSTR str, int bufsz, int roundNum) {
	_tcscpy_s(str, bufsz, roundName(roundNum, &GameStat).c_str());
}

/* �v�̖��O�̕������Ԃ� */
CodeConv::tstring inline TileName(TileCode tile) {
	switch (tile) {
		case CharacterOne:   return CodeConv::tstring(_T("����"));
		case CharacterTwo:   return CodeConv::tstring(_T("����"));
		case CharacterThree: return CodeConv::tstring(_T("�O��"));
		case CharacterFour:  return CodeConv::tstring(_T("�l��"));
		case CharacterFive:  return CodeConv::tstring(_T("����"));
		case CharacterSix:   return CodeConv::tstring(_T("�Z��"));
		case CharacterSeven: return CodeConv::tstring(_T("����"));
		case CharacterEight: return CodeConv::tstring(_T("����"));
		case CharacterNine:  return CodeConv::tstring(_T("����"));
		case CircleOne:      return CodeConv::tstring(_T("�ꓛ"));
		case CircleTwo:      return CodeConv::tstring(_T("��"));
		case CircleThree:    return CodeConv::tstring(_T("�O��"));
		case CircleFour:     return CodeConv::tstring(_T("�l��"));
		case CircleFive:     return CodeConv::tstring(_T("�ܓ�"));
		case CircleSix:      return CodeConv::tstring(_T("�Z��"));
		case CircleSeven:    return CodeConv::tstring(_T("����"));
		case CircleEight:    return CodeConv::tstring(_T("����"));
		case CircleNine:     return CodeConv::tstring(_T("�㓛"));
		case BambooOne:      return CodeConv::tstring(_T("���"));
		case BambooTwo:      return CodeConv::tstring(_T("���"));
		case BambooThree:    return CodeConv::tstring(_T("�O��"));
		case BambooFour:     return CodeConv::tstring(_T("�l��"));
		case BambooFive:     return CodeConv::tstring(_T("�܍�"));
		case BambooSix:      return CodeConv::tstring(_T("�Z��"));
		case BambooSeven:    return CodeConv::tstring(_T("����"));
		case BambooEight:    return CodeConv::tstring(_T("����"));
		case BambooNine:     return CodeConv::tstring(_T("���"));
		case EastWind:       return CodeConv::tstring(_T("��"));
		case SouthWind:      return CodeConv::tstring(_T("��"));
		case WestWind:       return CodeConv::tstring(_T("��"));
		case NorthWind:      return CodeConv::tstring(_T("�k"));
		case WhiteDragon:    return CodeConv::tstring(_T("��"));
		case GreenDragon:    return CodeConv::tstring(_T("�"));
		case RedDragon:      return CodeConv::tstring(_T("��"));
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("�ُ�Ȉ����ł�"));
			return CodeConv::tstring(_T("????"));
	}
}
__declspec(dllexport) void TileName(LPTSTR str, int bufsz, int tile) {
	_tcscpy_s(str, bufsz, TileName((TileCode)tile).c_str());
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
			size_t* sz = new size_t;
			getenv_s(sz, progfiles, 1024, "ProgramFiles");
			getenv_s(sz, appdata, 1024, "APPDATA");

			if (strstr(cur, progfiles) == cur) {
				// MakeSureDirectoryPathExists�����C�h�����Ή����ĂȂ��̂Ŏd���Ȃ�ANSI������
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
