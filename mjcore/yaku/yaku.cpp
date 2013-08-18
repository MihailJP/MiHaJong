#include "yaku.h"

#include <string>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <array>
#include <vector>
#include <cassert>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else /*_WIN32*/
#include <unistd.h>
#endif /*_WIN32*/
#include "../largenum.h"
#include "../except.h"
#include "../logging.h"
#include "../../common/strcode.h"
#include "../func.h"
#include "../haifu.h"
#include "yvalue.h"
#include "../ruletbl.h"

// 計算を実行(マルチスレッドで……大丈夫かなぁ)
#ifdef _WIN32
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculator(LPVOID lpParam)
#else /*_WIN32*/
void* yaku::yakuCalculator::CalculatorThread::calculator(void* lpParam)
#endif /*_WIN32*/
{
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
#ifdef _WIN32
	while (this->startedThreads < threads) Sleep(0); // 規定数のスレッドが開始するのを待ってから
	while (this->finishedThreads < threads) Sleep(0); // 終了するのを待つ
#else /*_WIN32*/
	while (this->startedThreads < threads) usleep(100); // 規定数のスレッドが開始するのを待ってから
	while (this->finishedThreads < threads) usleep(100); // 終了するのを待つ
#endif /*_WIN32*/
}

void yaku::yakuCalculator::CalculatorThread::recordThreadStart() {
	cs.syncDo<void>([this]() -> void {
		++startedThreads; // 開始したスレッド数をインクリメント
	});
}
void yaku::yakuCalculator::CalculatorThread::recordThreadFinish() {
	cs.syncDo<void>([this]() -> void {
		++finishedThreads; // 終了したスレッド数をインクリメント
	});
}

/* 翻を計算する */
#ifndef GUOBIAO
void yaku::yakuCalculator::doubling(yaku::YAKUSTAT* const yStat) {
	int totalHan = yStat->CoreHan + yStat->BonusHan; // 合計翻
	yStat->AgariPoints = (LNum)yStat->BasePoints; // 原点
	if (totalHan >= -2) {
		for (int i = 0; i < (totalHan + 2); i++) yStat->AgariPoints *= 2;
	} else {
		for (int i = 0; i < (totalHan + 2); i++) yStat->AgariPoints /= 2;
	}
	if (yStat->AgariPoints <= (LNum)0) yStat->AgariPoints = (LNum)1; // 最低1点にはなるようにする
}
#endif /* GUOBIAO */

/* 符と翻から点数を計算する */
void yaku::yakuCalculator::calculateScore(yaku::YAKUSTAT* const yStat) {
#ifdef GUOBIAO
	/* 中国ルールでは単に足し算するだけ、複雑な計算は不要 */
	yStat->AgariPoints = (LNum)yStat->CoreHan;
#else /* GUOBIAO */
	/* 縛りを満たしてなかったら0を返す
	   点数が0ならコア部分で錯和と判断される……はず */
	if ((yStat->CoreHan <= 0)&&(yStat->CoreSemiMangan <= 0)) {
		yStat->AgariPoints = (LNum)0;
		return;
	}

	int totalHan = yStat->CoreHan + yStat->BonusHan; // 合計翻
	int totalSemiMangan = yStat->CoreSemiMangan + yStat->BonusSemiMangan; // 満貫の半分単位

	if (!RuleData::chkRuleApplied("limitless")) { // 通常ルールの場合
		if ((totalHan < 6) && (totalSemiMangan < 3)) { // 満貫以下
			doubling(yStat); // 計算を実行
			if (yStat->AgariPoints > (LNum)2000) yStat->AgariPoints = (LNum)2000; // 満貫
			if (totalSemiMangan == 2) yStat->AgariPoints = (LNum)2000; // 固定満貫の場合
		}
		else if ((totalHan < 8) && (totalSemiMangan < 4)) yStat->AgariPoints = (LNum)3000; // 跳満
		else if (((totalHan < 10) || ((totalHan == 10) && (RuleData::chkRule("sanbaiman_border", "11han_or_more")))) &&
			(totalSemiMangan < 6)) yStat->AgariPoints = (LNum)4000; // 倍満
		else if (((totalHan < 12) || ((totalHan == 12) && (RuleData::chkRule("kazoe_border", "13han_or_more")))) &&
			(totalSemiMangan < 8)) yStat->AgariPoints = (LNum)6000; // 三倍満
		else if ((totalSemiMangan < 8) && (RuleData::chkRule("kazoe_border", "no")))
			yStat->AgariPoints = (LNum)6000; // 三倍満
		else if (totalSemiMangan < 16) yStat->AgariPoints = (LNum)8000; // 役満 or 数え
		else yStat->AgariPoints = (LNum)(8000 * (totalSemiMangan / 8)); // ダブル役満以上
	} else { // 青天井ルールの場合
		if (totalSemiMangan >= 8) yStat->AgariPoints = (LNum)2500000; // 役満を固定点にするルール
		else if (totalSemiMangan >= 6) yStat->AgariPoints = (LNum)1875000;
		else if (totalSemiMangan >= 4) yStat->AgariPoints = (LNum)1250000;
		else if (totalSemiMangan >= 3) yStat->AgariPoints = (LNum)937500;
		else if (totalSemiMangan >= 2) yStat->AgariPoints = (LNum)625000;
		else doubling(yStat); // 計算する
	}

	{ // トレース用
		CodeConv::tostringstream o;
		o << _T("計算結果は [");
		for (int i = DIGIT_GROUPS - 1; i >= 0; i--)
			o << std::setw(4) << std::dec << std::setfill(_T('0')) << yStat->AgariPoints.digitGroup[i] / 10000 <<
			_T(" ") << std::setw(4) << std::dec << std::setfill(_T('0')) << yStat->AgariPoints.digitGroup[i] % 10000 << (i ? _T(" ") : _T(""));
		o << _T("]");
		trace(o.str().c_str());
	}
#endif /* GUOBIAO */
}

