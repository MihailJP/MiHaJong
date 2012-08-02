#include "yaku.h"
#include "yvalue.h"

/* �V���O���g�� �C���X�^���X �A�N�Z�T */
yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// �v�Z�����s(�}���`�X���b�h�Łc�c���v���Ȃ�)
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculator(LPVOID lpParam) {
	((CalculatorParam*)lpParam)->instance->incThreadCount();
	return ((CalculatorParam*)lpParam)->instance->calculate(
		((CalculatorParam*)lpParam)->gameStat,
		&(((CalculatorParam*)lpParam)->analysis),
		&(((CalculatorParam*)lpParam)->pMode),
		&(((CalculatorParam*)lpParam)->result));
}

/* �����Ă���X���b�h���̊Ǘ��p */
int yaku::yakuCalculator::CalculatorThread::numOfRunningThreads() { // �����Ă���X���b�h�̐�
	return this->runningThreads;
}
int yaku::yakuCalculator::CalculatorThread::numOfStartedThreads() { // �J�n�����X���b�h�̐�
	return this->startedThreads;
}
void yaku::yakuCalculator::CalculatorThread::sync(int threads) { // �X���b�h�𓯊�
	while (this->startedThreads < threads) Sleep(0); // �K�萔�̃X���b�h���J�n����̂�҂��Ă���
	while (this->runningThreads > 0) Sleep(0); // �I������̂�҂�
}

void yaku::yakuCalculator::CalculatorThread::incThreadCount() {
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	++this->runningThreads; // �X���b�h���C���N�������g
	++this->startedThreads;
	LeaveCriticalSection(&this->cs);
}
void yaku::yakuCalculator::CalculatorThread::decThreadCount() {
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	--this->runningThreads; // �X���b�h���f�N�������g
	LeaveCriticalSection(&this->cs);
}

/* �|���v�Z���� */
void yaku::yakuCalculator::doubling(yaku::YAKUSTAT* const yStat) {
	int totalHan = yStat->CoreHan + yStat->BonusHan; // ���v�|
	yStat->AgariPoints = LargeNum::fromInt(yStat->BasePoints); // ���_
	if (totalHan >= -2) {
		for (int i = 0; i < (totalHan + 2); i++) yStat->AgariPoints *= 2;
	} else {
		for (int i = 0; i < (totalHan + 2); i++) yStat->AgariPoints /= 2;
	}
	if (yStat->AgariPoints <= LargeNum::fromInt(0)) yStat->AgariPoints = LargeNum::fromInt(1); // �Œ�1�_�ɂ͂Ȃ�悤�ɂ���
}

