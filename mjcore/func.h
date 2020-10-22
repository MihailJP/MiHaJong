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
#ifndef _WIN32
#include <sys/stat.h>
#endif
#include "../common/largenum.h"
#endif

// -------------------------------------------------------------------------

using mihajong_structs::InfoByPlayer;
using mihajong_structs::PlayerID;
using mihajong_structs::TileCode;

using mihajong_structs::SeatAbsolute;
using mihajong_structs::SeatRelative;
using mihajong_structs::PlayerRank;
using mihajong_structs::PlayerRankList;

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
/* あるプレイヤーに対して指定したプレイヤーがどこ(下家、対面、上家)にいるか調べる */
SeatRelative inline playerRelative(PlayerID targetPlayer, PlayerID basePlayer) {
	return static_cast<SeatRelative>((Players + targetPlayer - basePlayer) % Players);
}

/* あるプレイヤーの(下家、対面、上家)を調べる */
PlayerID inline RelativePositionOf(PlayerID targetPlayer, SeatRelative relative) {
	return static_cast<PlayerID>((static_cast<int>(targetPlayer) + static_cast<int>(relative)) % Players);
}

/* 一周するまでに必要な場の数 */
inline int roundLoopRate() {
#ifdef GUOBIAO
	return 16;
#else /* GUOBIAO */
	if (RuleData::chkRule("sudden_death_type", "continue_into_white")) return 28;
	else return 16;
#endif /* GUOBIAO */
}

/* 順位を計算する */
PlayerRankList calcRank(const GameTable* const gameStat);

/* 包かどうかの判定 */
bool isPao(const GameTable* const gameStat, PlayerID agariPlayer, PlayerID paoPlayer);
bool isPaoAgari(const GameTable* const gameStat, PlayerID agariPlayer);
bool isGotPao(const GameTable* const gameStat, PlayerID paoPlayer);
PlayerID getPaoPlayer(const GameTable* const gameStat, PlayerID agariPlayer);

/* ロンしたプレイヤーの数 */
int RonPlayers(const GameTable* const gameStat);

/* 「東家」「南家」「西家」「北家」の文字列を返す */
CodeConv::tstring inline windName(SeatAbsolute wind) {
	switch (wind) {
		case SeatAbsolute::east: return CodeConv::tstring(_T("東家"));
		case SeatAbsolute::south: return CodeConv::tstring(_T("南家"));
		case SeatAbsolute::west: return CodeConv::tstring(_T("西家"));
		case SeatAbsolute::north: return CodeConv::tstring(_T("北家"));
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です"));
			return CodeConv::tstring(_T("????")); break;
	}
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
#ifndef GUOBIAO
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
#endif /* GUOBIAO */
		switch (roundNum % Players) {
			case 0: roundNameTxt << _T("一局"); break;
			case 1: roundNameTxt << _T("二局"); break;
			case 2: roundNameTxt << _T("三局"); break;
			case 3: roundNameTxt << _T("四局"); break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です。同一場の5局目以降です。"));
				roundNameTxt << _T("??局"); break;
		}
#ifndef GUOBIAO
	}
#endif /* GUOBIAO */
	return CodeConv::tstring(roundNameTxt.str());
}

/* 牌の名前の文字列を返す */
CodeConv::tstring inline TileName(TileCode tile) {
	switch (tile) {
		case TileCode::characterOne:   return CodeConv::tstring(_T("一萬"));
		case TileCode::characterTwo:   return CodeConv::tstring(_T("二萬"));
		case TileCode::characterThree: return CodeConv::tstring(_T("三萬"));
		case TileCode::characterFour:  return CodeConv::tstring(_T("四萬"));
		case TileCode::characterFive:  return CodeConv::tstring(_T("五萬"));
		case TileCode::characterSix:   return CodeConv::tstring(_T("六萬"));
		case TileCode::characterSeven: return CodeConv::tstring(_T("七萬"));
		case TileCode::characterEight: return CodeConv::tstring(_T("八萬"));
		case TileCode::characterNine:  return CodeConv::tstring(_T("九萬"));
		case TileCode::circleOne:      return CodeConv::tstring(_T("一筒"));
		case TileCode::circleTwo:      return CodeConv::tstring(_T("二筒"));
		case TileCode::circleThree:    return CodeConv::tstring(_T("三筒"));
		case TileCode::circleFour:     return CodeConv::tstring(_T("四筒"));
		case TileCode::circleFive:     return CodeConv::tstring(_T("五筒"));
		case TileCode::circleSix:      return CodeConv::tstring(_T("六筒"));
		case TileCode::circleSeven:    return CodeConv::tstring(_T("七筒"));
		case TileCode::circleEight:    return CodeConv::tstring(_T("八筒"));
		case TileCode::circleNine:     return CodeConv::tstring(_T("九筒"));
		case TileCode::bambooOne:      return CodeConv::tstring(_T("一索"));
		case TileCode::bambooTwo:      return CodeConv::tstring(_T("二索"));
		case TileCode::bambooThree:    return CodeConv::tstring(_T("三索"));
		case TileCode::bambooFour:     return CodeConv::tstring(_T("四索"));
		case TileCode::bambooFive:     return CodeConv::tstring(_T("五索"));
		case TileCode::bambooSix:      return CodeConv::tstring(_T("六索"));
		case TileCode::bambooSeven:    return CodeConv::tstring(_T("七索"));
		case TileCode::bambooEight:    return CodeConv::tstring(_T("八索"));
		case TileCode::bambooNine:     return CodeConv::tstring(_T("九索"));
		case TileCode::eastWind:       return CodeConv::tstring(_T("東"));
		case TileCode::southWind:      return CodeConv::tstring(_T("南"));
		case TileCode::westWind:       return CodeConv::tstring(_T("西"));
		case TileCode::northWind:      return CodeConv::tstring(_T("北"));
		case TileCode::whiteDragon:    return CodeConv::tstring(_T("白"));
		case TileCode::greenDragon:    return CodeConv::tstring(_T("發"));
		case TileCode::redDragon:      return CodeConv::tstring(_T("中"));
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("異常な引数です"));
			return CodeConv::tstring(_T("????"));
	}
}

TileCode Wind2Tile(SeatAbsolute wind);

LargeNum BasePoint();
bool isAboveBase(const GameTable* const gameStat, PlayerID player);

CodeConv::tstring intstr(int val);

namespace confpath {
	bool isVista();

	std::string confPath();
}

bool isRichiReqSatisfied (const GameTable* const gameStat, PlayerID targetPlayer);

bool isDobon (const GameTable* const gameStat, PlayerID targetPlayer);
bool isTeppen (const GameTable* const gameStat, PlayerID targetPlayer);

#ifdef _WIN32
inline bool exist (LPCSTR filename) {
	return (GetFileAttributesA(filename) != -1);
}
#else /*_WIN32*/
inline bool exist (const char* filename) {
	struct stat st;
	return stat(filename, &st) == 0;
}
#endif /*_WIN32*/
#endif

MJCORE void cleanup();
