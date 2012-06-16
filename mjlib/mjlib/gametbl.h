#ifndef GAMETBL_H
#define GAMETBL_H

class GameTable {
public:
	int PlayerScore;
	int playerChip;
	int SumaroFlag;
	int YakitoriFlag;
	int PlayerID;
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int TurnRound;
	int Deposit;
	int AgariChain;
	int LastAgariPlayer;
	int Hand;
	int Discard;
	int Meld;
	int MenzenFlag;
	int HandStat;
	int NumberOfQuads;
	int RichiFlag;
	int OpenRichiWait;
	int FirstDrawFlag;
	int DoujunFuriten;
	int AgariHouki;
	int FlowerFlag;
	int NorthFlag;
	int KangFlag;
	int KangNum;
	int RichiCounter;
	int WaremePlayer;
	int DoukasenPlayer;
	int Dice1;
	int Dice2;
	int Dice1Direction;
	int Dice2Direction;
	int PaoFlag;
	int Deck;
	int DeadTiles;
	int ExtraRinshan;
	int ShibariFlag;
	int DoraFlag;
	int TilePointer;
	int DoraPointer;
	int RinshanPointer;
	int TianHuFlag;
	int PreviousMeld;
	int ConnectionLost;
	int CurrentPlayer;
	int DeclarationFlag;
	int TsumoAgariFlag;
	int CurrentDiscard;
	// Constructor
	GameTable() {
	}
}

#endif
