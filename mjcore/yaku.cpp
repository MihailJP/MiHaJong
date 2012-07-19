#include "yaku.h"

/* 構造体のサイズを返す */
int yaku::YAKUSTAT::getSize() {
	return sizeof(YAKUSTAT);
}

/* 既存のインスタンスを初期化する */
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

/* HSPからのアクセサ */
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
	}
}
void yaku::YAKUSTAT::setYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value) {
	switch (index) {
		case 17: strcpy_s(myInstance->yakuNameList, 1024, value); break;
		case 18: strcpy_s(myInstance->yakuValList, 1024, value); break;
		case 19: strcpy_s(myInstance->yakumanNameList, 1024, value); break;
		case 20: strcpy_s(myInstance->yakumanValList, 1024, value); break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
	}
}
void yaku::YAKUSTAT::addYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value) {
	switch (index) {
		case 17: strcat_s(myInstance->yakuNameList, 1024, value); break;
		case 18: strcat_s(myInstance->yakuValList, 1024, value); break;
		case 19: strcat_s(myInstance->yakumanNameList, 1024, value); break;
		case 20: strcat_s(myInstance->yakumanValList, 1024, value); break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, "ページが違います");
			memset(targetStr, 0, bufSize);
	}
}

// -------------------------------------------------------------------------

/* 順子の処理 */
bool yaku::mentsuParser::makementsu_shuntsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
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
void yaku::mentsuParser::makementsu_koutsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
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
void yaku::mentsuParser::makementsu(const GameTable* const gameStat,
	PLAYER_ID targetPlayer, ParseMode AtamaCode, int* const ProcessedMelds, MELD_BUF MianziDat)
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

// -------------------------------------------------------------------------

/* 指定の対子があるか数える */
int yaku::countingFacility::countPairs(
	const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles)
{
	// 指定した種類の対子を数える
	trace("対子の種類を調べます。");
	int yakuflagcount = 0;
	for (int i = 0; i < numOfTiles; i++)
		if (tileCount[targetTiles[i]] >= 2) yakuflagcount++;
	return yakuflagcount;
}

/* 数字の合計を数える(七対子版) */
int yaku::countingFacility::countTileNumerals(const Int8ByTile tileCount) {
	/* 数字の合計を数える */
	int Cifr = 0;
	for (int i = 1; i < (TILE_SUIT_HONORS - 1); i++)
		Cifr += tileCount[i] * (i % TILE_SUIT_STEP);
	return Cifr;
}
	
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 計数用関数 */
Int8ByTile yaku::countingFacility::countByMelds(
	const MELD_BUF MianziDat, int* const hits,
	std::function<bool (meldStat)> f)
{ // 計数ルーチン
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

/* 刻子の数を数える */
Int8ByTile yaku::countingFacility::countKez(const MELD_BUF MianziDat, int* const Kezi) { /* 刻子の数を数える */
	trace("刻子・槓子の種類を調べます。");
	return countByMelds(MianziDat, Kezi, [](meldStat x){return x >= meldTripletConcealed;});
}

/* 暗刻子の数を数える */
Int8ByTile yaku::countingFacility::countAnKez(const MELD_BUF MianziDat, int* const Kezi) { /* 暗刻子の数を数える */
	trace("暗刻子・暗槓子の種類を調べます。");
	return countByMelds(MianziDat, Kezi, [](meldStat x){return ((x == meldTripletConcealed)||(x == meldQuadConcealed));});
}

/* 対子・刻子・槓子の数を数える */
Int8ByTile yaku::countingFacility::countDuiz(const MELD_BUF MianziDat) { /* 対子・刻子・槓子の数を数える */
	trace("対子・刻子・槓子の種類を調べます。");
	auto DuiCount = countKez(MianziDat, NULL);
	++(DuiCount[MianziDat[0].tile]);
	return DuiCount;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 順子の数を数える */
Int8ByTile yaku::countingFacility::countShunz(const MELD_BUF MianziDat) { /* 順子の数を数える */
	trace("順子の種類を調べます。");
	return countByMelds(MianziDat, NULL, [](meldStat x){return x < meldTripletConcealed;});
}

/* 暗順子の数を数える */
Int8ByTile yaku::countingFacility::countAnShunz(const MELD_BUF MianziDat) { /* 暗順子の数を数える */
	trace("暗順子の種類を調べます。");
	return countByMelds(MianziDat, NULL, [](meldStat x){return x == meldSequenceConcealed;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 槓子の数を数える */
Int8ByTile yaku::countingFacility::countKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* 槓子の数を数える */
	trace("槓子の種類を調べます。");
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x >= meldQuadConcealed;});
}

/* 暗槓子の数を数える */
Int8ByTile yaku::countingFacility::countAnKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* 暗槓子の数を数える */
	trace("暗槓子の種類を調べます。");
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x == meldQuadConcealed;});
}

/* 加槓の数を数える */
Int8ByTile yaku::countingFacility::countKaKangz(const MELD_BUF MianziDat, int* const Kangzi) { /* 加槓の数を数える */
	trace("加槓の種類を調べます。");
	return countByMelds(MianziDat, Kangzi, [](meldStat x){return x >= meldQuadAddedLeft;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 指定の刻子・順子があるか数える */
int yaku::countingFacility::countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
	const tileCode* const targetShunz, int numOfShunz, bool Mode)
{ // 指定した種類の面子を数える
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

/* 数字の合計を数える */
int yaku::countingFacility::countMentzNumerals(const MELD_BUF MianziDat) { /* 数字の合計を数える */
	int Cifr = 0;
	for (int i = 0; i < SIZE_OF_MELD_BUFFER; i++) {
		if (MianziDat[i].tile % TILE_CODE_MAXIMUM < TILE_SUIT_HONORS) {
			if (i == 0) { // 雀頭
				Cifr += (MianziDat[0].tile % TILE_SUIT_STEP) * 2;
			} else { // 面子
				switch (MianziDat[i].mstat) {
				case meldSequenceConcealed: case meldSequenceExposedLower:
				case meldSequenceExposedMiddle: case meldSequenceExposedUpper:
					Cifr += (((MianziDat[i].tile % TILE_SUIT_STEP)+1) * 3); // 順子
					break;
				case meldTripletConcealed: case meldTripletExposedLeft:
				case meldTripletExposedCenter: case meldTripletExposedRight:
					Cifr += ((MianziDat[i].tile % TILE_SUIT_STEP) * 3); // 刻子
					break;
				default:
					Cifr += ((MianziDat[i].tile % TILE_SUIT_STEP) * 4); // 槓子
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