/* ���Ɩ|����_�����v�Z���� */
void yaku::yakuCalculator::calculateScore(yaku::YAKUSTAT* const yStat) {
	/* ����𖞂����ĂȂ�������0��Ԃ�
	   �_����0�Ȃ�R�A�����ō��a�Ɣ��f�����c�c�͂� */
	if ((yStat->CoreHan <= 0)&&(yStat->CoreSemiMangan <= 0)) {
		yStat->AgariPoints = LargeNum::fromInt(0);
		return;
	}

	int totalHan = yStat->CoreHan + yStat->BonusHan; // ���v�|
	int totalSemiMangan = yStat->CoreSemiMangan + yStat->CoreSemiMangan; // ���т̔����P��

	if (getRule(RULE_LIMITLESS) == 0) { // �ʏ탋�[���̏ꍇ
		if ((totalHan < 6) && (totalSemiMangan < 3)) { // ���шȉ�
			doubling(yStat); // �v�Z�����s
			if (yStat->AgariPoints > LargeNum::fromInt(2000)) yStat->AgariPoints = LargeNum::fromInt(2000); // ����
		}
		else if ((totalHan < 8) && (totalSemiMangan < 4)) yStat->AgariPoints = LargeNum::fromInt(3000); // ����
		else if (((totalHan < 10) || ((totalHan == 10) && (getRule(RULE_SANBAIMAN_BORDER) == 0))) &&
			(totalSemiMangan < 6)) yStat->AgariPoints = LargeNum::fromInt(4000); // �{��
		else if (((totalHan < 12) || ((totalHan == 12) && (getRule(RULE_KAZOE_BORDER) == 0))) &&
			(totalSemiMangan < 8)) yStat->AgariPoints = LargeNum::fromInt(6000); // �O�{��
		else if ((totalSemiMangan < 8) && (getRule(RULE_KAZOE_BORDER) == 2))
			yStat->AgariPoints = LargeNum::fromInt(6000); // �O�{��
		else if (totalSemiMangan < 16) yStat->AgariPoints = LargeNum::fromInt(8000); // �� or ����
		else yStat->AgariPoints = LargeNum::fromInt(8000 * (totalSemiMangan / 8)); // �_�u���𖞈ȏ�
	} else { // �V�䃋�[���̏ꍇ
		if (totalSemiMangan >= 8) yStat->AgariPoints = LargeNum::fromInt(2500000); // �𖞂��Œ�_�ɂ��郋�[��
		else if (totalSemiMangan >= 6) yStat->AgariPoints = LargeNum::fromInt(1875000);
		else if (totalSemiMangan >= 4) yStat->AgariPoints = LargeNum::fromInt(1250000);
		else if (totalSemiMangan >= 3) yStat->AgariPoints = LargeNum::fromInt(937500);
		else if (totalSemiMangan >= 2) yStat->AgariPoints = LargeNum::fromInt(625000);
		else doubling(yStat); // �v�Z����
	}

	{ // �g���[�X�p
		std::ostringstream o;
		o << "�v�Z���ʂ� [";
		for (int i = DIGIT_GROUPS - 1; i >= 0; i--)
			o << std::setw(4) << std::dec << std::setfill('0') << yStat->AgariPoints.digitGroup[i] / 10000 <<
			" " << yStat->AgariPoints.digitGroup[i] % 10000 << (i ? " " : "");
		o << "]";
		trace(o.str().c_str());
	}
}

