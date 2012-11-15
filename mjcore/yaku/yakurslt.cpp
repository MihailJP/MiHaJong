#include "yakurslt.h"

/* 構造体のサイズを返す */
int yaku::YAKUSTAT::getSize() {
	return sizeof(YAKUSTAT);
}

/* 既存のインスタンスを初期化する */
void yaku::YAKUSTAT::Init(YAKUSTAT* const myInstance) {
	myInstance->isValid = false;
	myInstance->BasePoints = 20;
	myInstance->CoreHan = myInstance->CoreSemiMangan =
		myInstance->BonusHan = myInstance->BonusSemiMangan =
		myInstance->DoraQuantity = myInstance->UraDoraQuantity =
		myInstance->AkaDoraQuantity = myInstance->AoDoraQuantity =
		myInstance->AliceDora = myInstance->FlowerQuantity = 0;
	myInstance->AgariPoints = LargeNum::fromInt(0);
	memset(myInstance->yakuNameList, 0, nameBufSize);
	memset(myInstance->yakuValList, 0, nameBufSize);
	memset(myInstance->yakumanNameList, 0, nameBufSize);
	memset(myInstance->yakumanValList, 0, nameBufSize);
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
		case 1:
			myInstance->CoreHan = (value + LimitMinus) % SemiMangan - LimitMinus;
			myInstance->CoreSemiMangan = (value + LimitMinus) / SemiMangan - LimitMinus;
			break;
		case 2:
			myInstance->BonusHan = (value + LimitMinus) % SemiMangan - LimitMinus;
			myInstance->BonusSemiMangan = (value + LimitMinus) / SemiMangan - LimitMinus;
			break;
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("ページが違います"));
	}
}
void yaku::YAKUSTAT::setYakuInfo(YAKUSTAT* const myInstance, int index, LPCTSTR const value) {
	switch (index) {
		case 17: _tcscpy_s(myInstance->yakuNameList, nameBufSize, value); break;
		case 18: _tcscpy_s(myInstance->yakumanNameList, nameBufSize, value); break;
		case 19: _tcscpy_s(myInstance->yakuValList, nameBufSize, value); break;
		case 20: _tcscpy_s(myInstance->yakumanValList, nameBufSize, value); break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("ページが違います"));
	}
}

void yaku::YAKUSTAT::addYakuInfo(YAKUSTAT* const myInstance, int index, int value) {
	switch (index) {
		case 0: myInstance->BasePoints += value; break;
		case 1:
			myInstance->CoreHan += (value + LimitMinus) % SemiMangan - LimitMinus;
			myInstance->CoreSemiMangan += (value + LimitMinus) / SemiMangan - LimitMinus;
			break;
		case 2:
			myInstance->BonusHan += (value + LimitMinus) % SemiMangan - LimitMinus;
			myInstance->BonusSemiMangan += (value + LimitMinus) / SemiMangan - LimitMinus;
			break;
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("ページが違います"));
	}
}
void yaku::YAKUSTAT::addYakuInfo(YAKUSTAT* const myInstance, int index, LPCTSTR const value) {
	switch (index) {
		case 17: _tcscpy_s(myInstance->yakuNameList, nameBufSize, value); break;
		case 18: _tcscpy_s(myInstance->yakumanNameList, nameBufSize, value); break;
		case 19: _tcscpy_s(myInstance->yakuValList, nameBufSize, value); break;
		case 20: _tcscpy_s(myInstance->yakumanValList, nameBufSize, value); break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("ページが違います"));
	}
}

int yaku::YAKUSTAT::getYakuInfo(const YAKUSTAT* const myInstance, int index) {
	switch (index) {
		case 0: return myInstance->BasePoints;
		case 1: return myInstance->CoreHan + myInstance->CoreSemiMangan * SemiMangan;
		case 2: return myInstance->BonusHan + myInstance->BonusSemiMangan * SemiMangan;
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
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("ページが違います"));
			return 0x7fffffff;
	}
}
void yaku::YAKUSTAT::getYakuInfo(const YAKUSTAT* const myInstance,
	int index, LPTSTR const targetStr, int bufSize)
{
	switch (index) {
		case 17: _tcscpy_s(targetStr, bufSize, myInstance->yakuNameList); break;
		case 18: _tcscpy_s(targetStr, bufSize, myInstance->yakumanNameList); break;
		case 19: _tcscpy_s(targetStr, bufSize, myInstance->yakuValList); break;
		case 20: _tcscpy_s(targetStr, bufSize, myInstance->yakumanValList); break;
		default:
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_ARGUMENT, _T("ページが違います"));
			memset(targetStr, 0, bufSize);
	}
}
