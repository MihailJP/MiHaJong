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

// �v�Z�����s(�}���`�X���b�h�Łc�c���v���Ȃ�)
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

/* �����Ă���X���b�h���̊Ǘ��p */
int yaku::yakuCalculator::CalculatorThread::numOfFinishedThreads() { // �I������X���b�h�̐�
	return this->finishedThreads;
}
int yaku::yakuCalculator::CalculatorThread::numOfStartedThreads() { // �J�n�����X���b�h�̐�
	return this->startedThreads;
}
void yaku::yakuCalculator::CalculatorThread::sync(int threads) { // �X���b�h�𓯊�
#ifdef _WIN32
	while (this->startedThreads < threads) Sleep(0); // �K�萔�̃X���b�h���J�n����̂�҂��Ă���
	while (this->finishedThreads < threads) Sleep(0); // �I������̂�҂�
#else /*_WIN32*/
	while (this->startedThreads < threads) usleep(100); // �K�萔�̃X���b�h���J�n����̂�҂��Ă���
	while (this->finishedThreads < threads) usleep(100); // �I������̂�҂�
#endif /*_WIN32*/
}

void yaku::yakuCalculator::CalculatorThread::recordThreadStart() {
	cs.syncDo<void>([this]() -> void {
		++startedThreads; // �J�n�����X���b�h�����C���N�������g
	});
}
void yaku::yakuCalculator::CalculatorThread::recordThreadFinish() {
	cs.syncDo<void>([this]() -> void {
		++finishedThreads; // �I�������X���b�h�����C���N�������g
	});
}

/* �|���v�Z���� */
#ifndef GUOBIAO
void yaku::yakuCalculator::doubling(yaku::YAKUSTAT* const yStat) {
	int totalHan = yStat->CoreHan + yStat->BonusHan; // ���v�|
	yStat->AgariPoints = (LNum)yStat->BasePoints; // ���_
	if (totalHan >= -2) {
		for (int i = 0; i < (totalHan + 2); i++) yStat->AgariPoints *= 2;
	} else {
		for (int i = 0; i < (totalHan + 2); i++) yStat->AgariPoints /= 2;
	}
	if (yStat->AgariPoints <= (LNum)0) yStat->AgariPoints = (LNum)1; // �Œ�1�_�ɂ͂Ȃ�悤�ɂ���
}
#endif /* GUOBIAO */

/* ���Ɩ|����_�����v�Z���� */
void yaku::yakuCalculator::calculateScore(yaku::YAKUSTAT* const yStat) {
#ifdef GUOBIAO
	/* �������[���ł͒P�ɑ����Z���邾���A���G�Ȍv�Z�͕s�v */
	yStat->AgariPoints = (LNum)yStat->CoreHan;
#else /* GUOBIAO */
	/* ����𖞂����ĂȂ�������0��Ԃ�
	   �_����0�Ȃ�R�A�����ō��a�Ɣ��f�����c�c�͂� */
	if ((yStat->CoreHan <= 0)&&(yStat->CoreSemiMangan <= 0)) {
		yStat->AgariPoints = (LNum)0;
		return;
	}

	int totalHan = yStat->CoreHan + yStat->BonusHan; // ���v�|
	int totalSemiMangan = yStat->CoreSemiMangan + yStat->BonusSemiMangan; // ���т̔����P��

	if (!RuleData::chkRuleApplied("limitless")) { // �ʏ탋�[���̏ꍇ
		if ((totalHan < 6) && (totalSemiMangan < 3)) { // ���шȉ�
			doubling(yStat); // �v�Z�����s
			if (yStat->AgariPoints > (LNum)2000) yStat->AgariPoints = (LNum)2000; // ����
			if (totalSemiMangan == 2) yStat->AgariPoints = (LNum)2000; // �Œ薞�т̏ꍇ
		}
		else if ((totalHan < 8) && (totalSemiMangan < 4)) yStat->AgariPoints = (LNum)3000; // ����
		else if (((totalHan < 10) || ((totalHan == 10) && (RuleData::chkRule("sanbaiman_border", "11han_or_more")))) &&
			(totalSemiMangan < 6)) yStat->AgariPoints = (LNum)4000; // �{��
		else if (((totalHan < 12) || ((totalHan == 12) && (RuleData::chkRule("kazoe_border", "13han_or_more")))) &&
			(totalSemiMangan < 8)) yStat->AgariPoints = (LNum)6000; // �O�{��
		else if ((totalSemiMangan < 8) && (RuleData::chkRule("kazoe_border", "no")))
			yStat->AgariPoints = (LNum)6000; // �O�{��
		else if (totalSemiMangan < 16) yStat->AgariPoints = (LNum)8000; // �� or ����
		else yStat->AgariPoints = (LNum)(8000 * (totalSemiMangan / 8)); // �_�u���𖞈ȏ�
	} else { // �V�䃋�[���̏ꍇ
		if (totalSemiMangan >= 8) yStat->AgariPoints = (LNum)2500000; // �𖞂��Œ�_�ɂ��郋�[��
		else if (totalSemiMangan >= 6) yStat->AgariPoints = (LNum)1875000;
		else if (totalSemiMangan >= 4) yStat->AgariPoints = (LNum)1250000;
		else if (totalSemiMangan >= 3) yStat->AgariPoints = (LNum)937500;
		else if (totalSemiMangan >= 2) yStat->AgariPoints = (LNum)625000;
		else doubling(yStat); // �v�Z����
	}

	{ // �g���[�X�p
		CodeConv::tostringstream o;
		o << _T("�v�Z���ʂ� [");
		for (int i = DIGIT_GROUPS - 1; i >= 0; i--)
			o << std::setw(4) << std::dec << std::setfill(_T('0')) << yStat->AgariPoints.digitGroup[i] / 10000 <<
			_T(" ") << std::setw(4) << std::dec << std::setfill(_T('0')) << yStat->AgariPoints.digitGroup[i] % 10000 << (i ? _T(" ") : _T(""));
		o << _T("]");
		trace(o.str().c_str());
	}
#endif /* GUOBIAO */
}

