#include "yaku.h"
using std::max;

yaku::yakuCalculator::Yaku::HANFUNC::HANFUNC() {
	base_Han = YAKU_HAN();
}
yaku::yakuCalculator::Yaku::HANFUNC::HANFUNC(YAKU_HAN bHan) {
	base_Han = bHan;
}
yaku::yakuCalculator::Yaku::HANFUNC::HANFUNC(YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan) {
	base_Han = YAKU_HAN(cHan, dHan);
}

yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::HANFUNC::operator()
	(const GameTable* const, const MENTSU_ANALYSIS* const)
{
	return this->base_Han;
}
yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::FixedHan::operator()
	(const GameTable* const, const MENTSU_ANALYSIS* const)
{
	return this->base_Han;
}
yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::MenzenHan::operator()
	(const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysisDat)
{
	return gameStat->Player[analysisDat->player].MenzenFlag ? this->base_Han :
		yaku::yakuCalculator::Yaku::YAKU_HAN();
}
yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::KuisagariHan::operator()
	(const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysisDat)
{
	return gameStat->Player[analysisDat->player].MenzenFlag ? this->base_Han :
		yaku::yakuCalculator::Yaku::YAKU_HAN(
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(this->base_Han.coreHan.getHan() - 1, 0), this->base_Han.coreHan.getUnit()),
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(this->base_Han.bonusHan.getHan() - 1, 0), this->base_Han.bonusHan.getUnit())
		);
}

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
yaku::yakuCalculator::Yaku::HANFUNC yaku::yakuCalculator::Yaku::getHan() {return han;}
yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::getHan(
	const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu)
{return han(gameStat, mentsu);}
std::set<std::string> yaku::yakuCalculator::Yaku::getSuppression() {return suppressionList;}

// Constructor
#define YKCLASS_GENERAL_INIT \
	yakuName = std::string(name.begin(), name.end()); han = hanVal; yakuProc = YAKUFUNC(f); \
	suppressionList = std::set<std::string>()
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal, YAKUFUNC f) {
	YKCLASS_GENERAL_INIT;
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, std::string yk2, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, std::string yk2, std::string yk3, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, std::string yk7, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, std::string yk7, std::string yk8, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
	std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
	std::string yk6, std::string yk7, std::string yk8, std::string yk9, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
	suppressionList.insert(yk9);
}
yaku::yakuCalculator::Yaku::Yaku(std::string name, HANFUNC hanVal,
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
