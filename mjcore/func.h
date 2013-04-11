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
/* サイコロの出目を取得 */
extern "C" inline uint8_t diceSum(const GameTable* const gameStat) {
	return (gameStat->Dice[0].Number + gameStat->Dice[1].Number);
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
PlayerRankList calcRank(const GameTable* const gameStat);
__declspec(dllexport) void calcRank(int* Rank, const GameTable* const gameStat);

/* 包かどうかの判定 */
bool isPao(const GameTable* const gameStat, PlayerID agariPlayer, PlayerID paoPlayer);
__declspec(dllexport) int isPao(const GameTable* const gameStat, int agariPlayer, int paoPlayer);

bool isPaoAgari(const GameTable* const gameStat, PlayerID agariPlayer);
__declspec(dllexport) int isPaoAgari(const GameTable* const gameStat, int agariPlayer);

bool isGotPao(const GameTable* const gameStat, PlayerID paoPlayer);
__declspec(dllexport) int isGotPao(const GameTable* const gameStat, int paoPlayer);

PlayerID getPaoPlayer(const GameTable* const gameStat, PlayerID agariPlayer);
__declspec(dllexport) int getPaoPlayer(const GameTable* const gameStat, int agariPlayer);

/* ロンしたプレイヤーの数 */
__declspec(dllexport) int RonPlayers(const GameTable* const gameStat);

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
__declspec(dllexport) void windName(LPTSTR str, int bufsz, int wind);

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
__declspec(dllexport) void roundName(LPTSTR str, int bufsz, int roundNum);

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
