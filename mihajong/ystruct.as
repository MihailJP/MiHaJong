/*=============================
 * HSP�����N���C�A���gMiHaJong
 *  [�𔻒茋�ʊi�[�p�\����]
 *=============================
 */

#module yakuStruct BasePoints, CoreHan, BonusHan, \
DoraQuantity, UraDoraQuantity, AkaDoraQuantity, AoDoraQuantity, AliceDora, FlowerQuantity, \
AgariPoints, yakuNameList, yakuValList, yakumanNameList, yakumanValList
#include "const.as"
#modinit
	BasePoints = 20
	CoreHan = 0
	BonusHan = 0
	DoraQuantity = 0
	UraDoraQuantity = 0
	AkaDoraQuantity = 0
	AoDoraQuantity = 0
	AliceDora = 0
	FlowerQuantity = 0
	dim AgariPoints, NUM_OF_DIGIT_GROUPS
	yakuNameList = ""
	yakuValList = ""
	yakumanNameList = ""
	yakumanValList = ""
	return
#modfunc exportYakuPoint array exportArray
	dim exportArray, NUM_OF_DIGIT_GROUPS
	repeat NUM_OF_DIGIT_GROUPS: exportArray(cnt) = AgariPoints(cnt): loop
	return
#modfunc setYakuInfo int Index, int value
	switch Index
		case YAKUINF_FU: BasePoints = value: swbreak
		case YAKUINF_HAN_BASE: CoreHan = value: swbreak
		case YAKUINF_HAN_BONUS: BonusHan = value: swbreak
		case YAKUINF_DORA: DoraQuantity = value: swbreak
		case YAKUINF_URADORA: UraDoraQuantity = value: swbreak
		case YAKUINF_AKADORA: AkaDoraQuantity = value: swbreak
		case YAKUINF_AODORA: AoDoraQuantity = value: swbreak
		case YAKUINF_ALICEDORA: AliceDora = value: swbreak
		case YAKUINF_FLOWER: FlowerQuantity = value: swbreak
		default: AgariPoints(Index-YAKUINF_POINTS) = value: swbreak
	swend
	return
#modfunc setYakuInfoTxt int Index, str value
	switch Index
		case YAKUINF_YAKULIST: yakuNameList = value: swbreak
		case YAKUINF_YAKUVALLIST: yakuValList = value: swbreak
		case YAKUINF_YAKUMANLIST: yakumanNameList = value: swbreak
		case YAKUINF_YAKUMANVALLIST: yakumanValList = value: swbreak
	swend
	return
#modfunc addYakuInfo int Index, int value
	switch Index
		case YAKUINF_FU: BasePoints += value: swbreak
		case YAKUINF_HAN_BASE: CoreHan += value: swbreak
		case YAKUINF_HAN_BONUS: BonusHan += value: swbreak
		case YAKUINF_DORA: DoraQuantity += value: swbreak
		case YAKUINF_URADORA: UraDoraQuantity += value: swbreak
		case YAKUINF_AKADORA: AkaDoraQuantity += value: swbreak
		case YAKUINF_AODORA: AoDoraQuantity += value: swbreak
		case YAKUINF_ALICEDORA: AliceDora += value: swbreak
		case YAKUINF_FLOWER: FlowerQuantity += value: swbreak
		default: AgariPoints(Index-YAKUINF_POINTS) += value: swbreak
	swend
	return
#modfunc addYakuInfoTxt int Index, str value
	switch Index
		case YAKUINF_YAKULIST: yakuNameList += value: swbreak
		case YAKUINF_YAKUVALLIST: yakuValList += value: swbreak
		case YAKUINF_YAKUMANLIST: yakumanNameList += value: swbreak
		case YAKUINF_YAKUMANVALLIST: yakumanValList += value: swbreak
	swend
	return
#modcfunc getYakuInfo int Index
	switch Index
		case YAKUINF_FU: return BasePoints: swbreak
		case YAKUINF_HAN_BASE: return CoreHan: swbreak
		case YAKUINF_HAN_BONUS: return BonusHan: swbreak
		case YAKUINF_DORA: return DoraQuantity: swbreak
		case YAKUINF_URADORA: return UraDoraQuantity: swbreak
		case YAKUINF_AKADORA: return AkaDoraQuantity: swbreak
		case YAKUINF_AODORA: return AoDoraQuantity: swbreak
		case YAKUINF_ALICEDORA: return AliceDora: swbreak
		case YAKUINF_FLOWER: return FlowerQuantity: swbreak
		case YAKUINF_YAKULIST: return yakuNameList: swbreak
		case YAKUINF_YAKUVALLIST: return yakuValList: swbreak
		case YAKUINF_YAKUMANLIST: return yakumanNameList: swbreak
		case YAKUINF_YAKUMANVALLIST: return yakumanValList: swbreak
		default: return AgariPoints(Index-YAKUINF_POINTS): swbreak
	swend
	return 0x7fffffff
#global
