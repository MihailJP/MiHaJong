#include "yaku.h"
#include "yvalue.h"

// 計算を実行(マルチスレッドで……大丈夫かなぁ)
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculator(LPVOID lpParam) {
	((CalculatorParam*)lpParam)->instance->recordThreadStart();
	return ((CalculatorParam*)lpParam)->instance->calculate(
		((CalculatorParam*)lpParam)->gameStat,
		&(((CalculatorParam*)lpParam)->analysis),
		&(((CalculatorParam*)lpParam)->pMode),
		&(((CalculatorParam*)lpParam)->result));
}

/* 動いているスレッド数の管理用 */
int yaku::yakuCalculator::CalculatorThread::numOfFinishedThreads() { // 終わったスレッドの数
	return this->finishedThreads;
}
int yaku::yakuCalculator::CalculatorThread::numOfStartedThreads() { // 開始したスレッドの数
	return this->startedThreads;
}
void yaku::yakuCalculator::CalculatorThread::sync(int threads) { // スレッドを同期
	while (this->startedThreads < threads) Sleep(0); // 規定数のスレッドが開始するのを待ってから
	while (this->finishedThreads < threads) Sleep(0); // 終了するのを待つ
}

void yaku::yakuCalculator::CalculatorThread::recordThreadStart() {
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	++this->startedThreads; // 開始したスレッド数をインクリメント
	LeaveCriticalSection(&this->cs);
}
void yaku::yakuCalculator::CalculatorThread::recordThreadFinish() {
	while (TryEnterCriticalSection(&this->cs) == 0) Sleep(0);
	++this->finishedThreads; // 終了したスレッド数をインクリメント
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
	int totalSemiMangan = yStat->CoreSemiMangan + yStat->BonusSemiMangan; // 満貫の半分単位

	if (!RuleData::chkRuleApplied("limitless")) { // 通常ルールの場合
		if ((totalHan < 6) && (totalSemiMangan < 3)) { // 満貫以下
			doubling(yStat); // 計算を実行
			if (yStat->AgariPoints > LargeNum::fromInt(2000)) yStat->AgariPoints = LargeNum::fromInt(2000); // 満貫
		}
		else if ((totalHan < 8) && (totalSemiMangan < 4)) yStat->AgariPoints = LargeNum::fromInt(3000); // 跳満
		else if (((totalHan < 10) || ((totalHan == 10) && (RuleData::chkRule("sanbaiman_border", "11han_or_more")))) &&
			(totalSemiMangan < 6)) yStat->AgariPoints = LargeNum::fromInt(4000); // 倍満
		else if (((totalHan < 12) || ((totalHan == 12) && (RuleData::chkRule("kazoe_border", "13han_or_more")))) &&
			(totalSemiMangan < 8)) yStat->AgariPoints = LargeNum::fromInt(6000); // 三倍満
		else if ((totalSemiMangan < 8) && (RuleData::chkRule("kazoe_border", "no")))
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
			" " << std::setw(4) << std::dec << std::setfill('0') << yStat->AgariPoints.digitGroup[i] % 10000 << (i ? " " : "");
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
		if ((RuleData::chkRuleApplied("kaimenkaze")) && (analysis->MianziDat[0].tile == // 開門風牌
			playerwind(gameStat, gameStat->WaremePlayer, gameStat->GameRound)))
			fu += 2;
		if ((RuleData::chkRuleApplied("urakaze")) && (analysis->MianziDat[0].tile == // 裏風牌
			playerwind(gameStat, analysis->player + 2, gameStat->GameRound)))
			fu += 2;
		if ((!RuleData::chkRuleApplied("double_yaku_wind_pair")) && (fu > 22)) fu = 22; // ダブ風雀頭を2符と見なすルールの場合
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
			if ((!(gameStat->TsumoAgariFlag) || (RuleData::chkRuleApplied("tsumo_pinfu")))) { // ツモピンありか、出和了の場合
				analysis->isPinfu = true; fu = 20;
			} else {
				fu += 2; // ツモ符
			}
		} else {
			analysis->Machi = machiRyanmen; // 強制両面扱い
			if (RuleData::chkRule("exposed_pinfu", "30fu")) {
				fu = 30; /* 門前でなければ、３０符とする */
			}
			else if (RuleData::chkRule("exposed_pinfu", "1han_30fu_unbound") || RuleData::chkRule("exposed_pinfu", "1han_30fu")) {
				fu = 30; analysis->isPinfu = true;
			}
			else if (RuleData::chkRule("exposed_pinfu", "1han_20fu_unbound") || RuleData::chkRule("exposed_pinfu", "1han_20fu")) {
				analysis->isPinfu = true; /* 喰い平を役にする場合 */
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

/* ドラ計数 */
void yaku::yakuCalculator::countDora
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
	YAKUSTAT* const result, PLAYER_ID targetPlayer)
{
	auto doraText =
		[](YAKUSTAT* const result, const char* const label, int quantity) {
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
		};
	const bool uradoraEnabled = ((RuleData::chkRuleApplied("uradora")) && // 裏ドラありのルールで、
		(gameStat->Player[targetPlayer].MenzenFlag) && // 門前であり、
		(gameStat->Player[targetPlayer].RichiFlag.RichiFlag)); // 立直をかけているなら
	int omote = 0; int ura = 0; int red = 0; int blue = 0; int alice = 0;
	int flower = 0; int north = 0;
	/* ドラを計算する */
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == NoTile) continue;
		omote += gameStat->DoraFlag.Omote[gameStat->Player[targetPlayer].Hand[i].tile];
		if (uradoraEnabled) // 裏ドラ適用
			ura += gameStat->DoraFlag.Ura[gameStat->Player[targetPlayer].Hand[i].tile];
	}
	/* 鳴き面子のドラを数える */
	for (int i = 1; i < gameStat->Player[targetPlayer].MeldPointer; i++) {
		auto k = &gameStat->Player[targetPlayer].Meld[i];
		switch (k->mstat) {
		case meldSequenceExposedLower: case meldSequenceExposedMiddle: case meldSequenceExposedUpper: // 順子
			omote += gameStat->DoraFlag.Omote[k->tile] + gameStat->DoraFlag.Omote[k->tile + 1] +
				gameStat->DoraFlag.Omote[k->tile + 2];
			if (uradoraEnabled)
				ura += gameStat->DoraFlag.Ura[k->tile] + gameStat->DoraFlag.Ura[k->tile + 1] +
				gameStat->DoraFlag.Ura[k->tile + 2];
			break;
		case meldTripletExposedLeft: case meldTripletExposedCenter: case meldTripletExposedRight: // 刻子
			omote += gameStat->DoraFlag.Omote[k->tile] * 3;
			if (uradoraEnabled) ura += gameStat->DoraFlag.Ura[k->tile] * 3;
			break;
		case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight: // 槓子
		case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight: case meldQuadConcealed:
			omote += gameStat->DoraFlag.Omote[k->tile] * 4;
			if (uradoraEnabled) ura += gameStat->DoraFlag.Ura[k->tile] * 4;
			break;
		}
	}
	/* 赤ドラ・青ドラ */
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == NoTile) continue;
		else if (gameStat->Player[targetPlayer].Hand[i].tile >= TILE_NONFLOWER_MAX) continue;
		switch (gameStat->Player[targetPlayer].Hand[i].red) {
			case AkaDora: ++red; break;
			case AoDora: ++blue; break;
		}
	}
	for (int i = 1; i < gameStat->Player[targetPlayer].MeldPointer; i++) {
		auto k = &gameStat->Player[targetPlayer].Meld[i];
		for (int j = 0; j < (k->mstat >= meldQuadConcealed ? 4 : 3); j++) {
			switch (gameStat->Player[targetPlayer].Meld[i].red[j]) {
				case AkaDora: ++red; break;
				case AoDora: ++blue; break;
			}
		}
	}
	/* アリスドラ */
	if ((RuleData::chkRuleApplied("alice")) && (gameStat->Player[targetPlayer].MenzenFlag)) {
		// アリスありルールで門前でないと駄目
		auto AlicePointer = gameStat->DoraPointer;
		// 牌譜記録ルーチンはスレッドセーフじゃなかったはずなので別の場所でやる
		while (AlicePointer <= gameStat->TilePointer) {
			AlicePointer -= 2;
			if (analysis != nullptr) {
				if (analysis->TileCount[gameStat->Deck[AlicePointer].tile] > 0) ++alice;
				else break;
			} else {
				Int8ByTile tiles = countTilesInHand(gameStat, targetPlayer);
				if (tiles[gameStat->Deck[AlicePointer].tile] > 0) ++alice;
				else break;
			}
		}
	}
	/* 花牌・三麻のガリ */
	if (RuleData::chkRuleApplied("flower_tiles")) {
		if (chkGameType(gameStat, AllSanma)) {
			north = gameStat->Player[targetPlayer].NorthFlag;
			omote += north * (gameStat->DoraFlag.Omote[NorthWind] + 1);
			if (uradoraEnabled) ura += north * gameStat->DoraFlag.Ura[NorthWind];
			result->FlowerQuantity = north;
		} else {
			if (gameStat->Player[targetPlayer].FlowerFlag.Spring) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Summer) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Autumn) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Winter) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Plum) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Orchid) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Chrys) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Bamboo) ++flower;
			omote += flower * (gameStat->DoraFlag.Omote[Flower] + 1);
			if (uradoraEnabled) ura += flower * gameStat->DoraFlag.Ura[Flower];
			result->FlowerQuantity = flower;
		}
	}
	/* 計数結果を反映 */
	if (omote) {
		result->DoraQuantity += omote; result->BonusHan += omote;
		doraText(result, "ドラ", omote);
	}
	if (ura) {
		result->DoraQuantity += ura; result->BonusHan += ura; result->UraDoraQuantity = ura;
		doraText(result, "裏ドラ", ura);
	}
	if (red) {
		if (RuleData::chkRule("redtile_chip", "menzen_only") ||
			RuleData::chkRule("redtile_chip", "riichi_only") ||
			RuleData::chkRule("redtile_chip", "always")) {
			result->DoraQuantity += red; result->BonusHan += red;
		}
		result->AkaDoraQuantity = red;
		doraText(result, "赤ドラ", red);
	}
	if (blue) {
		result->AoDoraQuantity = blue;
		if (RuleData::chkRule("blue_tiles", "1han")) {
			result->DoraQuantity += blue; result->BonusHan += blue;
			doraText(result, "青ドラ", blue);
		} else if (RuleData::chkRule("blue_tiles", "2han")) {
			result->DoraQuantity += blue * 2; result->BonusHan += blue * 2;
			doraText(result, "青ドラ 2x", blue);
		} else if (RuleData::chkRule("blue_tiles", "-1han")) {
			result->DoraQuantity -= blue; result->BonusHan -= blue;
			doraText(result, "青ドラ -", blue);
		}
	}
	if (alice) {
		result->AliceDora = alice;
		doraText(result, "アリス祝儀", alice);
	}
}

