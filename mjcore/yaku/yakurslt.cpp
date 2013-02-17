#include "yakurslt.h"

#include "../except.h"
#include "../../common/strcode.h"

/* 既存のインスタンスを初期化する */
void yaku::YAKUSTAT::Init(YAKUSTAT* const myInstance) {
	myInstance->isValid = false;
	myInstance->BasePoints = 20;
	myInstance->CoreHan = myInstance->CoreSemiMangan =
		myInstance->BonusHan = myInstance->BonusSemiMangan =
		myInstance->DoraQuantity = myInstance->UraDoraQuantity =
		myInstance->AkaDoraQuantity = myInstance->AoDoraQuantity =
		myInstance->AliceDora = myInstance->FlowerQuantity = 0;
	myInstance->AgariPoints = (LNum)0;
	memset(myInstance->yakuNameList, 0, nameBufSize);
	memset(myInstance->yakuValList, 0, nameBufSize);
	memset(myInstance->yakumanNameList, 0, nameBufSize);
	memset(myInstance->yakumanValList, 0, nameBufSize);
}
