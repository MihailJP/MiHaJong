#include "yaku.h"

/* �萔���` */
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(1, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(2, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(3, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_4han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(4, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_5han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(5, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_6han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(6, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_7han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(7, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_8han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(8, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_mangan =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(2, SemiMangan);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_haneman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(3, SemiMangan);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_baiman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(4, SemiMangan);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3baiman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(6, SemiMangan);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(1, Yakuman);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(2, Yakuman);

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

void yaku::yakuCalculator::CalculatorThread::incThreadCount() {
	trace("incThreadCount()");
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	++this->runningThreads; // �X���b�h���C���N�������g
	LeaveCriticalSection(&this->cs);
}
void yaku::yakuCalculator::CalculatorThread::decThreadCount() {
	trace("decThreadCount()");
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	--this->runningThreads; // �X���b�h���f�N�������g
	LeaveCriticalSection(&this->cs);
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
				std::set<std::string> sup = yaku.getSuppression();
				suppression.insert(sup.begin(), sup.end()); // ���ʖ��̃��X�g������
			}
	});
	/* ���ʖ����������� */
	std::for_each(suppression.begin(), suppression.end(), [&yakuHan](std::string yaku) {
		yakuHan.erase(yaku);
	});
	/* �|�����v���� */
	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	totalHan = totalSemiMangan = totalBonusHan = totalBonusSemiMangan = 0;
	std::for_each(yakuHan.begin(), yakuHan.end(),
		[&totalHan, &totalSemiMangan, &totalBonusHan, &totalBonusSemiMangan](std::pair<std::string, Yaku::YAKU_HAN> yHan) {
			switch (yHan.second.coreHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalHan += yHan.second.coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::SemiMangan: totalSemiMangan += yHan.second.coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalSemiMangan += yHan.second.coreHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "�P�ʂ��ُ�ł�");
			}
			switch (yHan.second.bonusHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalBonusHan += yHan.second.bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::SemiMangan: totalBonusSemiMangan += yHan.second.bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalBonusSemiMangan += yHan.second.bonusHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "�P�ʂ��ُ�ł�");
			}
	});
	/* �ʐ����v�Z�������ʂ��������� */
	result->CoreHan = totalHan; result->CoreSemiMangan = totalSemiMangan;
	result->BonusHan = totalBonusHan; result->BonusSemiMangan = totalBonusSemiMangan;
	/* �I������ */
	decThreadCount(); // �I�������X���b�h���f�N�������g
	return S_OK;
}

/* �R���X�g���N�^�ƃf�X�g���N�^ */
yaku::yakuCalculator::CalculatorThread::CalculatorThread() {
	InitializeCriticalSection(&cs); runningThreads = 0;
}
yaku::yakuCalculator::CalculatorThread::~CalculatorThread() {
	while (runningThreads) {
		while (runningThreads) Sleep(0);
		Sleep(0);
	}
	DeleteCriticalSection(&cs);
}
		
/* �����̏����Ƃ� */
void yaku::yakuCalculator::analysisNonLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
	SHANTEN* const shanten, YAKUSTAT* const yakuInfo)
{
	trace("AnalysisNonLoop()");
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
	DWORD ThreadID;
	HANDLE Thread = CreateThread(NULL, 0, CalculatorThread::calculator, (LPVOID)calcprm, 0, &ThreadID);
	Sleep(0);
	while (calculator->numOfRunningThreads() > 0) Sleep(0); // ����(�ȗ��Ȏ���)
	// ���_�@�̏���
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
	assert(calculator->numOfRunningThreads() == 0);
	delete calcprm; delete calculator; // �p�����ς񂾂�ЂÂ��܂��傤
}
void yaku::yakuCalculator::analysisLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
	SHANTEN* const shanten, YAKUSTAT* const yakuInfo)
{
	trace("AnalysisLoop()");
	CalculatorThread* calculator = new CalculatorThread; // �C���X�^���X�̏���
	// �ϐ���p��
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(SHANTEN[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
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
	while (calculator->numOfRunningThreads() > 0) Sleep(0); // ����(�ȗ��Ȏ���)
	// ���_�@�̏���
	for (int i = 4; i < 160; i++) {
		if (yakuInfo->AgariPoints < calcprm[i].result.AgariPoints)
			memcpy(yakuInfo, &calcprm[i].result, sizeof(YAKUSTAT));
	}
	// �p�����ς񂾂�ЂÂ��܂��傤
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
