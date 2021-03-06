﻿#include "functbl.h"

#include "../func.h"

bool aiscript::table::functable::gametbl::luafunc::riskchk::issameasprevious
	(const GameTable* const gameStat, PlayerID player, int index) { // 合わせ打ちできるかどうか
		PlayerID prev_p = RelativePositionOf(player, SeatRelative::left); // 上家のプレイヤー番号
		if (gameStat->chkGameType(GameTypeID::sanmaT) && (prev_p == 3)) prev_p = 2;
		else if (gameStat->chkGameType(GameTypeID::sanma4) && (gameStat->playerwind(prev_p) == SeatAbsolute::north))
			prev_p = RelativePositionOf(player, SeatRelative::opposite);
		if (gameStat->Player[prev_p].Discard[gameStat->Player[prev_p].DiscardPointer].tcode.tile ==
			gameStat->Player[player].Hand[index].tile)
			return true;
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isdora
	(const GameTable* const gameStat, PlayerID player, int index) { // ドラかどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		return (t == TileCode::noTile) ? false : gameStat->DoraFlag.Omote[t];
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isdorasuji
	(const GameTable* const gameStat, PlayerID player, int index) { // ドラ筋かどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) {return false;}
		else {
			if (((getTileNumber(t)) >= 4) &&
				(gameStat->DoraFlag.Omote[offsetTileNumber(t, -3)] > 0)) return true;
			if (((getTileNumber(t)) <= 6) &&
				(gameStat->DoraFlag.Omote[offsetTileNumber(t, +3)] > 0)) return true;
		}
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isdorasoba
	(const GameTable* const gameStat, PlayerID player, int index) { // ドラそばかどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) {return false;}
		else {
			if (((getTileNumber(t)) >= 2) &&
				(gameStat->DoraFlag.Omote[offsetTileNumber(t, -1)] > 0)) return true;
			if (((getTileNumber(t)) >= 3) &&
				(gameStat->DoraFlag.Omote[offsetTileNumber(t, -2)] > 0)) return true;
			if (((getTileNumber(t)) <= 7) &&
				(gameStat->DoraFlag.Omote[offsetTileNumber(t, +2)] > 0)) return true;
			if (((getTileNumber(t)) <= 8) &&
				(gameStat->DoraFlag.Omote[offsetTileNumber(t, +1)] > 0)) return true;
		}
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isnochance
	(const GameTable* const gameStat, PlayerID player, int index) { // ノーチャンスかどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		const Int8ByTile s = countseentiles(gameStat);
		const Int8ByTile h = countTilesInHand(gameStat, player);
		if (!Tile(t).isNumber()) {return false;}
		else {
			if (((getTileNumber(t)) == 1) && (s[offsetTileNumber(t, +2)] + h[offsetTileNumber(t, +2)] >= 4)) return true;
			if (((getTileNumber(t)) <= 2) && (s[offsetTileNumber(t, +1)] + h[offsetTileNumber(t, +1)] >= 4)) return true;
			if (((getTileNumber(t)) >= 8) && (s[offsetTileNumber(t, -1)] + h[offsetTileNumber(t, -1)] <= 4)) return true;
			if (((getTileNumber(t)) == 9) && (s[offsetTileNumber(t, -2)] + h[offsetTileNumber(t, -2)] <= 4)) return true;
		}
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isonechance
	(const GameTable* const gameStat, PlayerID player, int index) { // ワンチャンスかどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		const Int8ByTile s = countseentiles(gameStat);
		const Int8ByTile h = countTilesInHand(gameStat, player);
		if (!Tile(t).isNumber()) {return false;}
		else {
			if (((getTileNumber(t)) == 1) && (s[offsetTileNumber(t, +2)] + h[offsetTileNumber(t, +2)] == 3)) return true;
			if (((getTileNumber(t)) <= 2) && (s[offsetTileNumber(t, +1)] + h[offsetTileNumber(t, +1)] == 3)) return true;
			if (((getTileNumber(t)) >= 8) && (s[offsetTileNumber(t, -1)] + h[offsetTileNumber(t, -1)] == 3)) return true;
			if (((getTileNumber(t)) == 9) && (s[offsetTileNumber(t, -2)] + h[offsetTileNumber(t, -2)] == 3)) return true;
		}
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isneverdiscarded
	(const GameTable* const gameStat, PlayerID player, int index) { // 生牌かどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		const Int8ByTile s = countseentiles(gameStat);
		if (t == TileCode::noTile) return false;
		else return (s[t] == 0);
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isseenfour
	(const GameTable* const gameStat, PlayerID player, int index) { // 場に4枚見えているかどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		const Int8ByTile s = countseentiles(gameStat);
		const Int8ByTile h = countTilesInHand(gameStat, player);
		if (t == TileCode::noTile) return false;
		else return (s[t] + h[t] == 4);
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::isgembutsu
	(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index) { // 現物かどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (t == TileCode::noTile) return false;
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++)
			if (gameStat->Player[tplayer].Discard[i].tcode.tile == t)
				return true;
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::issuji
	(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index) { // 筋かどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) return false; // 字牌と空白は除外
		if (((getTileNumber(t)) >= 4) && ((getTileNumber(t)) <= 6)) return false; // 筋心牌以外にのみ適用可能
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++)
			if (((getTileNumber(t)) < 4) && (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 3)))
				return true;
			else if (((getTileNumber(t)) > 6) && (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -3)))
				return true;
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::isnakasuji
	(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index) { // 中筋かどうか
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) return false; // 字牌と空白は除外
		if (((getTileNumber(t)) < 4) || ((getTileNumber(t)) > 6)) return false; // 筋心牌にのみ適用可能
		bool flag1 = false; bool flag2 = false;
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++)
			if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -3))
				flag1 = true;
			else if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 3))
				flag2 = true;
		return flag1 && flag2;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::isurasuji
	(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index) { // 裏筋かどうか
		/*
		 *  裏筋とは……ある牌に対し、その隣の筋である
		 *
		 *  ①２・・５⑥・・・　/　・②３・・６⑦・・　/　・・③４・・７⑧・
		 *  ・・・④５・・８⑨　/　１・・４⑤６・・９
		 */
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) return false; // 字牌と空白は除外
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((getTileNumber(t)) >= 2) && ((getTileNumber(t)) <= 6))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -1))
					return true;
			if (((getTileNumber(t)) >= 5) && ((getTileNumber(t)) <= 9))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -4))
					return true;
			if (((getTileNumber(t)) >= 4) && ((getTileNumber(t)) <= 8))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 1))
					return true;
			if (((getTileNumber(t)) >= 1) && ((getTileNumber(t)) <= 5))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 4))
					return true;
		}
		return false;
}


