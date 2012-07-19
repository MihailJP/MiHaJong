#include "yaku.h"

yaku::yakuCalculator::YakuCatalog* yaku::yakuCalculator::YakuCatalog::Instantiate() {
	// Singleton instance accessor
	static YakuCatalog instance;
	return &instance;
}

// 設定したルールに基づいて役インスタンスを初期化する
__declspec(dllexport) void yaku::yakuCalculator::init() {
	YakuCatalog::Instantiate()->catalog.clear(); // リセット
	info("役カタログをリセットしました。");
	/*
	 * ここにコンストラクタを並べる
	 */
	YakuCatalog::Instantiate()->catalog.push_back(Yaku( // テスト用のダミーの役
		"ダミー", Yaku::YAKU_HAN::HAN(1, Han), Yaku::YAKU_HAN::HAN(),
		[](const GameTable* const, const MENTSU_ANALYSIS* const) {
			return true;
		}
	));
	info("役カタログの構築を完了しました。");
}

// 計算を実行(マルチスレッドで……大丈夫かなぁ)
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculator(LPVOID lpParam) {
	incThreadCount();
	return ((CalculatorParam*)lpParam)->instance->calculate(
		((CalculatorParam*)lpParam)->gameStat,
		&(((CalculatorParam*)lpParam)->analysis),
		&(((CalculatorParam*)lpParam)->pMode));
}

/* 動いているスレッド数の管理用 */
CRITICAL_SECTION yaku::yakuCalculator::CalculatorThread::cs;
int yaku::yakuCalculator::CalculatorThread::runningThreads = 0;

int yaku::yakuCalculator::CalculatorThread::numOfRunningThreads() { // 動いているスレッドの数
	return runningThreads;
}

void yaku::yakuCalculator::CalculatorThread::incThreadCount() {
	EnterCriticalSection(&cs); ++runningThreads; LeaveCriticalSection(&cs); // スレッド数インクリメント
}
void yaku::yakuCalculator::CalculatorThread::decThreadCount() {
	EnterCriticalSection(&cs); --runningThreads; LeaveCriticalSection(&cs); // スレッド数デクリメント
}

/* 計算ルーチン */
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculate
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, const ParseMode* const pMode)
{
	/* 面子解析処理 */
	if (analysis->shanten[shantenRegular] == -1) {
		int NumOfMelds = 0;
		mentsuParser::makementsu(gameStat, analysis->player, *pMode, &NumOfMelds, analysis->MianziDat);
		if (NumOfMelds < SIZE_OF_MELD_BUFFER) { // 条件を満たしてないなら抜けます
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
	/* 役判定ループ */
	std::map<std::string, Yaku::YAKU_HAN> yakuHan; // 受け皿初期化
	std::set<std::string> suppression; // 無効化する役
	std::for_each(YakuCatalog::Instantiate()->catalog.begin(), // 役カタログの最初から
		YakuCatalog::Instantiate()->catalog.end(), // カタログの末尾まで
		[&yakuHan, gameStat, analysis, &suppression](Yaku& yaku) -> void { // 役ごとに判定処理
			if (yaku.checkYaku(gameStat, analysis)) { // 成立条件を満たしていたら
				yakuHan[yaku.getName()] = yaku.getHan(); // 飜数を記録
				suppression.insert(yaku.getSuppression().begin(), yaku.getSuppression().end()); // 下位役のリストを結合
			}
	});
	/* 下位役を除去する */
	std::for_each(suppression.begin(), suppression.end(), [&yakuHan](std::string yaku) {
		yakuHan.erase(yaku);
	});
	/* 翻を合計する */
	int totalHan, totalMangan, totalBonusHan, totalBonusMangan;
	totalHan = totalMangan = totalBonusHan = totalBonusMangan = 0;
	std::for_each(yakuHan.begin(), yakuHan.end(),
		[&totalHan, &totalMangan, &totalBonusHan, &totalBonusMangan](std::pair<std::string, Yaku::YAKU_HAN> yHan) {
			switch (yHan.second.coreHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalHan += yHan.second.coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Mangan: totalMangan += yHan.second.coreHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalMangan += yHan.second.coreHan.getHan() * 4; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が異常です");
			}
			switch (yHan.second.bonusHan.getUnit()) {
				case yaku::yakuCalculator::hanUnit::Han: totalBonusHan += yHan.second.bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Mangan: totalBonusMangan += yHan.second.bonusHan.getHan(); break;
				case yaku::yakuCalculator::hanUnit::Yakuman: totalBonusMangan += yHan.second.bonusHan.getHan() * 4; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が異常です");
			}
	});
	/* 終了処理 */
	decThreadCount(); // 終わったらスレッド数デクリメント
	return S_OK;
}

/* コンストラクタとデストラクタ */
yaku::yakuCalculator::CalculatorThread::CalculatorThread() {
	InitializeCriticalSection(&cs);
}
yaku::yakuCalculator::CalculatorThread::~CalculatorThread() {
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
	// 計算ルーチンに渡すパラメータの準備
	CalculatorParam* calcprm = new CalculatorParam; memset(calcprm, 0, sizeof(CalculatorParam));
	calcprm->gameStat = gameStat; calcprm->instance = calculator;
	memcpy(&calcprm->analysis, &analysis, sizeof(MENTSU_ANALYSIS));
	YAKUSTAT::Init(&calcprm->result);
	// 計算を実行
	CalculatorThread::calculator(&calcprm);
	while (CalculatorThread::numOfRunningThreads() > 0) Sleep(0); // 同期(簡略な実装)
	// 高点法の処理
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
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
	// 計算ルーチンに渡すパラメータの準備
	CalculatorParam* calcprm = new CalculatorParam[160]; memset(calcprm, 0, sizeof(CalculatorParam[160]));
	for (int i = 0; i < 160; i++) {
		calcprm[i].gameStat = gameStat; calcprm[i].instance = calculator;
		calcprm[i].pMode.AtamaCode = (tileCode)(i / 4);
		calcprm[i].pMode.Order = (ParseOrder)(i % 4);
		memcpy(&calcprm[i].analysis, &analysis, sizeof(MENTSU_ANALYSIS));
		YAKUSTAT::Init(&calcprm[i].result);
	}
	// 計算を実行
	for (int i = 4; i < 160; i++) { // 0～3はNoTileなのでやらなくていい
		while (CalculatorThread::numOfRunningThreads() >= CalculatorThread::threadLimit)
			Sleep(0); // スレッド数制限のチェック
		CalculatorThread::calculator(&(calcprm[i]));
	}
	while (CalculatorThread::numOfRunningThreads() > 0) Sleep(0); // 同期(簡略な実装)
	// 高点法の処理
	for (int i = 4; i < 160; i++) {
		if (yakuInfo->AgariPoints < calcprm[i].result.AgariPoints)
			memcpy(yakuInfo, &calcprm[i].result, sizeof(YAKUSTAT));
	}
	// 用事が済んだら片づけましょう
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
		analysisNonLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	else // 七対子、国士無双、その他特殊な和了
		analysisLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	return yakuInfo;
}
__declspec(dllexport) void yaku::yakuCalculator::countyaku(const GameTable* const gameStat,
	YAKUSTAT* const yakuInfo, int targetPlayer)
{
	*yakuInfo = countyaku(gameStat, (PLAYER_ID)targetPlayer);
}