/* �����v�Z���� */
#ifndef GUOBIAO
void yaku::yakuCalculator::CalculatorThread::calcbasepoints
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis)
{
	trace(_T("���v�Z�̏����ɓ���܂��B"));
	int fu = 20; // ����Q�O��

	/* ��������(��v�݂̂Q��) */
	switch (analysis->MianziDat[0].tile) { /* ���v�͏����ɂ���Ė�v */
	case EastWind: case SouthWind: case WestWind: case NorthWind:
		if (analysis->MianziDat[0].tile ==
			Wind2Tile((uint8_t)(gameStat->GameRound / 4))) // �ꕗ�v
			fu += 2;
		if (analysis->MianziDat[0].tile ==
			Wind2Tile(gameStat->playerwind(analysis->player))) // �����v
			fu += 2;
		if ((RuleData::chkRuleApplied("kaimenkaze")) && (analysis->MianziDat[0].tile == // �J�啗�v
			Wind2Tile(gameStat->playerwind(gameStat->WaremePlayer))))
			fu += 2;
		if ((RuleData::chkRuleApplied("urakaze")) && (analysis->MianziDat[0].tile == // �����v
			Wind2Tile(gameStat->playerwind(analysis->player + 2))))
			fu += 2;
		if ((!RuleData::chkRuleApplied("double_yaku_wind_pair")) && (fu > 22)) fu = 22; // �_�u��������2���ƌ��Ȃ����[���̏ꍇ
		break;
	case WhiteDragon: case GreenDragon: case RedDragon: /* �O���v�͏�ɖ�v */
		fu += 2;
		break;
	}

	/* �ʎq���� */
	for (int i = 1; i < SizeOfMeldBuffer; i++) {
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
	const TileCode* tsumoTile = &(gameStat->Player[analysis->player].Tsumohai().tile); // shorthand
	if (analysis->MianziDat[0].tile == *tsumoTile) analysis->Machi = machiTanki; // �P�R�҂�
	for (int i = 1; i < SizeOfMeldBuffer; i++) { // �҂��̎�ނ𒲂ׂ�c�c
		switch (analysis->MianziDat[i].mstat) {
		case meldSequenceConcealed: case meldSequenceExposedLower:
		case meldSequenceExposedMiddle: case meldSequenceExposedUpper: /* ���q */
			if (analysis->MianziDat[i].tile == ((*tsumoTile) - 1)) analysis->Machi = machiKanchan;
			if (analysis->MianziDat[i].tile == *tsumoTile) {
				if (analysis->MianziDat[i].tile % TileSuitStep == 7) analysis->Machi = machiPenchan; // �Ӓ��҂�
				else {analysis->Machi = machiRyanmen; LiangMianFlag = true;} // ���ʑ҂�
			}
			if (analysis->MianziDat[i].tile == ((*tsumoTile) - 2)) {
				if (analysis->MianziDat[i].tile % TileSuitStep == 1) analysis->Machi = machiPenchan; // �Ӓ��҂�
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
			if ((!(gameStat->TsumoAgariFlag) || (RuleData::chkRuleApplied("tsumo_pinfu")))) { // �c���s�����肩�A�o�a���̏ꍇ
				analysis->isPinfu = true; fu = 20;
			} else {
				fu += 2; // �c����
			}
		} else {
			analysis->Machi = machiRyanmen; // �������ʈ���
			if (RuleData::chkRule("exposed_pinfu", "30fu")) {
				fu = 30; /* ��O�łȂ���΁A�R�O���Ƃ��� */
			}
			else if (RuleData::chkRule("exposed_pinfu", "1han_30fu_unbound") || RuleData::chkRule("exposed_pinfu", "1han_30fu")) {
				fu = 30; analysis->isPinfu = true;
			}
			else if (RuleData::chkRule("exposed_pinfu", "1han_20fu_unbound") || RuleData::chkRule("exposed_pinfu", "1han_20fu")) {
				analysis->isPinfu = true; /* �򂢕�����ɂ���ꍇ */
			}
		}
	} else if (gameStat->TsumoAgariFlag) {
		fu += 2; /* ���a�łȂ��c���a����͂Q�� */
	}

	/* ��O����(�P�O��) */
	if ((gameStat->Player[analysis->player].MenzenFlag) && (!(gameStat->TsumoAgariFlag)))
		fu += 10;

	/* �I������ */
	CodeConv::tostringstream o; o << _T("���̎�v�� [") << fu << _T("] ���ł��B"); trace(o.str().c_str());
	analysis->BasePoint = (uint8_t)fu;
}
#endif /* GUOBIAO */

/* �h���v�� */
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
	const bool uradoraEnabled = ((RuleData::chkRuleApplied("uradora")) && // ���h������̃��[���ŁA
		(gameStat->Player[targetPlayer].MenzenFlag) && // ��O�ł���A
		(gameStat->Player[targetPlayer].RichiFlag.RichiFlag)); // �����������Ă���Ȃ�
	int omote = 0; int ura = 0; int red = 0; int blue = 0; int alice = 0;
	int flower = 0; int north = 0;
	/* �h�����v�Z���� */
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == NoTile) continue;
		omote += gameStat->DoraFlag.Omote[gameStat->Player[targetPlayer].Hand[i].tile];
		if (uradoraEnabled) // ���h���K�p
			ura += gameStat->DoraFlag.Ura[gameStat->Player[targetPlayer].Hand[i].tile];
	}
	/* ���ʎq�̃h���𐔂��� */
	for (int i = 1; i <= gameStat->Player[targetPlayer].MeldPointer; i++) {
		auto k = &gameStat->Player[targetPlayer].Meld[i];
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
	/* �A���X�h�� */
	if ((RuleData::chkRuleApplied("alice")) && (gameStat->Player[targetPlayer].MenzenFlag)) {
		// �A���X���胋�[���Ŗ�O�łȂ��Ƒʖ�
		auto AlicePointer = gameStat->DoraPointer;
		// �v���L�^���[�`���̓X���b�h�Z�[�t����Ȃ������͂��Ȃ̂ŕʂ̏ꏊ�ł��
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
	/* �Ԕv�E�O���̃K�� */
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
	/* �v�����ʂ𔽉f */
	if (omote) {
		result->DoraQuantity += omote; result->BonusHan += omote;
		doraText(result, _T("�h��"), omote);
	}
	if (ura) {
		result->DoraQuantity += ura; result->BonusHan += ura; result->UraDoraQuantity = ura;
		doraText(result, _T("���h��"), ura);
	}
	if (red) {
		if (RuleData::chkRule("redtile_chip", "menzen_only") ||
			RuleData::chkRule("redtile_chip", "riichi_only") ||
			RuleData::chkRule("redtile_chip", "always")) {
			result->DoraQuantity += red; result->BonusHan += red;
		}
		result->AkaDoraQuantity = red;
		doraText(result, _T("�ԃh��"), red);
	}
	if (blue) {
		result->AoDoraQuantity = blue;
		if (RuleData::chkRule("blue_tiles", "1han")) {
			result->DoraQuantity += blue; result->BonusHan += blue;
			doraText(result, _T("�h��"), blue);
		} else if (RuleData::chkRule("blue_tiles", "2han")) {
			result->DoraQuantity += blue * 2; result->BonusHan += blue * 2;
			doraText(result, _T("�h�� 2x"), blue);
		} else if (RuleData::chkRule("blue_tiles", "-1han")) {
			result->DoraQuantity -= blue; result->BonusHan -= blue;
			doraText(result, _T("�h�� -"), blue);
		}
	}
	if (alice) {
		result->AliceDora = alice;
		doraText(result, _T("�A���X�j�V"), alice);
	}
#endif /* GUOBIAO */
}

/* �h���v�� */
void yaku::yakuCalculator::CalculatorThread::countDora
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result)
{
	yaku::yakuCalculator::countDora(gameStat, analysis, result, analysis->player);
}

