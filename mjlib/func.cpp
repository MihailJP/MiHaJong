#include "func.h"

TileCount countTilesInHand(GameTable gameStat, PLAYER_ID playerID) {
	// 手牌に存在する牌を種類別にカウントする（鳴き面子・暗槓は除く）
	TileCount count = TileCount(); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = tileCode(gameStat.Player[playerID].Hand[i].tile)) != tileCode::NoTile)
			count.count[tmpTC]++;
	}
	return TileCount(count);
}

/* -------------------------------------------------------------------------
 *  function.hsp から移植
 * -------------------------------------------------------------------------
 */

/* プレイヤーの自風がどれか調べる */
seatAbsolute inline playerwind(GameTable* gameStat, PLAYER_ID player, PLAYER_ID currentRound) {
	if (chkGameType(gameStat, SanmaT))
		return (seatAbsolute)((player + 24 - (currentRound - ( currentRound / 4))) % 3);
	else return (seatAbsolute)((player + 32 - currentRound) % 4);
}
__declspec(dllexport) inline int playerwind(int player, int currentRound) {
	return (int)playerwind(&GameStat, (PLAYER_ID)player, (PLAYER_ID)currentRound);
}

/* あるプレイヤーに対して指定したプレイヤーがどこ(下家、対面、上家)にいるか調べる */
seatRelative inline playerRelative(PLAYER_ID targetPlayer, PLAYER_ID basePlayer) {
	return (seatRelative)((PLAYERS + targetPlayer - basePlayer) % PLAYERS);
}
__declspec(dllexport) inline int playerRelative(int targetPlayer, int basePlayer) {
	return (int)playerRelative((PLAYER_ID)targetPlayer, (PLAYER_ID)basePlayer);
}

/* あるプレイヤーの(下家、対面、上家)を調べる */
PLAYER_ID inline RelativePositionOf(PLAYER_ID targetPlayer, seatRelative relative) {
	return (PLAYER_ID)(((int)targetPlayer + (int)relative) % PLAYERS);
}
__declspec(dllexport) inline int RelativePositionOf(int targetPlayer, int relative) {
	return (int)RelativePositionOf((PLAYER_ID)targetPlayer, (seatRelative)relative);
}

/* 一周するまでに必要な場の数 */
__declspec(dllexport) inline int roundLoopRate() {
	if (getRule(RULE_SUDDEN_DEATH_TYPE) == 1) return 28;
	else return 16;
}

/* 王牌を除いた山牌の残り枚数 */
__declspec(dllexport) inline int tilesLeft(GameTable* gameStat) {
	return ((int)gameStat->RinshanPointer -
		((int)gameStat->DeadTiles - 1) -
		(int)gameStat->TilePointer);
}

/* 順位を計算する */
PlayerRankList calcRanc(GameTable* gameStat) {
	PlayerRankList rankList;
	rankList.fill(0);
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		rankList[i] = 1;
		for (int j = 0; j < ACTUAL_PLAYERS; j++) {
			if (gameStat->Player[j].PlayerScore > gameStat->Player[i].PlayerScore)
				rankList[i]++;
		}
	}
	return rankList;
}
__declspec(dllexport) void calcRanc(int* Rank, GameTable* gameStat) {
	const PlayerRankList rankList = calcRanc(gameStat);
	assert(rankList[0] > 0); assert(rankList[1] > 0);
	for (int i = 0; i < PLAYERS; i++) *(Rank + i) = rankList[i];
}

/* 包かどうかの判定 */
bool isPao(GameTable* gameStat, PLAYER_ID agariPlayer, PLAYER_ID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if ((paoPlayer == gameStat->PaoFlag[i].paoPlayer) &&
			(agariPlayer == gameStat->PaoFlag[i].agariPlayer))
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isPao(GameTable* gameStat, int agariPlayer, int paoPlayer) {
	return (int)isPao(gameStat, (PLAYER_ID)agariPlayer, (PLAYER_ID)paoPlayer);
}

bool isPaoAgari(GameTable* gameStat, PLAYER_ID agariPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if (agariPlayer == gameStat->PaoFlag[i].agariPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isPaoAgari(GameTable* gameStat, int agariPlayer) {
	return (int)isPaoAgari(gameStat, (PLAYER_ID)agariPlayer);
}

bool isGotPao(GameTable* gameStat, PLAYER_ID paoPlayer) {
	bool paoFlag = false;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if (paoPlayer == gameStat->PaoFlag[i].paoPlayer)
		{ paoFlag = true; break;}
	}
	return paoFlag;
}
__declspec(dllexport) int isGotPao(GameTable* gameStat, int paoPlayer) {
	return (int)isGotPao(gameStat, (PLAYER_ID)paoPlayer);
}

PLAYER_ID getPaoPlayer(GameTable* gameStat, PLAYER_ID agariPlayer) {
	PLAYER_ID paoPlayer = -1;
	for (int i = 0; i < PAO_YAKU_PAGES; i++) {
		if ((gameStat->PaoFlag[i].paoPlayer >= 0) &&
			(gameStat->PaoFlag[i].agariPlayer == agariPlayer))
			paoPlayer = gameStat->PaoFlag[i].paoPlayer;
	}
	return paoPlayer;
}
__declspec(dllexport) int getPaoPlayer(GameTable* gameStat, int agariPlayer) {
	return (int)getPaoPlayer(gameStat, (PLAYER_ID)agariPlayer);
}

/* ロンしたプレイヤーの数 */
__declspec(dllexport) int RonPlayers(GameTable* gameStat) {
	int qualified = 0;
	for (int i = 0; i < PLAYERS; i++)
		if (gameStat->Player[i].DeclarationFlag.Ron)
			qualified++;
	return qualified;
}
