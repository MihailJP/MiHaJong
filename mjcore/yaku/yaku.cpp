#include "yaku.h"
#include "yvalue.h"

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
int yaku::yakuCalculator::CalculatorThread::numOfStartedThreads() { // 開始したスレッドの数
	return this->startedThreads;
}
void yaku::yakuCalculator::CalculatorThread::sync(int threads) { // スレッドを同期
	while (this->startedThreads < threads) Sleep(0); // 規定数のスレッドが開始するのを待ってから
	while (this->runningThreads > 0) Sleep(0); // 終了するのを待つ
}

void yaku::yakuCalculator::CalculatorThread::incThreadCount() {
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	++this->runningThreads; // スレッド数インクリメント
	++this->startedThreads;
	LeaveCriticalSection(&this->cs);
}
void yaku::yakuCalculator::CalculatorThread::decThreadCount() {
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	--this->runningThreads; // スレッド数デクリメント
	LeaveCriticalSection(&this->cs);
}

/* 翻を計算する */
void yaku::yakuCalculator::doubling(yaku::YAKUSTAT* const yStat) {
	int totalHan = yStat->CoreHan + yStat->BonusHan; // 合計翻
	yStat->AgariPoints = LargeNum::fromInt(yStat->BasePoints); // 原点
	if (totalHan >= -2) {
		for (int i = 0; i < (totalHan + 2); i++) yStat->AgariPoints *= 2;
	} else {
		for (int i = 0; i < (totalHan + 2); i++) yStat->AgariPoints /= 2;
	}
	if (yStat->AgariPoints <= LargeNum::fromInt(0)) yStat->AgariPoints = LargeNum::fromInt(1); // 最低1点にはなるようにする
}