/* �����v�Z���� */
void yaku::yakuCalculator::CalculatorThread::calcbasepoints
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis)
{
	trace("���v�Z�̏����ɓ���܂��B");
	int fu = 20; // ����Q�O��

	/* ��������(��v�݂̂Q��) */
	switch (analysis->MianziDat[0].tile) { /* ���v�͏����ɂ���Ė�v */
	case EastWind: case SouthWind: case WestWind: case NorthWind:
		if (analysis->MianziDat[0].tile ==
			Wind2Tile((uint8_t)(gameStat->GameRound / 4))) // �ꕗ�v
			fu += 2;
		if (analysis->MianziDat[0].tile ==
			playerwind(gameStat, analysis->player, gameStat->GameRound)) // �����v
			fu += 2;
		if ((getRule(RULE_KAIMENKAZE) != 0) && (analysis->MianziDat[0].tile == // �J�啗�v
			playerwind(gameStat, gameStat->WaremePlayer, gameStat->GameRound)))
			fu += 2;
		if ((getRule(RULE_URAKAZE) != 0) && (analysis->MianziDat[0].tile == // �����v
			playerwind(gameStat, analysis->player + 2, gameStat->GameRound)))
			fu += 2;
		if ((getRule(RULE_DOUBLE_YAKU_WIND_PAIR) == 1) && (fu > 22)) fu = 22; // �_�u��������2���ƌ��Ȃ����[���̏ꍇ
		break;
	case WhiteDragon: case GreenDragon: case RedDragon: /* �O���v�͏�ɖ�v */
		fu += 2;
		break;
	}

	/* �ʎq���� */
	for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++) {
		switch (analysis->MianziDat[i].mstat) {
		case meldTripletExposedLeft: case meldTripletExposedCenter: case meldTripletExposedRight:
			fu += isYaojiu(analysis->MianziDat[i].tile) ? 4 : 2; /* �����q */
			break;
		case meldTripletConcealed:
			fu += isYaojiu(analysis->MianziDat[i].tile) ? 8 : 4; /* �Í��q */
			break;
		case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight:
		case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight:
			fu += isYaojiu(analysis->MianziDat[i].tile) ? 16 : 8; /* ���Ȏq */
			break;
		case meldQuadConcealed:
			fu += isYaojiu(analysis->MianziDat[i].tile) ? 32 : 16; /* �ÞȎq */
			break;
		}
	}
	/* ��v�������ł͂Ȃ��A���q��Ȏq���Ȃ��ꍇ�A�t���O�𗧂Ă� */
	bool NoTriplets = (fu == 20); bool LiangMianFlag = false;

	/* ���v�`���� */
	analysis->Machi = machiInvalid; // ������
	const tileCode* tsumoTile = &(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND].tile); // shorthand
	if (analysis->MianziDat[0].tile == *tsumoTile) analysis->Machi = machiTanki; // �P�R�҂�
	for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++) { // �҂��̎�ނ𒲂ׂ�c�c
		switch (analysis->MianziDat[i].mstat) {
		case meldSequenceConcealed: case meldSequenceExposedLower:
		case meldSequenceExposedMiddle: case meldSequenceExposedUpper: /* ���q */
			if (analysis->MianziDat[i].tile == ((*tsumoTile) - 1)) analysis->Machi = machiKanchan;
			if (analysis->MianziDat[i].tile == *tsumoTile) {
				if (analysis->MianziDat[i].tile % TILE_SUIT_STEP == 7) analysis->Machi = machiPenchan; // �Ӓ��҂�
				else {analysis->Machi = machiRyanmen; LiangMianFlag = true;} // ���ʑ҂�
			}
			if (analysis->MianziDat[i].tile == ((*tsumoTile) - 2)) {
				if (analysis->MianziDat[i].tile % TILE_SUIT_STEP == 1) analysis->Machi = machiPenchan; // �Ӓ��҂�
				else {analysis->Machi = machiRyanmen; LiangMianFlag = true;} // ���ʑ҂�
			}
			break;
		default: /* ����ȊO */
			if (analysis->MianziDat[i].tile == *tsumoTile) analysis->Machi = machiShanpon; // �o�|���҂�
			break;
		}
	}
	/* �ƒ��A�Ӓ��A�P�R�́{�Q���u�s���ȑ҂��ɂ͂Q�_�t���v */
	switch (analysis->Machi) {
	case machiKanchan: case machiPenchan: case machiTanki:
		fu += 2; break;
	}
	/* �o�|���҂��Ń��������ꍇ�̗�O�F�����Ƃ��Ĉ������߂̌��_ */
	if ((analysis->Machi == machiShanpon)&&(!gameStat->TsumoAgariFlag))
		fu -= isYaojiu(*tsumoTile) ? 4 : 2;

	/* ���a������������ꍇ */
	analysis->isPinfu = false; // �O�̈�
	if (NoTriplets && LiangMianFlag) {
		if (gameStat->Player[analysis->player].MenzenFlag) {
			/* ��O�ł���΁A���Ƃ��ĕ��a���������� */
			analysis->Machi = machiRyanmen; // �������ʈ���
			if ((!(gameStat->TsumoAgariFlag) || (getRule(RULE_TSUMO_PINFU) == 0))) { // �c���s�����肩�A�o�a���̏ꍇ
				analysis->isPinfu = true; fu = 20;
			} else {
				fu += 2; // �c����
			}
		} else {
			analysis->Machi = machiRyanmen; // �������ʈ���
			switch (getRule(RULE_EXPOSED_PINFU)) {
			case 0:
				fu = 30; break; /* ��O�łȂ���΁A�R�O���Ƃ��� */
			case 2: case 4:
				fu = 30;
				/* FALLTHRU */
			case 3: case 5: /* �򂢕�����ɂ���ꍇ */
				analysis->isPinfu = true;
				break;
			}
		}
	} else if (gameStat->TsumoAgariFlag) {
		fu += 2; /* ���a�łȂ��c���a����͂Q�� */
	}

	/* ��O����(�P�O��) */
	if ((gameStat->Player[analysis->player].MenzenFlag) && (!(gameStat->TsumoAgariFlag)))
		fu += 10;

	/* �I������ */
	std::ostringstream o; o << "���̎�v�� [" << fu << "] ���ł��B"; trace(o.str().c_str());
	analysis->BasePoint = (uint8_t)fu;
}