/* ドラ計数 */
void yaku::yakuCalculator::CalculatorThread::countDora
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result)
{
	yaku::yakuCalculator::countDora(gameStat, analysis, result, analysis->player);
}

/* 翻を合計する */
void yaku::yakuCalculator::CalculatorThread::hanSummation(
	int& totalHan, int& totalSemiMangan, int& totalBonusHan, int& totalBonusSemiMangan,
	std::map<std::string, Yaku::YAKU_HAN> &yakuHan, std::vector<std::string> &yakuOrd, YAKUSTAT* const result)
{
	totalHan = totalSemiMangan = totalBonusHan = totalBonusSemiMangan = 0; // こっちで初期化してあげよう
	for (auto yNameIter = yakuOrd.begin(); yNameIter != yakuOrd.end(); yNameIter++) {
		std::string yName = *yNameIter;
		if (RuleData::chkRule("limitless", "yakuman_considered_13han")) { /* 青点ルールで役満を13飜扱いとする場合 */
			switch (yakuHan[yName].coreHan.getUnit()) {
				case yaku::yakuCalculator::Han: totalHan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::SemiMangan:
					switch (yakuHan[yName].coreHan.getHan()) {
						case 2: totalHan += 5; break;   case 3: totalHan += 6; break;
						case 4: totalHan += 8; break;   case 6: totalHan += 11; break;
						case 8: totalHan += 13; break;  case 16: totalHan += 26; break;
					}
					break;
				case yaku::yakuCalculator::Yakuman: totalHan += yakuHan[yName].coreHan.getHan() * 13; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が異常です");
			}
			switch (yakuHan[yName].bonusHan.getUnit()) {
				case yaku::yakuCalculator::Han: totalBonusHan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::SemiMangan:
					switch (yakuHan[yName].bonusHan.getHan()) {
						case 2: totalBonusHan += 5; break;   case 3: totalBonusHan += 6; break;
						case 4: totalBonusHan += 8; break;   case 6: totalBonusHan += 11; break;
						case 8: totalBonusHan += 13; break;  case 16: totalBonusHan += 26; break;
					}
					break;
				case yaku::yakuCalculator::Yakuman: totalBonusHan += yakuHan[yName].bonusHan.getHan() * 13; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が異常です");
			}
		} else { /* 通常の翻計算 */
			switch (yakuHan[yName].coreHan.getUnit()) {
				case yaku::yakuCalculator::Han: totalHan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::SemiMangan: totalSemiMangan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::Yakuman: totalSemiMangan += yakuHan[yName].coreHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が異常です");
			}
			switch (yakuHan[yName].bonusHan.getUnit()) {
				case yaku::yakuCalculator::Han: totalBonusHan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::SemiMangan: totalBonusSemiMangan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::Yakuman: totalBonusSemiMangan += yakuHan[yName].bonusHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が異常です");
			}
		}
		/* 役の名前を書き込む */
		if (result == nullptr) continue; // 役の名前の出力がいらないなら次へ
		if ((yakuHan[yName].coreHan.getUnit() != yakuHan[yName].bonusHan.getUnit()) &&
			(yakuHan[yName].coreHan.getHan() * yakuHan[yName].bonusHan.getHan() != 0))
		{ /* 単位が混在！ */
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "単位が混在しています");
		}
		else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::Han) || (yakuHan[yName].coreHan.getHan() == 0)) &&
			((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::Han) || (yakuHan[yName].bonusHan.getHan() == 0)))
		{ /* 普通の役の時 */
			strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, yName.c_str());
