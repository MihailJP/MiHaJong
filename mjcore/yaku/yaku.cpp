#include "yaku.h"

/* 定数を定義 */
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

/* シングルトン インスタンス アクセサ */
yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// 計算を実行(マルチスレッドで……大丈夫かなぁ)
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculator(LPVOID lpParam) {
	((CalculatorParam*)lpParam)->instance->incThreadCount();
	return ((CalculatorParam*)lpParam)->instance->calculate(
		((CalculatorParam*)lpParam)->gameStat,
		&(((CalculatorParam*)lpParam)->analysis),
		&(((CalculatorParam*)lpParam)->pMode),
		&(((CalculatorParam*)lpParam)->result));
}

/* 動いているスレッド数の管理用 */
int yaku::yakuCalculator::CalculatorThread::numOfRunningThreads() { // 動いているスレッドの数
	return this->runningThreads;
}

void yaku::yakuCalculator::CalculatorThread::incThreadCount() {
	trace("incThreadCount()");
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	++this->runningThreads; // スレッド数インクリメント
	LeaveCriticalSection(&this->cs);
}
void yaku::yakuCalculator::CalculatorThread::decThreadCount() {
	trace("decThreadCount()");
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	--this->runningThreads; // スレッド数デクリメント
	LeaveCriticalSection(&this->cs);
}

/* 符を計算する */
void yaku::yakuCalculator::CalculatorThread::calcbasepoints
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis)
{
	trace("符計算の処理に入ります。");
	int fu = 20; // 副底２０符

	/* 雀頭加符(役牌のみ２符) */
	switch (analysis->MianziDat[0].tile) { /* 風牌は条件によって役牌 */
	case EastWind: case SouthWind: case WestWind: case NorthWind:
		if (analysis->MianziDat[0].tile ==
			Wind2Tile((uint8_t)(gameStat->GameRound / 4))) // 場風牌
			fu += 2;
		if (analysis->MianziDat[0].tile ==
			playerwind(gameStat, analysis->player, gameStat->GameRound)) // 自風牌
			fu += 2;
		if ((getRule(RULE_KAIMENKAZE) != 0) && (analysis->MianziDat[0].tile == // 開門風牌
			playerwind(gameStat, gameStat->WaremePlayer, gameStat->GameRound)))
			fu += 2;
		if ((getRule(RULE_URAKAZE) != 0) && (analysis->MianziDat[0].tile == // 裏風牌
			playerwind(gameStat, analysis->player + 2, gameStat->GameRound)))
			fu += 2;
		if ((getRule(RULE_DOUBLE_YAKU_WIND_PAIR) == 1) && (fu > 22)) fu = 22; // ダブ風雀頭を2符と見なすルールの場合
		break;
	case WhiteDragon: case GreenDragon: case RedDragon: /* 三元牌は常に役牌 */
		fu += 2;
		break;
	}

	/* 面子加符 */
	for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++) {
		switch (analysis->MianziDat[i].mstat) {
		case meldTripletExposedLeft: case meldTripletExposedCenter: case meldTripletExposedRight:
			fu += isYaojiu(analysis->MianziDat[i].tile) ? 4 : 2; /* 明刻子 */
			break;
		case meldTripletConcealed:
			fu += isYaojiu(analysis->MianziDat[i].tile) ? 8 : 4; /* 暗刻子 */
			break;
		case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight:
		case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight:
			fu += isYaojiu(analysis->MianziDat[i].tile) ? 16 : 8; /* 明槓子 */
			break;
		case meldQuadConcealed:
			fu += isYaojiu(analysis->MianziDat[i].tile) ? 32 : 16; /* 暗槓子 */
			break;
		}
	}
	/* 役牌が雀頭ではなく、刻子や槓子がない場合、フラグを立てる */
	bool NoTriplets = (fu == 20); bool LiangMianFlag = false;

	/* 聴牌形加符 */
	analysis->Machi = machiInvalid; // 初期化
	const tileCode* tsumoTile = &(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND].tile); // shorthand
	if (analysis->MianziDat[0].tile == *tsumoTile) analysis->Machi = machiTanki; // 単騎待ち
	for (int i = 1; i < SIZE_OF_MELD_BUFFER; i++) { // 待ちの種類を調べる……
		switch (analysis->MianziDat[i].mstat) {
		case meldSequenceConcealed: case meldSequenceExposedLower:
		case meldSequenceExposedMiddle: case meldSequenceExposedUpper: /* 順子 */
			if (analysis->MianziDat[i].tile == ((*tsumoTile) - 1)) analysis->Machi = machiKanchan;
			if (analysis->MianziDat[i].tile == *tsumoTile) {
				if (analysis->MianziDat[i].tile % TILE_SUIT_STEP == 7) analysis->Machi = machiPenchan; // 辺張待ち
				else {analysis->Machi = machiRyanmen; LiangMianFlag = true;} // 両面待ち
			}
			if (analysis->MianziDat[i].tile == ((*tsumoTile) - 2)) {
				if (analysis->MianziDat[i].tile % TILE_SUIT_STEP == 1) analysis->Machi = machiPenchan; // 辺張待ち
				else {analysis->Machi = machiRyanmen; LiangMianFlag = true;} // 両面待ち
			}
			break;
		default: /* それ以外 */
			if (analysis->MianziDat[i].tile == *tsumoTile) analysis->Machi = machiShanpon; // 双ポン待ち
			break;
		}
	}
	/* 嵌張、辺張、単騎は＋２符「不利な待ちには２点付く」 */
	switch (analysis->Machi) {
	case machiKanchan: case machiPenchan: case machiTanki:
		fu += 2; break;
	}
	/* 双ポン待ちでロンした場合の例外：明刻として扱うための減点 */
	if ((analysis->Machi == machiShanpon)&&(!gameStat->TsumoAgariFlag))
		fu -= isYaojiu(*tsumoTile) ? 4 : 2;

	/* 平和が成立しうる場合 */
	analysis->isPinfu = false; // 念の為
	if (NoTriplets && LiangMianFlag) {
		if (gameStat->Player[analysis->player].MenzenFlag) {
			/* 門前であれば、役として平和が成立する */
			analysis->Machi = machiRyanmen; // 強制両面扱い
			if ((!(gameStat->TsumoAgariFlag) || (getRule(RULE_TSUMO_PINFU) == 0))) { // ツモピンありか、出和了の場合
				analysis->isPinfu = true; fu = 20;
			} else {
				fu += 2; // ツモ符
			}
		} else {
			analysis->Machi = machiRyanmen; // 強制両面扱い
			switch (getRule(RULE_EXPOSED_PINFU)) {
			case 0:
				fu = 30; break; /* 門前でなければ、３０符とする */
			case 2: case 4:
				fu = 30;
				/* FALLTHRU */
			case 3: case 5: /* 喰い平を役にする場合 */
				analysis->isPinfu = true;
				break;
			}
		}
	} else if (gameStat->TsumoAgariFlag) {
		fu += 2; /* 平和でないツモ和了りは２符 */
	}

	/* 門前加符(１０符) */
	if ((gameStat->Player[analysis->player].MenzenFlag) && (!(gameStat->TsumoAgariFlag)))
		fu += 10;

	/* 終了処理 */
	std::ostringstream o; o << "この手牌は [" << fu << "] 符です。"; trace(o.str().c_str());
	analysis->BasePoint = (uint8_t)fu;
}