/* �h���̐��E�e�L�X�g */
void yaku::yakuCalculator::CalculatorThread::doraText
	(YAKUSTAT* const result, const char* const label, int quantity)
{
	strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, label);
	strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, " ");
	strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, intstr(quantity).c_str());
#ifdef WIN32
	strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, "\r\n");
	strcat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, "\r\n");
#else
	strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, "\n");
	strcat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, "\n");
#endif
}

/* �h���v�� */
void yaku::yakuCalculator::CalculatorThread::countDora
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result)
{
	const bool uradoraEnabled = ((getRule(RULE_URADORA) != 1) && // ���h������̃��[���ŁA
		(gameStat->Player[analysis->player].MenzenFlag) && // ��O�ł���A
		(gameStat->Player[analysis->player].RichiFlag.RichiFlag)); // �����������Ă���Ȃ�
	int omote = 0; int ura = 0; int red = 0; int blue = 0; int alice = 0;
	int flower = 0; int north = 0;
	/* �h�����v�Z���� */
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[analysis->player].Hand[i].tile == NoTile) continue;
		omote += gameStat->DoraFlag.Omote[gameStat->Player[analysis->player].Hand[i].tile];
		if (uradoraEnabled) // ���h���K�p
			ura += gameStat->DoraFlag.Ura[gameStat->Player[analysis->player].Hand[i].tile];
	}
	/* ���ʎq�̃h���𐔂��� */
	for (int i = 1; i < gameStat->Player[analysis->player].MeldPointer; i++) {
		auto k = &gameStat->Player[analysis->player].Meld[i];
		switch (k->mstat) {
		case meldSequenceExposedLower: case meldSequenceExposedMiddle: case meldSequenceExposedUpper: // ���q
			omote += gameStat->DoraFlag.Omote[k->tile] + gameStat->DoraFlag.Omote[k->tile + 1] +
				gameStat->DoraFlag.Omote[k->tile + 2];
			if (uradoraEnabled)
				ura += gameStat->DoraFlag.Ura[k->tile] + gameStat->DoraFlag.Ura[k->tile + 1] +
				gameStat->DoraFlag.Ura[k->tile + 2];
			break;
		case meldTripletExposedLeft: case meldTripletExposedCenter: case meldTripletExposedRight: // ���q
			omote += gameStat->DoraFlag.Omote[k->tile] * 3;
			if (uradoraEnabled) ura += gameStat->DoraFlag.Ura[k->tile] * 3;
			break;
		case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight: // �Ȏq
		case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight: case meldQuadConcealed:
			omote += gameStat->DoraFlag.Omote[k->tile] * 4;
			if (uradoraEnabled) ura += gameStat->DoraFlag.Ura[k->tile] * 4;
			break;
		}
	}
	/* �ԃh���E�h�� */
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[analysis->player].Hand[i].tile == NoTile) continue;
		else if (gameStat->Player[analysis->player].Hand[i].tile >= TILE_NONFLOWER_MAX) continue;
		switch (gameStat->Player[analysis->player].Hand[i].red) {
			case AkaDora: ++red; break;
			case AoDora: ++blue; break;
		}
	}
	for (int i = 1; i < gameStat->Player[analysis->player].MeldPointer; i++) {
		auto k = &gameStat->Player[analysis->player].Meld[i];
		for (int j = 0; j < (k->mstat >= meldQuadConcealed ? 4 : 3); j++) {
			switch (gameStat->Player[analysis->player].Meld[i].red[j]) {
				case AkaDora: ++red; break;
				case AoDora: ++blue; break;
			}
		}
	}
	/* �A���X�h�� */
	if ((getRule(RULE_ALICE) != 0) && (gameStat->Player[analysis->player].MenzenFlag)) {
		// �A���X���胋�[���Ŗ�O�łȂ��Ƒʖ�
		auto AlicePointer = gameStat->DoraPointer;
		// �v���L�^���[�`���̓X���b�h�Z�[�t����Ȃ������͂��Ȃ̂ŕʂ̏ꏊ�ł��
		while (AlicePointer <= gameStat->TilePointer) {
			AlicePointer -= 2;
			if (analysis->TileCount[gameStat->Deck[AlicePointer].tile] > 0) ++alice;
			else break;
		}
	}
	/* �Ԕv�E�O���̃K�� */
	if (getRule(RULE_FLOWER_TILES) != 0) {
		if (chkGameType(gameStat, AllSanma)) {
			north = gameStat->Player[analysis->player].NorthFlag;
			omote += north * (gameStat->DoraFlag.Omote[NorthWind] + 1);
			if (uradoraEnabled) ura += north * gameStat->DoraFlag.Ura[NorthWind];
			result->FlowerQuantity = north;
		} else {
			if (gameStat->Player[analysis->player].FlowerFlag.Spring) ++flower;
			if (gameStat->Player[analysis->player].FlowerFlag.Summer) ++flower;
			if (gameStat->Player[analysis->player].FlowerFlag.Autumn) ++flower;
			if (gameStat->Player[analysis->player].FlowerFlag.Winter) ++flower;
			if (gameStat->Player[analysis->player].FlowerFlag.Plum) ++flower;
			if (gameStat->Player[analysis->player].FlowerFlag.Orchid) ++flower;
			if (gameStat->Player[analysis->player].FlowerFlag.Chrys) ++flower;
			if (gameStat->Player[analysis->player].FlowerFlag.Bamboo) ++flower;
			omote += flower * (gameStat->DoraFlag.Omote[Flower] + 1);
			if (uradoraEnabled) ura += flower * gameStat->DoraFlag.Ura[Flower];
			result->FlowerQuantity = flower;
		}
	}
	/* �v�����ʂ𔽉f */
	if (omote) {
		result->DoraQuantity += omote; result->BonusHan += omote;
		doraText(result, "�h��", omote);
	}
	if (ura) {
		result->DoraQuantity += ura; result->BonusHan += ura; result->UraDoraQuantity = ura;
		doraText(result, "���h��", ura);
	}
	if (red) {
		if (getRule(RULE_REDTILE_CHIP) < 3) {
			result->DoraQuantity += red; result->BonusHan += red;
		}
		result->AkaDoraQuantity = red;
		doraText(result, "�ԃh��", red);
	}
	if (blue) {
		result->AoDoraQuantity = blue;
		switch (getRule(RULE_BLUE_TILES)) {
		case 0:
			result->DoraQuantity += blue; result->BonusHan += blue;
			doraText(result, "�h��", blue);
			break;
		case 1:
			result->DoraQuantity += blue * 2; result->BonusHan += blue * 2;
			doraText(result, "�h�� 2x", blue);
			break;
		case 2:
			result->DoraQuantity -= blue; result->BonusHan -= blue;
			doraText(result, "�h�� -", blue);
			break;
		}
	}
	if (alice) {
		result->AliceDora = alice;
		doraText(result, "�A���X�j�V", alice);
	}
}

