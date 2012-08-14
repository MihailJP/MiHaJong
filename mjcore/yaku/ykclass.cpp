#include "yaku.h"
using std::max;

yaku::yakuCalculator::Yaku::HANFUNC::HANFUNC() {
	hFunc = [](const MENTSU_ANALYSIS* const) {return yaku::yakuCalculator::Yaku::YAKU_HAN();};
}
yaku::yakuCalculator::Yaku::HANFUNC::HANFUNC(std::function<yaku::yakuCalculator::Yaku::YAKU_HAN (const MENTSU_ANALYSIS* const)> f) {
	hFunc = f;
}

yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::HANFUNC::operator()
	(const MENTSU_ANALYSIS* const analysis) const
{
	return this->hFunc(analysis);
}

yaku::yakuCalculator::Yaku::FixedHan::FixedHan(YAKU_HAN bHan)
{
	hFunc = [=](const MENTSU_ANALYSIS* const) {return bHan;};
}
yaku::yakuCalculator::Yaku::FixedHan::FixedHan(YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan)
{
	hFunc = [=](const MENTSU_ANALYSIS* const) {return yaku::yakuCalculator::Yaku::YAKU_HAN(cHan, dHan);};
}

yaku::yakuCalculator::Yaku::MenzenHan::MenzenHan(YAKU_HAN bHan)
{
	hFunc = [=](const MENTSU_ANALYSIS* const analysisDat) {
		return *(analysisDat->MenzenFlag) ? bHan : yaku::yakuCalculator::Yaku::YAKU_HAN();
	};
}
yaku::yakuCalculator::Yaku::MenzenHan::MenzenHan(YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan)
{
	hFunc = [=](const MENTSU_ANALYSIS* const analysisDat) {
		return *(analysisDat->MenzenFlag) ?
			yaku::yakuCalculator::Yaku::YAKU_HAN(cHan, dHan) : yaku::yakuCalculator::Yaku::YAKU_HAN();
	};
}

yaku::yakuCalculator::Yaku::KuisagariHan::KuisagariHan(YAKU_HAN bHan)
{
	hFunc = [=](const MENTSU_ANALYSIS* const analysisDat) {
		return *(analysisDat->MenzenFlag) ? bHan :
			yaku::yakuCalculator::Yaku::YAKU_HAN(
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(bHan.coreHan.getHan() - 1, 0), bHan.coreHan.getUnit()),
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(bHan.bonusHan.getHan() - 1, 0), bHan.bonusHan.getUnit())
			);
	};
}
yaku::yakuCalculator::Yaku::KuisagariHan::KuisagariHan(YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan)
{
	hFunc = [=](const MENTSU_ANALYSIS* const analysisDat) {
		return *(analysisDat->MenzenFlag) ? yaku::yakuCalculator::Yaku::YAKU_HAN(cHan, dHan) :
			yaku::yakuCalculator::Yaku::YAKU_HAN(
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(cHan.getHan() - 1, 0), cHan.getUnit()),
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(dHan.getHan() - 1, 0), dHan.getUnit())
			);
	};
}


// -------------------------------------------------------------------------

yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN() { han = 0; unit = Han; }
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(int8_t h) { han = h; unit = Han; }
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(int8_t h, hanUnit u) { han = h; unit = u; }
int8_t yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::getHan() const {return this->han;}
yaku::yakuCalculator::hanUnit yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::getUnit() const {return this->unit;}

yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN() {coreHan = HAN(); bonusHan = HAN();}
yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN(HAN han) {coreHan = han; bonusHan = HAN();}
yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN(HAN han, HAN bonus) {coreHan = han; bonusHan = bonus;}

bool yaku::yakuCalculator::Yaku::checkYaku(const MENTSU_ANALYSIS* const mentsu) {
	// –ð‚ð”»’è‚·‚é
	return this->yakuProc(mentsu);
}
std::string yaku::yakuCalculator::Yaku::getName() {return this->yakuName;} // –ð‚Ì–¼‘O‚ðŽæ“¾‚·‚é
yaku::yakuCalculator::Yaku::HANFUNC yaku::yakuCalculator::Yaku::getHan() {return han;}
yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::getHan(const MENTSU_ANALYSIS* const mentsu)
{
	return han(mentsu);
}
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
