#include "yaku.h"

/* �\���̂̃T�C�Y��Ԃ� */
int yaku::YAKUSTAT::getSize() {
	return sizeof(YAKUSTAT);
}

/* �����̃C���X�^���X������������ */
void yaku::YAKUSTAT::Init(YAKUSTAT* const myInstance) {
	myInstance->isValid = false;
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
void yaku::YAKUSTAT::exportYakuPoint(const YAKUSTAT* const myInstance, int* const exportArray) {
	for (int i = 0; i < DIGIT_GROUPS; i++)
		*(exportArray + i) =
			myInstance->AgariPoints.digitGroup[i] /
			(i ? 1 : (signed int)(100000000u / myInstance->AgariPoints.firstArg));
}

void yaku::YAKUSTAT::setYakuInfo(YAKUSTAT* const myInstance, int index, int value) {
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
void yaku::YAKUSTAT::setYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value) {
	switch (index) {
		case 17: strcpy_s(myInstance->yakuNameList, 1024, value); break;
		case 18: strcpy_s(myInstance->yakuValList, 1024, value); break;
		case 19: strcpy_s(myInstance->yakumanNameList, 1024, value); break;
		case 20: strcpy_s(myInstance->yakumanValList, 1024, value); break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�y�[�W���Ⴂ�܂�");
	}
}

void yaku::YAKUSTAT::addYakuInfo(YAKUSTAT* const myInstance, int index, int value) {
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
void yaku::YAKUSTAT::addYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value) {
	switch (index) {
		case 17: strcat_s(myInstance->yakuNameList, 1024, value); break;
		case 18: strcat_s(myInstance->yakuValList, 1024, value); break;
		case 19: strcat_s(myInstance->yakumanNameList, 1024, value); break;
		case 20: strcat_s(myInstance->yakumanValList, 1024, value); break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "�y�[�W���Ⴂ�܂�");
	}
}

int yaku::YAKUSTAT::getYakuInfo(const YAKUSTAT* const myInstance, int index) {
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
void yaku::YAKUSTAT::getYakuInfo(const YAKUSTAT* const myInstance,
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

// -------------------------------------------------------------------------

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
	trace("�Ύq�̎�ނ𒲂ׂ܂��B");
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
	const MELD_BUF MianziDat, int* const hits,
	std::function<bool (meldStat)> f)
{ // �v�����[�`��
	Int8ByTile hitCount; memset(&hitCount, 0, sizeof(hitCount));
	if (hits != NULL) *hits = 0;
	for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++) {
		if (f(MianziDat[i].mstat)&&(MianziDat[i].tile != NoTile)) {
			++(hitCount[MianziDat[i].tile]);
			if (hits != NULL) ++(*hits);
		}
	}
	return hitCount;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* ���q�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countKez(const MELD_BUF MianziDat, int* const Kezi) { /* ���q�̐��𐔂��� */
	trace("���q�E�Ȏq�̎�ނ𒲂ׂ܂��B");
	return countByMelds(MianziDat, Kezi, [](meldStat x){return x >= meldTripletConcealed;});
}

/* �Í��q�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countAnKez(const MELD_BUF MianziDat, int* const Kezi) { /* �Í��q�̐��𐔂��� */
	trace("�Í��q�E�ÞȎq�̎�ނ𒲂ׂ܂��B");
	return countByMelds(MianziDat, Kezi, [](meldStat x){return ((x == meldTripletConcealed)||(x == meldQuadConcealed));});
}

/* �Ύq�E���q�E�Ȏq�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countDuiz(const MELD_BUF MianziDat) { /* �Ύq�E���q�E�Ȏq�̐��𐔂��� */
	trace("�Ύq�E���q�E�Ȏq�̎�ނ𒲂ׂ܂��B");
	auto DuiCount = countKez(MianziDat, NULL);
	++(DuiCount[MianziDat[0].tile]);
	return DuiCount;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* ���q�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countShunz(const MELD_BUF MianziDat) { /* ���q�̐��𐔂��� */
	trace("���q�̎�ނ𒲂ׂ܂��B");
	return countByMelds(MianziDat, NULL, [](meldStat x){return x < meldTripletConcealed;});
}

/* �Ï��q�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countAnShunz(const MELD_BUF MianziDat) { /* �Ï��q�̐��𐔂��� */
	trace("�Ï��q�̎�ނ𒲂ׂ܂��B");
	return countByMelds(MianziDat, NULL, [](meldStat x){return x == meldSequenceConcealed;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* �Ȏq�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* �Ȏq�̐��𐔂��� */
	trace("�Ȏq�̎�ނ𒲂ׂ܂��B");
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x >= meldQuadConcealed;});
}