#ifdef WIN32
			strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, "\r\n");
#else
			strcat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, "\n");
#endif
			strcat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize,
				intstr(yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()).c_str());
#ifdef WIN32
			strcat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, "飜\r\n");
#else
			strcat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, "飜\n");
#endif
		}
		else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::SemiMangan) || (yakuHan[yName].coreHan.getHan() == 0)) &&
			((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::SemiMangan) || (yakuHan[yName].bonusHan.getHan() == 0)))
		{ /* 満貫 */
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
		else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::Yakuman) || (yakuHan[yName].coreHan.getHan() == 0)) &&
			((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::Yakuman) || (yakuHan[yName].bonusHan.getHan() == 0)))
		{ /* 役満 */
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
	}
}

/* 計算ルーチン */
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculate
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
	const ParseMode* const pMode, YAKUSTAT* const result)
{
	/* 面子解析処理 */
	if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1) {
		int NumOfMelds = 0;
		mentsuParser::makementsu(gameStat, analysis->player, *pMode, &NumOfMelds, analysis->MianziDat);
		if (NumOfMelds < SIZE_OF_MELD_BUFFER) { // 条件を満たしてないなら抜けます
			this->recordThreadFinish(); return S_OK;
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
		if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1) { // 七対子
			if (RuleData::chkRule("seven_pairs", "1han_50fu")) analysis->BasePoint = 50; // 1翻50符
			else analysis->BasePoint = 25; // 2翻25符
		}
		else if (analysis->shanten[ShantenAnalyzer::shantenOrphans] == -1) analysis->BasePoint = 30; // 国士は役満なのでこれは青天ルール用
		else if ((analysis->shanten[ShantenAnalyzer::shantenQuanbukao] == -1)&&(analysis->shanten[ShantenAnalyzer::shantenStellar] > -1)) {
			if (RuleData::chkRule("quanbukao", "3han_30fu"))
				analysis->BasePoint = 30;
			else if (RuleData::chkRule("quanbukao", "3han_40fu") || RuleData::chkRule("quanbukao", "4han_40fu"))
				analysis->BasePoint = 40;
		}
	}
	/* 役判定ループ */
	std::map<std::string, Yaku::YAKU_HAN> yakuHan; // 受け皿初期化
	std::set<std::string> suppression; // 無効化する役
	std::vector<std::string> yakuOrd; // 順序保存用
	std::for_each(YakuCatalog::Instantiate()->catalog.begin(), // 役カタログの最初から
		YakuCatalog::Instantiate()->catalog.end(), // カタログの末尾まで
		[&yakuHan, analysis, &suppression, &yakuOrd](Yaku& yaku) -> void { // 役ごとに判定処理
			if (yaku.checkYaku(analysis)) { // 成立条件を満たしていたら
				yakuHan[yaku.getName()] = yaku.getHan(analysis); // 飜数を記録
				yakuOrd.push_back(yaku.getName()); // 順序も記録しておく
				std::set<std::string> sup = yaku.getSuppression();
				suppression.insert(sup.begin(), sup.end()); // 下位役のリストを結合
			}
	});
	/* 実は成立していない役を除去する */
	for (auto k = yakuOrd.begin(); k != yakuOrd.end(); ) {
		if ((yakuHan[*k].coreHan.getHan() == 0) && (yakuHan[*k].bonusHan.getHan() == 0)) // 実は成立してない役
			k = yakuOrd.erase(k); // 数に数えない
		else ++k;
	}
	/* 後回しで判定する役 */
	checkPostponedYaku(gameStat, analysis, result, yakuHan, suppression, yakuOrd);
	/* 下位役を除去する */
	std::for_each(suppression.begin(), suppression.end(), [&yakuOrd](std::string yaku) {
		for (auto k = yakuOrd.begin(); k != yakuOrd.end(); ) {
			if (*k == yaku) k = yakuOrd.erase(k);
			else ++k;
		}
	});
	/* 翻を合計する */
	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	hanSummation(totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan, yakuHan, yakuOrd, result);
	/* 飜数を計算した結果を書き込む */
	result->CoreHan = totalHan; result->CoreSemiMangan = totalSemiMangan;
	result->BonusHan = totalBonusHan; result->BonusSemiMangan = totalBonusSemiMangan;
	if (analysis->BasePoint == 25) result->BasePoints = 25; // チートイの25符はそのまま
	else result->BasePoints = analysis->BasePoint + ((10 - analysis->BasePoint % 10) % 10); // 符ハネして反映
	/* ドラの数を数える */
	countDora(gameStat, analysis, result);
	/* 簡略ルール(全部30符)の場合 */
	if (RuleData::chkRule("simplified_scoring", "simplified")) {
		trace("簡略計算ルールのため30符として扱います。");
		result->BasePoints = 30;
	}
	/* 点数を計算する */
	calculateScore(result);
	/* 切り上げ満貫の処理 */
	if ((RuleData::chkRuleApplied("round_to_mangan")) && // 切り上げ満貫ルールがONで
		(!RuleData::chkRuleApplied("limitless")) && // 青天井ルールでない場合
		(result->AgariPoints == LargeNum::fromInt(1920))) // 子の7700・親の11600なら
			result->AgariPoints = LargeNum::fromInt(2000); // 満貫にする
	/* 終了処理 */
	recordThreadFinish(); // スレッドが終わったことを記録
	return S_OK;
}