/* �v�Z���[�`�� */
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculate
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
	const ParseMode* const pMode, YAKUSTAT* const result)
{
	/* �ʎq��͏��� */
	if (analysis->shanten[shantenRegular] == -1) {
		int NumOfMelds = 0;
		mentsuParser::makementsu(gameStat, analysis->player, *pMode, &NumOfMelds, analysis->MianziDat);
		if (NumOfMelds < SIZE_OF_MELD_BUFFER) { // �����𖞂����ĂȂ��Ȃ甲���܂�
			this->decThreadCount(); return S_OK;
		}
		calcbasepoints(gameStat, analysis); // �����v�Z����
		analysis->DuiziCount = countingFacility::countDuiz(analysis->MianziDat);
		analysis->KeziCount = countingFacility::countKez(analysis->MianziDat, &analysis->TotalKezi);
		analysis->AnKeziCount = countingFacility::countAnKez(analysis->MianziDat, &analysis->TotalAnKezi);
		analysis->ShunziCount = countingFacility::countShunz(analysis->MianziDat, &analysis->TotalShunzi);
		analysis->AnShunziCount = countingFacility::countAnShunz(analysis->MianziDat, &analysis->TotalAnShunzi);
		analysis->KangziCount = countingFacility::countKangz(analysis->MianziDat, &analysis->TotalKangzi);
		analysis->AnKangziCount = countingFacility::countAnKangz(analysis->MianziDat, &analysis->TotalAnKangzi);
		analysis->KaKangziCount = countingFacility::countKaKangz(analysis->MianziDat, &analysis->TotalKaKangzi);
	} else {
		if (analysis->shanten[shantenPairs] == -1) { // ���Ύq
			if (getRule(RULE_SEVEN_PAIRS) == 1) analysis->BasePoint = 50; // 1�|50��
			else analysis->BasePoint = 25; // 2�|25��
		}
		else if (analysis->shanten[shantenOrphans] == -1) analysis->BasePoint = 30; // ���m�͖𖞂Ȃ̂ł���͐V���[���p
		else if ((analysis->shanten[shantenQuanbukao] == -1)&&(analysis->shanten[shantenStellar] > -1)) {
			switch (getRule(RULE_QUANBUKAO)) {
			case 1:
				analysis->BasePoint = 30; break;
			case 2: case 3:
				analysis->BasePoint = 40; break;
			}
		}
	}
	/* �𔻒胋�[�v */
	std::map<std::string, Yaku::YAKU_HAN> yakuHan; // �󂯎M������
	std::set<std::string> suppression; // �����������
	std::vector<std::string> yakuOrd; // �����ۑ��p
	std::for_each(YakuCatalog::Instantiate()->catalog.begin(), // ���J�^���O�̍ŏ�����
		YakuCatalog::Instantiate()->catalog.end(), // �J�^���O�̖����܂�
		[&yakuHan, gameStat, analysis, &suppression, &yakuOrd](Yaku& yaku) -> void { // �����Ƃɔ��菈��
			if (yaku.checkYaku(analysis)) { // ���������𖞂����Ă�����
				yakuHan[yaku.getName()] = yaku.getHan(analysis); // �ʐ����L�^
				yakuOrd.push_back(yaku.getName()); // �������L�^���Ă���
				std::set<std::string> sup = yaku.getSuppression();
				suppression.insert(sup.begin(), sup.end()); // ���ʖ��̃��X�g������
			}
	});
	/* ���͐������Ă��Ȃ������������� */
	for (auto k = yakuOrd.begin(); k != yakuOrd.end(); ) {
		if ((yakuHan[*k].coreHan.getHan() == 0) && (yakuHan[*k].bonusHan.getHan() == 0)) // ���͐������ĂȂ���
			yakuOrd.erase(k++); // ���ɐ����Ȃ�
		else ++k;
	}
	/* ���ʖ����������� */
	std::for_each(suppression.begin(), suppression.end(), [&yakuOrd](std::string yaku) {
		for (auto k = yakuOrd.begin(); k != yakuOrd.end(); ) {
			if (*k == yaku) yakuOrd.erase(k++);
			else ++k;
		}
	});
	/* �|�����v���� */
	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	totalHan = totalSemiMangan = totalBonusHan = totalBonusSemiMangan = 0;
	std::for_each(yakuOrd.begin(), yakuOrd.end(),
		[&totalHan, &totalSemiMangan, &totalBonusHan, &totalBonusSemiMangan, &result, &yakuHan]
		(std::string yName) {
			/* TODO: �V�䃋�[���ɑΉ������� */
			switch (yakuHan[yName].coreHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalHan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::SemiMangan: totalSemiMangan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalSemiMangan += yakuHan[yName].coreHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "�P�ʂ��ُ�ł�");
			}
			switch (yakuHan[yName].bonusHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalBonusHan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::SemiMangan: totalBonusSemiMangan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalBonusSemiMangan += yakuHan[yName].bonusHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "�P�ʂ��ُ�ł�");
			}
			/* ���̖��O���������� */
			if ((yakuHan[yName].coreHan.getUnit() != yakuHan[yName].bonusHan.getUnit()) &&
				(yakuHan[yName].coreHan.getHan() * yakuHan[yName].bonusHan.getHan() != 0))
			{ /* �P�ʂ����݁I */
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "�P�ʂ����݂��Ă��܂�");
			}
			else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::hanUnit::Han) || (yakuHan[yName].coreHan.getHan() == 0)) &&
				((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::hanUnit::Han) || (yakuHan[yName].bonusHan.getHan() == 0)))
			{ /* ���ʂ̖��̎� */
				strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, yName.c_str());