/* �ÞȎq�̐��𐔂��� */
Int8ByTile yaku::countingFacility::countAnKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* �ÞȎq�̐��𐔂��� */
	trace("�ÞȎq�̎�ނ𒲂ׂ܂��B");
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x == meldQuadConcealed;});
}

/* ���Ȃ̐��𐔂��� */
Int8ByTile yaku::countingFacility::countKaKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* ���Ȃ̐��𐔂��� */
	trace("���Ȃ̎�ނ𒲂ׂ܂��B");
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x >= meldQuadAddedLeft;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* �w��̍��q�E���q�����邩������ */
int yaku::countingFacility::countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
	const tileCode* const targetShunz, int numOfShunz, bool Mode)
{ // �w�肵����ނ̖ʎq�𐔂���
	int kz = 0; int sz = 0;
	auto DuiCount = Mode ? countKez(MianziDat, NULL) : countDuiz(MianziDat);
	auto ShunzCount = countShunz(MianziDat);
	int yakuflagcount = 0;
	for (int i = 0; i < numOfKez; i++) {
		if (targetKez[i] == NoTile) continue;
		if (DuiCount[targetKez[i]] >= 1) ++yakuflagcount;
	}
	for (int i = 0; i < numOfShunz; i++) {
		if (targetShunz[i] == NoTile) continue;
		if (ShunzCount[targetShunz[i]] >= 1) ++yakuflagcount;
	}
	return yakuflagcount;
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

// -------------------------------------------------------------------------

yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// �ݒ肵�����[���Ɋ�Â��Ė��C���X�^���X������������
__declspec(dllexport) void yaku::yakuCalculator::init() {
	YakuCatalog::Instantiate()->catalog.clear(); // ���Z�b�g
	info("���J�^���O�����Z�b�g���܂����B");
	/*
	 * �����ɃR���X�g���N�^����ׂ�
	 */
	YakuCatalog::Instantiate()->catalog.push_back(Yaku( // �e�X�g�p�̃_�~�[�̖�
		"�_�~�[", Yaku::YAKU_HAN::HAN(1, Han), Yaku::YAKU_HAN::HAN(),
		[](const GameTable* const, const MENTSU_ANALYSIS* const) {
			return true;
		}
	));
	info("���J�^���O�̍\�z���������܂����B");
}

// �v�Z�����s(�}���`�X���b�h�Łc�c���v���Ȃ�)
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculator(LPVOID lpParam) {
	incThreadCount();
	return ((CalculatorParam*)lpParam)->instance->calculate(
		((CalculatorParam*)lpParam)->gameStat,
		&(((CalculatorParam*)lpParam)->analysis),
		&(((CalculatorParam*)lpParam)->pMode));
}

/* �����Ă���X���b�h���̊Ǘ��p */
CRITICAL_SECTION yaku::yakuCalculator::CalculatorThread::cs;
int yaku::yakuCalculator::CalculatorThread::runningThreads = 0;

int yaku::yakuCalculator::CalculatorThread::numOfRunningThreads() { // �����Ă���X���b�h�̐�
	return runningThreads;
}

void yaku::yakuCalculator::CalculatorThread::incThreadCount() {
	EnterCriticalSection(&cs); ++runningThreads; LeaveCriticalSection(&cs); // �X���b�h���C���N�������g
}
void yaku::yakuCalculator::CalculatorThread::decThreadCount() {
	EnterCriticalSection(&cs); --runningThreads; LeaveCriticalSection(&cs); // �X���b�h���f�N�������g
}

/* �v�Z���[�`�� */
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculate
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, const ParseMode* const pMode)
{
	/* �ʎq��͏��� */
	if (analysis->shanten[shantenRegular] == -1) {
		int NumOfMelds = 0;
		mentsuParser::makementsu(gameStat, analysis->player, *pMode, &NumOfMelds, analysis->MianziDat);
		if (NumOfMelds < SIZE_OF_MELD_BUFFER) { // �����𖞂����ĂȂ��Ȃ甲���܂�
			decThreadCount(); return S_OK;
		}
		analysis->DuiziCount = countingFacility::countDuiz(analysis->MianziDat);
		analysis->KeziCount = countingFacility::countKez(analysis->MianziDat, NULL);
		analysis->AnKeziCount = countingFacility::countAnKez(analysis->MianziDat, NULL);
		analysis->ShunziCount = countingFacility::countShunz(analysis->MianziDat);
		analysis->AnShunziCount = countingFacility::countAnShunz(analysis->MianziDat);
		analysis->KangziCount = countingFacility::countKangz(analysis->MianziDat, NULL);
		analysis->AnKangziCount = countingFacility::countAnKangz(analysis->MianziDat, NULL);
		analysis->KaKangziCount = countingFacility::countKaKangz(analysis->MianziDat, NULL);
	}
	/* �𔻒胋�[�v */
	std::map<std::string, Yaku::YAKU_HAN> yakuHan; // �󂯎M������
	std::set<std::string> suppression; // �����������
	std::for_each(YakuCatalog::Instantiate()->catalog.begin(), // ���J�^���O�̍ŏ�����
		YakuCatalog::Instantiate()->catalog.end(), // �J�^���O�̖����܂�
		[&yakuHan, gameStat, analysis, &suppression](Yaku& yaku) -> void { // �����Ƃɔ��菈��
			if (yaku.checkYaku(gameStat, analysis)) { // ���������𖞂����Ă�����
				yakuHan[yaku.getName()] = yaku.getHan(); // �ʐ����L�^
				suppression.insert(yaku.getSuppression().begin(), yaku.getSuppression().end()); // ���ʖ��̃��X�g������
			}
	});
	/* ���ʖ����������� */
	std::for_each(suppression.begin(), suppression.end(), [&yakuHan](std::string yaku) {
		yakuHan.erase(yaku);
	});
	/* �|�����v���� */
	int totalHan, totalMangan, totalBonusHan, totalBonusMangan;
	totalHan = totalMangan = totalBonusHan = totalBonusMangan = 0;
	std::for_each(yakuHan.begin(), yakuHan.end(),
		[&totalHan, &totalMangan, &totalBonusHan, &totalBonusMangan](std::pair<std::string, Yaku::YAKU_HAN> yHan) {
			switch (yHan.second.coreHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalHan += yHan.second.coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Mangan: totalMangan += yHan.second.coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalMangan += yHan.second.coreHan.getHan() * 4; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "�P�ʂ��ُ�ł�");
			}
			switch (yHan.second.bonusHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalBonusHan += yHan.second.bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Mangan: totalBonusMangan += yHan.second.bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalBonusMangan += yHan.second.bonusHan.getHan() * 4; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "�P�ʂ��ُ�ł�");
			}
	});
	/* �I������ */
	decThreadCount(); // �I�������X���b�h���f�N�������g
	return S_OK;
}