/* 符を計算する */
#ifndef GUOBIAO
void yaku::yakuCalculator::CalculatorThread::calcbasepoints
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis)
{
	trace(_T("符計算の処理に入ります。"));
	int fu = 20; // 副底２０符

	/* 雀頭加符(役牌のみ２符) */
	switch (analysis->MianziDat[0].tile) { /* 風牌は条件によって役牌 */
	case EastWind: case SouthWind: case WestWind: case NorthWind:
		if (analysis->MianziDat[0].tile ==
			Wind2Tile((uint8_t)(gameStat->GameRound / 4))) // 場風牌
			fu += 2;
		if (analysis->MianziDat[0].tile ==
			Wind2Tile(gameStat->playerwind(analysis->player))) // 自風牌
			fu += 2;
		if ((RuleData::chkRuleApplied("kaimenkaze")) && (analysis->MianziDat[0].tile == // 開門風牌
			Wind2Tile(gameStat->playerwind(gameStat->WaremePlayer))))
			fu += 2;
		if ((RuleData::chkRuleApplied("urakaze")) && (analysis->MianziDat[0].tile == // 裏風牌
			Wind2Tile(gameStat->playerwind(analysis->player + 2))))
			fu += 2;
		if ((!RuleData::chkRuleApplied("double_yaku_wind_pair")) && (fu > 22)) fu = 22; // ダブ風雀頭を2符と見なすルールの場合
		break;
	case WhiteDragon: case GreenDragon: case RedDragon: /* 三元牌は常に役牌 */
		fu += 2;
		break;
	}

	/* 面子加符 */
	for (int i = 1; i < SizeOfMeldBuffer; i++) {
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
	const TileCode* tsumoTile = &(gameStat->Player[analysis->player].Tsumohai().tile); // shorthand
	if (analysis->MianziDat[0].tile == *tsumoTile) analysis->Machi = machiTanki; // 単騎待ち
	for (int i = 1; i < SizeOfMeldBuffer; i++) { // 待ちの種類を調べる……
		switch (analysis->MianziDat[i].mstat) {
		case meldSequenceConcealed: case meldSequenceExposedLower:
		case meldSequenceExposedMiddle: case meldSequenceExposedUpper: /* 順子 */
			if (analysis->MianziDat[i].tile == ((*tsumoTile) - 1)) analysis->Machi = machiKanchan;
			if (analysis->MianziDat[i].tile == *tsumoTile) {
				if (analysis->MianziDat[i].tile % TileSuitStep == 7) analysis->Machi = machiPenchan; // 辺張待ち
				else {analysis->Machi = machiRyanmen; LiangMianFlag = true;} // 両面待ち
			}
			if (analysis->MianziDat[i].tile == ((*tsumoTile) - 2)) {
				if (analysis->MianziDat[i].tile % TileSuitStep == 1) analysis->Machi = machiPenchan; // 辺張待ち
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
	CodeConv::tostringstream o; o << _T("この手牌は [") << fu << _T("] 符です。"); trace(o.str().c_str());
	analysis->BasePoint = (uint8_t)fu;
}
#endif /* GUOBIAO */

/* ドラ計数 */
void yaku::yakuCalculator::countDora
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
	YAKUSTAT* const result, PlayerID targetPlayer)
{
#ifndef GUOBIAO
	auto doraText =
		[](YAKUSTAT* const result, LPCTSTR const label, int quantity) {
#if defined(_MSC_VER)
			_tcscat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, label);
			_tcscat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, _T(" "));
			_tcscat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, intstr(quantity).c_str());
			_tcscat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, _T("\r\n"));
			_tcscat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, _T("\r\n"));
