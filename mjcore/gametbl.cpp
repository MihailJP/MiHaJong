#include "gametbl.h"

GameTable GameStat, StatSandBox;

inline bool chkGameType(const GameTable* const gameStat, gameTypeID gameType) {
	return ((gameStat->gameType) & gameType);
};

extern "C" {

	/* ---------------------------------------------------------------------
	 *
	 *  HSPからのアクセサ
	 *
	 * ---------------------------------------------------------------------
	 */

	__declspec(dllexport) void setScore(GameTable* const gameStat, int Player, int Digit, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].PlayerScore.digitGroup[Digit] = value *
			(Digit ? 1 : (signed int)(100000000u / gameStat->Player[Player].PlayerScore.firstArg)); return;
	}
	__declspec(dllexport) void addScore(GameTable* const gameStat, int Player, int Digit, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].PlayerScore.digitGroup[Digit] += value *
			(Digit ? 1 : (signed int)(100000000u / gameStat->Player[Player].PlayerScore.firstArg)); return;
	}
	__declspec(dllexport) int getScore(const GameTable* const gameStat, int Player, int Digit) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].PlayerScore.digitGroup[Digit] /
			(Digit ? 1 : (signed int)(100000000u / gameStat->Player[Player].PlayerScore.firstArg));
	}
	__declspec(dllexport) void exportScore(const GameTable* const gameStat, int* const exportArray) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		for (int j = 0; j < DIGIT_GROUPS; j++) {
			for (int i = 0; i < PLAYERS; i++) {
				*(exportArray + j * PLAYERS + i) =
					gameStat->Player[i].PlayerScore.digitGroup[j] /
					(j ? 1 : (signed int)(100000000u / gameStat->Player[i].PlayerScore.firstArg));
			}
		}
	}
	__declspec(dllexport) void importScore(GameTable* const gameStat, const int* const importArray) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		for (int j = 0; j < DIGIT_GROUPS; j++) {
			for (int i = 0; i < PLAYERS; i++) {
				gameStat->Player[i].PlayerScore.digitGroup[j] =
					*(importArray + j * PLAYERS + i) *
					(j ? 1 : (signed int)(100000000u / gameStat->Player[i].PlayerScore.firstArg));
			}
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setChip(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].playerChip = value; return;
	}
	__declspec(dllexport) void addChip(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].playerChip += value; return;
	}
	__declspec(dllexport) int getChip(const GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].playerChip;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setSumaroFlag(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].SumaroFlag = value; return;
	}
	__declspec(dllexport) int getSumaroFlag(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].SumaroFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setYakitori(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].YakitoriFlag = value; return;
	}
	__declspec(dllexport) int getYakitori(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].YakitoriFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPlayer(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->PlayerID = value; return;
	}
	__declspec(dllexport) int getPlayer(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->PlayerID;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getGameLength(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->GameLength;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRound(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->GameRound = value; return;
	}
	__declspec(dllexport) void incRound(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->GameRound++; return;
	}
	__declspec(dllexport) int getRound(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->GameRound;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRoundLoop(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->LoopRound = value; return;
	}
	__declspec(dllexport) void incRoundLoop(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->LoopRound++; return;
	}
	__declspec(dllexport) int getRoundLoop(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->LoopRound;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHonba(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Honba = value; return;
	}
	__declspec(dllexport) void incHonba(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Honba++; return;
	}
	__declspec(dllexport) int getHonba(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Honba;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setTurn(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TurnRound = value; return;
	}
	__declspec(dllexport) void incTurn(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TurnRound++; return;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDeposit(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Deposit = value; return;
	}
	__declspec(dllexport) void addDeposit(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Deposit += value; return;
	}
	__declspec(dllexport) int getDeposit(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Deposit;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setAgariChain(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->AgariChain = value; return;
	}
	__declspec(dllexport) void incAgariChain(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->AgariChain++; return;
	}
	__declspec(dllexport) int getAgariChain(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->AgariChain;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setLastAgariPlayer(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->LastAgariPlayer = (PLAYER_ID)value; return;
	}
	__declspec(dllexport) int getLastAgariPlayer(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return (int)gameStat->LastAgariPlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHand(GameTable* const gameStat, int Page, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Player[Player].Hand[Index].tile = (tileCode)value;
			break;
		case 1:
			gameStat->Player[Player].Hand[Index].red = (doraCol)value;
			break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			break;
		}
		return;
	}
	__declspec(dllexport) int getHand(const GameTable* const gameStat, int Page, int Index, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return (int)gameStat->Player[Player].Hand[Index].tile;
		case 1:
			return (int)gameStat->Player[Player].Hand[Index].red;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDiscard(GameTable* const gameStat, int Page, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Player[Player].Discard[Index].dstat = (discardStat)(value / SUTEHAI_TYPE_STEP);
			gameStat->Player[Player].Discard[Index].tcode.tile = (tileCode)(value % SUTEHAI_TYPE_STEP);
			break;
		case 1:
			gameStat->Player[Player].Discard[Index].tcode.red = (doraCol)value;
			break;
		case 2:
			gameStat->Player[Player].Discard[Index].isDiscardThrough = (bool)value;
			break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			break;
		}
		return;
	}
	__declspec(dllexport) int getDiscard(const GameTable* const gameStat, int Page, int Index, int Player) {
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			return 0;
		}
	}
	__declspec(dllexport) int DiscardPointer(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].DiscardPointer;
	}
	__declspec(dllexport) void DiscardPointerIncrement(GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].DiscardPointer++;
		return;
	}
	__declspec(dllexport) void flagDiscard(GameTable* const gameStat, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].Discard[Index].dstat = (discardStat)
			(gameStat->Player[Player].Discard[Index].dstat + value / SUTEHAI_TYPE_STEP);
		return;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setMeld(GameTable* const gameStat, int Page, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Player[Player].Meld[Index].tile = (tileCode)(value % MELD_TYPE_STEP);
			gameStat->Player[Player].Meld[Index].mstat = (meldStat)(value / MELD_TYPE_STEP);
			break;
		case 1:
			for (int i = 0; i < 8; i++) {
				if (value & (1 << i))
					gameStat->Player[Player].Meld[Index].red[i % 4] = (doraCol)(i / 4 + 1);
				else gameStat->Player[Player].Meld[Index].red[i % 4] = Normal;
			}
			break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			break;
		}
		return;
	}
	__declspec(dllexport) int getMeld(const GameTable* const gameStat, int Page, int Index, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		int tmp = 0;
		switch (Page) {
		case 0:
			return gameStat->Player[Player].Meld[Index].mstat * MELD_TYPE_STEP
				+ gameStat->Player[Player].Meld[Index].tile;
		case 1:
			for (int i = 0; i < 8; i++)
				if (gameStat->Player[Player].Meld[Index].red[i % 4] == (doraCol)(i / 4 + 1))
					tmp |= (1 << i);
			return tmp;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			return 0;
		}
	}
	__declspec(dllexport) int MeldPointer(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].MeldPointer;
	}
	__declspec(dllexport) void MeldPointerIncrement(GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].MeldPointer++;
		return;
	}
	__declspec(dllexport) void flagMeld(GameTable* const gameStat, int Page, int Index, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Player[Player].Meld[Index].mstat = (meldStat)(
				gameStat->Player[Player].Meld[Index].mstat + value / MELD_TYPE_STEP);
			break;
		case 1:
			for (int i = 0; i < 8; i++)
				if (value & (1 << i))
					gameStat->Player[Player].Meld[Index].red[i % 4] = (doraCol)(i / 4 + 1);
			break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			break;
		}
		return;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setMenzen(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].MenzenFlag = value;
		return;
	}
	__declspec(dllexport) int getMenzen(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].MenzenFlag ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHandStat(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].HandStat = value;
		return;
	}
	__declspec(dllexport) int getHandStat(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].HandStat;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void incNumberOfQuads(GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].NumberOfQuads++;
		return;
	}
	__declspec(dllexport) int getNumberOfQuads(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].NumberOfQuads;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRichiFlag(GameTable* const gameStat, int Page, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
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
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "正しくない値が指定されました");
			}
			break;
		case 1:
			gameStat->Player[Player].RichiFlag.OpenFlag = value;
			break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "正しくないページが指定されました");
		}
	}
	__declspec(dllexport) int getRichiFlag(const GameTable* const gameStat, int Page, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "正しくないページが指定されました");
			return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setFirstDrawFlag(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].FirstDrawFlag = value;
		return;
	}
	__declspec(dllexport) int getFirstDrawFlag(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].FirstDrawFlag ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoujunFuriten(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].DoujunFuriten = value;
		return;
	}
	__declspec(dllexport) int getDoujunFuriten(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].DoujunFuriten ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setAgariHouki(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].AgariHouki = value;
		return;
	}
	__declspec(dllexport) int getAgariHouki(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].AgariHouki ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setFlowerFlag(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].FlowerFlag.Spring |= (bool)(value & 0x01);
		gameStat->Player[Player].FlowerFlag.Summer |= (bool)(value & 0x02);
		gameStat->Player[Player].FlowerFlag.Autumn |= (bool)(value & 0x04);
		gameStat->Player[Player].FlowerFlag.Winter |= (bool)(value & 0x08);
		gameStat->Player[Player].FlowerFlag.Plum   |= (bool)(value & 0x10);
		gameStat->Player[Player].FlowerFlag.Orchid |= (bool)(value & 0x20);
		gameStat->Player[Player].FlowerFlag.Chrys  |= (bool)(value & 0x40);
		gameStat->Player[Player].FlowerFlag.Bamboo |= (bool)(value & 0x80);
		return;
	}
	__declspec(dllexport) int getFlowerFlag(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		int ans = 0;
		ans |= gameStat->Player[Player].FlowerFlag.Spring ? 0x01 : 0;
		ans |= gameStat->Player[Player].FlowerFlag.Summer ? 0x02 : 0;
		ans |= gameStat->Player[Player].FlowerFlag.Autumn ? 0x04 : 0;
		ans |= gameStat->Player[Player].FlowerFlag.Winter ? 0x08 : 0;
		ans |= gameStat->Player[Player].FlowerFlag.Plum   ? 0x10 : 0;
		ans |= gameStat->Player[Player].FlowerFlag.Orchid ? 0x20 : 0;
		ans |= gameStat->Player[Player].FlowerFlag.Chrys  ? 0x40 : 0;
		ans |= gameStat->Player[Player].FlowerFlag.Bamboo ? 0x80 : 0;
		return ans;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void addNorthFlag(GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].NorthFlag++;
		return;
	}
	__declspec(dllexport) int getNorthFlag(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].NorthFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setKangFlag(GameTable* const gameStat, int Page, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->KangFlag.kangFlag = (uint8_t)value; break;
			case 1: gameStat->KangFlag.chainFlag = (uint8_t)value; break;
			case 2: gameStat->KangFlag.topFlag = (uint8_t)value; break;
			case 3: gameStat->KangFlag.chankanFlag = (uint8_t)value; break;
			default: RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います"); break;
		}
		return;
	}
	__declspec(dllexport) void incKangFlag(GameTable* const gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->KangFlag.kangFlag++; break;
			case 1: gameStat->KangFlag.chainFlag++; break;
			case 2: gameStat->KangFlag.topFlag++; break;
			case 3: gameStat->KangFlag.chankanFlag++; break;
			default: RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います"); break;
		}
		return;
	}
	__declspec(dllexport) int getKangFlag(const GameTable* const gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->KangFlag.kangFlag;
			case 1: return (int)gameStat->KangFlag.chainFlag;
			case 2: return (int)gameStat->KangFlag.topFlag;
			case 3: return (int)gameStat->KangFlag.chankanFlag;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
				return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void incTotalKang(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->KangNum++;
		return;
	}
	__declspec(dllexport) int getTotalKang(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->KangNum;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setRichiCounterFlag(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->RichiCounter = (value > 0);
		gameStat->DoubleRichiCounter = (value == 2);
		return;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void calcWareme(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		if (chkGameType(gameStat, AllSanma)) {
			if (RuleData::chkRuleApplied("wareme") || RuleData::chkRuleApplied("kaimenkaze")) {
				gameStat->WaremePlayer = ((gameStat->GameRound-(gameStat->GameRound/4))+24
					+diceSum(gameStat)-1) % 3;
				if (chkGameType(gameStat, Sanma4)) {
					gameStat->WaremePlayer =
						tobePlayed(gameStat, (24+diceSum(gameStat)-1) % 3);
				}
			}
		} else {
			if (RuleData::chkRuleApplied("wareme") || RuleData::chkRuleApplied("kaimenkaze")) {
				gameStat->WaremePlayer = ((gameStat->GameRound % 4)+32+diceSum(gameStat)-1) % 4;
			}
		}
		return;
	}
	__declspec(dllexport) int getWareme(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->WaremePlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getDoukasen(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->DoukasenPlayer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getShibari(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return (int)gameStat->ShibariFlag;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPao(GameTable* const gameStat, int Page, int Yaku, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->PaoFlag[Yaku].paoPlayer = (PLAYER_ID)value; break;
			case 1: gameStat->PaoFlag[Yaku].agariPlayer = (PLAYER_ID)value; break;
			default: RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います"); break;
		}
		return;
	}
	__declspec(dllexport) int getPao(const GameTable* const gameStat, int Page, int Yaku) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->PaoFlag[Yaku].paoPlayer;
			case 1: return (int)gameStat->PaoFlag[Yaku].agariPlayer;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
				return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDice(GameTable* const gameStat, int ID, int Direction, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (ID+Direction*2) {
			case 0: gameStat->Dice[0].Number = value; break;
			case 1: gameStat->Dice[1].Number = value; break;
			case 2: gameStat->Dice[0].Direction = value; break;
			case 3: gameStat->Dice[1].Direction = value; break;
		}
		return;
	}
	__declspec(dllexport) int getDice(const GameTable* const gameStat, int ID, int Direction) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (ID+Direction*2) {
			case 0: return gameStat->Dice[0].Number; break;
			case 1: return gameStat->Dice[1].Number; break;
			case 2: return gameStat->Dice[0].Direction ? 1 : 0; break;
			case 3: return gameStat->Dice[1].Direction ? 1 : 0; break;
			default:
				Raise(EXCEPTION_MJCORE_INVALID_ARGUMENT, "正しくない値が指定されました");
				return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void addDeadWallLength(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->DeadTiles += value;
		return;
	}
	__declspec(dllexport) int getDeadWallLength(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->DeadTiles;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setWall(GameTable* const gameStat, int Page, int Index, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->Deck[Index].tile = (tileCode)value;
			break;
		case 1:
			gameStat->Deck[Index].red = (doraCol)value;
			break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			break;
		}
		return;
	}
	__declspec(dllexport) int getWall(const GameTable* const gameStat, int Page, int Index) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return (int)gameStat->Deck[Index].tile;
		case 1:
			return (int)gameStat->Deck[Index].red;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) int getRinshanExtension(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->ExtraRinshan;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void incDrawPointer(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TilePointer++; return;
	}
	__declspec(dllexport) int getDrawPointer(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->TilePointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDoraPointer(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->DoraPointer = value; return;
	}
	__declspec(dllexport) int getDoraPointer(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->DoraPointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void decRinshanPointer(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->RinshanPointer--; return;
	}
	__declspec(dllexport) int getRinshanPointer(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->RinshanPointer;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setHeavenHandFlag(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TianHuFlag = value;
		return;
	}
	__declspec(dllexport) int getHeavenHandFlag(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->TianHuFlag ? 1: 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setPreviousMeld(GameTable* const gameStat, int Page, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->PreviousMeld.Discard = (tileCode) value; break;
			case 1: gameStat->PreviousMeld.Stepped = (tileCode) value; break;
			default: RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います"); break;
		}
		return;
	}
	__declspec(dllexport) int getPreviousMeld(const GameTable* const gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->PreviousMeld.Discard;
			case 1: return (int)gameStat->PreviousMeld.Stepped;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
				return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDisconnectFlag(GameTable* const gameStat, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->Player[Player].ConnectionLost = value;
		return;
	}
	__declspec(dllexport) int getDisconnectFlag(const GameTable* const gameStat, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->Player[Player].ConnectionLost ? 1 : 0;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void incDoraFlag(GameTable* const gameStat, int Page, int Tile) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->DoraFlag.Omote[Tile]++; break;
			case 1: gameStat->DoraFlag.Ura[Tile]++; break;
			default: RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います"); break;
		}
		return;
	}
	__declspec(dllexport) int getDoraFlag(const GameTable* const gameStat, int Page, int Tile) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->DoraFlag.Omote[Tile];
			case 1: return (int)gameStat->DoraFlag.Ura[Tile];
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
				return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setCurrentDiscard(GameTable* const gameStat, int Page, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			gameStat->CurrentDiscard.tile = (tileCode)value;
			break;
		case 1:
			gameStat->CurrentDiscard.red = (doraCol)value;
			break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			break;
		}
		return;
	}
	__declspec(dllexport) int getCurrentDiscard(const GameTable* const gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
		case 0:
			return (int)gameStat->CurrentDiscard.tile;
		case 1:
			return (int)gameStat->CurrentDiscard.red;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setTsumoAgariFlag(GameTable* const gameStat, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->TsumoAgariFlag = gameStat->AgariSpecialStat = value;
		return;
	}
	__declspec(dllexport) int getTsumoAgariFlag(const GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		return gameStat->AgariSpecialStat;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setDeclarationFlag(GameTable* const gameStat, int Page, int Player, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch(Page) {
			case 0: gameStat->Player[Player].DeclarationFlag.Ron = value; break;
			case 1: gameStat->Player[Player].DeclarationFlag.Pon = value; break;
			case 2: gameStat->Player[Player].DeclarationFlag.Chi = (int8_t)value; break;
			case 3: gameStat->Player[Player].DeclarationFlag.Kan = value; break;
			default: RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います"); break;
		}
		return;
	}
	__declspec(dllexport) void resetDeclarationFlag(GameTable* const gameStat) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		for (int pl = 0; pl < PLAYERS; pl++) {
			gameStat->Player[pl].DeclarationFlag.Ron =
				gameStat->Player[pl].DeclarationFlag.Pon =
				gameStat->Player[pl].DeclarationFlag.Kan = false;
			gameStat->Player[pl].DeclarationFlag.Chi = 0;
		}
		return;
	}
	__declspec(dllexport) int getDeclarationFlag(const GameTable* const gameStat, int Page, int Player) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch(Page) {
			case 0: return gameStat->Player[Player].DeclarationFlag.Ron ? 1 : 0;
			case 1: return gameStat->Player[Player].DeclarationFlag.Pon ? 1 : 0;
			case 2: return (int)gameStat->Player[Player].DeclarationFlag.Chi;
			case 3: return gameStat->Player[Player].DeclarationFlag.Kan ? 1 : 0;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
				return 0;
		}
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void setCurrentPlayer(GameTable* const gameStat, int Page, int value) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: gameStat->CurrentPlayer.Active = (PLAYER_ID)value; break;
			case 1: gameStat->CurrentPlayer.Passive = (PLAYER_ID)value; break;
			case 2: gameStat->CurrentPlayer.Agari = (PLAYER_ID)value; break;
			case 3: gameStat->CurrentPlayer.Furikomi = (PLAYER_ID)value; break;
			default: RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います"); break;
		}
		return;
	}
	__declspec(dllexport) int getCurrentPlayer(const GameTable* const gameStat, int Page) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		switch (Page) {
			case 0: return (int)gameStat->CurrentPlayer.Active;
			case 1: return (int)gameStat->CurrentPlayer.Passive;
			case 2: return (int)gameStat->CurrentPlayer.Agari;
			case 3: return (int)gameStat->CurrentPlayer.Furikomi;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
				return 0;
		}
	}

	/* ---------------------------------------------------------------------
	 *
	 *  初期化処理
	 *
	 * ---------------------------------------------------------------------
	 */

	__declspec(dllexport) void inittable(GameTable* const gameStat) { /* 局単位での初期化 */
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		gameStat->ShibariFlag = //二飜縛り
			((gameStat->Honba >= 5)&&(RuleData::chkRule("ryanshiba", "from_5honba"))) ||
			((gameStat->Honba >= 4)&&(RuleData::chkRule("ryanshiba", "from_4honba")));

		for (int i = 0; i < PAO_YAKU_PAGES; i++) // 包フラグ（-1…なし、0～3…該当プレイヤー）
			gameStat->PaoFlag[i].agariPlayer = gameStat->PaoFlag[i].paoPlayer = -1;

		for (int i = 0; i < SIZE_OF_DECKBUF; i++) { // ちゃんと初期化してあげましょうね
			gameStat->Deck[i].tile = NoTile;
			gameStat->Deck[i].red = Normal;
		}

		if (chkGameType(gameStat, AllSanma)) {
			gameStat->DeadTiles = 14; // 王牌の数
			gameStat->ExtraRinshan = RuleData::chkRuleApplied("flower_tiles") ? 4 : 0;
		} else {
			if (RuleData::chkRule("flower_tiles", "no")) gameStat->DeadTiles = 14; // 王牌の数
			else if (RuleData::chkRule("flower_tiles", "8tiles")) gameStat->DeadTiles = 22; // 王牌の数(花牌を入れる時は特別に２２枚残しとする)
			else gameStat->DeadTiles = 18; // 王牌の数
		}

		for (int i = 0; i < TILE_NONFLOWER_MAX; i++) // プンリーの待ち牌(ＣＯＭに意図的な放銃を起こさせないために使用)
			gameStat->OpenRichiWait[i] = false;
		gameStat->KangFlag.kangFlag = gameStat->KangFlag.chainFlag = // 嶺上開花；連開花と槓振り；頭槓和；搶槓の判定に使う
			gameStat->KangFlag.topFlag = gameStat->KangFlag.chankanFlag = 0;
		gameStat->TurnRound =  // 現在の巡目
			gameStat->KangNum = 0; // 四槓流局、四槓子などの判定に使う
		gameStat->RichiCounter =
			gameStat->DoubleRichiCounter = false; // リーチをカウンター(宣言牌をロン)
		gameStat->WaremePlayer = // 割れ目の位置(-1で割れ目なし)
			gameStat->DoukasenPlayer = -1; // 導火線の位置(-1で導火線なし)
		gameStat->DoraPointer = 999;
		gameStat->Dice[0].Number = gameStat->Dice[1].Number = 0;
		gameStat->Dice[0].Direction = gameStat->Dice[1].Direction = false;
		gameStat->TilePointer = 0;

		if (chkGameType(gameStat, AllSanma)) {
			gameStat->RinshanPointer = 107;
		} else {
			if (RuleData::chkRule("flower_tiles", "no")) gameStat->RinshanPointer = 135;
			else if ((RuleData::chkRule("flower_tiles", "seasons")) || (RuleData::chkRule("flower_tiles", "flowers")))
				gameStat->RinshanPointer = 139;
			else if (RuleData::chkRule("flower_tiles", "8tiles")) gameStat->RinshanPointer = 143;
			else {
				error("flower_tiles異常。花牌無しルールとみなして処理します。");
				gameStat->RinshanPointer = 135; // 設定異常時のフォールバック
			}
		}

		gameStat->TianHuFlag = true; // 親の第一打牌がまだ（天和の判定などに使う）
		gameStat->PreviousMeld.Discard = // 先ほど鳴いた牌（喰い替えの判定に使う）
			gameStat->PreviousMeld.Stepped = NoTile;
		for (int i = 0; i < TILE_NONFLOWER_MAX; i++) // ドラ判定の配列
			gameStat->DoraFlag.Omote[i] = gameStat->DoraFlag.Ura[i] = 0;
		gameStat->TsumoAgariFlag = false;
		gameStat->AgariSpecialStat = 0;
		resetDeclarationFlag(gameStat);
		gameStat->CurrentDiscard.tile = NoTile;
		gameStat->CurrentDiscard.red = Normal;
		gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive =
			gameStat->CurrentPlayer.Agari = gameStat->CurrentPlayer.Furikomi = (PLAYER_ID)-1;

		for (int pl = 0; pl < PLAYERS; pl++) {
			gameStat->Player[pl].ConnectionLost = false; // 回線切断による和了り放棄
			for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) { // 手牌の配列(４人分)
				gameStat->Player[pl].Hand[i].tile = NoTile;
				gameStat->Player[pl].Hand[i].red = Normal;
			}
			gameStat->Player[pl].DiscardPointer = 0; // ちゃんとリセットしてあげましょうね
			for (int i = 0; i < SIZE_OF_DISCARD_BUFFER; i++) {
				// 捨牌の配列(４人分)
				gameStat->Player[pl].Discard[i].tcode.tile = NoTile;
				gameStat->Player[pl].Discard[i].tcode.red = Normal;
				gameStat->Player[pl].Discard[i].dstat = discardNormal;
				gameStat->Player[pl].Discard[i].isDiscardThrough = false;
			}
			gameStat->Player[pl].MenzenFlag = true; // 門前フラグ
			gameStat->Player[pl].HandStat = 0; // 手牌の状態（立てる・見せる・伏せる）
			gameStat->Player[pl].MeldPointer = 0; // 最初変な数字が入ってたりするんで……
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
				// 鳴き面子を格納
				gameStat->Player[pl].Meld[i].tile = NoTile;
				for (int j = 0; j < 4; j++) gameStat->Player[pl].Meld[i].red[j] = Normal;
				gameStat->Player[pl].Meld[i].mstat = (meldStat)0;
			}
			gameStat->Player[pl].NumberOfQuads = 0; // 槓子の数（四槓流局、三槓子、四槓子などの判定に使う）
			gameStat->Player[pl].RichiFlag.RichiFlag = // リーチしているかどうか
				gameStat->Player[pl].RichiFlag.IppatsuFlag =
				gameStat->Player[pl].RichiFlag.DoubleFlag =
				gameStat->Player[pl].RichiFlag.OpenFlag = false;
			gameStat->Player[pl].FirstDrawFlag = true; // １巡目である（地和、ダブル立直の判定に使う）
			gameStat->Player[pl].DoujunFuriten = // 同順振聴である
				gameStat->Player[pl].AgariHouki = false; // 和了り放棄の罰則中かどうか
			gameStat->Player[pl].FlowerFlag.Spring = // 晒している花牌を格納するフラグ
				gameStat->Player[pl].FlowerFlag.Summer =
				gameStat->Player[pl].FlowerFlag.Autumn =
				gameStat->Player[pl].FlowerFlag.Winter =
				gameStat->Player[pl].FlowerFlag.Plum =
				gameStat->Player[pl].FlowerFlag.Orchid =
				gameStat->Player[pl].FlowerFlag.Chrys =
				gameStat->Player[pl].FlowerFlag.Bamboo = false;
			gameStat->Player[pl].NorthFlag = 0; // 晒している北風牌を格納するフラグ
		}
		assert(gameStat->Player[0].DiscardPointer == 0); // 初期化できてるかチェック（デバッグ用）
	}

	void doInitializeGameTable(GameTable* const gameStat, int gameType) { // 半荘単位の初期化処理
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		/* 内部処理用でエクスポートしない */
		memset(gameStat, 0, sizeof(GameTable));
		gameStat->gameType = (gameTypeID)gameType;

		for (int i = 0; i < PLAYERS; i++) {
			if (i < ACTUAL_PLAYERS) {
				gameStat->Player[i].PlayerScore =
					LargeNum::fromInt(std::atoi(RuleData::chkRule("starting_point").c_str()), 1000000u);
			} else {
				gameStat->Player[i].PlayerScore = LargeNum::fromInt(0, 1000000u);
			}
		}

		if (RuleData::chkRule("game_length", "east_south_game"))
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 6 : 7;
		else if (RuleData::chkRule("game_length", "east_wind_game") ||
			RuleData::chkRule("game_length", "east_only_game"))
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 2 : 3;
		else if (RuleData::chkRule("game_length", "full_round_game") ||
			RuleData::chkRule("game_length", "east_north_game"))
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 14 : 15;
		else if (RuleData::chkRule("game_length", "single_round_game"))
			gameStat->GameLength = 0;
		else if (RuleData::chkRule("game_length", "twice_east_game"))
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 18 : 19;
		else if (RuleData::chkRule("game_length", "east_south_west_game") ||
			RuleData::chkRule("game_length", "east_west_game"))
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 10 : 11;
		else {
			error("game_length異常値。半荘戦とみなします。");
			gameStat->GameLength = chkGameType(&GameStat, SanmaT) ? 6 : 7;
		}
		gameStat->GameRound = gameStat->Honba = gameStat->PlayerID =
			gameStat->Deposit = gameStat->LoopRound = gameStat->AgariChain = 0;
		gameStat->LastAgariPlayer = -1;
		for (int i = 0; i < PLAYERS; i++) {
			gameStat->Player[i].SumaroFlag = false;
			gameStat->Player[i].YakitoriFlag = RuleData::chkRuleApplied("yakitori");
			gameStat->Player[i].playerChip = 0;
		}

		inittable(gameStat); // 局ごとの初期化も行う
	}

	__declspec(dllexport) const GameTable* const initializeGameTable(int gameType) { // 半荘単位の初期化処理
		doInitializeGameTable(&GameStat, gameType);
		return &GameStat;
	}

	// ---------------------------------------------------------------------

	__declspec(dllexport) void makesandBox(int* const Sandbox, const GameTable* const gameStat, int targetPlayer) {
		/* 卓の状態のサンドボックスを作る */
		GameTable* const sandbox = &StatSandBox;
		doInitializeGameTable(sandbox, gameStat->gameType);
		for (int p = 0; p < PLAYERS; p++) {
			sandbox->Player[p].PlayerScore = LargeNum(gameStat->Player[p].PlayerScore);
			sandbox->Player[p].playerChip = gameStat->Player[p].playerChip;
			sandbox->Player[p].SumaroFlag = gameStat->Player[p].SumaroFlag;
			sandbox->Player[p].YakitoriFlag = gameStat->Player[p].YakitoriFlag;
			if ((gameStat->Player[p].RichiFlag.OpenFlag)||(p == targetPlayer)) {
				for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
					sandbox->Player[p].Hand[i].tile = gameStat->Player[p].Hand[i].tile;
					sandbox->Player[p].Hand[i].red = gameStat->Player[p].Hand[i].red;
				}
			}
			sandbox->Player[p].RichiFlag = gameStat->Player[p].RichiFlag;
			for (int i = 0; i < SIZE_OF_DISCARD_BUFFER; i++) {
				sandbox->Player[p].Discard[i].isDiscardThrough = gameStat->Player[p].Discard[i].isDiscardThrough;
				sandbox->Player[p].Discard[i].dstat = gameStat->Player[p].Discard[i].dstat;
				sandbox->Player[p].Discard[i].tcode.tile = gameStat->Player[p].Discard[i].tcode.tile;
				sandbox->Player[p].Discard[i].tcode.red = gameStat->Player[p].Discard[i].tcode.red;
			}
			sandbox->Player[p].DiscardPointer = gameStat->Player[p].DiscardPointer;
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
				sandbox->Player[p].Meld[i].mstat = gameStat->Player[p].Meld[i].mstat;
				sandbox->Player[p].Meld[i].tile = gameStat->Player[p].Meld[i].tile;
				for (int j = 0; j < 4; j++)
					sandbox->Player[p].Meld[i].red[j] = gameStat->Player[p].Meld[i].red[j];
			}
			sandbox->Player[p].MeldPointer = gameStat->Player[p].MeldPointer;
			sandbox->Player[p].MenzenFlag = gameStat->Player[p].MenzenFlag;
			sandbox->Player[p].NumberOfQuads = gameStat->Player[p].NumberOfQuads;
			sandbox->Player[p].RichiFlag.RichiFlag = gameStat->Player[p].RichiFlag.RichiFlag;
			sandbox->Player[p].RichiFlag.IppatsuFlag = gameStat->Player[p].RichiFlag.IppatsuFlag;
			sandbox->Player[p].RichiFlag.DoubleFlag = gameStat->Player[p].RichiFlag.DoubleFlag;
			sandbox->Player[p].RichiFlag.OpenFlag = gameStat->Player[p].RichiFlag.OpenFlag;
			sandbox->Player[p].FirstDrawFlag = gameStat->Player[p].FirstDrawFlag;
			sandbox->Player[p].DoujunFuriten = gameStat->Player[p].DoujunFuriten;
			sandbox->Player[p].AgariHouki = gameStat->Player[p].AgariHouki;
			sandbox->Player[p].FlowerFlag = gameStat->Player[p].FlowerFlag;
			sandbox->Player[p].NorthFlag = gameStat->Player[p].NorthFlag;
			sandbox->Player[p].ConnectionLost = gameStat->Player[p].ConnectionLost;
		}
		sandbox->PlayerID = gameStat->PlayerID;
		sandbox->GameRound = gameStat->GameRound;
		sandbox->LoopRound = gameStat->LoopRound;
		sandbox->Honba = gameStat->Honba;
		sandbox->TurnRound = gameStat->TurnRound;
		sandbox->Deposit = gameStat->Deposit;
		sandbox->AgariChain = gameStat->AgariChain;
		sandbox->LastAgariPlayer = gameStat->LastAgariPlayer;
		for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
			sandbox->OpenRichiWait[i] = gameStat->OpenRichiWait[i];
		sandbox->KangFlag.kangFlag = gameStat->KangFlag.kangFlag;
		sandbox->KangFlag.chainFlag = gameStat->KangFlag.chainFlag;
		sandbox->KangFlag.topFlag = gameStat->KangFlag.topFlag;
		sandbox->KangFlag.chankanFlag = gameStat->KangFlag.chankanFlag;
		sandbox->KangNum = gameStat->KangNum;
		sandbox->RichiCounter = gameStat->RichiCounter;
		sandbox->DoubleRichiCounter = gameStat->DoubleRichiCounter;
		sandbox->DoukasenPlayer = gameStat->DoukasenPlayer;
		for (int i = 0; i < PAO_YAKU_PAGES; i++) {
			sandbox->PaoFlag[i].paoPlayer = gameStat->PaoFlag[i].paoPlayer;
			sandbox->PaoFlag[i].agariPlayer = gameStat->PaoFlag[i].agariPlayer;
		}
		sandbox->Dice[0].Number = gameStat->Dice[0].Number;
		sandbox->Dice[1].Number = gameStat->Dice[1].Number;
		sandbox->Dice[0].Direction = gameStat->Dice[0].Direction;
		sandbox->Dice[1].Direction = gameStat->Dice[1].Direction;
		for (int i = 0; i < 6; i++) {
			if (chkGameType(gameStat, AllSanma)) {
				if (gameStat->DoraPointer <= (102 - gameStat->ExtraRinshan - i * 2))
					sandbox->Deck[102 - gameStat->ExtraRinshan - i * 2].tile =
					gameStat->Deck[102 - gameStat->ExtraRinshan - i * 2].tile;
			} else {
				if (gameStat->DoraPointer <= (130 - i * 2))
					sandbox->Deck[130 - i * 2].tile = gameStat->Deck[130 - i * 2].tile;
			}
		}
		sandbox->TilePointer = gameStat->TilePointer;
		sandbox->DoraPointer = gameStat->DoraPointer;
		sandbox->RinshanPointer = gameStat->RinshanPointer;
		sandbox->TianHuFlag = gameStat->TianHuFlag;
		sandbox->PreviousMeld.Discard = gameStat->PreviousMeld.Discard;
		sandbox->PreviousMeld.Stepped = gameStat->PreviousMeld.Stepped;
		sandbox->Deposit = gameStat->Deposit;
		sandbox->Deposit = gameStat->Deposit;
		sandbox->Deposit = gameStat->Deposit;
		sandbox->Deposit = gameStat->Deposit;
		for (int i = 0; i < TILE_NONFLOWER_MAX; i++) {
			sandbox->DoraFlag.Omote[i] = gameStat->DoraFlag.Omote[i];
			sandbox->DoraFlag.Ura[i] = gameStat->DoraFlag.Ura[i];
		}
		sandbox->CurrentDiscard.tile = gameStat->CurrentDiscard.tile;
		sandbox->CurrentDiscard.red = gameStat->CurrentDiscard.red;
		sandbox->CurrentPlayer.Active = gameStat->CurrentPlayer.Active;
		sandbox->CurrentPlayer.Passive = gameStat->CurrentPlayer.Passive;
		sandbox->CurrentPlayer.Agari = gameStat->CurrentPlayer.Agari;
		sandbox->CurrentPlayer.Furikomi = gameStat->CurrentPlayer.Furikomi;

		*Sandbox = (int)sandbox;
	}

	// ---------------------------------------------------------------------

}

__declspec(dllexport) void setGameType(int gameType) {
	GameStat.gameType = (gameTypeID)gameType;
}