bool aiscript::table::functable::gametbl::luafunc::riskchk::isaida4ken
	(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index) { // 間4ケンの筋かどうか
		/*
		 *  ①２・・５⑥・・・　/　・②３・・６⑦・・　/　・・③４・・７⑧・
		 *  ・・・④５・・８⑨
		 */
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) return false; // 字牌と空白は除外
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((getTileNumber(t)) >= 2) && ((getTileNumber(t)) <= 5))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -1))
					if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 4))
						return true;
			if (((getTileNumber(t)) >= 5) && ((getTileNumber(t)) <= 8))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -4))
					if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 1))
						return true;
		}
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::issenkisuji
	(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index) { // 疝気筋かどうか
		/*
		 *  疝気筋とは……ある牌に対し、その隣の隣の筋である
		 *
		 *  １・・４・⑥・・・　/　・２・・５・⑦・・　/　①・３・・６・⑧・
		 *  ・②・４・・７・⑨　/　・・③・５・・８・　/　・・・④・６・・９
		 */
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) return false; // 字牌と空白は除外
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((getTileNumber(t)) >= 3) && ((getTileNumber(t)) <= 6))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -2))
					return true;
			if (((getTileNumber(t)) >= 6) && ((getTileNumber(t)) <= 9))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -5))
					return true;
			if (((getTileNumber(t)) >= 4) && ((getTileNumber(t)) <= 7))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 2))
					return true;
			if (((getTileNumber(t)) >= 1) && ((getTileNumber(t)) <= 4))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 5))
					return true;
		}
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::ismatagisuji
	(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index) { // 跨ぎ筋かどうか
		/*
		 *  跨ぎ筋
		 *
		 *  １②・４・・・・・　/　１２③４５・・・・　/　・２３④５６・・・
		 *  ・・３４⑤６７・・　/　・・・４５⑥７８・　/　・・・・５６⑦８９
		 *  ・・・・・６・⑧９
		 */
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) return false; // 字牌と空白は除外
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((getTileNumber(t)) >= 2) && ((getTileNumber(t)) <= 7)) {
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -1))
					return true;
				else if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 2))
					return true;
			}
			if (((getTileNumber(t)) >= 3) && ((getTileNumber(t)) <= 8)) {
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -2))
					return true;
				else if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 1))
					return true;
			}
		}
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::isnamakurasuji
	(const GameTable* const gameStat, PlayerID player, PlayerID tplayer, int index) { // 鈍筋かどうか
		/*
		 *  鈍筋
		 *
		 *  ①・・④・・７・・　/　・②・・⑤・・８・　/　・・③・・⑥・・９
		 *  １・・④・・⑦・・　/　・２・・⑤・・⑧・　/　・・３・・⑥・・⑨
		 */
		TileCode t = gameStat->Player[player].Hand[index].tile;
		if (!Tile(t).isNumber()) return false; // 字牌と空白は除外
		bool flag1 = false; bool flag2 = false;
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((getTileNumber(t)) >= 1) && ((getTileNumber(t)) <= 3)) {
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 6))
					flag1 = true;
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 3))
					flag2 = true;
			}
			if (((getTileNumber(t)) >= 4) && ((getTileNumber(t)) <= 6)) {
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, 3))
					flag1 = true;
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -3))
					flag1 = true;
			}
			if (((getTileNumber(t)) >= 7) && ((getTileNumber(t)) <= 9)) {
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -6))
					flag1 = true;
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == offsetTileNumber(t, -3))
					flag2 = true;
			}
			if (gameStat->Player[tplayer].Discard[i].tcode.tile == t)
				flag2 = true; // 現物があったら解除
		}
		return flag1 && (!flag2);
}