#elif defined(_WIN32)
			_tcsncat(result->yakuNameList, label, yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, _T(" "), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, intstr(quantity).c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, _T("\r\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuValList, _T("\r\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
#else
			_tcsncat(result->yakuNameList, label, yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, _T(" "), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, intstr(quantity).c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, _T("\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuValList, _T("\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
#endif
		};
	const bool uradoraEnabled = ((RuleData::chkRuleApplied("uradora")) && // 裏ドラありのルールで、
		(gameStat->Player[targetPlayer].MenzenFlag) && // 門前であり、
		(gameStat->Player[targetPlayer].RichiFlag.RichiFlag)); // 立直をかけているなら
	int omote = 0; int ura = 0; int red = 0; int blue = 0; int alice = 0;
	int flower = 0; int north = 0;
	/* ドラを計算する */
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == NoTile) continue;
		omote += gameStat->DoraFlag.Omote[gameStat->Player[targetPlayer].Hand[i].tile];
		if (uradoraEnabled) // 裏ドラ適用
			ura += gameStat->DoraFlag.Ura[gameStat->Player[targetPlayer].Hand[i].tile];
	}
	/* 鳴き面子のドラを数える */
	for (int i = 1; i <= gameStat->Player[targetPlayer].MeldPointer; i++) {
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
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == NoTile) continue;
		else if (gameStat->Player[targetPlayer].Hand[i].tile >= TileNonflowerMax) continue;
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
		while (AlicePointer >= gameStat->TilePointer) {
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
#endif /* GUOBIAO */
	/* 花牌・三麻のガリ */
#ifdef GUOBIAO
	int omote = 0, flower = 0;
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("flower_tiles")) {
		if (gameStat->chkGameType(AllSanma)) {
			north = gameStat->Player[targetPlayer].NorthFlag;
			omote += north * (gameStat->DoraFlag.Omote[NorthWind] + 1);
			if (uradoraEnabled) ura += north * gameStat->DoraFlag.Ura[NorthWind];
			result->FlowerQuantity = north;
		} else {
#endif /* GUOBIAO */
			if (gameStat->Player[targetPlayer].FlowerFlag.Spring) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Summer) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Autumn) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Winter) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Plum) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Orchid) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Chrys) ++flower;
			if (gameStat->Player[targetPlayer].FlowerFlag.Bamboo) ++flower;
			omote += flower * (gameStat->DoraFlag.Omote[Flower] + 1);
#ifndef GUOBIAO
			if (uradoraEnabled) ura += flower * gameStat->DoraFlag.Ura[Flower];
#endif /* GUOBIAO */
			result->FlowerQuantity = flower;
#ifndef GUOBIAO
		}
	}
	/* 計数結果を反映 */
	if (omote) {
		result->DoraQuantity += omote; result->BonusHan += omote;
		doraText(result, _T("ドラ"), omote);
	}
	if (ura) {
		result->DoraQuantity += ura; result->BonusHan += ura; result->UraDoraQuantity = ura;
		doraText(result, _T("裏ドラ"), ura);
	}
	if (red) {
		if (RuleData::chkRule("redtile_chip", "menzen_only") ||
			RuleData::chkRule("redtile_chip", "riichi_only") ||
			RuleData::chkRule("redtile_chip", "always")) {
			result->DoraQuantity += red; result->BonusHan += red;
		}
		result->AkaDoraQuantity = red;
		doraText(result, _T("赤ドラ"), red);
	}
	if (blue) {
		result->AoDoraQuantity = blue;
		if (RuleData::chkRule("blue_tiles", "1han")) {
			result->DoraQuantity += blue; result->BonusHan += blue;
			doraText(result, _T("青ドラ"), blue);
		} else if (RuleData::chkRule("blue_tiles", "2han")) {
			result->DoraQuantity += blue * 2; result->BonusHan += blue * 2;
			doraText(result, _T("青ドラ 2x"), blue);
		} else if (RuleData::chkRule("blue_tiles", "-1han")) {
			result->DoraQuantity -= blue; result->BonusHan -= blue;
			doraText(result, _T("青ドラ -"), blue);
		}
	}
	if (alice) {
		result->AliceDora = alice;
		doraText(result, _T("アリス祝儀"), alice);
	}
#endif /* GUOBIAO */
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
	std::map<CodeConv::tstring, Yaku::YAKU_HAN> &yakuHan, std::vector<CodeConv::tstring> &yakuOrd, YAKUSTAT* const result)
{
	totalHan = totalSemiMangan = totalBonusHan = totalBonusSemiMangan = 0; // こっちで初期化してあげよう
	for (auto yNameIter = yakuOrd.begin(); yNameIter != yakuOrd.end(); yNameIter++) {
		CodeConv::tstring yName = *yNameIter;
#ifdef GUOBIAO
		totalHan += yakuHan[yName].coreHan.getHan();
#else /* GUOBIAO */
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
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("単位が異常です"));
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
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("単位が異常です"));
			}
		} else { /* 通常の翻計算 */
			switch (yakuHan[yName].coreHan.getUnit()) {
				case yaku::yakuCalculator::Han: totalHan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::SemiMangan: totalSemiMangan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::Yakuman: totalSemiMangan += yakuHan[yName].coreHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("単位が異常です"));
			}
			switch (yakuHan[yName].bonusHan.getUnit()) {
				case yaku::yakuCalculator::Han: totalBonusHan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::SemiMangan: totalBonusSemiMangan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::Yakuman: totalBonusSemiMangan += yakuHan[yName].bonusHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("単位が異常です"));
			}
		}
