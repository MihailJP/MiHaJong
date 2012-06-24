#include "gametbl.h"

static GameTable GameStat;

inline bool chkGameType(GameTable* gameStat, gameTypeID gameType) {
	return ((gameStat->gameType) & gameType);
};

extern "C" {

	__declspec(dllexport) GameTable* initTable(int gameType) {
		GameStat = GameTable();
		GameStat.gameType = (gameTypeID)gameType;
		return &GameStat;
	}

	/* ---------------------------------------------------------------------
	 *
	 *  HSPからのアクセサ
	 *
	 * ---------------------------------------------------------------------
	 */

	__declspec(dllexport) void setScore(GameTable* gameStat, int Player, int Digit, int value) {
		gameStat->Player[Player].PlayerScore[Digit] = value; return;
	}
	__declspec(dllexport) void addScore(GameTable* gameStat, int Player, int Digit, int value) {
		gameStat->Player[Player].PlayerScore[Digit] += value; return;
	}
	__declspec(dllexport) int getScore(GameTable* gameStat, int Player, int Digit) {
		return gameStat->Player[Player].PlayerScore[Digit];
	}
	/* exportScore */
	/* importScore */

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setChip(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].playerChip = value; return;
	}
	__declspec(dllexport) void addChip(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].playerChip += value; return;
	}
	__declspec(dllexport) int getChip(GameTable* gameStat, int Player, int value) {
		return gameStat->Player[Player].playerChip;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setSumaroFlag(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].SumaroFlag = value; return;
	}
	__declspec(dllexport) int getSumaroFlag(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].SumaroFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setYakitori(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].YakitoriFlag = value; return;
	}
	__declspec(dllexport) int getYakitori(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].YakitoriFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPlayer(GameTable* gameStat, int value) {
		gameStat->PlayerID = value; return;
	}
	__declspec(dllexport) int getPlayer(GameTable* gameStat) {
		return gameStat->PlayerID;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getGameLength(GameTable* gameStat) {
		return gameStat->GameLength;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRound(GameTable* gameStat, int value) {
		gameStat->GameRound = value; return;
	}
	__declspec(dllexport) void incRound(GameTable* gameStat) {
		gameStat->GameRound++; return;
	}
	__declspec(dllexport) int getRound(GameTable* gameStat) {
		return gameStat->GameRound;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRoundLoop(GameTable* gameStat, int value) {
		gameStat->LoopRound = value; return;
	}
	__declspec(dllexport) void incRoundLoop(GameTable* gameStat) {
		gameStat->LoopRound++; return;
	}
	__declspec(dllexport) int getRoundLoop(GameTable* gameStat) {
		return gameStat->LoopRound;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHonba(GameTable* gameStat, int value) {
		gameStat->Honba = value; return;
	}
	__declspec(dllexport) void incHonba(GameTable* gameStat) {
		gameStat->Honba++; return;
	}
	__declspec(dllexport) int getHonba(GameTable* gameStat) {
		return gameStat->Honba;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setTurn(GameTable* gameStat, int value) {
		gameStat->TurnRound = value; return;
	}
	__declspec(dllexport) void incTurn(GameTable* gameStat) {
		gameStat->TurnRound++; return;
	}
	__declspec(dllexport) int getTurn(GameTable* gameStat) {
		return gameStat->TurnRound;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDeposit(GameTable* gameStat, int value) {
		gameStat->Deposit = value; return;
	}
	__declspec(dllexport) void addDeposit(GameTable* gameStat, int value) {
		gameStat->Deposit += value; return;
	}
	__declspec(dllexport) int getDeposit(GameTable* gameStat) {
		return gameStat->Deposit;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setAgariChain(GameTable* gameStat, int value) {
		gameStat->AgariChain = value; return;
	}
	__declspec(dllexport) void incAgariChain(GameTable* gameStat) {
		gameStat->AgariChain++; return;
	}
	__declspec(dllexport) int getAgariChain(GameTable* gameStat) {
		return gameStat->AgariChain;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setLastAgariPlayer(GameTable* gameStat, int value) {
		gameStat->LastAgariPlayer = value; return;
	}
	__declspec(dllexport) int getLastAgariPlayer(GameTable* gameStat) {
		return gameStat->LastAgariPlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHand(GameTable* gameStat, int Page, int Index, int Player, int value) {
		switch (Page) {
		case 0:
			gameStat->Player[Player].Hand[Index].tile = (tileCode)value;
			break;
		case 1:
			gameStat->Player[Player].Hand[Index].red = (uint8_t)value;
			break;
		default:
			throw(std::domain_error("setHand(): ページが違います"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getHand(GameTable* gameStat, int Page, int Index, int Player) {
		switch (Page) {
		case 0:
			return (int)gameStat->Player[Player].Hand[Index].tile;
		case 1:
			return (int)gameStat->Player[Player].Hand[Index].red;
		default:
			throw(std::domain_error("setHand(): ページが違います"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDiscard(GameTable* gameStat, int Page, int Index, int Player, int value) {
		switch (Page) {
		case 0:
			gameStat->Player[Player].Discard[Index].dstat = (discardStat)(value / SUTEHAI_TYPE_STEP);
			gameStat->Player[Player].Discard[Index].tcode.tile = (tileCode)(value % SUTEHAI_TYPE_STEP);
			break;
		case 1:
			gameStat->Player[Player].Discard[Index].tcode.red = (uint8_t)value;
			break;
		case 2:
			gameStat->Player[Player].Discard[Index].isDiscardThrough = (bool)value;
			break;
		default:
			throw(std::domain_error("setDiscard(): ページが違います"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getDiscard(GameTable* gameStat, int Page, int Index, int Player) {
		switch (Page) {
		case 0:
			return gameStat->Player[Player].Discard[Index].dstat * SUTEHAI_TYPE_STEP
				+ gameStat->Player[Player].Discard[Index].tcode.tile;
		case 1:
			return (int)gameStat->Player[Player].Discard[Index].tcode.red;
		case 2:
			return gameStat->Player[Player].Discard[Index].isDiscardThrough ? 1 : 0;
		default:
			throw(std::domain_error("getDiscard(): ページが違います"));
			break;
		}
	}
	__declspec(dllexport) int DiscardPointer(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].DiscardPointer;
	}
	__declspec(dllexport) void DiscardPointerIncrement(GameTable* gameStat, int Player) {
		gameStat->Player[Player].DiscardPointer++;
		return;
	}
	__declspec(dllexport) void flagDiscard(GameTable* gameStat, int Index, int Player, int value) {
		gameStat->Player[Player].Discard[Index].dstat = (discardStat)
			(gameStat->Player[Player].Discard[Index].dstat + value / SUTEHAI_TYPE_STEP);
		return;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setMeld(GameTable* gameStat, int Page, int Index, int Player, int value) {
		switch (Page) {
		case 0:
			gameStat->Player[Player].Meld[Index].tcode.tile = (tileCode)(value % MELD_TYPE_STEP);
			gameStat->Player[Player].Meld[Index].mstat = (meldStat)(value / MELD_TYPE_STEP);
			break;
		case 1:
			gameStat->Player[Player].Meld[Index].tcode.red = (uint8_t)value;
			break;
		default:
			throw(std::domain_error("setMeld(): ページが違います"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getMeld(GameTable* gameStat, int Page, int Index, int Player) {
		switch (Page) {
		case 0:
			return gameStat->Player[Player].Meld[Index].mstat * MELD_TYPE_STEP
				+ gameStat->Player[Player].Meld[Index].tcode.tile;
		case 1:
			return (int)gameStat->Player[Player].Meld[Index].tcode.red;
		default:
			throw(std::domain_error("getMeld(): ページが違います"));
		}
	}
	__declspec(dllexport) int MeldPointer(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].MeldPointer;
	}
	__declspec(dllexport) void MeldPointerIncrement(GameTable* gameStat, int Player) {
		gameStat->Player[Player].MeldPointer++;
		return;
	}
	__declspec(dllexport) void MeldPointerDecrement(GameTable* gameStat, int Player) {
		gameStat->Player[Player].MeldPointer--;
		return;
	}
	__declspec(dllexport) void flagMeld(GameTable* gameStat, int Page, int Index, int Player, int value) {
		switch (Page) {
		case 0:
			gameStat->Player[Player].Meld[Index].mstat = (meldStat)(
				gameStat->Player[Player].Meld[Index].mstat + value / MELD_TYPE_STEP);
			break;
		default:
			throw(std::domain_error("flagMeld(): ページが違います"));
			break;
		}
		return;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setMenzen(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].MenzenFlagAb = value;
		if (value == 0) gameStat->Player[Player].MenzenFlag = false;
		else if (value == 1) gameStat->Player[Player].MenzenFlag = true;
		return;
	}
	__declspec(dllexport) int getMenzen(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].MenzenFlagAb;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHandStat(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].HandStat = value;
		return;
	}
	__declspec(dllexport) int getHandStat(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].HandStat;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setNumberOfQuads(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].NumberOfQuads = value;
		return;
	}
	__declspec(dllexport) void incNumberOfQuads(GameTable* gameStat, int Player) {
		gameStat->Player[Player].NumberOfQuads++;
		return;
	}
	__declspec(dllexport) int getNumberOfQuads(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].NumberOfQuads;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRichiFlag(GameTable* gameStat, int Page, int Player, int value) {
		switch (Page) {
		case 0:
			switch (value) {
			case 0:
				gameStat->Player[Player].RichiFlag.RichiFlag = false;
				gameStat->Player[Player].RichiFlag.IppatsuFlag = false;
				gameStat->Player[Player].RichiFlag.DoubleFlag = false;
				break;
			case 1:
				gameStat->Player[Player].RichiFlag.RichiFlag = true;
				gameStat->Player[Player].RichiFlag.IppatsuFlag = false;
				gameStat->Player[Player].RichiFlag.DoubleFlag = false;
				break;
			case 2:
				gameStat->Player[Player].RichiFlag.RichiFlag = true;
				gameStat->Player[Player].RichiFlag.IppatsuFlag = true;
				gameStat->Player[Player].RichiFlag.DoubleFlag = false;
				break;
			case 3:
				gameStat->Player[Player].RichiFlag.RichiFlag = true;
				gameStat->Player[Player].RichiFlag.IppatsuFlag = false;
				gameStat->Player[Player].RichiFlag.DoubleFlag = true;
				break;
			case 4:
				gameStat->Player[Player].RichiFlag.RichiFlag = true;
				gameStat->Player[Player].RichiFlag.IppatsuFlag = true;
				gameStat->Player[Player].RichiFlag.DoubleFlag = true;
				break;
			default:
				throw(std::domain_error("setRichiFlag(): 正しくない値が指定されました"));
			}
			break;
		case 1:
			gameStat->Player[Player].RichiFlag.OpenFlag = value;
			break;
		default:
			throw(std::domain_error("setRichiFlag(): 正しくないページが指定されました"));
		}
	}
	__declspec(dllexport) int getRichiFlag(GameTable* gameStat, int Page, int Player) {
		switch (Page) {
		case 0:
			return gameStat->Player[Player].RichiFlag.RichiFlag ?
				1 +
				(gameStat->Player[Player].RichiFlag.IppatsuFlag ? 1 : 0) +
				(gameStat->Player[Player].RichiFlag.DoubleFlag ? 2 : 0)
				: 0;
		case 1:
			return gameStat->Player[Player].RichiFlag.OpenFlag ? 1 : 0;
		default:
			throw(std::domain_error("getRichiFlag(): 正しくないページが指定されました"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setOpenWait(GameTable* gameStat, int Tile, int value) {
		gameStat->OpenRichiWait[Tile] = value;
		return;
	}
	__declspec(dllexport) int getOpenWait(GameTable* gameStat, int Tile) {
		return gameStat->OpenRichiWait[Tile] ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setFirstDrawFlag(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].FirstDrawFlag = value;
		return;
	}
	__declspec(dllexport) int getFirstDrawFlag(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].FirstDrawFlag ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoujunFuriten(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].DoujunFuriten = value;
		return;
	}
	__declspec(dllexport) int getDoujunFuriten(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].DoujunFuriten ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setAgariHouki(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].AgariHouki = value;
		return;
	}
	__declspec(dllexport) int getAgariHouki(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].AgariHouki ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void putFlowerFlag(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].FlowerFlag = value;
		return;
	}
	__declspec(dllexport) void setFlowerFlag(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].FlowerFlag |= value;
		return;
	}
	__declspec(dllexport) int getFlowerFlag(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].FlowerFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setNorthFlag(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].NorthFlag = value;
		return;
	}
	__declspec(dllexport) void addNorthFlag(GameTable* gameStat, int Player) {
		gameStat->Player[Player].NorthFlag++;
		return;
	}
	__declspec(dllexport) int getNorthFlag(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].NorthFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setKangFlag(GameTable* gameStat, int Page, int value) {
		switch (Page) {
			case 0: gameStat->KangFlag.kangFlag = (uint8_t)value; break;
			case 1: gameStat->KangFlag.chainFlag = (uint8_t)value; break;
			case 2: gameStat->KangFlag.topFlag = (uint8_t)value; break;
			case 3: gameStat->KangFlag.chankanFlag = (uint8_t)value; break;
			default: throw(std::domain_error("setKangFlag(): ページが違います")); break;
		}
		return;
	}
	__declspec(dllexport) void incKangFlag(GameTable* gameStat, int Page) {
		switch (Page) {
			case 0: gameStat->KangFlag.kangFlag++; break;
			case 1: gameStat->KangFlag.chainFlag++; break;
			case 2: gameStat->KangFlag.topFlag++; break;
			case 3: gameStat->KangFlag.chankanFlag++; break;
			default: throw(std::domain_error("incKangFlag(): ページが違います")); break;
		}
		return;
	}
	__declspec(dllexport) int getKangFlag(GameTable* gameStat, int Page) {
		switch (Page) {
			case 0: return (int)gameStat->KangFlag.kangFlag;
			case 1: return (int)gameStat->KangFlag.chainFlag;
			case 2: return (int)gameStat->KangFlag.topFlag;
			case 3: return (int)gameStat->KangFlag.chankanFlag;
			default: throw(std::domain_error("getKangFlag(): ページが違います"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setTotalKang(GameTable* gameStat, int value) {
		gameStat->KangNum = value;
		return;
	}
	__declspec(dllexport) void incTotalKang(GameTable* gameStat) {
		gameStat->KangNum++;
		return;
	}
	__declspec(dllexport) int getTotalKang(GameTable* gameStat) {
		return gameStat->KangNum;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRichiCounterFlag(GameTable* gameStat, int value) {
		gameStat->RichiCounter = value;
		return;
	}
	__declspec(dllexport) int getRichiCounterFlag(GameTable* gameStat) {
		return gameStat->RichiCounter;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void calcWareme(GameTable* gameStat) {
		if (chkGameType(gameStat, AllSanma)) {
			if ((getRule(RULE_WAREME) != 0)||(getRule(RULE_KAIMENKAZE) != 0)) {
				gameStat->WaremePlayer = ((gameStat->GameRound-(gameStat->GameRound/4))+24
					+(gameStat->Dice1+gameStat->Dice2)-1) % 3;
				if (chkGameType(gameStat, Sanma4)) {
					gameStat->WaremePlayer = ((0)+24+(gameStat->Dice1+gameStat->Dice2)-1) % 3;
					std::array<PLAYER_ID, 3> tobePlayed = {
						(gameStat->GameRound % PLAYERS),
						((gameStat->GameRound + 1) % PLAYERS),
						((gameStat->GameRound + 2) % PLAYERS)
					};
					gameStat->WaremePlayer = tobePlayed[gameStat->WaremePlayer];
				}
			}
		} else {
			if ((getRule(RULE_WAREME) != 0)||(getRule(RULE_KAIMENKAZE) != 0)) {
				gameStat->WaremePlayer = ((gameStat->GameRound % 4)+32+(gameStat->Dice1+gameStat->Dice2)-1) % 4;
			}
		}
		return;
	}
	__declspec(dllexport) int getWareme(GameTable* gameStat) {
		return gameStat->WaremePlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoukasen(GameTable* gameStat, int value) {
		gameStat->DoukasenPlayer = value;
		return;
	}
	__declspec(dllexport) int getDoukasen(GameTable* gameStat) {
		return gameStat->DoukasenPlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getShibari(GameTable* gameStat) {
		return (int)gameStat->ShibariFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPao(GameTable* gameStat, int Page, int Yaku, int value) {
		switch (Page) {
			case 0: gameStat->PaoFlag[Yaku].paoPlayer = (PLAYER_ID)value; break;
			case 1: gameStat->PaoFlag[Yaku].agariPlayer = (PLAYER_ID)value; break;
			default: throw(std::domain_error("setPao(): ページが違います")); break;
		}
		return;
	}
	__declspec(dllexport) int getPao(GameTable* gameStat, int Page, int Yaku) {
		switch (Page) {
			case 0: return (int)gameStat->PaoFlag[Yaku].paoPlayer;
			case 1: return (int)gameStat->PaoFlag[Yaku].agariPlayer;
			default: throw(std::domain_error("getPao(): ページが違います"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDice(GameTable* gameStat, int ID, int Direction, int value) {
		switch (ID+Direction*2) {
			case 0: gameStat->Dice1 = value; break;
			case 1: gameStat->Dice2 = value; break;
			case 2: gameStat->Dice1Direction = value; break;
			case 3: gameStat->Dice2Direction = value; break;
		}
		return;
	}
	__declspec(dllexport) int getDice(GameTable* gameStat, int ID, int Direction) {
		switch (ID+Direction*2) {
			case 0: return gameStat->Dice1; break;
			case 1: return gameStat->Dice2; break;
			case 2: return gameStat->Dice1Direction ? 1 : 0; break;
			case 3: return gameStat->Dice2Direction ? 1 : 0; break;
			default: throw(std::domain_error("getDice(): 正しくない値が指定されました")); break;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void addDeadWallLength(GameTable* gameStat, int value) {
		gameStat->DeadTiles += value;
		return;
	}
	__declspec(dllexport) int getDeadWallLength(GameTable* gameStat) {
		return gameStat->DeadTiles;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setWall(GameTable* gameStat, int Page, int Index, int value) {
		switch (Page) {
		case 0:
			if (chkGameType(gameStat, Yonma)) {
				switch (getRule(RULE_FLOWER_TILES)) {
				case 3:
					gameStat->Deck.deck144[Index].tile = (tileCode)value;
					break;
				case 0:
					gameStat->Deck.deck136[Index].tile = (tileCode)value;
					break;
				default:
					gameStat->Deck.deck140[Index].tile = (tileCode)value;
					break;
				}
			} else {
				gameStat->Deck.deck108[Index].tile = (tileCode)value;
			}
			break;
		case 1:
			if (chkGameType(gameStat, Yonma)) {
				switch (getRule(RULE_FLOWER_TILES)) {
				case 3:
					gameStat->Deck.deck144[Index].red = (uint8_t)value;
					break;
				case 0:
					gameStat->Deck.deck136[Index].red = (uint8_t)value;
					break;
				default:
					gameStat->Deck.deck140[Index].red = (uint8_t)value;
					break;
				}
			} else {
				gameStat->Deck.deck108[Index].red = (uint8_t)value;
			}
			break;
		default:
			throw(std::domain_error("setWall(): ページが違います"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getWall(GameTable* gameStat, int Page, int Index) {
		switch (Page) {
		case 0:
			if (chkGameType(gameStat, Yonma)) {
				switch (getRule(RULE_FLOWER_TILES)) {
				case 3:
					return (int)gameStat->Deck.deck144[Index].tile;
					break;
				case 0:
					return (int)gameStat->Deck.deck136[Index].tile;
					break;
				default:
					return (int)gameStat->Deck.deck140[Index].tile;
					break;
				}
			} else {
				return (int)gameStat->Deck.deck108[Index].tile;
			}
			break;
		case 1:
			if (chkGameType(gameStat, Yonma)) {
				switch (getRule(RULE_FLOWER_TILES)) {
				case 3:
					return (int)gameStat->Deck.deck144[Index].red;
					break;
				case 0:
					return (int)gameStat->Deck.deck136[Index].red;
					break;
				default:
					return (int)gameStat->Deck.deck140[Index].red;
					break;
				}
			} else {
				return (int)gameStat->Deck.deck108[Index].red;
			}
			break;
		default:
			throw(std::domain_error("getWall(): ページが違います"));
			break;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getRinshanExtension(GameTable* gameStat) {
		return gameStat->ExtraRinshan;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDrawPointer(GameTable* gameStat, int value) {
		gameStat->TilePointer = value; return;
	}
	__declspec(dllexport) void incDrawPointer(GameTable* gameStat) {
		gameStat->TilePointer++; return;
	}
	__declspec(dllexport) int getDrawPointer(GameTable* gameStat) {
		return gameStat->TilePointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoraPointer(GameTable* gameStat, int value) {
		gameStat->DoraPointer = value; return;
	}
	__declspec(dllexport) int getDoraPointer(GameTable* gameStat) {
		return gameStat->DoraPointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRinshanPointer(GameTable* gameStat, int value) {
		gameStat->RinshanPointer = value; return;
	}
	__declspec(dllexport) void decRinshanPointer(GameTable* gameStat) {
		gameStat->RinshanPointer--; return;
	}
	__declspec(dllexport) int getRinshanPointer(GameTable* gameStat) {
		return gameStat->RinshanPointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHeavenHandFlag(GameTable* gameStat, int value) {
		gameStat->TianHuFlag = value;
		return;
	}
	__declspec(dllexport) int getHeavenHandFlag(GameTable* gameStat) {
		return gameStat->TianHuFlag ? 1: 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPreviousMeld(GameTable* gameStat, int Page, int value) {
		switch (Page) {
			case 0: gameStat->PreviousMeld.Discard = (tileCode) value; break;
			case 1: gameStat->PreviousMeld.Stepped = (tileCode) value; break;
			default: throw(std::domain_error("setPreviousMeld(): ページが違います")); break;
		}
		return;
	}
	__declspec(dllexport) int getPreviousMeld(GameTable* gameStat, int Page) {
		switch (Page) {
			case 0: return (int)gameStat->PreviousMeld.Discard;
			case 1: return (int)gameStat->PreviousMeld.Stepped;
			default: throw(std::domain_error("getPreviousMeld(): ページが違います")); break;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDisconnectFlag(GameTable* gameStat, int Player, int value) {
		gameStat->Player[Player].ConnectionLost = value;
		return;
	}
	__declspec(dllexport) int getDisconnectFlag(GameTable* gameStat, int Player) {
		return gameStat->Player[Player].ConnectionLost ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoraFlag(GameTable* gameStat, int Page, int Tile, int value) {
		switch (Page) {
			case 0: gameStat->DoraFlag.Omote[Tile] = value; break;
			case 1: gameStat->DoraFlag.Ura[Tile] = value; break;
			default: throw(std::domain_error("setDoraFlag(): ページが違います")); break;
		}
		return;
	}
	__declspec(dllexport) void incDoraFlag(GameTable* gameStat, int Page, int Tile) {
		switch (Page) {
			case 0: gameStat->DoraFlag.Omote[Tile]++; break;
			case 1: gameStat->DoraFlag.Ura[Tile]++; break;
			default: throw(std::domain_error("incDoraFlag(): ページが違います")); break;
		}
		return;
	}
	__declspec(dllexport) int getDoraFlag(GameTable* gameStat, int Page, int Tile) {
		switch (Page) {
			case 0: return (int)gameStat->DoraFlag.Omote[Tile];
			case 1: return (int)gameStat->DoraFlag.Ura[Tile];
			default: throw(std::domain_error("getDoraFlag(): ページが違います"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setCurrentDiscard(GameTable* gameStat, int Page, int value) {
		switch (Page) {
		case 0:
			gameStat->CurrentDiscard.tile = (tileCode)value;
			break;
		case 1:
			gameStat->CurrentDiscard.red = (uint8_t)value;
			break;
		default:
			throw(std::domain_error("setCurrentDiscard(): ページが違います"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getCurrentDiscard(GameTable* gameStat, int Page) {
		switch (Page) {
		case 0:
			return (int)gameStat->CurrentDiscard.tile;
		case 1:
			return (int)gameStat->CurrentDiscard.red;
		default:
			throw(std::domain_error("getCurrentDiscard(): ページが違います"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setTsumoAgariFlag(GameTable* gameStat, int value) {
		gameStat->TsumoAgariFlag = gameStat->AgariSpecialStat = value;
		return;
	}
	__declspec(dllexport) int getTsumoAgariFlag(GameTable* gameStat) {
		return gameStat->AgariSpecialStat;
	}

	// ---------------------------------------------------------------------

/*
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
