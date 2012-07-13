#include "yaku.h"

namespace yaku {
	/* 構造体のサイズを返す */
	int YAKUSTAT::getSize() {
		return sizeof(YAKUSTAT);
	}

	/* 既存のインスタンスを初期化する */
	void YAKUSTAT::Init(YAKUSTAT* const myInstance) {
		myInstance->BasePoints = 20;
		myInstance->CoreHan = myInstance->BonusHan =
			myInstance->DoraQuantity = myInstance->UraDoraQuantity =
			myInstance->AkaDoraQuantity = myInstance->AoDoraQuantity =
			myInstance->AliceDora = myInstance->FlowerQuantity = 0;
		myInstance->AgariPoints = LargeNum::fromInt(0);
		memset(myInstance->yakuNameList, 0, 1024);
		memset(myInstance->yakuValList, 0, 1024);
		memset(myInstance->yakumanNameList, 0, 1024);
		memset(myInstance->yakumanValList, 0, 1024);
	}

	/* HSPからのアクセサ */
	void YAKUSTAT::exportYakuPoint(const YAKUSTAT* const myInstance, int* const exportArray) {
		for (int i = 0; i < DIGIT_GROUPS; i++)
			*(exportArray + i) =
				myInstance->AgariPoints.digitGroup[i] /
				(i ? 1 : (signed int)(100000000u / myInstance->AgariPoints.firstArg));
	}