#endif /* GUOBIAO */
		/* 役の名前を書き込む */
		if (result == nullptr) continue; // 役の名前の出力がいらないなら次へ
#ifndef GUOBIAO
		if ((yakuHan[yName].coreHan.getUnit() != yakuHan[yName].bonusHan.getUnit()) &&
			(yakuHan[yName].coreHan.getHan() * yakuHan[yName].bonusHan.getHan() != 0))
		{ /* 単位が混在！ */
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("単位が混在しています"));
		}
		else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::Han) || (yakuHan[yName].coreHan.getHan() == 0)) &&
			((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::Han) || (yakuHan[yName].bonusHan.getHan() == 0)))
		{ /* 普通の役の時 */
#endif /* GUOBIAO */
#if defined(_MSC_VER)
			_tcscat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, yName.c_str());
			_tcscat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, _T("\r\n"));
			_tcscat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize,
				intstr(yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()).c_str());
			_tcscat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, _T("飜\r\n"));
#elif defined(_WIN32)
			_tcsncat(result->yakuNameList, yName.c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, _T("\r\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuValList,
				intstr(yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()).c_str(),
					yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
			_tcsncat(result->yakuValList, _T("飜\r\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
#else
			_tcsncat(result->yakuNameList, yName.c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, _T("\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuValList,
				intstr(yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()).c_str(),
					yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
			_tcsncat(result->yakuValList, _T("飜\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
#endif
#ifndef GUOBIAO
		}
		else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::SemiMangan) || (yakuHan[yName].coreHan.getHan() == 0)) &&
			((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::SemiMangan) || (yakuHan[yName].bonusHan.getHan() == 0)))
		{ /* 満貫 */
#if defined(_MSC_VER)
			_tcscat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, yName.c_str());
			_tcscat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, _T("\r\n"));
			TCHAR hstr[16]; _stprintf_s(hstr, 16, _T("%d\r\n"),
				(int)((yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()) * yaku::YAKUSTAT::SemiMangan));
			_tcscat_s(result->yakumanValList, yaku::YAKUSTAT::nameBufSize, hstr);
#elif defined(_WIN32)
			_tcsncat(result->yakumanNameList, yName.c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanNameList));
			_tcsncat(result->yakumanNameList, _T("\r\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanNameList));
			TCHAR hstr[16]; _sntprintf(hstr, 15, _T("%d\r\n"),
				(int)((yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()) * yaku::YAKUSTAT::SemiMangan));
			_tcsncat(result->yakumanValList, hstr, yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanValList));
#else
			_tcsncat(result->yakumanNameList, yName.c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanNameList));
			_tcsncat(result->yakumanNameList, _T("\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanNameList));
			TCHAR hstr[16]; _sntprintf(hstr, 15, _T("%d\n"),
				(int)((yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()) * yaku::YAKUSTAT::SemiMangan));
			_tcsncat(result->yakumanValList, hstr, yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanValList));
#endif
		}
		else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::Yakuman) || (yakuHan[yName].coreHan.getHan() == 0)) &&
			((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::Yakuman) || (yakuHan[yName].bonusHan.getHan() == 0)))
		{ /* 役満 */
#if defined(_MSC_VER)
			_tcscat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, yName.c_str());
			_tcscat_s(result->yakumanNameList, yaku::YAKUSTAT::nameBufSize, _T("\r\n"));
			TCHAR hstr[16]; _stprintf_s(hstr, 16, _T("%d\r\n"),
				(int)((yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()) * yaku::YAKUSTAT::SemiMangan * 8));
			_tcscat_s(result->yakumanValList, yaku::YAKUSTAT::nameBufSize, hstr);
#elif defined(_WIN32)
			_tcsncat(result->yakumanNameList, yName.c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanNameList));
			_tcsncat(result->yakumanNameList, _T("\r\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanNameList));
			TCHAR hstr[16]; _sntprintf(hstr, 15, _T("%d\r\n"),
				(int)((yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()) * yaku::YAKUSTAT::SemiMangan * 8));
			_tcsncat(result->yakumanValList, hstr, yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanValList));
#else
			_tcsncat(result->yakumanNameList, yName.c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanNameList));
			_tcsncat(result->yakumanNameList, _T("\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanNameList));
			TCHAR hstr[16]; _sntprintf(hstr, 15, _T("%d\n"),
				(int)((yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()) * yaku::YAKUSTAT::SemiMangan * 8));
			_tcsncat(result->yakumanValList, hstr, yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakumanValList));
#endif
		}