/* �R���X�g���N�^�ƃf�X�g���N�^ */
yaku::yakuCalculator::CalculatorThread::CalculatorThread() {
	InitializeCriticalSection(&cs);
}
yaku::yakuCalculator::CalculatorThread::~CalculatorThread() {
	DeleteCriticalSection(&cs);
}
		
/* �����̏����Ƃ� */
void yaku::yakuCalculator::analysisNonLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
	SHANTEN* const shanten, YAKUSTAT* const yakuInfo)
{
	CalculatorThread* calculator = new CalculatorThread; // �C���X�^���X�̏���
	// �ϐ���p��
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(SHANTEN[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
	// �v�Z���[�`���ɓn���p�����[�^�̏���
	CalculatorParam* calcprm = new CalculatorParam; memset(calcprm, 0, sizeof(CalculatorParam));
	calcprm->gameStat = gameStat; calcprm->instance = calculator;
	memcpy(&calcprm->analysis, &analysis, sizeof(MENTSU_ANALYSIS));
	YAKUSTAT::Init(&calcprm->result);
	// �v�Z�����s
	CalculatorThread::calculator(&calcprm);
	while (CalculatorThread::numOfRunningThreads() > 0) Sleep(0); // ����(�ȗ��Ȏ���)
	// ���_�@�̏���
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
	delete calcprm; delete calculator; // �p�����ς񂾂�ЂÂ��܂��傤
}
void yaku::yakuCalculator::analysisLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
	SHANTEN* const shanten, YAKUSTAT* const yakuInfo)
{
	CalculatorThread* calculator = new CalculatorThread; // �C���X�^���X�̏���
	// �ϐ���p��
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(SHANTEN[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
	// �v�Z���[�`���ɓn���p�����[�^�̏���
	CalculatorParam* calcprm = new CalculatorParam[160]; memset(calcprm, 0, sizeof(CalculatorParam[160]));
	for (int i = 0; i < 160; i++) {
		calcprm[i].gameStat = gameStat; calcprm[i].instance = calculator;
		calcprm[i].pMode.AtamaCode = (tileCode)(i / 4);
		calcprm[i].pMode.Order = (ParseOrder)(i % 4);
		memcpy(&calcprm[i].analysis, &analysis, sizeof(MENTSU_ANALYSIS));
		YAKUSTAT::Init(&calcprm[i].result);
	}
	// �v�Z�����s
	for (int i = 4; i < 160; i++) { // 0�`3��NoTile�Ȃ̂ł��Ȃ��Ă���
		while (CalculatorThread::numOfRunningThreads() >= CalculatorThread::threadLimit)
			Sleep(0); // �X���b�h�������̃`�F�b�N
		CalculatorThread::calculator(&(calcprm[i]));
	}
	while (CalculatorThread::numOfRunningThreads() > 0) Sleep(0); // ����(�ȗ��Ȏ���)
	// ���_�@�̏���
	for (int i = 4; i < 160; i++) {
		if (yakuInfo->AgariPoints < calcprm[i].result.AgariPoints)
			memcpy(yakuInfo, &calcprm[i].result, sizeof(YAKUSTAT));
	}
	// �p�����ς񂾂�ЂÂ��܂��傤
	delete[] calcprm; delete calculator;
}

// �����������Ă��邩���肷��
yaku::YAKUSTAT yaku::yakuCalculator::countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// �𔻒�
	std::ostringstream o;
	o << "�𔻒菈�����J�n �v���C���[ [" << (int)targetPlayer << "]";
	debug(o.str().c_str());
	// ������
	YAKUSTAT yakuInfo; YAKUSTAT::Init(&yakuInfo);
	// �V�����e�������`�F�b�N
	SHANTEN shanten[SHANTEN_PAGES];
	for (int i = 0; i < SHANTEN_PAGES; i++)
		shanten[i] = calcShanten(gameStat, targetPlayer, (shantenType)i);
	// �a�����Ă邩����
	if (shanten[shantenAll] > -1) {
		trace("�a�����Ă��Ȃ��̂Ŕ����܂�");
		return yakuInfo;
	}
	// �a�����Ă���Ȃ�
	if (shanten[shantenRegular] == -1) // ��ʌ`�̘a��
		analysisNonLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	else // ���Ύq�A���m���o�A���̑�����Șa��
		analysisLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	return yakuInfo;
}
__declspec(dllexport) void yaku::yakuCalculator::countyaku(const GameTable* const gameStat,
	YAKUSTAT* const yakuInfo, int targetPlayer)
{
	*yakuInfo = countyaku(gameStat, (PLAYER_ID)targetPlayer);
}