	void YAKUSTAT::setYakuInfo(YAKUSTAT* const myInstance, int index, int value) {
		switch (index) {
			case 0: myInstance->BasePoints = value; break;
			case 1: myInstance->CoreHan = value; break;
			case 2: myInstance->BonusHan = value; break;
			case 3: myInstance->DoraQuantity = value; break;
			case 4: myInstance->UraDoraQuantity = value; break;
			case 5: myInstance->AkaDoraQuantity = value; break;
			case 6: myInstance->AoDoraQuantity = value; break;
			case 7: myInstance->AliceDora = value; break;
			case 8: myInstance->FlowerQuantity = value; break;
			case 9: case 10: case 11: case 12:
			case 13: case 14: case 15: case 16:
				myInstance->AgariPoints.digitGroup[index - 9] = value;
				break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
		}
	}
	void YAKUSTAT::setYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value) {
		switch (index) {
			case 17: strcpy_s(myInstance->yakuNameList, 1024, value); break;
			case 18: strcpy_s(myInstance->yakuValList, 1024, value); break;
			case 19: strcpy_s(myInstance->yakumanNameList, 1024, value); break;
			case 20: strcpy_s(myInstance->yakumanValList, 1024, value); break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
		}
	}

	void YAKUSTAT::addYakuInfo(YAKUSTAT* const myInstance, int index, int value) {
		switch (index) {
			case 0: myInstance->BasePoints += value; break;
			case 1: myInstance->CoreHan += value; break;
			case 2: myInstance->BonusHan += value; break;
			case 3: myInstance->DoraQuantity += value; break;
			case 4: myInstance->UraDoraQuantity += value; break;
			case 5: myInstance->AkaDoraQuantity += value; break;
			case 6: myInstance->AoDoraQuantity += value; break;
			case 7: myInstance->AliceDora += value; break;
			case 8: myInstance->FlowerQuantity += value; break;
			case 9: case 10: case 11: case 12:
			case 13: case 14: case 15: case 16:
				myInstance->AgariPoints.digitGroup[index - 9] += value;
				break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
		}
	}
	void YAKUSTAT::addYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value) {
		switch (index) {
			case 17: strcat_s(myInstance->yakuNameList, 1024, value); break;
			case 18: strcat_s(myInstance->yakuValList, 1024, value); break;
			case 19: strcat_s(myInstance->yakumanNameList, 1024, value); break;
			case 20: strcat_s(myInstance->yakumanValList, 1024, value); break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
		}
	}

	int YAKUSTAT::getYakuInfo(const YAKUSTAT* const myInstance, int index) {
		switch (index) {
			case 0: return myInstance->BasePoints;
			case 1: return myInstance->CoreHan;
			case 2: return myInstance->BonusHan;
			case 3: return myInstance->DoraQuantity;
			case 4: return myInstance->UraDoraQuantity;
			case 5: return myInstance->AkaDoraQuantity;
			case 6: return myInstance->AoDoraQuantity;
			case 7: return myInstance->AliceDora;
			case 8: return myInstance->FlowerQuantity;
			case 9: case 10: case 11: case 12:
			case 13: case 14: case 15: case 16:
				return myInstance->AgariPoints.digitGroup[index - 9];
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
				return 0x7fffffff;
		}
	}
	void YAKUSTAT::getYakuInfo(const YAKUSTAT* const myInstance,
		int index, char* const targetStr, int bufSize)
	{
		switch (index) {
			case 17: strcpy_s(targetStr, bufSize, myInstance->yakuNameList); break;
			case 18: strcpy_s(targetStr, bufSize, myInstance->yakuValList); break;
			case 19: strcpy_s(targetStr, bufSize, myInstance->yakumanNameList); break;
			case 20: strcpy_s(targetStr, bufSize, myInstance->yakumanValList); break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
				memset(targetStr, 0, bufSize);
		}
	}

	// ---------------------------------------------------------------------

	namespace mentsuParser { // 面子パーサ

		/* 順子の処理 */
		bool makementsu_shuntsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile)
		{ /* 順子の処理 */
			if ((countForMentsu[tile] >= 1)&&
				(countForMentsu[tile+1] >= 1)&&
				(countForMentsu[tile+2] >= 1)) {
					MianziDat[*ProcessedMelds].mstat = meldSequenceConcealed;
					MianziDat[(*ProcessedMelds)++].tile = tile;
					--countForMentsu[tile]; --countForMentsu[tile+1];
					--countForMentsu[tile+2];
					return true;
			}
			return false;
		}

		/* 刻子の処理 */
		void makementsu_koutsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile)
		{ /* 刻子の処理 */
			if (countForMentsu[tile] >= 3) {
				MianziDat[*ProcessedMelds].mstat = meldTripletConcealed;
				MianziDat[(*ProcessedMelds)++].tile = tile;
				countForMentsu[tile] -= 3;
			}
			return;
		}

		/* 面子に分解する */
		void makementsu(const GameTable* const gameStat,
			PLAYER_ID targetPlayer, ParseMode AtamaCode,
			int* const ProcessedMelds, MELD_BUF MianziDat)
		{ /* 面子に分解する */
			memset(MianziDat, 0, sizeof(MELD_BUF)); // 初期化
			auto countForMentsu = countTilesInHand(gameStat, targetPlayer);
			*ProcessedMelds = 0;

			// 雀頭となり得ない牌なら戻る
			if (countForMentsu[AtamaCode.AtamaCode] < 2) return;
			MianziDat[0].tile = AtamaCode.AtamaCode; (*ProcessedMelds)++;
			countForMentsu[AtamaCode.AtamaCode] -= 2;

			// 順子(順子優先正順モードの時)
			if (AtamaCode.Order == Shun_Ke)
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i))
						--i;
			// 順子(順子優先逆順モードの時)
			if (AtamaCode.Order == Shun_Ke_Rev)
				for (int i = TILE_SUIT_HONORS - 1; i > 0; i--)
					if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i))
						++i;

			// 暗刻(正順モードの時)
			if ((AtamaCode.Order == Ke_Shun)||(AtamaCode.Order == Shun_Ke))
				for (int i = 1; i < TILE_NONFLOWER_MAX; i++)
					makementsu_koutsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i);
			// 暗刻(逆順モードの時)
			if ((AtamaCode.Order == Ke_Shun_Rev)||(AtamaCode.Order == Shun_Ke_Rev))
				for (int i = TILE_NONFLOWER_MAX - 1; i > 0; i--)
					makementsu_koutsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i);

			// 順子(暗刻優先正順モードの時)
			if (AtamaCode.Order == Ke_Shun)
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i))
						--i;
			// 順子(暗刻優先逆順モードの時)
			if (AtamaCode.Order == Ke_Shun_Rev)
				for (int i = TILE_SUIT_HONORS - 1; i > 0; i--)
					if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i))
						++i;

			// 鳴いた面子、暗槓
			for (int i = 1; i <= gameStat->Player[targetPlayer].MeldPointer; i++) {
				MianziDat[*ProcessedMelds].mstat = gameStat->Player[targetPlayer].Meld[i].mstat;
				MianziDat[*ProcessedMelds].tile = gameStat->Player[targetPlayer].Meld[i].tile;
				/*for (int j = 0; j < 4; j++)
					MianziDat[*ProcessedMelds].red[j] = gameStat->Player[targetPlayer].Meld[i].red[j];*/
				++(*ProcessedMelds);
			}
		}
		__declspec(dllexport) void makementsu(const GameTable* const gameStat, void *,
			int targetPlayer, int AtamaCode, int* const ProcessedMelds, int* const MianziDat)
		{
			ParseMode pMode; MELD_BUF mnzDat;
			pMode.AtamaCode = tileCode(AtamaCode/4);
			pMode.Order = ParseOrder(AtamaCode%4);
			makementsu(gameStat, (PLAYER_ID)targetPlayer, pMode, ProcessedMelds, mnzDat);
			for (int i = 0; i < 5; i++)
				MianziDat[i] = (int)mnzDat[i].tile + (int)mnzDat[i].mstat * MELD_TYPE_STEP;
		}
	}
}
