#include "gametbl.h"

GameTable GameStat, StatSandBox;

inline bool chkGameType(GameTable* gameStat, gameTypeID gameType) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	return ((gameStat->gameType) & gameType);
};

extern "C" {

	/* ---------------------------------------------------------------------
	 *
	 *  HSP����̃A�N�Z�T
	 *
	 * ---------------------------------------------------------------------
	 */

	__declspec(dllexport) void setScore(GameTable* gameStat, int Player, int Digit, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].PlayerScore[Digit] = value *
			(Digit ? 1 : (100000000u / gameStat->Player[Player].PlayerScore.getFirstArg())); return;
	}
	__declspec(dllexport) void addScore(GameTable* gameStat, int Player, int Digit, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].PlayerScore[Digit] += value *
			(Digit ? 1 : (100000000u / gameStat->Player[Player].PlayerScore.getFirstArg())); return;
	}
	__declspec(dllexport) int getScore(GameTable* gameStat, int Player, int Digit) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].PlayerScore[Digit] /
			(Digit ? 1 : (100000000u / gameStat->Player[Player].PlayerScore.getFirstArg()));
	}
	__declspec(dllexport) void exportScore(GameTable* gameStat, int* exportArray) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		for (int j = 0; j < DIGIT_GROUPS; j++) {
			for (int i = 0; i < PLAYERS; i++) {
				*(exportArray + j * PLAYERS + i) =
					gameStat->Player[i].PlayerScore[j] /
					(j ? 1 : (100000000u / gameStat->Player[i].PlayerScore.getFirstArg()));
			}
		}
	}
	__declspec(dllexport) void importScore(GameTable* gameStat, int* importArray) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		for (int j = 0; j < DIGIT_GROUPS; j++) {
			for (int i = 0; i < PLAYERS; i++) {
				gameStat->Player[i].PlayerScore[j] =
					*(importArray + j * PLAYERS + i) *
					(j ? 1 : (100000000u / gameStat->Player[i].PlayerScore.getFirstArg()));
			}
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setChip(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].playerChip = value; return;
	}
	__declspec(dllexport) void addChip(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].playerChip += value; return;
	}
	__declspec(dllexport) int getChip(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].playerChip;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setSumaroFlag(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].SumaroFlag = value; return;
	}
	__declspec(dllexport) int getSumaroFlag(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].SumaroFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setYakitori(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].YakitoriFlag = value; return;
	}
	__declspec(dllexport) int getYakitori(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].YakitoriFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPlayer(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->PlayerID = value; return;
	}
	__declspec(dllexport) int getPlayer(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->PlayerID;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getGameLength(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->GameLength;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRound(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->GameRound = value; return;
	}
	__declspec(dllexport) void incRound(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->GameRound++; return;
	}
	__declspec(dllexport) int getRound(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->GameRound;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRoundLoop(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->LoopRound = value; return;
	}
	__declspec(dllexport) void incRoundLoop(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->LoopRound++; return;
	}
	__declspec(dllexport) int getRoundLoop(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->LoopRound;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHonba(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Honba = value; return;
	}
	__declspec(dllexport) void incHonba(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Honba++; return;
	}
	__declspec(dllexport) int getHonba(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Honba;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setTurn(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TurnRound = value; return;
	}
	__declspec(dllexport) void incTurn(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TurnRound++; return;
	}
	__declspec(dllexport) int getTurn(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->TurnRound;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDeposit(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Deposit = value; return;
	}
	__declspec(dllexport) void addDeposit(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Deposit += value; return;
	}
	__declspec(dllexport) int getDeposit(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Deposit;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setAgariChain(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->AgariChain = value; return;
	}
	__declspec(dllexport) void incAgariChain(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->AgariChain++; return;
	}
	__declspec(dllexport) int getAgariChain(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->AgariChain;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setLastAgariPlayer(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->LastAgariPlayer = value; return;
	}
	__declspec(dllexport) int getLastAgariPlayer(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->LastAgariPlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHand(GameTable* gameStat, int Page, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Player[Player].Hand[Index].tile = (tileCode)value;
			break;
		case 1:
			gameStat->Player[Player].Hand[Index].red = (uint8_t)value;
			break;
		default:
			throw(std::domain_error("setHand(): �y�[�W���Ⴂ�܂�"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getHand(GameTable* gameStat, int Page, int Index, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return (int)gameStat->Player[Player].Hand[Index].tile;
		case 1:
			return (int)gameStat->Player[Player].Hand[Index].red;
		default:
			throw(std::domain_error("setHand(): �y�[�W���Ⴂ�܂�"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDiscard(GameTable* gameStat, int Page, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
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
			throw(std::domain_error("setDiscard(): �y�[�W���Ⴂ�܂�"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getDiscard(GameTable* gameStat, int Page, int Index, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return gameStat->Player[Player].Discard[Index].dstat * SUTEHAI_TYPE_STEP
				+ gameStat->Player[Player].Discard[Index].tcode.tile;
		case 1:
			return (int)gameStat->Player[Player].Discard[Index].tcode.red;
		case 2:
			return gameStat->Player[Player].Discard[Index].isDiscardThrough ? 1 : 0;
		default:
			throw(std::domain_error("getDiscard(): �y�[�W���Ⴂ�܂�"));
			break;
		}
	}
	__declspec(dllexport) int DiscardPointer(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].DiscardPointer;
	}
	__declspec(dllexport) void DiscardPointerIncrement(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].DiscardPointer++;
		return;
	}
	__declspec(dllexport) void flagDiscard(GameTable* gameStat, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].Discard[Index].dstat = (discardStat)
			(gameStat->Player[Player].Discard[Index].dstat + value / SUTEHAI_TYPE_STEP);
		return;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setMeld(GameTable* gameStat, int Page, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Player[Player].Meld[Index].tcode.tile = (tileCode)(value % MELD_TYPE_STEP);
			gameStat->Player[Player].Meld[Index].mstat = (meldStat)(value / MELD_TYPE_STEP);
			break;
		case 1:
			gameStat->Player[Player].Meld[Index].tcode.red = (uint8_t)value;
			break;
		default:
			throw(std::domain_error("setMeld(): �y�[�W���Ⴂ�܂�"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getMeld(GameTable* gameStat, int Page, int Index, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return gameStat->Player[Player].Meld[Index].mstat * MELD_TYPE_STEP
				+ gameStat->Player[Player].Meld[Index].tcode.tile;
		case 1:
			return (int)gameStat->Player[Player].Meld[Index].tcode.red;
		default:
			throw(std::domain_error("getMeld(): �y�[�W���Ⴂ�܂�"));
		}
	}
	__declspec(dllexport) int MeldPointer(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].MeldPointer;
	}
	__declspec(dllexport) void MeldPointerIncrement(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].MeldPointer++;
		return;
	}
	__declspec(dllexport) void MeldPointerDecrement(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].MeldPointer--;
		return;
	}
	__declspec(dllexport) void flagMeld(GameTable* gameStat, int Page, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Player[Player].Meld[Index].mstat = (meldStat)(
				gameStat->Player[Player].Meld[Index].mstat + value / MELD_TYPE_STEP);
			break;
		case 1:
			gameStat->Player[Player].Meld[Index].tcode.red += (uint8_t)value;
			break;
		default:
			throw(std::domain_error("flagMeld(): �y�[�W���Ⴂ�܂�"));
			break;
		}
		return;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setMenzen(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].MenzenFlagAb = value;
		if (value == 0) gameStat->Player[Player].MenzenFlag = false;
		else if (value == 1) gameStat->Player[Player].MenzenFlag = true;
		return;
	}
	__declspec(dllexport) int getMenzen(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].MenzenFlagAb;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHandStat(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].HandStat = value;
		return;
	}
	__declspec(dllexport) int getHandStat(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].HandStat;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setNumberOfQuads(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].NumberOfQuads = value;
		return;
	}
	__declspec(dllexport) void incNumberOfQuads(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].NumberOfQuads++;
		return;
	}
	__declspec(dllexport) int getNumberOfQuads(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].NumberOfQuads;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRichiFlag(GameTable* gameStat, int Page, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			switch (value) {
			case 0:
				gameStat->Player[Player].RichiFlag.reset(RichiFlag);
				gameStat->Player[Player].RichiFlag.reset(IppatsuFlag);
				gameStat->Player[Player].RichiFlag.reset(DoubleFlag);
				break;
			case 1:
				gameStat->Player[Player].RichiFlag.set(RichiFlag);
				gameStat->Player[Player].RichiFlag.reset(IppatsuFlag);
				gameStat->Player[Player].RichiFlag.reset(DoubleFlag);
				break;
			case 2:
				gameStat->Player[Player].RichiFlag.set(RichiFlag);
				gameStat->Player[Player].RichiFlag.set(IppatsuFlag);
				gameStat->Player[Player].RichiFlag.reset(DoubleFlag);
				break;
			case 3:
				gameStat->Player[Player].RichiFlag.set(RichiFlag);
				gameStat->Player[Player].RichiFlag.reset(IppatsuFlag);
				gameStat->Player[Player].RichiFlag.set(DoubleFlag);
				break;
			case 4:
				gameStat->Player[Player].RichiFlag.set(RichiFlag);
				gameStat->Player[Player].RichiFlag.set(IppatsuFlag);
				gameStat->Player[Player].RichiFlag.set(DoubleFlag);
				break;
			default:
				throw(std::domain_error("setRichiFlag(): �������Ȃ��l���w�肳��܂���"));
			}
			break;
		case 1:
			gameStat->Player[Player].RichiFlag.set(OpenFlag, value);
			break;
		default:
			throw(std::domain_error("setRichiFlag(): �������Ȃ��y�[�W���w�肳��܂���"));
		}
	}
	__declspec(dllexport) int getRichiFlag(GameTable* gameStat, int Page, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return gameStat->Player[Player].RichiFlag[RichiFlag] ?
				1 +
				(gameStat->Player[Player].RichiFlag[IppatsuFlag] ? 1 : 0) +
				(gameStat->Player[Player].RichiFlag[DoubleFlag] ? 2 : 0)
				: 0;
		case 1:
			return gameStat->Player[Player].RichiFlag[OpenFlag] ? 1 : 0;
		default:
			throw(std::domain_error("getRichiFlag(): �������Ȃ��y�[�W���w�肳��܂���"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setOpenWait(GameTable* gameStat, int Tile, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->OpenRichiWait[Tile] = value;
		return;
	}
	__declspec(dllexport) int getOpenWait(GameTable* gameStat, int Tile) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->OpenRichiWait[Tile] ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setFirstDrawFlag(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].FirstDrawFlag = value;
		return;
	}
	__declspec(dllexport) int getFirstDrawFlag(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].FirstDrawFlag ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoujunFuriten(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].DoujunFuriten = value;
		return;
	}
	__declspec(dllexport) int getDoujunFuriten(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].DoujunFuriten ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setAgariHouki(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].AgariHouki = value;
		return;
	}
	__declspec(dllexport) int getAgariHouki(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].AgariHouki ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void putFlowerFlag(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].FlowerFlag = value;
		return;
	}
	__declspec(dllexport) void setFlowerFlag(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].FlowerFlag |= value;
		return;
	}
	__declspec(dllexport) int getFlowerFlag(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].FlowerFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setNorthFlag(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].NorthFlag = value;
		return;
	}
	__declspec(dllexport) void addNorthFlag(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].NorthFlag++;
		return;
	}
	__declspec(dllexport) int getNorthFlag(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].NorthFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setKangFlag(GameTable* gameStat, int Page, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->KangFlag.kangFlag = (uint8_t)value; break;
			case 1: gameStat->KangFlag.chainFlag = (uint8_t)value; break;
			case 2: gameStat->KangFlag.topFlag = (uint8_t)value; break;
			case 3: gameStat->KangFlag.chankanFlag = (uint8_t)value; break;
			default: throw(std::domain_error("setKangFlag(): �y�[�W���Ⴂ�܂�")); break;
		}
		return;
	}
	__declspec(dllexport) void incKangFlag(GameTable* gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->KangFlag.kangFlag++; break;
			case 1: gameStat->KangFlag.chainFlag++; break;
			case 2: gameStat->KangFlag.topFlag++; break;
			case 3: gameStat->KangFlag.chankanFlag++; break;
			default: throw(std::domain_error("incKangFlag(): �y�[�W���Ⴂ�܂�")); break;
		}
		return;
	}
	__declspec(dllexport) int getKangFlag(GameTable* gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->KangFlag.kangFlag;
			case 1: return (int)gameStat->KangFlag.chainFlag;
			case 2: return (int)gameStat->KangFlag.topFlag;
			case 3: return (int)gameStat->KangFlag.chankanFlag;
			default: throw(std::domain_error("getKangFlag(): �y�[�W���Ⴂ�܂�"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setTotalKang(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->KangNum = value;
		return;
	}
	__declspec(dllexport) void incTotalKang(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->KangNum++;
		return;
	}
	__declspec(dllexport) int getTotalKang(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->KangNum;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRichiCounterFlag(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->RichiCounter = value;
		return;
	}
	__declspec(dllexport) int getRichiCounterFlag(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->RichiCounter;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void calcWareme(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
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
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->WaremePlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoukasen(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->DoukasenPlayer = value;
		return;
	}
	__declspec(dllexport) int getDoukasen(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->DoukasenPlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getShibari(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return (int)gameStat->ShibariFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPao(GameTable* gameStat, int Page, int Yaku, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->PaoFlag[Yaku].paoPlayer = (PLAYER_ID)value; break;
			case 1: gameStat->PaoFlag[Yaku].agariPlayer = (PLAYER_ID)value; break;
			default: throw(std::domain_error("setPao(): �y�[�W���Ⴂ�܂�")); break;
		}
		return;
	}
	__declspec(dllexport) int getPao(GameTable* gameStat, int Page, int Yaku) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->PaoFlag[Yaku].paoPlayer;
			case 1: return (int)gameStat->PaoFlag[Yaku].agariPlayer;
			default: throw(std::domain_error("getPao(): �y�[�W���Ⴂ�܂�"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDice(GameTable* gameStat, int ID, int Direction, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (ID+Direction*2) {
			case 0: gameStat->Dice1 = value; break;
			case 1: gameStat->Dice2 = value; break;
			case 2: gameStat->Dice1Direction = value; break;
			case 3: gameStat->Dice2Direction = value; break;
		}
		return;
	}
	__declspec(dllexport) int getDice(GameTable* gameStat, int ID, int Direction) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (ID+Direction*2) {
			case 0: return gameStat->Dice1; break;
			case 1: return gameStat->Dice2; break;
			case 2: return gameStat->Dice1Direction ? 1 : 0; break;
			case 3: return gameStat->Dice2Direction ? 1 : 0; break;
			default: throw(std::domain_error("getDice(): �������Ȃ��l���w�肳��܂���")); break;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void addDeadWallLength(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->DeadTiles += value;
		return;
	}
	__declspec(dllexport) int getDeadWallLength(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->DeadTiles;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setWall(GameTable* gameStat, int Page, int Index, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Deck[Index].tile = (tileCode)value;
			break;
		case 1:
			gameStat->Deck[Index].red = (uint8_t)value;
			break;
		default:
			throw(std::domain_error("setWall(): �y�[�W���Ⴂ�܂�"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getWall(GameTable* gameStat, int Page, int Index) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return (int)gameStat->Deck[Index].tile;
			break;
		case 1:
			return (int)gameStat->Deck[Index].red;
			break;
		default:
			throw(std::domain_error("getWall(): �y�[�W���Ⴂ�܂�"));
			break;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getRinshanExtension(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->ExtraRinshan;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDrawPointer(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TilePointer = value; return;
	}
	__declspec(dllexport) void incDrawPointer(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TilePointer++; return;
	}
	__declspec(dllexport) int getDrawPointer(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->TilePointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoraPointer(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->DoraPointer = value; return;
	}
	__declspec(dllexport) int getDoraPointer(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->DoraPointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRinshanPointer(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->RinshanPointer = value; return;
	}
	__declspec(dllexport) void decRinshanPointer(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->RinshanPointer--; return;
	}
	__declspec(dllexport) int getRinshanPointer(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->RinshanPointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHeavenHandFlag(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TianHuFlag = value;
		return;
	}
	__declspec(dllexport) int getHeavenHandFlag(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->TianHuFlag ? 1: 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPreviousMeld(GameTable* gameStat, int Page, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->PreviousMeld.Discard = (tileCode) value; break;
			case 1: gameStat->PreviousMeld.Stepped = (tileCode) value; break;
			default: throw(std::domain_error("setPreviousMeld(): �y�[�W���Ⴂ�܂�")); break;
		}
		return;
	}
	__declspec(dllexport) int getPreviousMeld(GameTable* gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->PreviousMeld.Discard;
			case 1: return (int)gameStat->PreviousMeld.Stepped;
			default: throw(std::domain_error("getPreviousMeld(): �y�[�W���Ⴂ�܂�")); break;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDisconnectFlag(GameTable* gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].ConnectionLost = value;
		return;
	}
	__declspec(dllexport) int getDisconnectFlag(GameTable* gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].ConnectionLost ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoraFlag(GameTable* gameStat, int Page, int Tile, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->DoraFlag.Omote[Tile] = value; break;
			case 1: gameStat->DoraFlag.Ura[Tile] = value; break;
			default: throw(std::domain_error("setDoraFlag(): �y�[�W���Ⴂ�܂�")); break;
		}
		return;
	}
	__declspec(dllexport) void incDoraFlag(GameTable* gameStat, int Page, int Tile) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->DoraFlag.Omote[Tile]++; break;
			case 1: gameStat->DoraFlag.Ura[Tile]++; break;
			default: throw(std::domain_error("incDoraFlag(): �y�[�W���Ⴂ�܂�")); break;
		}
		return;
	}
	__declspec(dllexport) int getDoraFlag(GameTable* gameStat, int Page, int Tile) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->DoraFlag.Omote[Tile];
			case 1: return (int)gameStat->DoraFlag.Ura[Tile];
			default: throw(std::domain_error("getDoraFlag(): �y�[�W���Ⴂ�܂�"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setCurrentDiscard(GameTable* gameStat, int Page, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->CurrentDiscard.tile = (tileCode)value;
			break;
		case 1:
			gameStat->CurrentDiscard.red = (uint8_t)value;
			break;
		default:
			throw(std::domain_error("setCurrentDiscard(): �y�[�W���Ⴂ�܂�"));
			break;
		}
		return;
	}
	__declspec(dllexport) int getCurrentDiscard(GameTable* gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return (int)gameStat->CurrentDiscard.tile;
		case 1:
			return (int)gameStat->CurrentDiscard.red;
		default:
			throw(std::domain_error("getCurrentDiscard(): �y�[�W���Ⴂ�܂�"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setTsumoAgariFlag(GameTable* gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TsumoAgariFlag = gameStat->AgariSpecialStat = value;
		return;
	}
	__declspec(dllexport) int getTsumoAgariFlag(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->AgariSpecialStat;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDeclarationFlag(GameTable* gameStat, int Page, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch(Page) {
			case 0: gameStat->Player[Player].DeclarationFlag.Ron = value; break;
			case 1: gameStat->Player[Player].DeclarationFlag.Pon = value; break;
			case 2: gameStat->Player[Player].DeclarationFlag.Chi = (int8_t)value; break;
			case 3: gameStat->Player[Player].DeclarationFlag.Kan = value; break;
			default: throw(std::domain_error("setDeclarationFlag(): �y�[�W���Ⴂ�܂�")); break;
		}
		return;
	}
	__declspec(dllexport) void resetDeclarationFlag(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		for (int pl = 0; pl < PLAYERS; pl++) {
			gameStat->Player[pl].DeclarationFlag.Ron =
				gameStat->Player[pl].DeclarationFlag.Pon =
				gameStat->Player[pl].DeclarationFlag.Kan = false;
			gameStat->Player[pl].DeclarationFlag.Chi = 0;
		}
		return;
	}
	__declspec(dllexport) int getDeclarationFlag(GameTable* gameStat, int Page, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch(Page) {
			case 0: return gameStat->Player[Player].DeclarationFlag.Ron ? 1 : 0;
			case 1: return gameStat->Player[Player].DeclarationFlag.Pon ? 1 : 0;
			case 2: return (int)gameStat->Player[Player].DeclarationFlag.Chi;
			case 3: return gameStat->Player[Player].DeclarationFlag.Kan ? 1 : 0;
			default: throw(std::domain_error("getDeclarationFlag(): �y�[�W���Ⴂ�܂�"));
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setCurrentPlayer(GameTable* gameStat, int Page, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->CurrentPlayer.Active = (PLAYER_ID)value; break;
			case 1: gameStat->CurrentPlayer.Passive = (PLAYER_ID)value; break;
			case 2: gameStat->CurrentPlayer.Agari = (PLAYER_ID)value; break;
			case 3: gameStat->CurrentPlayer.Furikomi = (PLAYER_ID)value; break;
			default: throw(std::domain_error("setCurrentPlayer(): �y�[�W���Ⴂ�܂�")); break;
		}
		return;
	}
	__declspec(dllexport) void resetCurrentPlayer(GameTable* gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->CurrentPlayer.Active =
			gameStat->CurrentPlayer.Passive =
			gameStat->CurrentPlayer.Agari =
			gameStat->CurrentPlayer.Furikomi =
			(PLAYER_ID)-1;
		return;
	}
	__declspec(dllexport) int getCurrentPlayer(GameTable* gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->CurrentPlayer.Active;
			case 1: return (int)gameStat->CurrentPlayer.Passive;
			case 2: return (int)gameStat->CurrentPlayer.Agari;
			case 3: return (int)gameStat->CurrentPlayer.Furikomi;
			default: throw(std::domain_error("getCurrentPlayer(): �y�[�W���Ⴂ�܂�"));
		}
	}

	/* ---------------------------------------------------------------------
	 *
	 *  ����������
	 *
	 * ---------------------------------------------------------------------
	 */

	__declspec(dllexport) void inittable(GameTable* gameStat) { /* �ǒP�ʂł̏����� */
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->ShibariFlag = //���ʔ���
			((gameStat->Honba >= 5)&&(getRule(RULE_RYANSHIBA) == 1)) ||
			((gameStat->Honba >= 4)&&(getRule(RULE_RYANSHIBA) == 2));

		std::for_each(gameStat->PaoFlag.begin(), gameStat->PaoFlag.end(), [](PAOSTAT& k) {
			k.agariPlayer = k.paoPlayer = -1; // ��t���O�i-1�c�Ȃ��A0�`3�c�Y���v���C���[�j
		});

		std::for_each(gameStat->Deck.begin(), gameStat->Deck.end(), [](TILE& k) {
			k.tile = NoTile; k.red = (uint8_t)0; // �����Ə��������Ă����܂��傤��
		});

		if (chkGameType(gameStat, AllSanma)) {
			gameStat->DeadTiles = 14; // ���v�̐�
			gameStat->ExtraRinshan = (getRule(RULE_FLOWER_TILES) != 0) ? 4 : 0;
		} else {
			switch (getRule(RULE_FLOWER_TILES)) {
			case 0:
				gameStat->DeadTiles = 14; // ���v�̐�
				break;
			case 3:
				gameStat->DeadTiles = 22; // ���v�̐�(�Ԕv�����鎞�͓��ʂɂQ�Q���c���Ƃ���)
				break;
			default:
				gameStat->DeadTiles = 18; // ���v�̐�
				break;
			}
		}

		gameStat->OpenRichiWait.fill(false); // �v�����[�̑҂��v(�b�n�l�ɈӐ}�I�ȕ��e���N�������Ȃ����߂Ɏg�p)
		gameStat->KangFlag.kangFlag = gameStat->KangFlag.chainFlag = // ���J�ԁG�A�J�ԂƞȐU��G���Șa�G���Ȃ̔���Ɏg��
			gameStat->KangFlag.topFlag = gameStat->KangFlag.chankanFlag = 0;
		gameStat->TurnRound =  // ���݂̏���
			gameStat->KangNum = 0; // �l�ȗ��ǁA�l�Ȏq�Ȃǂ̔���Ɏg��
		gameStat->RichiCounter = false; // ���[�`���J�E���^�[(�錾�v������)
		gameStat->WaremePlayer = // ����ڂ̈ʒu(-1�Ŋ���ڂȂ�)
			gameStat->DoukasenPlayer = -1; // ���ΐ��̈ʒu(-1�œ��ΐ��Ȃ�)
		gameStat->DoraPointer = 999;
		gameStat->Dice1 = gameStat->Dice2 = 0;
		gameStat->Dice1Direction = gameStat->Dice2Direction = false;
		gameStat->TilePointer = 0;

		if (chkGameType(gameStat, AllSanma)) {
			gameStat->RinshanPointer = 107;
		} else {
			switch (getRule(RULE_FLOWER_TILES)) {
			case 0: noflower:
				gameStat->RinshanPointer = 135;
				break;
			case 1: case 2:
				gameStat->RinshanPointer = 139;
				break;
			case 3:
				gameStat->RinshanPointer = 143;
				break;
			default:
				error("RULE_FLOWER_TILES�ُ�B�Ԕv�������[���Ƃ݂Ȃ��ď������܂��B");
				goto noflower; // �ݒ�ُ펞�̃t�H�[���o�b�N�B������goto���g���B
			}
		}

		gameStat->TianHuFlag = true; // �e�̑��Ŕv���܂��i�V�a�̔���ȂǂɎg���j
		gameStat->PreviousMeld.Discard = // ��قǖ����v�i�򂢑ւ��̔���Ɏg���j
			gameStat->PreviousMeld.Stepped = (tileCode)-999;
		gameStat->DoraFlag.Omote.fill(0); // �h������̔z��
		gameStat->DoraFlag.Ura.fill(0); // �h������̔z��
		gameStat->TsumoAgariFlag = false;
		resetDeclarationFlag(gameStat);
		gameStat->CurrentDiscard.tile = NoTile;
		gameStat->CurrentDiscard.red = 0;
		resetCurrentPlayer(gameStat);

		std::for_each(gameStat->Player.begin(), gameStat->Player.end(), [](PlayerTable& pl) {
			pl.ConnectionLost = false; // ����ؒf�ɂ��a�������
			std::for_each(pl.Hand.begin(), pl.Hand.end(), [](TILE& k) {
				k.tile = NoTile; k.red = 0; // ��v�̔z��(�S�l��)
			});
			pl.DiscardPointer = 0; // �����ƃ��Z�b�g���Ă����܂��傤��
			std::for_each(pl.Discard.begin(), pl.Discard.end(), [](discardTile& k) {
				// �̔v�̔z��(�S�l��)
				k.tcode.tile = NoTile; k.tcode.red = 0;
				k.dstat = discardNormal; k.isDiscardThrough = false;
			});
			pl.MenzenFlag = true; // ��O�t���O
			pl.HandStat = 0; // ��v�̏�ԁi���Ă�E������E������j
			pl.MeldPointer = 0; // �ŏ��ςȐ����������Ă��肷���Łc�c
			std::for_each(pl.Meld.begin(), pl.Meld.end(), [](meldCode& k) {
				// ���ʎq���i�[
				k.tcode.tile = NoTile; k.tcode.red = 0;
				k.mstat = (meldStat)0;
			});
			pl.NumberOfQuads = 0; // �Ȏq�̐��i�l�ȗ��ǁA�O�Ȏq�A�l�Ȏq�Ȃǂ̔���Ɏg���j
			pl.RichiFlag.reset(); // ���[�`���Ă��邩�ǂ���
			pl.FirstDrawFlag = true; // �P���ڂł���i�n�a�A�_�u�������̔���Ɏg���j
			pl.DoujunFuriten = // �����U���ł���
				pl.AgariHouki = false; // �a��������̔��������ǂ���
			pl.FlowerFlag = // �N���Ă���Ԕv���i�[����t���O
				pl.NorthFlag = 0; // �N���Ă���k���v���i�[����t���O
		});
		assert(gameStat->Player[0].DiscardPointer == 0); // �������ł��Ă邩�`�F�b�N�i�f�o�b�O�p�j
	}

	void doInitializeGameTable(GameTable* gameStat, int gameType) { // �����P�ʂ̏���������
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		/* ���������p�ŃG�N�X�|�[�g���Ȃ� */
		*gameStat = GameTable();
		gameStat->gameType = (gameTypeID)gameType;

		for (int i = 0; i < PLAYERS; i++) {
			if (i < ACTUAL_PLAYERS) {
				if (chkGameType(&GameStat, SanmaT)) {
					switch (getRule(RULE_STARTING_POINT)) {
					case 0: snmdflt:
						gameStat->Player[i].PlayerScore = LargeNum(35000, 1000000u);
						break;
					case 1:
						gameStat->Player[i].PlayerScore = LargeNum(40000, 1000000u);
						break;
					case 2:
						gameStat->Player[i].PlayerScore = LargeNum(45000, 1000000u);
						break;
					case 3:
						gameStat->Player[i].PlayerScore = LargeNum(50000, 1000000u);
						break;
					case 4: case 7:
						gameStat->Player[i].PlayerScore = LargeNum(25000, 1000000u);
						break;
					case 5:
						gameStat->Player[i].PlayerScore = LargeNum(27000, 1000000u);
						break;
					case 6: case 8:
						gameStat->Player[i].PlayerScore = LargeNum(30000, 1000000u);
						break;
					default:
						error("RULE_STARTING_POINT�ُ�B�����_��25000�Ƃ��ď������܂��B");
						goto snmdflt; // �t�H�[���o�b�N�p�B������goto���g���܂�
					}
				} else {
					switch (getRule(RULE_STARTING_POINT)) {
					case 0: dflt:
						gameStat->Player[i].PlayerScore = LargeNum(25000, 1000000u);
						break;
					case 1:
						gameStat->Player[i].PlayerScore = LargeNum(27000, 1000000u);
						break;
					case 2:
						gameStat->Player[i].PlayerScore = LargeNum(30000, 1000000u);
						break;
					case 3:
						gameStat->Player[i].PlayerScore = LargeNum(35000, 1000000u);
						break;
					case 4:
						gameStat->Player[i].PlayerScore = LargeNum(40000, 1000000u);
						break;
					case 5:
						gameStat->Player[i].PlayerScore = LargeNum(20000, 1000000u);
						break;
					default:
						error("RULE_STARTING_POINT�ُ�B�����_��25000�Ƃ��ď������܂��B");
						goto dflt; // �t�H�[���o�b�N�p�B������goto���g���܂�
					}
				}
			} else {
				gameStat->Player[i].PlayerScore = LargeNum(0);
			}
		}

		switch (getRule(RULE_GAME_LENGTH)) {
		case 0: hanchan:
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 6 : 7;
			break;
		case 1: case 7:
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 2 : 3;
			break;
		case 2: case 3:
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 14 : 15;
			break;
		case 4:
			gameStat->GameLength = 0;
			break;
		case 5:
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 18 : 19;
			break;
		case 6:
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 10 : 11;
			break;
		default:
			error("RULE_GAME_LENGTH�ُ�l�B������Ƃ݂Ȃ��܂��B");
			goto hanchan; // �����͊�����goto���g��
		}
		gameStat->GameRound = gameStat->Honba = gameStat->PlayerID =
			gameStat->Deposit = gameStat->LoopRound = gameStat->AgariChain = 0;
		gameStat->LastAgariPlayer = -1;
		for (int i = 0; i < PLAYERS; i++) {
			gameStat->Player[i].SumaroFlag = false;
			gameStat->Player[i].YakitoriFlag = (getRule(RULE_YAKITORI) != 0);
			gameStat->Player[i].playerChip = 0;
		}

		inittable(gameStat); // �ǂ��Ƃ̏��������s��
	}

	__declspec(dllexport) GameTable* initializeGameTable(int gameType) { // �����P�ʂ̏���������
		doInitializeGameTable(&GameStat, gameType);
		return &GameStat;
	}

	__declspec(dllexport) GameTable* initializeSandbox(int gameType) { /* ��̏�Ԃ̃T���h�{�b�N�X����� */
		doInitializeGameTable(&StatSandBox, gameType);
		return &StatSandBox;
	}

	// ---------------------------------------------------------------------

}