/* 符と翻から点数を計算する */
void yaku::yakuCalculator::calculateScore(yaku::YAKUSTAT* const yStat) {
	/* 縛りを満たしてなかったら0を返す
	   点数が0ならコア部分で錯和と判断される……はず */
	if ((yStat->CoreHan <= 0)&&(yStat->CoreSemiMangan <= 0)) {
		yStat->AgariPoints = LargeNum::fromInt(0);
		return;
	}

	int totalHan = yStat->CoreHan + yStat->BonusHan; // 合計翻
	int totalSemiMangan = yStat->CoreSemiMangan + yStat->CoreSemiMangan; // 満貫の半分単位

	if (getRule(RULE_LIMITLESS) == 0) { // 通常ルールの場合
		if ((totalHan < 6) && (totalSemiMangan < 3)) { // 満貫以下
			doubling(yStat); // 計算を実行
			if (yStat->AgariPoints > LargeNum::fromInt(2000)) yStat->AgariPoints = LargeNum::fromInt(2000); // 満貫
		}
		else if ((totalHan < 8) && (totalSemiMangan < 4)) yStat->AgariPoints = LargeNum::fromInt(3000); // 跳満
		else if (((totalHan < 10) || ((totalHan == 10) && (getRule(RULE_SANBAIMAN_BORDER) == 0))) &&
			(totalSemiMangan < 6)) yStat->AgariPoints = LargeNum::fromInt(4000); // 倍満
		else if (((totalHan < 12) || ((totalHan == 12) && (getRule(RULE_KAZOE_BORDER) == 0))) &&
			(totalSemiMangan < 8)) yStat->AgariPoints = LargeNum::fromInt(6000); // 三倍満
		else if ((totalSemiMangan < 8) && (getRule(RULE_KAZOE_BORDER) == 2))
			yStat->AgariPoints = LargeNum::fromInt(6000); // 三倍満
		else if (totalSemiMangan < 16) yStat->AgariPoints = LargeNum::fromInt(8000); // 役満 or 数え
		else yStat->AgariPoints = LargeNum::fromInt(8000 * (totalSemiMangan / 8)); // ダブル役満以上
	} else { // 青天井ルールの場合
		if (totalSemiMangan >= 8) yStat->AgariPoints = LargeNum::fromInt(2500000); // 役満を固定点にするルール
		else if (totalSemiMangan >= 6) yStat->AgariPoints = LargeNum::fromInt(1875000);
		else if (totalSemiMangan >= 4) yStat->AgariPoints = LargeNum::fromInt(1250000);
		else if (totalSemiMangan >= 3) yStat->AgariPoints = LargeNum::fromInt(937500);
		else if (totalSemiMangan >= 2) yStat->AgariPoints = LargeNum::fromInt(625000);
		else doubling(yStat); // 計算する
	}

	{ // トレース用
		std::ostringstream o;
		o << "計算結果は [";
		for (int i = DIGIT_GROUPS - 1; i >= 0; i--)
			o << std::setw(4) << std::dec << std::setfill('0') << yStat->AgariPoints.digitGroup[i] / 10000 <<
			" " << yStat->AgariPoints.digitGroup[i] % 10000 << (i ? " " : "");
		o << "]";
		trace(o.str().c_str());
	}
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
		analysis->KeziCount = countingFacility::countKez(analysis->MianziDat, &analysis->TotalKezi);
		analysis->AnKeziCount = countingFacility::countAnKez(analysis->MianziDat, &analysis->TotalAnKezi);
		analysis->ShunziCount = countingFacility::countShunz(analysis->MianziDat, &analysis->TotalShunzi);
		analysis->AnShunziCount = countingFacility::countAnShunz(analysis->MianziDat, &analysis->TotalAnShunzi);
		analysis->KangziCount = countingFacility::countKangz(analysis->MianziDat, &analysis->TotalKangzi);
		analysis->AnKangziCount = countingFacility::countAnKangz(analysis->MianziDat, &analysis->TotalAnKangzi);
		analysis->KaKangziCount = countingFacility::countKaKangz(analysis->MianziDat, &analysis->TotalKaKangzi);
	} else {
		if (analysis->shanten[shantenPairs] == -1) { // 七対子
			if (getRule(RULE_SEVEN_PAIRS) == 1) analysis->BasePoint = 50; // 1翻50符
			else analysis->BasePoint = 25; // 2翻25符
		}
		else if (analysis->shanten[shantenOrphans] == -1) analysis->BasePoint = 30; // 国士は役満なのでこれは青天ルール用
		else if ((analysis->shanten[shantenQuanbukao] == -1)&&(analysis->shanten[shantenStellar] > -1)) {
			switch (getRule(RULE_QUANBUKAO)) {
			case 1:
				analysis->BasePoint = 30; break;
			case 2: case 3:
				analysis->BasePoint = 40; break;
			}
		}
	}
	/* 役判定ループ */
	std::map<std::string, Yaku::YAKU_HAN> yakuHan; // 受け皿初期化
	std::set<std::string> suppression; // 無効化する役
	std::for_each(YakuCatalog::Instantiate()->catalog.begin(), // 役カタログの最初から
		YakuCatalog::Instantiate()->catalog.end(), // カタログの末尾まで
		[&yakuHan, gameStat, analysis, &suppression](Yaku& yaku) -> void { // 役ごとに判定処理
			if (yaku.checkYaku(gameStat, analysis)) { // 成立条件を満たしていたら
				yakuHan[yaku.getName()] = yaku.getHan(gameStat, analysis); // 飜数を記録
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
	/* TODO: ドラの数を数える */
	/* TODO: 簡略ルール(全部30符) */
	/* TODO: 十三不塔 */
	/* TODO: 切り上げ満貫 */
	/* 点数を計算する */
	calculateScore(result);
	/* 終了処理 */
	decThreadCount(); // 終わったらスレッド数デクリメント
	return S_OK;
}

/* コンストラクタとデストラクタ */
yaku::yakuCalculator::CalculatorThread::CalculatorThread() {
	InitializeCriticalSection(&cs); runningThreads = startedThreads = 0;
}
yaku::yakuCalculator::CalculatorThread::~CalculatorThread() {
	/* 終了するときは必ず同期してから行うこと！！ */
	DeleteCriticalSection(&cs);
}
		
/* 引数の準備とか */
void yaku::yakuCalculator::analysisNonLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
	SHANTEN* const shanten, YAKUSTAT* const yakuInfo)
{
	CalculatorThread* calculator = new CalculatorThread; // インスタンスの準備
	// 変数を用意
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(SHANTEN[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
	analysis.TileCount = countTilesInHand(gameStat, targetPlayer);
	analysis.SeenTiles = countseentiles(gameStat);
	// 計算ルーチンに渡すパラメータの準備
	CalculatorParam* calcprm = new CalculatorParam; memset(calcprm, 0, sizeof(CalculatorParam));
	calcprm->gameStat = gameStat; calcprm->instance = calculator;
	memcpy(&calcprm->analysis, &analysis, sizeof(MENTSU_ANALYSIS));
	YAKUSTAT::Init(&calcprm->result);
	// 計算を実行
	DWORD ThreadID;
	HANDLE Thread = CreateThread(NULL, 0, CalculatorThread::calculator, (LPVOID)calcprm, 0, &ThreadID);
	calculator->sync(1); // 同期(簡略な実装)
	// 高点法の処理
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
	assert(calculator->numOfStartedThreads() == 1);
	assert(calculator->numOfRunningThreads() == 0);
	delete calcprm; delete calculator; // 用事が済んだら片づけましょう
}
void yaku::yakuCalculator::analysisLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
	SHANTEN* const shanten, YAKUSTAT* const yakuInfo)
{
	CalculatorThread* calculator = new CalculatorThread; // インスタンスの準備
	// 変数を用意
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(SHANTEN[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
	analysis.TileCount = countTilesInHand(gameStat, targetPlayer);
<<<<<<< HEAD
=======
	analysis.SeenTiles = countseentiles(gameStat);
>>>>>>> yakuport
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
	calculator->sync(156); // 同期(簡略な実装)
	// 高点法の処理
	for (int i = 4; i < 160; i++) {
		if (yakuInfo->AgariPoints < calcprm[i].result.AgariPoints)
			memcpy(yakuInfo, &calcprm[i].result, sizeof(YAKUSTAT));
	}
	// 用事が済んだら片づけましょう
	assert(calculator->numOfStartedThreads() == 156);
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