/* 計算ルーチン */
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculate
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
	const ParseMode* const pMode, YAKUSTAT* const result)
{
	/* 面子解析処理 */
	if (analysis->shanten[shantenRegular] == -1) {
		int NumOfMelds = 0;
		mentsuParser::makementsu(gameStat, analysis->player, *pMode, &NumOfMelds, analysis->MianziDat);
		if (NumOfMelds < SIZE_OF_MELD_BUFFER) { // 条件を満たしてないなら抜けます
			this->decThreadCount(); return S_OK;
		}
		calcbasepoints(gameStat, analysis); // 符を計算する
		analysis->DuiziCount = countingFacility::countDuiz(analysis->MianziDat);
		analysis->KeziCount = countingFacility::countKez(analysis->MianziDat, NULL);
		analysis->AnKeziCount = countingFacility::countAnKez(analysis->MianziDat, NULL);
		analysis->ShunziCount = countingFacility::countShunz(analysis->MianziDat);
		analysis->AnShunziCount = countingFacility::countAnShunz(analysis->MianziDat);
		analysis->KangziCount = countingFacility::countKangz(analysis->MianziDat, NULL);
		analysis->AnKangziCount = countingFacility::countAnKangz(analysis->MianziDat, NULL);
		analysis->KaKangziCount = countingFacility::countKaKangz(analysis->MianziDat, NULL);
	}
	/* 役判定ループ */
	std::map<std::string, Yaku::YAKU_HAN> yakuHan; // 受け皿初期化
	std::set<std::string> suppression; // 無効化する役
	std::for_each(YakuCatalog::Instantiate()->catalog.begin(), // 役カタログの最初から
		YakuCatalog::Instantiate()->catalog.end(), // カタログの末尾まで
		[&yakuHan, gameStat, analysis, &suppression](Yaku& yaku) -> void { // 役ごとに判定処理
			if (yaku.checkYaku(gameStat, analysis)) { // 成立条件を満たしていたら
				yakuHan[yaku.getName()] = yaku.getHan(); // 飜数を記録
				std::set<std::string> sup = yaku.getSuppression();
				suppression.insert(sup.begin(), sup.end()); // 下位役のリストを結合
			}
	});
	/* 下位役を除去する */
	std::for_each(suppression.begin(), suppression.end(), [&yakuHan](std::string yaku) {
		yakuHan.erase(yaku);
	});
	/* 翻を合計する */
	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	totalHan = totalSemiMangan = totalBonusHan = totalBonusSemiMangan = 0;
	std::for_each(yakuHan.begin(), yakuHan.end(),
		[&totalHan, &totalSemiMangan, &totalBonusHan, &totalBonusSemiMangan](std::pair<std::string, Yaku::YAKU_HAN> yHan) {
			switch (yHan.second.coreHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalHan += yHan.second.coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::SemiMangan: totalSemiMangan += yHan.second.coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalSemiMangan += yHan.second.coreHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が異常です");
			}
			switch (yHan.second.bonusHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalBonusHan += yHan.second.bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::SemiMangan: totalBonusSemiMangan += yHan.second.bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalBonusSemiMangan += yHan.second.bonusHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が異常です");
			}
	});
	/* 飜数を計算した結果を書き込む */
	result->CoreHan = totalHan; result->CoreSemiMangan = totalSemiMangan;
	result->BonusHan = totalBonusHan; result->BonusSemiMangan = totalBonusSemiMangan;
	/* 終了処理 */
	decThreadCount(); // 終わったらスレッド数デクリメント
	return S_OK;
}

