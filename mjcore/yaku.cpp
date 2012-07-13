#include "yaku.h"

namespace yaku {
	/* �\���̂̃T�C�Y��Ԃ� */
	int YAKUSTAT::getSize() {
		return sizeof(YAKUSTAT);
	}

	/* �����̃C���X�^���X������������ */
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

	/* HSP����̃A�N�Z�T */
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
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�y�[�W���Ⴂ�܂�");
		}
	}
	void YAKUSTAT::setYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value) {
		switch (index) {
			case 17: strcpy_s(myInstance->yakuNameList, 1024, value); break;
			case 18: strcpy_s(myInstance->yakuValList, 1024, value); break;
			case 19: strcpy_s(myInstance->yakumanNameList, 1024, value); break;
			case 20: strcpy_s(myInstance->yakumanValList, 1024, value); break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�y�[�W���Ⴂ�܂�");
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
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�y�[�W���Ⴂ�܂�");
		}
	}
	void YAKUSTAT::addYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value) {
		switch (index) {
			case 17: strcat_s(myInstance->yakuNameList, 1024, value); break;
			case 18: strcat_s(myInstance->yakuValList, 1024, value); break;
			case 19: strcat_s(myInstance->yakumanNameList, 1024, value); break;
			case 20: strcat_s(myInstance->yakumanValList, 1024, value); break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�y�[�W���Ⴂ�܂�");
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
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�y�[�W���Ⴂ�܂�");
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
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�y�[�W���Ⴂ�܂�");
				memset(targetStr, 0, bufSize);
		}
	}

	// ---------------------------------------------------------------------

	namespace mentsuParser { // �ʎq�p�[�T

		/* ���q�̏��� */
		bool makementsu_shuntsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile)
		{ /* ���q�̏��� */
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

		/* ���q�̏��� */
		void makementsu_koutsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile)
		{ /* ���q�̏��� */
			if (countForMentsu[tile] >= 3) {
				MianziDat[*ProcessedMelds].mstat = meldTripletConcealed;
				MianziDat[(*ProcessedMelds)++].tile = tile;
				countForMentsu[tile] -= 3;
			}
			return;
		}

		/* �ʎq�ɕ������� */
		void makementsu(const GameTable* const gameStat,
			PLAYER_ID targetPlayer, ParseMode AtamaCode,
			int* const ProcessedMelds, MELD_BUF MianziDat)
		{ /* �ʎq�ɕ������� */
			memset(MianziDat, 0, sizeof(MELD_BUF)); // ������
			auto countForMentsu = countTilesInHand(gameStat, targetPlayer);
			*ProcessedMelds = 0;

			// �����ƂȂ蓾�Ȃ��v�Ȃ�߂�
			if (countForMentsu[AtamaCode.AtamaCode] < 2) return;
			MianziDat[0].tile = AtamaCode.AtamaCode; (*ProcessedMelds)++;
			countForMentsu[AtamaCode.AtamaCode] -= 2;

			// ���q(���q�D�搳�����[�h�̎�)
			if (AtamaCode.Order == Shun_Ke)
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i))
						--i;
			// ���q(���q�D��t�����[�h�̎�)
			if (AtamaCode.Order == Shun_Ke_Rev)
				for (int i = TILE_SUIT_HONORS - 1; i > 0; i--)
					if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i))
						++i;

			// �Í�(�������[�h�̎�)
			if ((AtamaCode.Order == Ke_Shun)||(AtamaCode.Order == Shun_Ke))
				for (int i = 1; i < TILE_NONFLOWER_MAX; i++)
					makementsu_koutsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i);
			// �Í�(�t�����[�h�̎�)
			if ((AtamaCode.Order == Ke_Shun_Rev)||(AtamaCode.Order == Shun_Ke_Rev))
				for (int i = TILE_NONFLOWER_MAX - 1; i > 0; i--)
					makementsu_koutsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i);

			// ���q(�Í��D�搳�����[�h�̎�)
			if (AtamaCode.Order == Ke_Shun)
				for (int i = 1; i < TILE_SUIT_HONORS; i++)
					if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i))
						--i;
			// ���q(�Í��D��t�����[�h�̎�)
			if (AtamaCode.Order == Ke_Shun_Rev)
				for (int i = TILE_SUIT_HONORS - 1; i > 0; i--)
					if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (tileCode)i))
						++i;

			// �����ʎq�A�Þ�
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
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++)
				MianziDat[i] = (int)mnzDat[i].tile + (int)mnzDat[i].mstat * MELD_TYPE_STEP;
		}

		/* �t�ϊ� */
		void inline ReadAgainMentsu(MELD_BUF target, const int* const source) { /* �t�ϊ� */
			for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
				target[i].tile = (tileCode)(source[i] % MELD_TYPE_STEP);
				target[i].mstat = (meldStat)(source[i] / MELD_TYPE_STEP);
			}
		}
	}

	// ---------------------------------------------------------------------

	namespace countingFacility {
		/* �w��̑Ύq�����邩������ */
		int countPairs(
			const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles)
		{
			// �w�肵����ނ̑Ύq�𐔂���
			trace("�Ύq�̎�ނ𒲂ׂ܂��B");
			int yakuflagcount = 0;
			for (int i = 0; i < numOfTiles; i++)
				if (tileCount[targetTiles[i]] >= 2) yakuflagcount++;
			return yakuflagcount;
		}
		__declspec(dllexport) int countPairs(
			const int* const tileCount, const int* const targetTiles, int numOfTiles)
		{
			Int8ByTile tlCount; tileCode* tlCode = new tileCode[numOfTiles];
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) tlCount[i] = tileCount[i];
			for (int i = 0; i < numOfTiles; i++) tlCode[i] = (tileCode)(targetTiles[i]);
			int ans = countPairs(tlCount, tlCode, numOfTiles);
			delete[] tlCode;
			return ans;
		}

		/* �����̍��v�𐔂���(���Ύq��) */
		int countTileNumerals(const Int8ByTile tileCount) {
			/* �����̍��v�𐔂��� */
			int Cifr = 0;
			for (int i = 1; i < (TILE_SUIT_HONORS - 1); i++)
				Cifr += tileCount[i] * (i % TILE_SUIT_STEP);
			return Cifr;
		}
		__declspec(dllexport) int countTileNumerals(const int* const tileCount) {
			Int8ByTile tlCount;
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) tlCount[i] = tileCount[i];
			int ans = countTileNumerals(tlCount);
			return ans;
		}
	
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		/* �v���p�֐� */
		Int8ByTile countByMelds(
			const MELD_BUF MianziDat, int* const hits,
			std::function<bool (meldStat)> f)
		{ // �v�����[�`��
			Int8ByTile hitCount; memset(&hitCount, 0, sizeof(hitCount));
			if (hits != NULL) *hits = 0;
			for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++) {
				if (f(MianziDat[i].mstat)) {
					++(hitCount[MianziDat[i].tile]);
					if (hits != NULL) ++(*hits);
				}
			}
			return hitCount;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		/* ���q�̐��𐔂��� */
		Int8ByTile countKez(const MELD_BUF MianziDat, int* const Kezi) { /* ���q�̐��𐔂��� */
			trace("���q�E�Ȏq�̎�ނ𒲂ׂ܂��B");
			return countByMelds(MianziDat, Kezi, [](meldStat x){return x >= meldTripletConcealed;});
		}
		__declspec(dllexport) int countKez(int* const KezCount, const int* const MianziDat) {
			MELD_BUF mnzDat; mentsuParser::ReadAgainMentsu(mnzDat, MianziDat);
			int Kezi; auto kzCount = countKez(mnzDat, &Kezi);
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) KezCount[i] = kzCount[i];
			return Kezi;
		}

		/* �Í��q�̐��𐔂��� */
		Int8ByTile countAnKez(const MELD_BUF MianziDat, int* const Kezi) { /* �Í��q�̐��𐔂��� */
			trace("�Í��q�E�ÞȎq�̎�ނ𒲂ׂ܂��B");
			return countByMelds(MianziDat, Kezi, [](meldStat x){return ((x == meldTripletConcealed)||(x == meldQuadConcealed));});
		}
		__declspec(dllexport) int countAnKez(int* const KezCount, const int* const MianziDat) {
			MELD_BUF mnzDat; mentsuParser::ReadAgainMentsu(mnzDat, MianziDat);
			int Kezi; auto kzCount = countAnKez(mnzDat, &Kezi);
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) KezCount[i] = kzCount[i];
			return Kezi;
		}

		/* �Ύq�E���q�E�Ȏq�̐��𐔂��� */
		Int8ByTile countDuiz(const MELD_BUF MianziDat) { /* �Ύq�E���q�E�Ȏq�̐��𐔂��� */
			trace("�Ύq�E���q�E�Ȏq�̎�ނ𒲂ׂ܂��B");
			auto DuiCount = countKez(MianziDat, NULL);
			++(DuiCount[MianziDat[0].tile]);
			return DuiCount;
		}
		__declspec(dllexport) void countDuiz(int* const DuiCount, const int* const MianziDat) {
			MELD_BUF mnzDat; mentsuParser::ReadAgainMentsu(mnzDat, MianziDat);
			auto dCount = countDuiz(mnzDat);
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) DuiCount[i] = dCount[i];
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		/* ���q�̐��𐔂��� */
		Int8ByTile countShunz(const MELD_BUF MianziDat) { /* ���q�̐��𐔂��� */
			trace("���q�̎�ނ𒲂ׂ܂��B");
			return countByMelds(MianziDat, NULL, [](meldStat x){return x < meldTripletConcealed;});
		}
		__declspec(dllexport) void countShunz(int* const ShunzCount, const int* const MianziDat) {
			MELD_BUF mnzDat; mentsuParser::ReadAgainMentsu(mnzDat, MianziDat);
			auto szCount = countShunz(mnzDat);
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) ShunzCount[i] = szCount[i];
		}

		/* �Ï��q�̐��𐔂��� */
		Int8ByTile countAnShunz(const MELD_BUF MianziDat) { /* �Ï��q�̐��𐔂��� */
			trace("�Ï��q�̎�ނ𒲂ׂ܂��B");
			return countByMelds(MianziDat, NULL, [](meldStat x){return x == meldSequenceConcealed;});
		}
		__declspec(dllexport) void countAnShunz(int* const ShunzCount, const int* const MianziDat) {
			MELD_BUF mnzDat; mentsuParser::ReadAgainMentsu(mnzDat, MianziDat);
			auto szCount = countAnShunz(mnzDat);
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) ShunzCount[i] = szCount[i];
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		/* �Ȏq�̐��𐔂��� */
		Int8ByTile countKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* �Ȏq�̐��𐔂��� */
			trace("�Ȏq�̎�ނ𒲂ׂ܂��B");
			return countByMelds(MianziDat, Kangzi, [](meldStat x){return x >= meldQuadConcealed;});
		}
		__declspec(dllexport) int countKangz(int* const KangzCount, const int* const MianziDat) {
			MELD_BUF mnzDat; mentsuParser::ReadAgainMentsu(mnzDat, MianziDat);
			int Kangzi; auto kzCount = countKangz(mnzDat, &Kangzi);
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) KangzCount[i] = kzCount[i];
			return Kangzi;
		}

		/* �ÞȎq�̐��𐔂��� */
		Int8ByTile countAnKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* �ÞȎq�̐��𐔂��� */
			trace("�ÞȎq�̎�ނ𒲂ׂ܂��B");
			return countByMelds(MianziDat, Kangzi, [](meldStat x){return x == meldQuadConcealed;});
		}
		__declspec(dllexport) int countAnKangz(int* const KangzCount, const int* const MianziDat) {
			MELD_BUF mnzDat; mentsuParser::ReadAgainMentsu(mnzDat, MianziDat);
			int Kangzi; auto kzCount = countAnKangz(mnzDat, &Kangzi);
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) KangzCount[i] = kzCount[i];
			return Kangzi;
		}

		/* ���Ȃ̐��𐔂��� */
		Int8ByTile countKaKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* ���Ȃ̐��𐔂��� */
			trace("���Ȃ̎�ނ𒲂ׂ܂��B");
			return countByMelds(MianziDat, Kangzi, [](meldStat x){return x >= meldQuadAddedLeft;});
		}
		__declspec(dllexport) int countKaKangz(int* const KangzCount, const int* const MianziDat) {
			MELD_BUF mnzDat; mentsuParser::ReadAgainMentsu(mnzDat, MianziDat);
			int Kangzi; auto kzCount = countKaKangz(mnzDat, &Kangzi);
			for (int i = 0; i < TILE_NONFLOWER_MAX; i++) KangzCount[i] = kzCount[i];
			return Kangzi;
		}

	}
}