#endif /* GUOBIAO */
	}
}

/* 計算ルーチン */
#ifdef _WIN32
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculate
#else /*_WIN32*/
void* yaku::yakuCalculator::CalculatorThread::calculate
#endif /*_WIN32*/
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
	const ParseMode* const pMode, YAKUSTAT* const result)
{
	/* 面子解析処理 */
	if (analysis->shanten[shantenRegular] == -1) {
		int NumOfMelds = 0;
		mentsuParser::makementsu(gameStat, analysis->player, *pMode, &NumOfMelds, analysis->MianziDat);
		if (NumOfMelds < SizeOfMeldBuffer) { // 条件を満たしてないなら抜けます
			this->recordThreadFinish();
#ifdef _WIN32
			return S_OK;
#else /*_WIN32*/
			return nullptr;
#endif /*_WIN32*/
		}
#ifndef GUOBIAO
		calcbasepoints(gameStat, analysis); // 符を計算する
#endif /* GUOBIAO */
		analysis->DuiziCount = countingFacility::countDuiz(analysis->MianziDat);
		analysis->KeziCount = countingFacility::countKez(analysis->MianziDat, &analysis->TotalKezi);
		analysis->AnKeziCount = countingFacility::countAnKez(analysis->MianziDat, &analysis->TotalAnKezi);
		analysis->ShunziCount = countingFacility::countShunz(analysis->MianziDat, &analysis->TotalShunzi);
		analysis->AnShunziCount = countingFacility::countAnShunz(analysis->MianziDat, &analysis->TotalAnShunzi);
		analysis->KangziCount = countingFacility::countKangz(analysis->MianziDat, &analysis->TotalKangzi);
		analysis->AnKangziCount = countingFacility::countAnKangz(analysis->MianziDat, &analysis->TotalAnKangzi);
		analysis->KaKangziCount = countingFacility::countKaKangz(analysis->MianziDat, &analysis->TotalKaKangzi);
#ifndef GUOBIAO
	} else {
		if (analysis->shanten[shantenPairs] == -1) { // 七対子
			if (RuleData::chkRule("seven_pairs", "1han_50fu")) analysis->BasePoint = 50; // 1翻50符
			else analysis->BasePoint = 25; // 2翻25符
		}
		else if (analysis->shanten[shantenOrphans] == -1) analysis->BasePoint = 30; // 国士は役満なのでこれは青天ルール用
		else if ((analysis->shanten[shantenQuanbukao] == -1)&&(analysis->shanten[shantenStellar] > -1)) {
			if (RuleData::chkRule("quanbukao", "3han_30fu"))
				analysis->BasePoint = 30;
			else if (RuleData::chkRule("quanbukao", "3han_40fu") || RuleData::chkRule("quanbukao", "4han_40fu"))
				analysis->BasePoint = 40;
#endif /* GUOBIAO */
		}
		else if (analysis->shanten[shantenZuhelong] == -1) { // 組合龍
			mentsuParser::makementsu(gameStat, analysis->player, *pMode, nullptr, analysis->MianziDat);
#ifndef GUOBIAO
			calcbasepoints(gameStat, analysis); // 符を計算する
		}
		else analysis->BasePoint = 30;
#endif /* GUOBIAO */
	}
	/* 役判定ループ */
	std::map<CodeConv::tstring, Yaku::YAKU_HAN> yakuHan; // 受け皿初期化
	std::set<CodeConv::tstring> suppression; // 無効化する役
	std::vector<CodeConv::tstring> yakuOrd; // 順序保存用
	std::for_each(YakuCatalog::Instantiate()->catalog.begin(), // 役カタログの最初から
		YakuCatalog::Instantiate()->catalog.end(), // カタログの末尾まで
		[&yakuHan, analysis, &suppression, &yakuOrd](Yaku& yaku) -> void { // 役ごとに判定処理
			//trace(yaku.getName().c_str());
			if (yaku.checkYaku(analysis)) { // 成立条件を満たしていたら
				//trace(_T("...は、成立しています。"));
				yakuHan[yaku.getName()] = yaku.getHan(analysis); // 飜数を記録
				yakuOrd.push_back(yaku.getName()); // 順序も記録しておく
				std::set<CodeConv::tstring> sup = yaku.getSuppression();
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
	std::for_each(suppression.begin(), suppression.end(), [&yakuOrd](CodeConv::tstring yaku) {
		for (auto k = yakuOrd.begin(); k != yakuOrd.end(); ) {
			if (*k == yaku) k = yakuOrd.erase(k);
			else ++k;
		}
	});
	/* 翻を合計する */
	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	hanSummation(totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan, yakuHan, yakuOrd, result);
	/* 飜数を計算した結果を書き込む */
	result->CoreHan = totalHan;
#ifndef GUOBIAO
	result->CoreSemiMangan = totalSemiMangan;
	result->BonusHan = totalBonusHan; result->BonusSemiMangan = totalBonusSemiMangan;
	if (analysis->BasePoint == 25) result->BasePoints = 25; // チートイの25符はそのまま
	else result->BasePoints = analysis->BasePoint + ((10 - analysis->BasePoint % 10) % 10); // 符ハネして反映
#endif /* GUOBIAO */
	/* ドラの数を数える */
	countDora(gameStat, analysis, result);
#ifndef GUOBIAO
	/* 簡略ルール(全部30符)の場合 */
	if (RuleData::chkRule("simplified_scoring", "simplified")) {
		trace(_T("簡略計算ルールのため30符として扱います。"));
		result->BasePoints = 30;
	}
#endif /* GUOBIAO */
	/* 点数を計算する */
	calculateScore(result);
#ifndef GUOBIAO
	/* 切り上げ満貫の処理 */
	if ((RuleData::chkRuleApplied("round_to_mangan")) && // 切り上げ満貫ルールがONで
		(!RuleData::chkRuleApplied("limitless")) && // 青天井ルールでない場合
		(result->AgariPoints == (LNum)1920)) // 子の7700・親の11600なら
			result->AgariPoints = (LNum)2000; // 満貫にする
#endif /* GUOBIAO */
	/* 終了処理 */
	recordThreadFinish(); // スレッドが終わったことを記録
#ifdef _WIN32
	return S_OK;
#else /*_WIN32*/
	return nullptr;
#endif /*_WIN32*/
}

/* コンストラクタとデストラクタ */
yaku::yakuCalculator::CalculatorThread::CalculatorThread() {
	finishedThreads = startedThreads = 0;
}
yaku::yakuCalculator::CalculatorThread::~CalculatorThread() {
	/* 終了するときは必ず同期してから行うこと！！ */
}
		
/* 引数の準備とか */
void yaku::yakuCalculator::analysisNonLoop(const GameTable* const gameStat, PlayerID targetPlayer,
	Shanten* const shanten, YAKUSTAT* const yakuInfo)
{
	CalculatorThread* calculator = new CalculatorThread; // インスタンスの準備
	// 変数を用意
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(Shanten[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
	analysis.TileCount = countTilesInHand(gameStat, targetPlayer);
	analysis.SeenTiles = countseentiles(gameStat);
	analysis.MachiInfo = chkFuriten(gameStat, targetPlayer);
	analysis.GameStat = gameStat;
	analysis.PlayerStat = &(gameStat->Player[targetPlayer]);
	analysis.TsumoHai = &(gameStat->Player[targetPlayer].Tsumohai());
	analysis.MenzenFlag = &(gameStat->Player[targetPlayer].MenzenFlag);
	analysis.TsumoAgariFlag = &(gameStat->TsumoAgariFlag);
	// 計算ルーチンに渡すパラメータの準備
	CalculatorParam* calcprm = new CalculatorParam; memset(calcprm, 0, sizeof(CalculatorParam));
	calcprm->gameStat = gameStat; calcprm->instance = calculator;
	memcpy(&calcprm->analysis, &analysis, sizeof(MENTSU_ANALYSIS));
	YAKUSTAT::Init(&calcprm->result);
	// 計算を実行
#ifdef _WIN32
	DWORD ThreadID;
	HANDLE Thread = CreateThread(nullptr, 0, CalculatorThread::calculator, (LPVOID)calcprm, 0, &ThreadID);
#else /*_WIN32*/
	pthread_t hThread;
	pthread_create(&hThread, nullptr, CalculatorThread::calculator, (void*)calcprm);
	pthread_detach(hThread);
#endif /*_WIN32*/
	calculator->sync(1); // 同期(簡略な実装)
	// 高点法の処理
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
	assert(calculator->numOfStartedThreads() == 1);
	assert(calculator->numOfFinishedThreads() == 1);
	delete calcprm; delete calculator; // 用事が済んだら片づけましょう
}
void yaku::yakuCalculator::analysisLoop(const GameTable* const gameStat, PlayerID targetPlayer,
	Shanten* const shanten, YAKUSTAT* const yakuInfo)
{
	CalculatorThread* calculator = new CalculatorThread; // インスタンスの準備
	// 変数を用意
	MENTSU_ANALYSIS analysis;
	memset(&analysis, 0, sizeof(MENTSU_ANALYSIS));
	memcpy(analysis.shanten, shanten, sizeof(Shanten[SHANTEN_PAGES]));
	analysis.player = targetPlayer;
	analysis.TileCount = countTilesInHand(gameStat, targetPlayer);
	analysis.SeenTiles = countseentiles(gameStat);
	analysis.MachiInfo = chkFuriten(gameStat, targetPlayer);
	analysis.GameStat = gameStat;
	analysis.PlayerStat = &(gameStat->Player[targetPlayer]);
	analysis.TsumoHai = &(gameStat->Player[targetPlayer].Tsumohai());
	analysis.MenzenFlag = &(gameStat->Player[targetPlayer].MenzenFlag);
	analysis.TsumoAgariFlag = &(gameStat->TsumoAgariFlag);
	// 計算ルーチンに渡すパラメータの準備
	CalculatorParam* calcprm = new CalculatorParam[160]; memset(calcprm, 0, sizeof(CalculatorParam[160]));
#ifdef _WIN32
	DWORD ThreadID[160]; HANDLE Thread[160];
#else /*_WIN32*/
	pthread_t Thread[160];
#endif /*_WIN32*/
	for (int i = 0; i < 160; i++) {
		calcprm[i].instance = calculator;
		calcprm[i].gameStat = gameStat; calcprm[i].instance = calculator;
		calcprm[i].pMode.AtamaCode = (TileCode)(i / 4);
		calcprm[i].pMode.Order = (ParseOrder)(i % 4);
		memcpy(&calcprm[i].analysis, &analysis, sizeof(MENTSU_ANALYSIS));
		YAKUSTAT::Init(&calcprm[i].result);
	}
	// 計算を実行
	for (int i = 4; i < 160; i++) { // 0～3はNoTileなのでやらなくていい
		while (calculator->numOfFinishedThreads() - calculator->numOfStartedThreads() >= CalculatorThread::threadLimit)
#ifdef _WIN32
			Sleep(1); // スレッド数制限のチェック
#else /*_WIN32*/
			usleep(1000); // スレッド数制限のチェック
#endif /*_WIN32*/
		do {
#ifdef _WIN32
			Thread[i] = CreateThread(nullptr, 0, CalculatorThread::calculator, (LPVOID)(&(calcprm[i])), 0, &(ThreadID[i]));
			if (Thread[i]) break; // 成功したらそれでよし
#else /*_WIN32*/
			if (!pthread_create(Thread + i, nullptr, CalculatorThread::calculator, (void*)(calcprm + i))) {
				pthread_detach(Thread[i]);
				break;
			}
#endif /*_WIN32*/
			{ // なんで失敗すんねんこのドアホ……
				CodeConv::tostringstream o;
				o << _T("スレッドの開始に失敗しました。 ループ [") << i <<
#ifdef _WIN32
					_T("] エラーコード [") <<
					std::hex << std::setw(8) << std::setfill(_T('0')) << GetLastError() <<
#endif /*_WIN32*/
					_T(']');
				error(o.str().c_str());
#ifdef _WIN32
				Sleep(100);
#else /*_WIN32*/
				usleep(100000);
#endif /*_WIN32*/
			}
		} while (true);
#ifdef _WIN32
		Sleep(1);
#else /*_WIN32*/
		usleep(1000);
#endif /*_WIN32*/
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
yaku::YAKUSTAT yaku::yakuCalculator::countyaku(const GameTable* const gameStat, PlayerID targetPlayer) {
	// 役判定
	CodeConv::tostringstream o;
	o << _T("役判定処理を開始 プレイヤー [") << (int)targetPlayer << _T("]");
	debug(o.str().c_str());
	// 初期化
	YAKUSTAT yakuInfo; YAKUSTAT::Init(&yakuInfo);
	// シャンテン数をチェック
	Shanten shanten[SHANTEN_PAGES];
	for (int i = 0; i < SHANTEN_PAGES; i++)
		shanten[i] = ShantenAnalyzer::calcShanten(gameStat, targetPlayer, (ShantenType)i);
	// 和了ってるか判定(和了ってなかった場合十三不塔か判定する)
	if (shanten[shantenAll] > -1) {
#ifndef GUOBIAO
		/* 十三不塔 */
		if (gameStat->Player[targetPlayer].FirstDrawFlag) { // 鳴きがなくて一巡目の時だけ判定する
			if (chkShisanBuDa(gameStat, targetPlayer)) { // 十三不塔になってる
				trace(_T("十三不塔です！！"));
				yakuInfo.isValid = true; yakuInfo.BasePoints = 30;
				if (RuleData::chkRule("limitless", "yakuman_considered_13han")) 
					yakuInfo.CoreHan = (RuleData::chkRule("shiisan_puutaa", "mangan")) ? 5 : 13;
				else yakuInfo.CoreSemiMangan = (RuleData::chkRule("shiisan_puutaa", "mangan")) ? 2 : 8;
				calculateScore(&yakuInfo);
#if defined(_MSC_VER)
				_tcscat_s((RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList,
					YAKUSTAT::nameBufSize, _T("十三不搭\r\n"));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcscat_s(yakuInfo.yakuValList, YAKUSTAT::nameBufSize,
					(RuleData::chkRule("shiisan_puutaa", "mangan")) ? _T("５飜\r\n") : _T("13飜\r\n"));
#elif defined(_WIN32)
				LPTSTR target = (RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList;
				_tcsncat(target, _T("十三不搭\r\n"), YAKUSTAT::nameBufSize - _tcslen(target));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcsncat(yakuInfo.yakuValList,
					(RuleData::chkRule("shiisan_puutaa", "mangan")) ? _T("５飜\r\n") : _T("13飜\r\n"),
					YAKUSTAT::nameBufSize - _tcslen(yakuInfo.yakuValList));
#else
				LPTSTR target = (RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList;
				_tcsncat(target, _T("十三不搭\n"), YAKUSTAT::nameBufSize - _tcslen(target));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcsncat(yakuInfo.yakuValList,
					(RuleData::chkRule("shiisan_puutaa", "mangan")) ? _T("５飜\n") : _T("13飜\n"),
					YAKUSTAT::nameBufSize - _tcslen(yakuInfo.yakuValList));
#endif
				countDora(gameStat, nullptr, &yakuInfo, targetPlayer); // ドラは数えてあげましょうね
			}
			/* 十四不塔 */
			else if (chkShisiBuDa(gameStat, targetPlayer)) { // 十四不塔になってる
				trace(_T("十四不塔です！！"));
				yakuInfo.isValid = true; yakuInfo.BasePoints = 30;
				if (RuleData::chkRule("limitless", "yakuman_considered_13han")) yakuInfo.CoreHan = 13;
				else yakuInfo.CoreSemiMangan = 8;
				calculateScore(&yakuInfo);
#if defined(_MSC_VER)
				_tcscat_s((RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList,
					YAKUSTAT::nameBufSize, _T("十三無靠\r\n"));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcscat_s(yakuInfo.yakuValList, YAKUSTAT::nameBufSize, _T("13飜\r\n"));
#elif defined(_WIN32)
				LPTSTR target = (RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList;
				_tcsncat(target, _T("十三無靠\r\n"), YAKUSTAT::nameBufSize - _tcslen(target));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcsncat(yakuInfo.yakuValList, _T("13飜\r\n"), YAKUSTAT::nameBufSize - _tcslen(yakuInfo.yakuValList));
#else
				LPTSTR target = (RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList;
				_tcsncat(target, _T("十三無靠\n"), YAKUSTAT::nameBufSize - _tcslen(target));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcsncat(yakuInfo.yakuValList, _T("13飜\n"), YAKUSTAT::nameBufSize - _tcslen(yakuInfo.yakuValList));
#endif
				countDora(gameStat, nullptr, &yakuInfo, targetPlayer); // ドラを数えるのです
			}
		}
#endif /* GUOBIAO */
		trace(_T("和了っていないので抜けます"));
		return yakuInfo;
	}
	// 和了っているなら
	if (shanten[shantenRegular] == -1) // 一般形の和了
		analysisLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	else // 七対子、国士無双、その他特殊な和了
		analysisNonLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	yakuInfo.isValid = true; // 有効であることをマーク（縛りを満たしているかはチェックしていない）
#ifndef GUOBIAO
	/* アリスドラの牌譜記録 */
	if ((RuleData::chkRuleApplied("alice")) && (gameStat->Player[targetPlayer].MenzenFlag)) {
		uint16_t AlicePointer = gameStat->DoraPointer;
		auto tiles = countTilesInHand(gameStat, targetPlayer);
		haifu::haifuresetalicedora();
		yakuInfo.AliceDora = 0;
		while (1) {
			AlicePointer -= 2;
			if (AlicePointer <= gameStat->TilePointer) {
				AlicePointer += 2; break;
			}
			haifu::haifurecalicedora(gameStat->Deck[AlicePointer].tile);
			if (tiles[gameStat->Deck[AlicePointer].tile] <= 0) break;
			++(yakuInfo.AliceDora);
		}
	}
#endif /* GUOBIAO */
	/* おわり */
	return yakuInfo;
}

/* 縛りを満たしているか調べる */
bool yaku::yakuCalculator::checkShibari(const GameTable* const gameStat, const YAKUSTAT* const yakuStat) {
#ifdef GUOBIAO
	return yakuStat->CoreHan >= 8;
#else /* GUOBIAO */
	if ((yakuStat->CoreHan >= 2) || (yakuStat->CoreSemiMangan >= 1))
		return true; // 2翻以上あったら縛りを満たす
	else if ((yakuStat->CoreHan == 1) && (RuleData::chkRule("ryanshiba", "no")))
		return true; // リャンシバなしのルールの場合で1翻
	else if ((yakuStat->CoreHan == 1) && (gameStat->Honba < 5) && (RuleData::chkRule("ryanshiba", "from_5honba")))
		return true; // 5本場からリャンシバだが4本場までで1翻
	else if ((yakuStat->CoreHan == 1) && (gameStat->Honba < 4) && (RuleData::chkRule("ryanshiba", "from_4honba")))
		return true; // 4本場からリャンシバだが3本場までで1翻
	else return false; // 縛りを満たしていない場合
#endif /* GUOBIAO */
}
