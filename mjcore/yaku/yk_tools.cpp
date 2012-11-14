#include "yaku.h"

/* ���q�̏��� */
bool yaku::mentsuParser::makementsu_shuntsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
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
void yaku::mentsuParser::makementsu_koutsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
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
void yaku::mentsuParser::makementsu(const GameTable* const gameStat,
	PLAYER_ID targetPlayer, ParseMode AtamaCode, int* const ProcessedMelds, MELD_BUF MianziDat)
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

// -------------------------------------------------------------------------

/* �w��̑Ύq�����邩������ */
int yaku::countingFacility::countPairs(
	const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles)
{
	// �w�肵����ނ̑Ύq�𐔂���
	trace(_T("�Ύq�̎�ނ𒲂ׂ܂��B"));
	int yakuflagcount = 0;
	for (int i = 0; i < numOfTiles; i++)
		if (tileCount[targetTiles[i]] >= 2) yakuflagcount++;
	return yakuflagcount;
}

/* �����̍��v�𐔂���(���Ύq��) */
int yaku::countingFacility::countTileNumerals(const Int8ByTile tileCount) {
	/* �����̍��v�𐔂��� */
	int Cifr = 0;
	for (int i = 1; i < (TILE_SUIT_HONORS - 1); i++)
		Cifr += tileCount[i] * (i % TILE_SUIT_STEP);
	return Cifr;
}
	
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* �v���p�֐� */
Int8ByTile yaku::countingFacility::countByMelds(
	const MELD_BUF MianziDat, uint8_t* const hits,
	std::function<bool (meldStat)> f)
{ // �v�����[�`��
	Int8ByTile hitCount; memset(&hitCount, 0, sizeof(hitCount));
	if (hits != nullptr) *hits = 0;
	for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++) {
		if (f(MianziDat[i].mstat)&&(MianziDat[i].tile != NoTile)) {
			++(hitCount[MianziDat[i].tile]);
			if (hits != nullptr) ++(*hits);
		}
	}
	return hitCount;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* ���q�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countKez(const MELD_BUF MianziDat, uint8_t* const Kezi) { /* ���q�̐��𐔂��� */
	trace(_T("���q�E�Ȏq�̎�ނ𒲂ׂ܂��B"));
	return countByMelds(MianziDat, Kezi, [](meldStat x){return x >= meldTripletConcealed;});
}

/* �Í��q�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countAnKez(const MELD_BUF MianziDat, uint8_t* const Kezi) { /* �Í��q�̐��𐔂��� */
	trace(_T("�Í��q�E�ÞȎq�̎�ނ𒲂ׂ܂��B"));
	return countByMelds(MianziDat, Kezi, [](meldStat x){return ((x == meldTripletConcealed)||(x == meldQuadConcealed));});
}

/* �Ύq�E���q�E�Ȏq�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countDuiz(const MELD_BUF MianziDat) { /* �Ύq�E���q�E�Ȏq�̐��𐔂��� */
	trace(_T("�Ύq�E���q�E�Ȏq�̎�ނ𒲂ׂ܂��B"));
	auto DuiCount = countKez(MianziDat, nullptr);
	++(DuiCount[MianziDat[0].tile]);
	return DuiCount;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* ���q�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countShunz(const MELD_BUF MianziDat, uint8_t* const Shunzi) { /* ���q�̐��𐔂��� */
	trace(_T("���q�̎�ނ𒲂ׂ܂��B"));
	return countByMelds(MianziDat, Shunzi, [](meldStat x){return x < meldTripletConcealed;});
}

/* �Ï��q�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countAnShunz(const MELD_BUF MianziDat, uint8_t* const Shunzi) { /* �Ï��q�̐��𐔂��� */
	trace(_T("�Ï��q�̎�ނ𒲂ׂ܂��B"));
	return countByMelds(MianziDat, Shunzi, [](meldStat x){return x == meldSequenceConcealed;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* �Ȏq�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countKangz(const MELD_BUF MianziDat, uint8_t* const Kangzi) { /* �Ȏq�̐��𐔂��� */
	trace(_T("�Ȏq�̎�ނ𒲂ׂ܂��B"));
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x >= meldQuadConcealed;});
}

/* �ÞȎq�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countAnKangz(const MELD_BUF MianziDat, uint8_t* const Kangzi) { /* �ÞȎq�̐��𐔂��� */
	trace(_T("�ÞȎq�̎�ނ𒲂ׂ܂��B"));
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x == meldQuadConcealed;});
}

/* ���Ȃ̐��𐔂��� */
Int8ByTile yaku::countingFacility::countKaKangz(const MELD_BUF MianziDat, uint8_t* const Kangzi) { /* ���Ȃ̐��𐔂��� */
	trace(_T("���Ȃ̎�ނ𒲂ׂ܂��B"));
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x >= meldQuadAddedLeft;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* �w��̍��q�E���q�����邩������ */
inline int yaku::countingFacility::countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
	const tileCode* const targetShunz, int numOfShunz, bool Mode, bool allowDup)
{ // �w�肵����ނ̖ʎq�𐔂���
	int kz = 0; int sz = 0;
	auto DuiCount = Mode ? countKez(MianziDat, nullptr) : countDuiz(MianziDat);
	auto ShunzCount = countShunz(MianziDat, nullptr);
	int yakuflagcount = 0;
	for (int i = 0; i < numOfKez; i++) {
		if (targetKez[i] == NoTile) continue;
		if (DuiCount[targetKez[i]] >= 1) ++yakuflagcount;
	}
	for (int i = 0; i < numOfShunz; i++) {
		if (targetShunz[i] == NoTile) continue;
		if (ShunzCount[targetShunz[i]] >= 1)
			yakuflagcount += allowDup ? ShunzCount[targetShunz[i]] : 1;
	}
	return yakuflagcount;
}
int yaku::countingFacility::countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
	const tileCode* const targetShunz, int numOfShunz, bool Mode)
{
	return countSpecMentz(MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, Mode, false);
}
int yaku::countingFacility::countSpecMentzWithDup(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
	const tileCode* const targetShunz, int numOfShunz, bool Mode)
{
	return countSpecMentz(MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, Mode, true);
}

/* �����̍��v�𐔂��� */
int yaku::countingFacility::countMentzNumerals(const MELD_BUF MianziDat) { /* �����̍��v�𐔂��� */
	int Cifr = 0;
	for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
		if (MianziDat[i].tile % TILE_CODE_MAXIMUM < TILE_SUIT_HONORS) {
			if (i == 0) { // ����
				Cifr += (MianziDat[0].tile % TILE_SUIT_STEP) * 2;
			} else { // �ʎq
				switch (MianziDat[i].mstat) {
				case meldSequenceConcealed: case meldSequenceExposedLower:
				case meldSequenceExposedMiddle: case meldSequenceExposedUpper:
					Cifr += (((MianziDat[i].tile % TILE_SUIT_STEP)+1) * 3); // ���q
					break;
				case meldTripletConcealed: case meldTripletExposedLeft:
				case meldTripletExposedCenter: case meldTripletExposedRight:
					Cifr += ((MianziDat[i].tile % TILE_SUIT_STEP) * 3); // ���q
					break;
				default:
					Cifr += ((MianziDat[i].tile % TILE_SUIT_STEP) * 4); // �Ȏq
					break;
				}
			}
		}
	}
	return Cifr;
}
