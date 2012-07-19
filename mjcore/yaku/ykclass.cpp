#include "yaku.h"

yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN() { han = 0; unit = Han; }
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(int8_t h) { han = h; unit = Han; }
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(int8_t h, hanUnit u) { han = h; unit = u; }
int8_t yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::getHan() {return this->han;}
yaku::yakuCalculator::hanUnit yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::getUnit() {return this->unit;}

yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN() {coreHan = HAN(); bonusHan = HAN();}
yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN(HAN han) {coreHan = han; bonusHan = HAN();}
yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN(HAN han, HAN bonus) {coreHan = han; bonusHan = bonus;}

bool yaku::yakuCalculator::Yaku::checkYaku(const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu) {
	// –ð‚ð”»’è‚·‚é
	return this->yakuProc(gameStat, mentsu);
}
std::string yaku::yakuCalculator::Yaku::getName() {return this->yakuName;} // –ð‚Ì–¼‘O‚ðŽæ“¾‚·‚é
yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::getHan() {return han;}
std::set<std::string> yaku::yakuCalculator::Yaku::getSuppression() {return suppressionList;}

// Constructor
#define YKCLASS_GENERAL_INIT \
	yakuName = std::string(name.begin(), name.end()); han = YAKU_HAN(cHan, bHan); yakuProc = YAKUFUNC(f); \
	suppressionList = std::set<std::string>()
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan, YAKUFUNC f) {
	YKCLASS_GENERAL_INIT;
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, std::string yk3, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, std::string yk7, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, std::string yk7, std::string yk8, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, std::string yk7, std::string yk8, std::string yk9, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
	suppressionList.insert(yk9);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, std::string yk7, std::string yk8, std::string yk9, std::string yk10,
	YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
	suppressionList.insert(yk9); suppressionList.insert(yk10);
}
#undef YKCLASS_GENERAL_INIT