#ifdef WIN32
				strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, "\r\n");
#else
				strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, "\n");
#endif
				strcat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize,
					intstr(yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()).c_str());
#ifdef WIN32
				strcat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, "��\r\n");
#else
				strcat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, "��\n");
#endif
			}
			else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::hanUnit::SemiMangan) || (yakuHan[yName].coreHan.getHan() == 0)) &&
				((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::hanUnit::SemiMangan) || (yakuHan[yName].bonusHan.getHan() == 0)))
			{ /* ���� */
				strcat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, yName.c_str());
#ifdef WIN32
				strcat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, "\r\n");
				char hstr[16]; sprintf_s(hstr, "%d\r\n",
#else
				strcat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, "\n");
				char hstr[16]; sprintf_s(hstr, "%d\n",
#endif
					(int)((yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()) * yaku::YAKUSTAT::SemiMangan));
				strcat_s(result->yakumanValList, yaku::YAKUSTAT::nameBufSize, hstr);
			}
			else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::hanUnit::Yakuman) || (yakuHan[yName].coreHan.getHan() == 0)) &&
				((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::hanUnit::Yakuman) || (yakuHan[yName].bonusHan.getHan() == 0)))
			{ /* �� */
				strcat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, yName.c_str());
#ifdef WIN32
				strcat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, "\r\n");
				char hstr[16]; sprintf_s(hstr, "%d\r\n",
#else
				strcat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, "\n");
				char hstr[16]; sprintf_s(hstr, "%d\n",
#endif
					(int)((yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()) * yaku::YAKUSTAT::SemiMangan * 8));
				strcat_s(result->yakumanValList, yaku::YAKUSTAT::nameBufSize, hstr);
			}
	});
	/* �ʐ����v�Z�������ʂ��������� */
	result->CoreHan = totalHan; result->CoreSemiMangan = totalSemiMangan;
	result->BonusHan = totalBonusHan; result->BonusSemiMangan = totalBonusSemiMangan;
	if (analysis->BasePoint == 25) result->BasePoints = 25; // �`�[�g�C��25���͂��̂܂�
	else result->BasePoints = analysis->BasePoint + ((10 - analysis->BasePoint % 10) % 10); // ���n�l���Ĕ��f
	/* �h���̐��𐔂��� */
	countDora(gameStat, analysis, result);
	/* �ȗ����[��(�S��30��)�̏ꍇ */
	if (getRule(RULE_SIMPLIFIED_SCORING) != 0) {
		trace("�ȗ��v�Z���[���̂���30���Ƃ��Ĉ����܂��B");
		result->BasePoints = 30;
	}
	/* TODO: �\�O�s�� */
	/* �_�����v�Z���� */
	calculateScore(result);
	/* �؂�グ���т̏��� */
	if ((getRule(RULE_ROUND_TO_MANGAN) == 1) && // �؂�グ���у��[����ON��
		(getRule(RULE_LIMITLESS) == 0) && // �V�䃋�[���łȂ��ꍇ
		(result->AgariPoints == LargeNum::fromInt(1920))) // �q��7700�E�e��11600�Ȃ�
			result->AgariPoints = LargeNum::fromInt(2000); // ���тɂ���
	/* �I������ */
	decThreadCount(); // �I�������X���b�h���f�N�������g
	return S_OK;
}

