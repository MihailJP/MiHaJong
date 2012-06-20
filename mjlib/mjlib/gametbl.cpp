#include "gametbl.h"

static GameTable GameStat;

extern "C" {

	__declspec(dllexport) GameTable* initTable() {
		GameStat = GameTable();
		return &GameStat;
	}

	/* HSPからのアクセサ */

	__declspec(dllexport) void setScore(GameTable* gameStat, int Player, int Digit, int value) {
		gameStat->PlayerScore[Player][Digit] = value; return;
	}
	__declspec(dllexport) void addScore(GameTable* gameStat, int Player, int Digit, int value) {
		gameStat->PlayerScore[Player][Digit] += value; return;
	}
	__declspec(dllexport) int getScore(GameTable* gameStat, int Player, int Digit) {
		return gameStat->PlayerScore[Player][Digit];
	}
	/* exportScore */
	/* importScore */

	__declspec(dllexport) void setChip(GameTable* gameStat, int Player, int value) {
		gameStat->playerChip[Player] = value; return;
	}
	__declspec(dllexport) void addChip(GameTable* gameStat, int Player, int value) {
		gameStat->playerChip[Player] += value; return;
	}
	__declspec(dllexport) int getChip(GameTable* gameStat, int Player, int value) {
		return gameStat->playerChip[Player];
	}

	__declspec(dllexport) void setSumaroFlag(GameTable* gameStat, int Player, int value) {
		gameStat->SumaroFlag[Player] = value; return;
	}
	__declspec(dllexport) int getSumaroFlag(GameTable* gameStat, int Player) {
		return gameStat->SumaroFlag[Player];
	}

	__declspec(dllexport) void setYakitori(GameTable* gameStat, int Player, int value) {
		gameStat->YakitoriFlag[Player] = value; return;
	}
	__declspec(dllexport) int getYakitori(GameTable* gameStat, int Player) {
		return gameStat->YakitoriFlag[Player];
	}

	__declspec(dllexport) void setPlayer(GameTable* gameStat, int value) {
		gameStat->PlayerID = value; return;
	}
	__declspec(dllexport) int getPlayer(GameTable* gameStat) {
		return gameStat->PlayerID;
	}

	__declspec(dllexport) int getGameLength(GameTable* gameStat) {
		return gameStat->GameLength;
	}

	__declspec(dllexport) void setRound(GameTable* gameStat, int value) {
		gameStat->GameRound = value; return;
	}
	__declspec(dllexport) void incRound(GameTable* gameStat) {
		gameStat->GameRound++; return;
	}
	__declspec(dllexport) int getRound(GameTable* gameStat) {
		return gameStat->GameRound;
	}

	__declspec(dllexport) void setRoundLoop(GameTable* gameStat, int value) {
		gameStat->LoopRound = value; return;
	}
	__declspec(dllexport) void incRoundLoop(GameTable* gameStat) {
		gameStat->LoopRound++; return;
	}
	__declspec(dllexport) int getRoundLoop(GameTable* gameStat) {
		return gameStat->LoopRound;
	}

	__declspec(dllexport) void setHonba(GameTable* gameStat, int value) {
		gameStat->Honba = value; return;
	}
	__declspec(dllexport) void incHonba(GameTable* gameStat) {
		gameStat->Honba++; return;
	}
	__declspec(dllexport) int getHonba(GameTable* gameStat) {
		return gameStat->Honba;
	}

	__declspec(dllexport) void setTurn(GameTable* gameStat, int value) {
		gameStat->TurnRound = value; return;
	}
	__declspec(dllexport) void incTurn(GameTable* gameStat) {
		gameStat->TurnRound++; return;
	}
	__declspec(dllexport) int getTurn(GameTable* gameStat) {
		return gameStat->TurnRound;
	}

	__declspec(dllexport) void setDeposit(GameTable* gameStat, int value) {
		gameStat->Deposit = value; return;
	}
	__declspec(dllexport) void addDeposit(GameTable* gameStat, int value) {
		gameStat->Deposit += value; return;
	}
	__declspec(dllexport) int getDeposit(GameTable* gameStat) {
		return gameStat->Deposit;
	}

	__declspec(dllexport) void setAgariChain(GameTable* gameStat, int value) {
		gameStat->AgariChain = value; return;
	}
	__declspec(dllexport) void incAgariChain(GameTable* gameStat) {
		gameStat->AgariChain++; return;
	}
	__declspec(dllexport) int getAgariChain(GameTable* gameStat) {
		return gameStat->AgariChain;
	}

	__declspec(dllexport) void setLastAgariPlayer(GameTable* gameStat, int value) {
		gameStat->LastAgariPlayer = value; return;
	}
	__declspec(dllexport) int getLastAgariPlayer(GameTable* gameStat) {
		return gameStat->LastAgariPlayer;
	}

	__declspec(dllexport) void setHand(GameTable* gameStat, int Page, int Index, int Player, int value) {
		gameStat->Hand[Page][Player][Index] = (tileCode)value;
		return;
	}
	__declspec(dllexport) int getHand(GameTable* gameStat, int Page, int Index, int Player) {
		return gameStat->Hand[Page][Player][Index];
	}

	__declspec(dllexport) void setDiscard(GameTable* gameStat, int Page, int Index, int Player, int value) {
		gameStat->Discard[Page][Player][Index].dstat = (discardStat)(value / SUTEHAI_TYPE_STEP);
		gameStat->Discard[Page][Player][Index].tcode = (tileCode)(value % SUTEHAI_TYPE_STEP);
		return;
	}
	__declspec(dllexport) int getDiscard(GameTable* gameStat, int Page, int Index, int Player) {
		return gameStat->Discard[Page][Player][Index].dstat * SUTEHAI_TYPE_STEP
			+ gameStat->Discard[Page][Player][Index].tcode;
	}
	__declspec(dllexport) int DiscardPointer(GameTable* gameStat, int Player) {
		return gameStat->DiscardPointer[Player];
	}
	__declspec(dllexport) void DiscardPointerIncrement(GameTable* gameStat, int Player) {
		gameStat->DiscardPointer[Player]++;
		return;
	}
	__declspec(dllexport) void flagDiscard(GameTable* gameStat, int Index, int Player, int value) {
		gameStat->Discard[dTileCode][Player][Index].dstat = (discardStat)
			(gameStat->Discard[dTileCode][Player][Index].dstat + value / SUTEHAI_TYPE_STEP);
		return;
	}

/*
__declspec(dllexport) void setMeld(GameTable* gameStat, int Page, int Index, int Player, int value) {
	gameStat->Meld(Index, Player, Page) = value
	return
__declspec(dllexport) int getMeld(GameTable* gameStat, int Page, int Index, int Player) {
	return gameStat->Meld(Index, Player, Page)
__declspec(dllexport) int MeldPointer(GameTable* gameStat, int Player) {
	return gameStat->Meld(0, Player, MELD_TILECODE)
__declspec(dllexport) void MeldPointerIncrement(GameTable* gameStat, int Player) {
	gameStat->Meld(0, Player, MELD_TILECODE)++
	return
__declspec(dllexport) void MeldPointerDecrement(GameTable* gameStat, int Player) {
	gameStat->Meld(0, Player, MELD_TILECODE)--
	return
__declspec(dllexport) void flagMeld(GameTable* gameStat, int Page, int Index, int Player, int value) {
	gameStat->Meld(Index, Player, Page) += value
	return

__declspec(dllexport) void setMenzen(GameTable* gameStat, int Player, int value) {
	MenzenFlag[Player] = value
	return
__declspec(dllexport) int getMenzen(GameTable* gameStat, int Player) {
	return MenzenFlag[Player]

__declspec(dllexport) void setHandStat(GameTable* gameStat, int Player, int value) {
	HandStat[Player] = value
	return
__declspec(dllexport) int getHandStat(GameTable* gameStat, int Player) {
	return HandStat[Player]

__declspec(dllexport) void setNumberOfQuads(GameTable* gameStat, int Player, int value) {
	NumberOfQuads[Player] = value
	return
__declspec(dllexport) void incNumberOfQuads(GameTable* gameStat, int Player) {
	NumberOfQuads[Player]++
	return
__declspec(dllexport) int getNumberOfQuads(GameTable* gameStat, int Player) {
	return NumberOfQuads[Player]

__declspec(dllexport) void setRichiFlag(GameTable* gameStat, int Page, int Player, int value) {
	RichiFlag(Player, Page) = value
	return
__declspec(dllexport) int getRichiFlag(GameTable* gameStat, int Page, int Player) {
	return RichiFlag(Player, Page)

__declspec(dllexport) void setOpenWait(GameTable* gameStat, int Tile, int value) {
	OpenRichiWait(Tile) = value
	return
__declspec(dllexport) int getOpenWait(GameTable* gameStat, int Tile) {
	return OpenRichiWait(Tile)

__declspec(dllexport) void setFirstDrawFlag(GameTable* gameStat, int Player, int value) {
	FirstDrawFlag[Player] = value
	return
__declspec(dllexport) int getFirstDrawFlag(GameTable* gameStat, int Player) {
	return FirstDrawFlag[Player]

__declspec(dllexport) void setDoujunFuriten(GameTable* gameStat, int Player, int value) {
	DoujunFuriten[Player] = value
	return
__declspec(dllexport) int getDoujunFuriten(GameTable* gameStat, int Player) {
	return DoujunFuriten[Player]

__declspec(dllexport) void setAgariHouki(GameTable* gameStat, int Player, int value) {
	AgariHouki[Player] = value
	return
__declspec(dllexport) int getAgariHouki(GameTable* gameStat, int Player) {
	return AgariHouki[Player]

__declspec(dllexport) void putFlowerFlag(GameTable* gameStat, int Player, int value) {
	FlowerFlag[Player] = value
	return
__declspec(dllexport) void setFlowerFlag(GameTable* gameStat, int Player, int value) {
	FlowerFlag[Player] |= value
	return
__declspec(dllexport) int getFlowerFlag(GameTable* gameStat, int Player) {
	return FlowerFlag[Player]

__declspec(dllexport) void setNorthFlag(GameTable* gameStat, int Player, int value) {
	NorthFlag[Player] = value
	return
__declspec(dllexport) void addNorthFlag(GameTable* gameStat, int Player) {
	NorthFlag[Player]++
	return
__declspec(dllexport) int getNorthFlag(GameTable* gameStat, int Player) {
	return NorthFlag[Player]

__declspec(dllexport) void setKangFlag(GameTable* gameStat, int Page, int value) {
	KangFlag(Page) = value
	return
__declspec(dllexport) void incKangFlag(GameTable* gameStat, int Page) {
	KangFlag(Page)++
	return
__declspec(dllexport) int getKangFlag(GameTable* gameStat, int Page) {
	return KangFlag(Page)

__declspec(dllexport) void setTotalKang(GameTable* gameStat, int value) {
	KangNum = value
	return
__declspec(dllexport) void incTotalKang
	KangNum++
	return
__declspec(dllexport) int getTotalKang
	return KangNum

__declspec(dllexport) void setRichiCounterFlag(GameTable* gameStat, int value) {
	RichiCounter = value
	return
__declspec(dllexport) int getRichiCounterFlag
	return RichiCounter

__declspec(dllexport) void calcWareme
#ifdef ALLSANMA
	if ((getRule(RULE_WAREME) != 0)||(getRule(RULE_KAIMENKAZE) != 0)) {
		WaremePlayer = ((GameRound-(GameRound/4))+24+(Dice1+Dice2)-1)\3
#ifdef SANMA4
		WaremePlayer = ((0)+24+(Dice1+Dice2)-1)\3
		switch (GameRound\NUM_OF_PLAYERS)
			case 0: tobePlayed = 0, 1, 2: swbreak
			case 1: tobePlayed = 1, 2, 3: swbreak
			case 2: tobePlayed = 2, 3, 0: swbreak
			case 3: tobePlayed = 3, 0, 1: swbreak
		swend
		WaremePlayer = tobePlayed(WaremePlayer)
#endif
	}
#else
	if ((getRule(RULE_WAREME) != 0)||(getRule(RULE_KAIMENKAZE) != 0)) {
		WaremePlayer = ((GameRound\4)+32+(Dice1+Dice2)-1)\4
	}
#endif
	return
__declspec(dllexport) int getWareme
	return WaremePlayer

__declspec(dllexport) void setDoukasen(GameTable* gameStat, int value) {
	DoukasenPlayer = value
	return
__declspec(dllexport) int getDoukasen
	return DoukasenPlayer

__declspec(dllexport) int getShibari
	return ShibariFlag

__declspec(dllexport) void setPao(GameTable* gameStat, int Page, int Yaku, int value) {
	PaoFlag(Yaku, Page) = value
	return
__declspec(dllexport) int getPao(GameTable* gameStat, int Page, int Yaku) {
	return PaoFlag(Yaku, Page)

__declspec(dllexport) void setDice(GameTable* gameStat, int ID, int Direction, int value) {
	switch ID+Direction*2
		case 0: Dice1 = value: swbreak
		case 1: Dice2 = value: swbreak
		case 2: Dice1Direction = value: swbreak
		case 3: Dice2Direction = value: swbreak
	swend
	return
__declspec(dllexport) int getDice(GameTable* gameStat, int ID, int Direction) {
	switch ID+Direction*2
		case 0: return Dice1: swbreak
		case 1: return Dice2: swbreak
		case 2: return Dice1Direction: swbreak
		case 3: return Dice2Direction: swbreak
	swend
	return 0x7fffffff

__declspec(dllexport) void addDeadWallLength(GameTable* gameStat, int value) {
	DeadTiles += value
	return
__declspec(dllexport) int getDeadWallLength
	return DeadTiles

__declspec(dllexport) void setWall(GameTable* gameStat, int Page, int Index, int value) {
	Deck(Index, Page) = value
	return
__declspec(dllexport) int getWall(GameTable* gameStat, int Page, int Index) {
	return Deck(Index, Page)

__declspec(dllexport) int getRinshanExtension
	return ExtraRinshan

__declspec(dllexport) void setDrawPointer(GameTable* gameStat, int value) {
	TilePointer = value: return
__declspec(dllexport) void incDrawPointer
	TilePointer++: return
__declspec(dllexport) int getDrawPointer
	return TilePointer

__declspec(dllexport) void setDoraPointer(GameTable* gameStat, int value) {
	DoraPointer = value: return
__declspec(dllexport) int getDoraPointer
	return DoraPointer

__declspec(dllexport) void setRinshanPointer(GameTable* gameStat, int value) {
	RinshanPointer = value: return
__declspec(dllexport) void decRinshanPointer
	RinshanPointer--: return
__declspec(dllexport) int getRinshanPointer
	return RinshanPointer

__declspec(dllexport) void setHeavenHandFlag(GameTable* gameStat, int value) {
	TianHuFlag = value
	return
__declspec(dllexport) int getHeavenHandFlag
	return TianHuFlag

__declspec(dllexport) void setPreviousMeld(GameTable* gameStat, int Page, int value) {
	PreviousMeld(Page) = value
	return
__declspec(dllexport) int getPreviousMeld(GameTable* gameStat, int Page) {
	return PreviousMeld(Page)

__declspec(dllexport) void setDisconnectFlag(GameTable* gameStat, int Player, int value) {
	ConnectionLost[Player] = value
	return
__declspec(dllexport) int getDisconnectFlag(GameTable* gameStat, int Player) {
	return ConnectionLost[Player]

__declspec(dllexport) void setDoraFlag(GameTable* gameStat, int Page, int Tile, int value) {
	DoraFlag(Tile, Page) = value
	return
__declspec(dllexport) void incDoraFlag(GameTable* gameStat, int Page, int Tile) {
	DoraFlag(Tile, Page)++
	return
__declspec(dllexport) int getDoraFlag(GameTable* gameStat, int Page, int Tile) {
	return DoraFlag(Tile, Page)

__declspec(dllexport) void setCurrentDiscard(GameTable* gameStat, int Page, int value) {
	CurrentDiscard(Page) = value
	return
__declspec(dllexport) int getCurrentDiscard(GameTable* gameStat, int Page) {
	return CurrentDiscard(Page)

__declspec(dllexport) void setTsumoAgariFlag(GameTable* gameStat, int value) {
	TsumoAgariFlag = value
	return
__declspec(dllexport) int getTsumoAgariFlag
	return TsumoAgariFlag

__declspec(dllexport) void setDeclarationFlag(GameTable* gameStat, int Page, int Player, int value) {
	DeclarationFlag(Player, Page) = value
	return
__declspec(dllexport) void resetDeclarationFlag
	dim DeclarationFlag, NUM_OF_PLAYERS, DECLARATIONFLAG_PAGES
	return
__declspec(dllexport) int getDeclarationFlag(GameTable* gameStat, int Page, int Player) {
	return DeclarationFlag(Player, Page)

__declspec(dllexport) void setCurrentPlayer(GameTable* gameStat, int Page, int value) {
	CurrentPlayer(Page) = value
	return
__declspec(dllexport) void resetCurrentPlayer
	dim CurrentPlayer, CURRENTPLAYER_PAGES
	repeat CURRENTPLAYER_PAGES: CurrentPlayer(cnt) = -1: loop
	return
__declspec(dllexport) int getCurrentPlayer(GameTable* gameStat, int Page) {
	return CurrentPlayer(Page)
*/

}
