#include "functbl.h"

bool aiscript::table::functable::gametbl::luafunc::riskchk::issameasprevious
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // ���킹�ł��ł��邩�ǂ���
		PLAYER_ID prev_p = RelativePositionOf(player, sLeft); // ��Ƃ̃v���C���[�ԍ�
		if (chkGameType(gameStat, SanmaT) && (prev_p == 3)) prev_p = 2;
		else if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, prev_p, gameStat->GameRound) == sNorth))
			prev_p = RelativePositionOf(player, sOpposite);
		if (gameStat->Player[prev_p].Discard[gameStat->Player[prev_p].DiscardPointer].tcode.tile ==
			gameStat->Player[player].Hand[index].tile)
			return true;
		return false;
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isdora
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // �h�����ǂ���
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		return (t == NoTile) ? false : gameStat->DoraFlag.Omote[t];
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isdorasuji
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // �h���؂��ǂ���
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
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // �h�����΂��ǂ���
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
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // �m�[�`�����X���ǂ���
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
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // �����`�����X���ǂ���
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
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // ���v���ǂ���
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		Int8ByTile s = countseentiles(gameStat);
		if (t == NoTile) return false;
		else return (s[t] == 0);
}
bool aiscript::table::functable::gametbl::luafunc::riskchk::isseenfour
	(const GameTable* const gameStat, PLAYER_ID player, int index) { // ���4�������Ă��邩�ǂ���
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		Int8ByTile s = countseentiles(gameStat);
		Int8ByTile h = countTilesInHand(gameStat, player);
		if (t == NoTile) return false;
		else return (s[t] + h[t] == 4);
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::isgembutsu
	(const GameTable* const gameStat, PLAYER_ID player, PLAYER_ID tplayer, int index) { // �������ǂ���
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if (t == NoTile) return false;
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++)
			if (gameStat->Player[tplayer].Discard[i].tcode.tile == t)
				return true;
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::issuji
	(const GameTable* const gameStat, PLAYER_ID player, PLAYER_ID tplayer, int index) { // �؂��ǂ���
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) return false; // ���v�Ƌ󔒂͏��O
		if (((t % TILE_SUIT_STEP) >= 4) && ((t % TILE_SUIT_STEP) <= 6)) return false; // �ؐS�v�ȊO�ɂ̂ݓK�p�\
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++)
			if (((t % TILE_SUIT_STEP) < 4) && (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 3)))
				return true;
			else if (((t % TILE_SUIT_STEP) > 6) && (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 3)))
				return true;
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::isnakasuji
	(const GameTable* const gameStat, PLAYER_ID player, PLAYER_ID tplayer, int index) { // ���؂��ǂ���
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) return false; // ���v�Ƌ󔒂͏��O
		if (((t % TILE_SUIT_STEP) < 4) || ((t % TILE_SUIT_STEP) > 6)) return false; // �ؐS�v�ɂ̂ݓK�p�\
		bool flag1 = false; bool flag2 = false;
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++)
			if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 3))
				flag1 = true;
			else if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 3))
				flag2 = true;
		return flag1 && flag2;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::isurasuji
	(const GameTable* const gameStat, PLAYER_ID player, PLAYER_ID tplayer, int index) { // ���؂��ǂ���
		/*
		 *  ���؂Ƃ́c�c����v�ɑ΂��A���ׂ̗̋؂ł���
		 *
		 *  �@�Q�E�E�T�E�E�E�E�@/�@�E�A�R�E�E�U�F�E�E�@/�@�E�E�B�S�E�E�V�G�E
		 *  �E�E�E�C�T�E�E�W�H�@/�@�P�E�E�S�D�U�E�E�X
		 */
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) return false; // ���v�Ƌ󔒂͏��O
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((t % TILE_SUIT_STEP) >= 2) && ((t % TILE_SUIT_STEP) <= 6))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 1))
					return true;
			if (((t % TILE_SUIT_STEP) >= 5) && ((t % TILE_SUIT_STEP) <= 9))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 4))
					return true;
			if (((t % TILE_SUIT_STEP) >= 4) && ((t % TILE_SUIT_STEP) <= 8))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 1))
					return true;
			if (((t % TILE_SUIT_STEP) >= 1) && ((t % TILE_SUIT_STEP) <= 5))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 4))
					return true;
		}
		return false;
}