/* �R���X�g���N�^�ƃf�X�g���N�^ */
yaku::yakuCalculator::CalculatorThread::CalculatorThread() {
	InitializeCriticalSection(&cs); runningThreads = startedThreads = 0;
}
yaku::yakuCalculator::CalculatorThread::~CalculatorThread() {
	/* �I������Ƃ��͕K���������Ă���s�����ƁI�I */
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
	analysis.TileCount = countTilesInHand(gameStat, targetPlayer);
	analysis.SeenTiles = countseentiles(gameStat);
	analysis.MachiInfo = chkFuriten(gameStat, targetPlayer);
	analysis.GameStat = gameStat;
	analysis.PlayerStat = &(gameStat->Player[targetPlayer]);
	analysis.TsumoHai = &(gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1]);
	analysis.MenzenFlag = &(gameStat->Player[targetPlayer].MenzenFlag);
	analysis.TsumoAgariFlag = &(gameStat->TsumoAgariFlag);
	// �v�Z���[�`���ɓn���p�����[�^�̏���
	CalculatorParam* calcprm = new CalculatorParam; memset(calcprm, 0, sizeof(CalculatorParam));
	calcprm->gameStat = gameStat; calcprm->instance = calculator;
	memcpy(&calcprm->analysis, &analysis, sizeof(MENTSU_ANALYSIS));
	YAKUSTAT::Init(&calcprm->result);
	// �v�Z�����s
	DWORD ThreadID;
	HANDLE Thread = CreateThread(NULL, 0, CalculatorThread::calculator, (LPVOID)calcprm, 0, &ThreadID);
	calculator->sync(1); // ����(�ȗ��Ȏ���)
	// ���_�@�̏���
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
	assert(calculator->numOfStartedThreads() == 1);
	assert(calculator->numOfRunningThreads() == 0);
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
	analysis.TileCount = countTilesInHand(gameStat, targetPlayer);
	analysis.SeenTiles = countseentiles(gameStat);
	analysis.MachiInfo = chkFuriten(gameStat, targetPlayer);
	analysis.GameStat = gameStat;
	analysis.PlayerStat = &(gameStat->Player[targetPlayer]);
	analysis.TsumoHai = &(gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1]);
	analysis.MenzenFlag = &(gameStat->Player[targetPlayer].MenzenFlag);
	analysis.TsumoAgariFlag = &(gameStat->TsumoAgariFlag);
	// �v�Z���[�`���ɓn���p�����[�^�̏���
	CalculatorParam* calcprm = new CalculatorParam[160]; memset(calcprm, 0, sizeof(CalculatorParam[160]));
	DWORD ThreadID[160]; HANDLE Thread[160];
	for (int i = 0; i < 160; i++) {
		calcprm[i].instance = calculator;
		calcprm[i].gameStat = gameStat; calcprm[i].instance = calculator;
		calcprm[i].pMode.AtamaCode = (tileCode)(i / 4);
		calcprm[i].pMode.Order = (ParseOrder)(i % 4);
		memcpy(&calcprm[i].analysis, &analysis, sizeof(MENTSU_ANALYSIS));
		YAKUSTAT::Init(&calcprm[i].result);
	}
	// �v�Z�����s
	for (int i = 4; i < 160; i++) { // 0�`3��NoTile�Ȃ̂ł��Ȃ��Ă���
		while (calculator->numOfRunningThreads() >= CalculatorThread::threadLimit)
			Sleep(0); // �X���b�h�������̃`�F�b�N
		Thread[i] = CreateThread(NULL, 0, CalculatorThread::calculator, (LPVOID)(&(calcprm[i])), 0, &(ThreadID[i]));
		Sleep(0);
	}
	calculator->sync(156); // ����(�ȗ��Ȏ���)
	// ���_�@�̏���
	for (int i = 4; i < 160; i++) {
		if (yakuInfo->AgariPoints < calcprm[i].result.AgariPoints)
			memcpy(yakuInfo, &calcprm[i].result, sizeof(YAKUSTAT));
	}
	// �p�����ς񂾂�ЂÂ��܂��傤
	assert(calculator->numOfStartedThreads() == 156);
	assert(calculator->numOfRunningThreads() == 0);
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
		analysisLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	else // ���Ύq�A���m���o�A���̑�����Șa��
		analysisNonLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	return yakuInfo;
}
__declspec(dllexport) void yaku::yakuCalculator::countyaku(const GameTable* const gameStat,
	YAKUSTAT* const yakuInfo, int targetPlayer)
{
	*yakuInfo = countyaku(gameStat, (PLAYER_ID)targetPlayer);
}