/* コンストラクタとデストラクタ */
yaku::yakuCalculator::CalculatorThread::CalculatorThread() {
	InitializeCriticalSection(&cs); finishedThreads = startedThreads = 0;
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
	analysis.MachiInfo = chkFuriten(gameStat, targetPlayer);
	analysis.GameStat = gameStat;
	analysis.PlayerStat = &(gameStat->Player[targetPlayer]);
	analysis.TsumoHai = &(gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1]);
	analysis.MenzenFlag = &(gameStat->Player[targetPlayer].MenzenFlag);
	analysis.TsumoAgariFlag = &(gameStat->TsumoAgariFlag);
	// 計算ルーチンに渡すパラメータの準備
	CalculatorParam* calcprm = new CalculatorParam; memset(calcprm, 0, sizeof(CalculatorParam));
	calcprm->gameStat = gameStat; calcprm->instance = calculator;
	memcpy(&calcprm->analysis, &analysis, sizeof(MENTSU_ANALYSIS));
	YAKUSTAT::Init(&calcprm->result);
	// 計算を実行
	DWORD ThreadID;
	HANDLE Thread = CreateThread(nullptr, 0, CalculatorThread::calculator, (LPVOID)calcprm, 0, &ThreadID);
	calculator->sync(1); // 同期(簡略な実装)
	// 高点法の処理
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
	assert(calculator->numOfStartedThreads() == 1);
	assert(calculator->numOfFinishedThreads() == 1);
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
	analysis.SeenTiles = countseentiles(gameStat);
	analysis.MachiInfo = chkFuriten(gameStat, targetPlayer);
	analysis.GameStat = gameStat;
	analysis.PlayerStat = &(gameStat->Player[targetPlayer]);
	analysis.TsumoHai = &(gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1]);
	analysis.MenzenFlag = &(gameStat->Player[targetPlayer].MenzenFlag);
	analysis.TsumoAgariFlag = &(gameStat->TsumoAgariFlag);
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
		while (calculator->numOfFinishedThreads() - calculator->numOfStartedThreads() >= CalculatorThread::threadLimit)
			Sleep(0); // スレッド数制限のチェック
		Thread[i] = CreateThread(nullptr, 0, CalculatorThread::calculator, (LPVOID)(&(calcprm[i])), 0, &(ThreadID[i]));
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
	assert(calculator->numOfFinishedThreads() == 156);
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
		shanten[i] = ShantenAnalyzer::calcShanten(gameStat, targetPlayer, (ShantenAnalyzer::shantenType)i);
	// 和了ってるか判定(和了ってなかった場合十三不塔か判定する)
	if (shanten[ShantenAnalyzer::shantenAll] > -1) {
		/* 十三不塔 */
		if (gameStat->Player[targetPlayer].FirstDrawFlag) { // 鳴きがなくて一巡目の時だけ判定する
			if (chkShisanBuDa(gameStat, targetPlayer)) { // 十三不塔になってる
				trace("十三不塔です！！");
				yakuInfo.isValid = true; yakuInfo.BasePoints = 30;
				if (RuleData::chkRule("limitless", "yakuman_considered_13han")) 
					yakuInfo.CoreHan = (RuleData::chkRule("shiisan_puutaa", "mangan")) ? 5 : 13;
				else yakuInfo.CoreSemiMangan = (RuleData::chkRule("shiisan_puutaa", "mangan")) ? 2 : 8;
				calculateScore(&yakuInfo);
				strcpy_s((RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList,
					YAKUSTAT::nameBufSize,
#ifdef WIN32
					"十三不搭\r\n");
#else
					"十三不搭\n");
#endif
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					strcpy_s(yakuInfo.yakuValList, YAKUSTAT::nameBufSize,
#ifdef WIN32
						(RuleData::chkRule("shiisan_puutaa", "mangan")) ? "５飜\r\n" : "13飜\r\n");
#else
						(RuleData::chkRule("shiisan_puutaa", "mangan")) ? "５飜\n" : "13飜\n");
#endif
				countDora(gameStat, nullptr, &yakuInfo, targetPlayer); // ドラは数えてあげましょうね
			}
			/* 十四不塔 */
			else if (chkShisiBuDa(gameStat, targetPlayer)) { // 十四不塔になってる
				trace("十四不塔です！！");
				yakuInfo.isValid = true; yakuInfo.BasePoints = 30;
				if (RuleData::chkRule("limitless", "yakuman_considered_13han")) yakuInfo.CoreHan = 13;
				else yakuInfo.CoreSemiMangan = 8;
				calculateScore(&yakuInfo);
				strcpy_s((RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList,
					YAKUSTAT::nameBufSize,
#ifdef WIN32
					"十三無靠\r\n");
#else
					"十三無靠\n");
#endif
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					strcpy_s(yakuInfo.yakuValList, YAKUSTAT::nameBufSize,
#ifdef WIN32
						"13飜\r\n");
#else
						"13飜\n");
#endif
				countDora(gameStat, nullptr, &yakuInfo, targetPlayer); // ドラを数えるのです
			}
		}
		trace("和了っていないので抜けます");
		return yakuInfo;
	}
	// 和了っているなら
	if (shanten[ShantenAnalyzer::shantenRegular] == -1) // 一般形の和了
		analysisLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	else // 七対子、国士無双、その他特殊な和了
		analysisNonLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	yakuInfo.isValid = true; // 有効であることをマーク（縛りを満たしているかはチェックしていない）
	/* アリスドラの牌譜記録 */
	if ((RuleData::chkRuleApplied("alice")) && (gameStat->Player[targetPlayer].MenzenFlag)) {
		uint16_t AlicePointer = gameStat->DoraPointer;
		auto tiles = countTilesInHand(gameStat, targetPlayer);
		haifu::haifuresetalicedora();
		while (1) {
			AlicePointer -= 2;
			if (AlicePointer <= gameStat->TilePointer) {
				AlicePointer += 2; break;
			}
			haifu::haifurecalicedora(gameStat->Deck[AlicePointer].tile);
			if (tiles[gameStat->Deck[AlicePointer].tile] <= 0) break;
		}
	}
	/* おわり */
	return yakuInfo;
}
__declspec(dllexport) void yaku::yakuCalculator::countyaku(const GameTable* const gameStat,
	YAKUSTAT* const yakuInfo, int targetPlayer)
{
	*yakuInfo = countyaku(gameStat, (PLAYER_ID)targetPlayer);
}

/* 縛りを満たしているか調べる */
bool yaku::yakuCalculator::checkShibari(const GameTable* const gameStat, const YAKUSTAT* const yakuStat) {
	if ((yakuStat->CoreHan >= 2) || (yakuStat->CoreSemiMangan >= 1))
		return true; // 2翻以上あったら縛りを満たす
	else if ((yakuStat->CoreHan == 1) && (RuleData::getRule("ryanshiba") == 0))
		return true; // リャンシバなしのルールの場合で1翻
	else if ((yakuStat->CoreHan == 1) && (gameStat->Honba < 5) && (RuleData::getRule("ryanshiba") == 1))
		return true; // 5本場からリャンシバだが4本場までで1翻
	else if ((yakuStat->CoreHan == 1) && (gameStat->Honba < 4) && (RuleData::getRule("ryanshiba") == 2))
		return true; // 4本場からリャンシバだが3本場までで1翻
	else return false; // 縛りを満たしていない場合
}
__declspec(dllexport) int yaku::yakuCalculator::check_shibari(const GameTable* const gameStat, const YAKUSTAT* const yakuStat) {
	return checkShibari(gameStat, yakuStat) ? 1 : 0;
}