/* �|�����v���� */
void yaku::yakuCalculator::CalculatorThread::hanSummation(
	int& totalHan, int& totalSemiMangan, int& totalBonusHan, int& totalBonusSemiMangan,
	std::map<CodeConv::tstring, Yaku::YAKU_HAN> &yakuHan, std::vector<CodeConv::tstring> &yakuOrd, YAKUSTAT* const result)
{
	totalHan = totalSemiMangan = totalBonusHan = totalBonusSemiMangan = 0; // �������ŏ��������Ă����悤
	for (auto yNameIter = yakuOrd.begin(); yNameIter != yakuOrd.end(); yNameIter++) {
		CodeConv::tstring yName = *yNameIter;
#ifdef GUOBIAO
		totalHan += yakuHan[yName].coreHan.getHan();
#else /* GUOBIAO */
		if (RuleData::chkRule("limitless", "yakuman_considered_13han")) { /* �_���[���Ŗ𖞂�13�ʈ����Ƃ���ꍇ */
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
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("�P�ʂ��ُ�ł�"));
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
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("�P�ʂ��ُ�ł�"));
			}
		} else { /* �ʏ�̖|�v�Z */
			switch (yakuHan[yName].coreHan.getUnit()) {
				case yaku::yakuCalculator::Han: totalHan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::SemiMangan: totalSemiMangan += yakuHan[yName].coreHan.getHan(); break;
				case yaku::yakuCalculator::Yakuman: totalSemiMangan += yakuHan[yName].coreHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("�P�ʂ��ُ�ł�"));
			}
			switch (yakuHan[yName].bonusHan.getUnit()) {
				case yaku::yakuCalculator::Han: totalBonusHan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::SemiMangan: totalBonusSemiMangan += yakuHan[yName].bonusHan.getHan(); break;
				case yaku::yakuCalculator::Yakuman: totalBonusSemiMangan += yakuHan[yName].bonusHan.getHan() * 8; break;
				default:
					RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("�P�ʂ��ُ�ł�"));
			}
		}