/* コンストラクタとデストラクタ */
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
		
/* 引数の準備とか */
void yaku::yakuCalculator::analysisNonLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
	SHANTEN* const shanten, YAKUSTAT* const yakuInfo)
{
	trace("AnalysisNonLoop()");
	CalculatorThread* calculator = new CalculatorThread; // インスタンスの準備
	// 変数を用意
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(SHANTEN[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
	// 計算ルーチンに渡すパラメータの準備
	CalculatorParam* calcprm = new CalculatorParam; memset(calcprm, 0, sizeof(CalculatorParam));
	calcprm->gameStat = gameStat; calcprm->instance = calculator;
	memcpy(&calcprm->analysis, &analysis, sizeof(MENTSU_ANALYSIS));
	YAKUSTAT::Init(&calcprm->result);
	// 計算を実行
	DWORD ThreadID;
	HANDLE Thread = CreateThread(NULL, 0, CalculatorThread::calculator, (LPVOID)calcprm, 0, &ThreadID);
	Sleep(0);
	while (calculator->numOfRunningThreads() > 0) Sleep(0); // 同期(簡略な実装)
	// 高点法の処理
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
	assert(calculator->numOfRunningThreads() == 0);
	delete calcprm; delete calculator; // 用事が済んだら片づけましょう
}
void yaku::yakuCalculator::analysisLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
	SHANTEN* const shanten, YAKUSTAT* const yakuInfo)
{
	trace("AnalysisLoop()");
	CalculatorThread* calculator = new CalculatorThread; // インスタンスの準備
	// 変数を用意
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(SHANTEN[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
	// 計算ルーチンに渡すパラメータの準備
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
	// 計算を実行
	for (int i = 4; i < 160; i++) { // 0～3はNoTileなのでやらなくていい
		while (calculator->numOfRunningThreads() >= CalculatorThread::threadLimit)
			Sleep(0); // スレッド数制限のチェック
		Thread[i] = CreateThread(NULL, 0, CalculatorThread::calculator, (LPVOID)(&(calcprm[i])), 0, &(ThreadID[i]));
		Sleep(0);
	}
	while (calculator->numOfRunningThreads() > 0) Sleep(0); // 同期(簡略な実装)
	// 高点法の処理
	for (int i = 4; i < 160; i++) {
		if (yakuInfo->AgariPoints < calcprm[i].result.AgariPoints)
			memcpy(yakuInfo, &calcprm[i].result, sizeof(YAKUSTAT));
	}
	// 用事が済んだら片づけましょう
	assert(calculator->numOfRunningThreads() == 0);
	delete[] calcprm; delete calculator;
}

// 役が成立しているか判定する
yaku::YAKUSTAT yaku::yakuCalculator::countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// 役判定
	std::ostringstream o;
	o << "役判定処理を開始 プレイヤー [" << (int)targetPlayer << "]";
	debug(o.str().c_str());
	// 初期化
	YAKUSTAT yakuInfo; YAKUSTAT::Init(&yakuInfo);
	// シャンテン数をチェック
	SHANTEN shanten[SHANTEN_PAGES];
	for (int i = 0; i < SHANTEN_PAGES; i++)
		shanten[i] = calcShanten(gameStat, targetPlayer, (shantenType)i);
	// 和了ってるか判定
	if (shanten[shantenAll] > -1) {
		trace("和了っていないので抜けます");
		return yakuInfo;
	}
	// 和了っているなら
	if (shanten[shantenRegular] == -1) // 一般形の和了
		analysisLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	else // 七対子、国士無双、その他特殊な和了
		analysisNonLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	return yakuInfo;
}
__declspec(dllexport) void yaku::yakuCalculator::countyaku(const GameTable* const gameStat,
	YAKUSTAT* const yakuInfo, int targetPlayer)
{
	*yakuInfo = countyaku(gameStat, (PLAYER_ID)targetPlayer);
}
