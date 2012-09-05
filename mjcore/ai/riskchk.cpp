#include "functbl.h"

bool aiscript::table::functable::gametbl::luafunc::riskchk::issameasprevious
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // 合わせ打ちできるかどうか
		PLAYER_ID prev_p = RelativePositionOf(player, sLeft); // 上家のプレイヤー番号
		if (chkGameType(gameStat, SanmaT) && (prev_p == 3)) prev_p = 2;
		else if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, prev_p, gameStat->GameRound) == sNorth))
			prev_p = RelativePositionOf(player, sOpposite);
		for (int i = 1; i <= gameStat->Player[prev_p].DiscardPointer; i++)
			if (gameStat->Player[prev_p].Discard[i].tcode.tile == gameStat->Player[player].Hand[index].tile)
				return true;
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isdora
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // ドラかどうか
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		return (t == NoTile) ? false : gameStat->DoraFlag.Omote[t];
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isdorasuji
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // ドラ筋かどうか
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) {return false;}
		else {
			if (((t % TILE_SUIT_STEP) >= 4) &&
				(gameStat->DoraFlag.Omote[t - 3] > 0)) return true;
			if (((t % TILE_SUIT_STEP) <= 6) &&
				(gameStat->DoraFlag.Omote[t + 3] > 0)) return true;
		}
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isdorasoba
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // ドラそばかどうか
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) {return false;}
		else {
			if (((t % TILE_SUIT_STEP) >= 2) &&
				(gameStat->DoraFlag.Omote[t - 1] > 0)) return true;
			if (((t % TILE_SUIT_STEP) >= 3) &&
				(gameStat->DoraFlag.Omote[t - 2] > 0)) return true;
			if (((t % TILE_SUIT_STEP) <= 7) &&
				(gameStat->DoraFlag.Omote[t + 2] > 0)) return true;
			if (((t % TILE_SUIT_STEP) <= 8) &&
				(gameStat->DoraFlag.Omote[t + 1] > 0)) return true;
		}
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isnochance
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // ノーチャンスかどうか
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		Int8ByTile s = countseentiles(gameStat);
		Int8ByTile h = countTilesInHand(gameStat, player);
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) {return false;}
		else {
			if (((t % TILE_SUIT_STEP) == 1) && (s[t+2] + h[t+2] >= 4)) return true;
			if (((t % TILE_SUIT_STEP) <= 2) && (s[t+1] + h[t+1] >= 4)) return true;
			if (((t % TILE_SUIT_STEP) >= 8) && (s[t-1] + h[t-1] <= 4)) return true;
			if (((t % TILE_SUIT_STEP) == 9) && (s[t-2] + h[t-2] <= 4)) return true;
		}
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isonechance
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // ワンチャンスかどうか
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		Int8ByTile s = countseentiles(gameStat);
		Int8ByTile h = countTilesInHand(gameStat, player);
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) {return false;}
		else {
			if (((t % TILE_SUIT_STEP) == 1) && (s[t+2] + h[t+2] == 3)) return true;
			if (((t % TILE_SUIT_STEP) <= 2) && (s[t+1] + h[t+1] == 3)) return true;
			if (((t % TILE_SUIT_STEP) >= 8) && (s[t-1] + h[t-1] == 3)) return true;
			if (((t % TILE_SUIT_STEP) == 9) && (s[t-2] + h[t-2] == 3)) return true;
		}
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isneverdiscarded
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // 生牌かどうか
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		Int8ByTile s = countseentiles(gameStat);
		if (t == NoTile) return false;
		else return (s[t] == 0);
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isseenfour
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // 場に4枚見えているかどうか
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		Int8ByTile s = countseentiles(gameStat);
		Int8ByTile h = countTilesInHand(gameStat, player);
		if (t == NoTile) return false;
		else return (s[t] + h[t] == 4);
}
