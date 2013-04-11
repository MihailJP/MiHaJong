#pragma once

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <type_traits>
#endif
#include "mjexport.h"
#include "gametbl.h"
#include "../common/seatrank.h"
#ifdef MJCORE_EXPORTS
#include "ruletbl.h"
#endif

// -------------------------------------------------------------------------

using mihajong_structs::InfoByPlayer;
using mihajong_structs::PlayerID;
using mihajong_structs::TileCode;

using mihajong_structs::seatAbsolute;
using mihajong_structs::seatRelative;
using mihajong_structs::PlayerRank;
using mihajong_structs::PlayerRankList;

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
/* �T�C�R���̏o�ڂ��擾 */
extern "C" inline uint8_t diceSum(const GameTable* const gameStat) {
	return (gameStat->Dice[0].Number + gameStat->Dice[1].Number);
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
PlayerRankList calcRank(const GameTable* const gameStat);
__declspec(dllexport) void calcRank(int* Rank, const GameTable* const gameStat);

/* ��ǂ����̔��� */
bool isPao(const GameTable* const gameStat, PlayerID agariPlayer, PlayerID paoPlayer);
__declspec(dllexport) int isPao(const GameTable* const gameStat, int agariPlayer, int paoPlayer);

bool isPaoAgari(const GameTable* const gameStat, PlayerID agariPlayer);
__declspec(dllexport) int isPaoAgari(const GameTable* const gameStat, int agariPlayer);

bool isGotPao(const GameTable* const gameStat, PlayerID paoPlayer);
__declspec(dllexport) int isGotPao(const GameTable* const gameStat, int paoPlayer);

PlayerID getPaoPlayer(const GameTable* const gameStat, PlayerID agariPlayer);
__declspec(dllexport) int getPaoPlayer(const GameTable* const gameStat, int agariPlayer);

/* ���������v���C���[�̐� */
__declspec(dllexport) int RonPlayers(const GameTable* const gameStat);

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
__declspec(dllexport) void windName(LPTSTR str, int bufsz, int wind);

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
__declspec(dllexport) void roundName(LPTSTR str, int bufsz, int roundNum);

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
__declspec(dllexport) void TileName(LPTSTR str, int bufsz, int tile);

TileCode Wind2Tile(uint8_t wind);

int BasePoint();
__declspec(dllexport) int BasePointHSP();

bool isAboveBase(const GameTable* const gameStat, PlayerID player);
__declspec(dllexport) int isAboveBase(const GameTable* const gameStat, int player);

CodeConv::tstring intstr(int val);

namespace confpath {
	bool isVista();

	std::string confPath();
	__declspec(dllexport) void confPath(char* path, int bufsz);
}

bool isRichiReqSatisfied (const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) int isRichiReqSatisfied (const GameTable* const gameStat, int targetPlayer);

bool isDobon (const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) int isDobon (const GameTable* const gameStat, int targetPlayer);

bool isTeppen (const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) int isTeppen (const GameTable* const gameStat, int targetPlayer);

inline bool exist (LPCSTR filename) {
	return (GetFileAttributesA(filename) != -1);
}
#endif

MJCORE void cleanup();