bool aiscript::table::functable::gametbl::luafunc::riskchk::isaida4ken
	(const GameTable* const gameStat, PLAYER_ID player, PLAYER_ID tplayer, int index) { // ��4�P���̋؂��ǂ���
		/*
		 *  �@�Q�E�E�T�E�E�E�E�@/�@�E�A�R�E�E�U�F�E�E�@/�@�E�E�B�S�E�E�V�G�E
		 *  �E�E�E�C�T�E�E�W�H
		 */
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) return false; // ���v�Ƌ󔒂͏��O
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((t % TILE_SUIT_STEP) >= 2) && ((t % TILE_SUIT_STEP) <= 5))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 1))
					if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 4))
						return true;
			if (((t % TILE_SUIT_STEP) >= 5) && ((t % TILE_SUIT_STEP) <= 8))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 4))
					if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 1))
						return true;
		}
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::issenkisuji
	(const GameTable* const gameStat, PLAYER_ID player, PLAYER_ID tplayer, int index) { // �l�C�؂��ǂ���
		/*
		 *  �l�C�؂Ƃ́c�c����v�ɑ΂��A���ׂׂ̗̗̋؂ł���
		 *
		 *  �P�E�E�S�E�E�E�E�E�@/�@�E�Q�E�E�T�E�F�E�E�@/�@�@�E�R�E�E�U�E�G�E
		 *  �E�A�E�S�E�E�V�E�H�@/�@�E�E�B�E�T�E�E�W�E�@/�@�E�E�E�C�E�U�E�E�X
		 */
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) return false; // ���v�Ƌ󔒂͏��O
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((t % TILE_SUIT_STEP) >= 3) && ((t % TILE_SUIT_STEP) <= 6))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 2))
					return true;
			if (((t % TILE_SUIT_STEP) >= 6) && ((t % TILE_SUIT_STEP) <= 9))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 5))
					return true;
			if (((t % TILE_SUIT_STEP) >= 4) && ((t % TILE_SUIT_STEP) <= 7))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 2))
					return true;
			if (((t % TILE_SUIT_STEP) >= 1) && ((t % TILE_SUIT_STEP) <= 4))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 5))
					return true;
		}
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::ismatagisuji
	(const GameTable* const gameStat, PLAYER_ID player, PLAYER_ID tplayer, int index) { // �ׂ��؂��ǂ���
		/*
		 *  �ׂ���
		 *
		 *  �P�A�E�S�E�E�E�E�E�@/�@�P�Q�B�S�T�E�E�E�E�@/�@�E�Q�R�C�T�U�E�E�E
		 *  �E�E�R�S�D�U�V�E�E�@/�@�E�E�E�S�T�E�V�W�E�@/�@�E�E�E�E�T�U�F�W�X
		 *  �E�E�E�E�E�U�E�G�X
		 */
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) return false; // ���v�Ƌ󔒂͏��O
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((t % TILE_SUIT_STEP) >= 2) && ((t % TILE_SUIT_STEP) <= 7))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 1))
					return true;
				else if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 2))
					return true;
			if (((t % TILE_SUIT_STEP) >= 3) && ((t % TILE_SUIT_STEP) <= 8))
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 2))
					return true;
				else if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 1))
					return true;
		}
		return false;
}

bool aiscript::table::functable::gametbl::luafunc::riskchk::isnamakurasuji
	(const GameTable* const gameStat, PLAYER_ID player, PLAYER_ID tplayer, int index) { // �݋؂��ǂ���
		/*
		 *  �݋�
		 *
		 *  �@�E�E�C�E�E�V�E�E�@/�@�E�A�E�E�D�E�E�W�E�@/�@�E�E�B�E�E�E�E�E�X
		 *  �P�E�E�C�E�E�F�E�E�@/�@�E�Q�E�E�D�E�E�G�E�@/�@�E�E�R�E�E�E�E�E�H
		 */
		::tileCode t = gameStat->Player[player].Hand[index].tile;
		if ((t >= TILE_SUIT_HONORS)||(t == NoTile)) return false; // ���v�Ƌ󔒂͏��O
		bool flag1 = false; bool flag2 = false;
		for (int i = 1; i <= gameStat->Player[tplayer].DiscardPointer; i++) {
			if (((t % TILE_SUIT_STEP) >= 1) && ((t % TILE_SUIT_STEP) <= 3)) {
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 6))
					flag1 = true;
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 3))
					flag2 = true;
			}
			if (((t % TILE_SUIT_STEP) >= 4) && ((t % TILE_SUIT_STEP) <= 6)) {
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t + 3))
					flag1 = true;
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 3))
					flag1 = true;
			}
			if (((t % TILE_SUIT_STEP) >= 7) && ((t % TILE_SUIT_STEP) <= 9)) {
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 6))
					flag1 = true;
				if (gameStat->Player[tplayer].Discard[i].tcode.tile == (t - 3))
					flag2 = true;
			}
			if (gameStat->Player[tplayer].Discard[i].tcode.tile == t)
				flag2 = true; // �����������������
		}
		return flag1 && (!flag2);
}