#endif /* GUOBIAO */
		/* ���̖��O���������� */
		if (result == nullptr) continue; // ���̖��O�̏o�͂�����Ȃ��Ȃ玟��
#ifndef GUOBIAO
		if ((yakuHan[yName].coreHan.getUnit() != yakuHan[yName].bonusHan.getUnit()) &&
			(yakuHan[yName].coreHan.getHan() * yakuHan[yName].bonusHan.getHan() != 0))
		{ /* �P�ʂ����݁I */
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("�P�ʂ����݂��Ă��܂�"));
		}
		else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::Han) || (yakuHan[yName].coreHan.getHan() == 0)) &&
			((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::Han) || (yakuHan[yName].bonusHan.getHan() == 0)))
		{ /* ���ʂ̖��̎� */
#endif /* GUOBIAO */
#if defined(_MSC_VER)
			_tcscat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, yName.c_str());
			_tcscat_s(result->yakuNameList, yaku::YAKUSTAT::nameBufSize, _T("\r\n"));
			_tcscat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize,
				intstr(yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()).c_str());
			_tcscat_s(result->yakuValList, yaku::YAKUSTAT::nameBufSize, _T("��\r\n"));
#elif defined(_WIN32)
			_tcsncat(result->yakuNameList, yName.c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, _T("\r\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuValList,
				intstr(yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()).c_str(),
					yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
			_tcsncat(result->yakuValList, _T("��\r\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
#else
			_tcsncat(result->yakuNameList, yName.c_str(), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuNameList, _T("\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuNameList));
			_tcsncat(result->yakuValList,
				intstr(yakuHan[yName].coreHan.getHan() + yakuHan[yName].bonusHan.getHan()).c_str(),
					yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
			_tcsncat(result->yakuValList, _T("��\n"), yaku::YAKUSTAT::nameBufSize - _tcslen(result->yakuValList));
#endif
#ifndef GUOBIAO
		}
		else if ( ((yakuHan[yName].coreHan.getUnit() == yaku::yakuCalculator::SemiMangan) || (yakuHan[yName].coreHan.getHan() == 0)) &&
			((yakuHan[yName].bonusHan.getUnit() == yaku::yakuCalculator::SemiMangan) || (yakuHan[yName].bonusHan.getHan() == 0)))
		{ /* ���� */
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
		{ /* �� */
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

/* �v�Z���[�`�� */
#ifdef _WIN32
DWORD WINAPI yaku::yakuCalculator::CalculatorThread::calculate
#else /*_WIN32*/
void* yaku::yakuCalculator::CalculatorThread::calculate
#endif /*_WIN32*/
	(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
	const ParseMode* const pMode, YAKUSTAT* const result)
{
	/* �ʎq��͏��� */
	if (analysis->shanten[shantenRegular] == -1) {
		int NumOfMelds = 0;
		mentsuParser::makementsu(gameStat, analysis->player, *pMode, &NumOfMelds, analysis->MianziDat);
		if (NumOfMelds < SizeOfMeldBuffer) { // �����𖞂����ĂȂ��Ȃ甲���܂�
			this->recordThreadFinish();
#ifdef _WIN32
			return S_OK;
#else /*_WIN32*/
			return nullptr;
#endif /*_WIN32*/
		}
#ifndef GUOBIAO
		calcbasepoints(gameStat, analysis); // �����v�Z����
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
		if (analysis->shanten[shantenPairs] == -1) { // ���Ύq
			if (RuleData::chkRule("seven_pairs", "1han_50fu")) analysis->BasePoint = 50; // 1�|50��
			else analysis->BasePoint = 25; // 2�|25��
		}
		else if (analysis->shanten[shantenOrphans] == -1) analysis->BasePoint = 30; // ���m�͖𖞂Ȃ̂ł���͐V���[���p
		else if ((analysis->shanten[shantenQuanbukao] == -1)&&(analysis->shanten[shantenStellar] > -1)) {
			if (RuleData::chkRule("quanbukao", "3han_30fu"))
				analysis->BasePoint = 30;
			else if (RuleData::chkRule("quanbukao", "3han_40fu") || RuleData::chkRule("quanbukao", "4han_40fu"))
				analysis->BasePoint = 40;
#endif /* GUOBIAO */
		}
		else if (analysis->shanten[shantenZuhelong] == -1) { // �g����
			mentsuParser::makementsu(gameStat, analysis->player, *pMode, nullptr, analysis->MianziDat);
#ifndef GUOBIAO
			calcbasepoints(gameStat, analysis); // �����v�Z����
		}
		else analysis->BasePoint = 30;
#endif /* GUOBIAO */
	}
	/* �𔻒胋�[�v */
	std::map<CodeConv::tstring, Yaku::YAKU_HAN> yakuHan; // �󂯎M������
	std::set<CodeConv::tstring> suppression; // �����������
	std::vector<CodeConv::tstring> yakuOrd; // �����ۑ��p
	std::for_each(YakuCatalog::Instantiate()->catalog.begin(), // ���J�^���O�̍ŏ�����
		YakuCatalog::Instantiate()->catalog.end(), // �J�^���O�̖����܂�
		[&yakuHan, analysis, &suppression, &yakuOrd](Yaku& yaku) -> void { // �����Ƃɔ��菈��
			//trace(yaku.getName().c_str());
			if (yaku.checkYaku(analysis)) { // ���������𖞂����Ă�����
				//trace(_T("...�́A�������Ă��܂��B"));
				yakuHan[yaku.getName()] = yaku.getHan(analysis); // �ʐ����L�^
				yakuOrd.push_back(yaku.getName()); // �������L�^���Ă���
				std::set<CodeConv::tstring> sup = yaku.getSuppression();
				suppression.insert(sup.begin(), sup.end()); // ���ʖ��̃��X�g������
			}
	});
	/* ���͐������Ă��Ȃ������������� */
	for (auto k = yakuOrd.begin(); k != yakuOrd.end(); ) {
		if ((yakuHan[*k].coreHan.getHan() == 0) && (yakuHan[*k].bonusHan.getHan() == 0)) // ���͐������ĂȂ���
			k = yakuOrd.erase(k); // ���ɐ����Ȃ�
		else ++k;
	}
	/* ��񂵂Ŕ��肷��� */
	checkPostponedYaku(gameStat, analysis, result, yakuHan, suppression, yakuOrd);
	/* ���ʖ����������� */
	std::for_each(suppression.begin(), suppression.end(), [&yakuOrd](CodeConv::tstring yaku) {
		for (auto k = yakuOrd.begin(); k != yakuOrd.end(); ) {
			if (*k == yaku) k = yakuOrd.erase(k);
			else ++k;
		}
	});
	/* �|�����v���� */
	int totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan;
	hanSummation(totalHan, totalSemiMangan, totalBonusHan, totalBonusSemiMangan, yakuHan, yakuOrd, result);
	/* �ʐ����v�Z�������ʂ��������� */
	result->CoreHan = totalHan;
#ifndef GUOBIAO
	result->CoreSemiMangan = totalSemiMangan;
	result->BonusHan = totalBonusHan; result->BonusSemiMangan = totalBonusSemiMangan;
	if (analysis->BasePoint == 25) result->BasePoints = 25; // �`�[�g�C��25���͂��̂܂�
	else result->BasePoints = analysis->BasePoint + ((10 - analysis->BasePoint % 10) % 10); // ���n�l���Ĕ��f
#endif /* GUOBIAO */
	/* �h���̐��𐔂��� */
	countDora(gameStat, analysis, result);
#ifndef GUOBIAO
	/* �ȗ����[��(�S��30��)�̏ꍇ */
	if (RuleData::chkRule("simplified_scoring", "simplified")) {
		trace(_T("�ȗ��v�Z���[���̂���30���Ƃ��Ĉ����܂��B"));
		result->BasePoints = 30;
	}
#endif /* GUOBIAO */
	/* �_�����v�Z���� */
	calculateScore(result);
#ifndef GUOBIAO
	/* �؂�グ���т̏��� */
	if ((RuleData::chkRuleApplied("round_to_mangan")) && // �؂�グ���у��[����ON��
		(!RuleData::chkRuleApplied("limitless")) && // �V�䃋�[���łȂ��ꍇ
		(result->AgariPoints == (LNum)1920)) // �q��7700�E�e��11600�Ȃ�
			result->AgariPoints = (LNum)2000; // ���тɂ���
#endif /* GUOBIAO */
	/* �I������ */
	recordThreadFinish(); // �X���b�h���I��������Ƃ��L�^
#ifdef _WIN32
	return S_OK;
#else /*_WIN32*/
	return nullptr;
#endif /*_WIN32*/
}

/* �R���X�g���N�^�ƃf�X�g���N�^ */
yaku::yakuCalculator::CalculatorThread::CalculatorThread() {
	finishedThreads = startedThreads = 0;
}
yaku::yakuCalculator::CalculatorThread::~CalculatorThread() {
	/* �I������Ƃ��͕K���������Ă���s�����ƁI�I */
}
		
/* �����̏����Ƃ� */
void yaku::yakuCalculator::analysisNonLoop(const GameTable* const gameStat, PlayerID targetPlayer,
	Shanten* const shanten, YAKUSTAT* const yakuInfo)
{
	CalculatorThread* calculator = new CalculatorThread; // �C���X�^���X�̏���
	// �ϐ���p��
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
	// �v�Z���[�`���ɓn���p�����[�^�̏���
	CalculatorParam* calcprm = new CalculatorParam; memset(calcprm, 0, sizeof(CalculatorParam));
	calcprm->gameStat = gameStat; calcprm->instance = calculator;
	memcpy(&calcprm->analysis, &analysis, sizeof(MENTSU_ANALYSIS));
	YAKUSTAT::Init(&calcprm->result);
	// �v�Z�����s
#ifdef _WIN32
	DWORD ThreadID;
	HANDLE Thread = CreateThread(nullptr, 0, CalculatorThread::calculator, (LPVOID)calcprm, 0, &ThreadID);
#else /*_WIN32*/
	pthread_t hThread;
	pthread_create(&hThread, nullptr, CalculatorThread::calculator, (void*)calcprm);
	pthread_detach(hThread);
#endif /*_WIN32*/
	calculator->sync(1); // ����(�ȗ��Ȏ���)
	// ���_�@�̏���
	memcpy(yakuInfo, &calcprm->result, sizeof(YAKUSTAT));
	assert(calculator->numOfStartedThreads() == 1);
	assert(calculator->numOfFinishedThreads() == 1);
	delete calcprm; delete calculator; // �p�����ς񂾂�ЂÂ��܂��傤
}
void yaku::yakuCalculator::analysisLoop(const GameTable* const gameStat, PlayerID targetPlayer,
	Shanten* const shanten, YAKUSTAT* const yakuInfo)
{
	CalculatorThread* calculator = new CalculatorThread; // �C���X�^���X�̏���
	// �ϐ���p��
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
	// �v�Z���[�`���ɓn���p�����[�^�̏���
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
	// �v�Z�����s
	for (int i = 4; i < 160; i++) { // 0�`3��NoTile�Ȃ̂ł��Ȃ��Ă���
		while (calculator->numOfFinishedThreads() - calculator->numOfStartedThreads() >= CalculatorThread::threadLimit)
#ifdef _WIN32
			Sleep(1); // �X���b�h�������̃`�F�b�N
#else /*_WIN32*/
			usleep(1000); // �X���b�h�������̃`�F�b�N
#endif /*_WIN32*/
		do {
#ifdef _WIN32
			Thread[i] = CreateThread(nullptr, 0, CalculatorThread::calculator, (LPVOID)(&(calcprm[i])), 0, &(ThreadID[i]));
			if (Thread[i]) break; // ���������炻��ł悵
#else /*_WIN32*/
			if (!pthread_create(Thread + i, nullptr, CalculatorThread::calculator, (void*)(calcprm + i))) {
				pthread_detach(Thread[i]);
				break;
			}
#endif /*_WIN32*/
			{ // �Ȃ�Ŏ��s����˂񂱂̃h�A�z�c�c
				CodeConv::tostringstream o;
				o << _T("�X���b�h�̊J�n�Ɏ��s���܂����B ���[�v [") << i <<
#ifdef _WIN32
					_T("] �G���[�R�[�h [") <<
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
	calculator->sync(156); // ����(�ȗ��Ȏ���)
	// ���_�@�̏���
	for (int i = 4; i < 160; i++) {
		if (yakuInfo->AgariPoints < calcprm[i].result.AgariPoints)
			memcpy(yakuInfo, &calcprm[i].result, sizeof(YAKUSTAT));
	}
	// �p�����ς񂾂�ЂÂ��܂��傤
	assert(calculator->numOfStartedThreads() == 156);
	assert(calculator->numOfFinishedThreads() == 156);
	delete[] calcprm; delete calculator;
}

// �����������Ă��邩���肷��
yaku::YAKUSTAT yaku::yakuCalculator::countyaku(const GameTable* const gameStat, PlayerID targetPlayer) {
	// �𔻒�
	CodeConv::tostringstream o;
	o << _T("�𔻒菈�����J�n �v���C���[ [") << (int)targetPlayer << _T("]");
	debug(o.str().c_str());
	// ������
	YAKUSTAT yakuInfo; YAKUSTAT::Init(&yakuInfo);
	// �V�����e�������`�F�b�N
	Shanten shanten[SHANTEN_PAGES];
	for (int i = 0; i < SHANTEN_PAGES; i++)
		shanten[i] = ShantenAnalyzer::calcShanten(gameStat, targetPlayer, (ShantenType)i);
	// �a�����Ă邩����(�a�����ĂȂ������ꍇ�\�O�s�������肷��)
	if (shanten[shantenAll] > -1) {
#ifndef GUOBIAO
		/* �\�O�s�� */
		if (gameStat->Player[targetPlayer].FirstDrawFlag) { // �����Ȃ��Ĉꏄ�ڂ̎��������肷��
			if (chkShisanBuDa(gameStat, targetPlayer)) { // �\�O�s���ɂȂ��Ă�
				trace(_T("�\�O�s���ł��I�I"));
				yakuInfo.isValid = true; yakuInfo.BasePoints = 30;
				if (RuleData::chkRule("limitless", "yakuman_considered_13han")) 
					yakuInfo.CoreHan = (RuleData::chkRule("shiisan_puutaa", "mangan")) ? 5 : 13;
				else yakuInfo.CoreSemiMangan = (RuleData::chkRule("shiisan_puutaa", "mangan")) ? 2 : 8;
				calculateScore(&yakuInfo);
#if defined(_MSC_VER)
				_tcscat_s((RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList,
					YAKUSTAT::nameBufSize, _T("�\�O�s��\r\n"));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcscat_s(yakuInfo.yakuValList, YAKUSTAT::nameBufSize,
					(RuleData::chkRule("shiisan_puutaa", "mangan")) ? _T("�T��\r\n") : _T("13��\r\n"));
#elif defined(_WIN32)
				LPTSTR target = (RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList;
				_tcsncat(target, _T("�\�O�s��\r\n"), YAKUSTAT::nameBufSize - _tcslen(target));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcsncat(yakuInfo.yakuValList,
					(RuleData::chkRule("shiisan_puutaa", "mangan")) ? _T("�T��\r\n") : _T("13��\r\n"),
					YAKUSTAT::nameBufSize - _tcslen(yakuInfo.yakuValList));
#else
				LPTSTR target = (RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList;
				_tcsncat(target, _T("�\�O�s��\n"), YAKUSTAT::nameBufSize - _tcslen(target));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcsncat(yakuInfo.yakuValList,
					(RuleData::chkRule("shiisan_puutaa", "mangan")) ? _T("�T��\n") : _T("13��\n"),
					YAKUSTAT::nameBufSize - _tcslen(yakuInfo.yakuValList));
#endif
				countDora(gameStat, nullptr, &yakuInfo, targetPlayer); // �h���͐����Ă����܂��傤��
			}
			/* �\�l�s�� */
			else if (chkShisiBuDa(gameStat, targetPlayer)) { // �\�l�s���ɂȂ��Ă�
				trace(_T("�\�l�s���ł��I�I"));
				yakuInfo.isValid = true; yakuInfo.BasePoints = 30;
				if (RuleData::chkRule("limitless", "yakuman_considered_13han")) yakuInfo.CoreHan = 13;
				else yakuInfo.CoreSemiMangan = 8;
				calculateScore(&yakuInfo);
#if defined(_MSC_VER)
				_tcscat_s((RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList,
					YAKUSTAT::nameBufSize, _T("�\�O����\r\n"));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcscat_s(yakuInfo.yakuValList, YAKUSTAT::nameBufSize, _T("13��\r\n"));
#elif defined(_WIN32)
				LPTSTR target = (RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList;
				_tcsncat(target, _T("�\�O����\r\n"), YAKUSTAT::nameBufSize - _tcslen(target));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcsncat(yakuInfo.yakuValList, _T("13��\r\n"), YAKUSTAT::nameBufSize - _tcslen(yakuInfo.yakuValList));
#else
				LPTSTR target = (RuleData::chkRule("limitless", "yakuman_considered_13han")) ?
					yakuInfo.yakuNameList : yakuInfo.yakumanNameList;
				_tcsncat(target, _T("�\�O����\n"), YAKUSTAT::nameBufSize - _tcslen(target));
				if (RuleData::chkRule("limitless", "yakuman_considered_13han"))
					_tcsncat(yakuInfo.yakuValList, _T("13��\n"), YAKUSTAT::nameBufSize - _tcslen(yakuInfo.yakuValList));
#endif
				countDora(gameStat, nullptr, &yakuInfo, targetPlayer); // �h���𐔂���̂ł�
			}
		}
#endif /* GUOBIAO */
		trace(_T("�a�����Ă��Ȃ��̂Ŕ����܂�"));
		return yakuInfo;
	}
	// �a�����Ă���Ȃ�
	if (shanten[shantenRegular] == -1) // ��ʌ`�̘a��
		analysisLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	else // ���Ύq�A���m���o�A���̑�����Șa��
		analysisNonLoop(gameStat, targetPlayer, shanten, &yakuInfo);
	yakuInfo.isValid = true; // �L���ł��邱�Ƃ��}�[�N�i����𖞂����Ă��邩�̓`�F�b�N���Ă��Ȃ��j
#ifndef GUOBIAO
	/* �A���X�h���̔v���L�^ */
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
	/* ����� */
	return yakuInfo;
}

/* ����𖞂����Ă��邩���ׂ� */
bool yaku::yakuCalculator::checkShibari(const GameTable* const gameStat, const YAKUSTAT* const yakuStat) {
#ifdef GUOBIAO
	return yakuStat->CoreHan >= 8;
#else /* GUOBIAO */
	if ((yakuStat->CoreHan >= 2) || (yakuStat->CoreSemiMangan >= 1))
		return true; // 2�|�ȏ゠�����甛��𖞂���
	else if ((yakuStat->CoreHan == 1) && (RuleData::chkRule("ryanshiba", "no")))
		return true; // �������V�o�Ȃ��̃��[���̏ꍇ��1�|
	else if ((yakuStat->CoreHan == 1) && (gameStat->Honba < 5) && (RuleData::chkRule("ryanshiba", "from_5honba")))
		return true; // 5�{�ꂩ�烊�����V�o����4�{��܂ł�1�|
	else if ((yakuStat->CoreHan == 1) && (gameStat->Honba < 4) && (RuleData::chkRule("ryanshiba", "from_4honba")))
		return true; // 4�{�ꂩ�烊�����V�o����3�{��܂ł�1�|
	else return false; // ����𖞂����Ă��Ȃ��ꍇ
#endif /* GUOBIAO */
}
