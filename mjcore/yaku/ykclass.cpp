#include "yaku.h"
#include <algorithm>
using std::max;

yaku::yakuCalculator::Yaku::HANFUNC::HANFUNC() {
	hFunc = [](const MentsuAnalysis* const) {return yaku::yakuCalculator::Yaku::YAKU_HAN();};
}
yaku::yakuCalculator::Yaku::HANFUNC::HANFUNC(std::function<yaku::yakuCalculator::Yaku::YAKU_HAN (const MentsuAnalysis* const)> f) {
	hFunc = f;
}

yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::HANFUNC::operator()
	(const MentsuAnalysis* const analysis) const
{
	return this->hFunc(analysis);
}

yaku::yakuCalculator::Yaku::FixedHan::FixedHan(YAKU_HAN bHan)
{
	hFunc = [=](const MentsuAnalysis* const) {return bHan;};
}
yaku::yakuCalculator::Yaku::FixedHan::FixedHan(YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan)
{
	hFunc = [=](const MentsuAnalysis* const) {return yaku::yakuCalculator::Yaku::YAKU_HAN(cHan, dHan);};
}

#ifndef GUOBIAO
yaku::yakuCalculator::Yaku::MenzenHan::MenzenHan(YAKU_HAN bHan)
{
	hFunc = [=](const MentsuAnalysis* const analysisDat) {
		return analysisDat->MenzenFlag() ? bHan : yaku::yakuCalculator::Yaku::YAKU_HAN();
	};
}
yaku::yakuCalculator::Yaku::MenzenHan::MenzenHan(YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan)
{
	hFunc = [=](const MentsuAnalysis* const analysisDat) {
		return analysisDat->MenzenFlag() ?
			yaku::yakuCalculator::Yaku::YAKU_HAN(cHan, dHan) : yaku::yakuCalculator::Yaku::YAKU_HAN();
	};
}

yaku::yakuCalculator::Yaku::KuisagariHan::KuisagariHan(YAKU_HAN bHan)
{
	hFunc = [=](const MentsuAnalysis* const analysisDat) {
		return analysisDat->MenzenFlag() ? bHan :
			yaku::yakuCalculator::Yaku::YAKU_HAN(
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(bHan.coreHan.getHan() - 1, 0), bHan.coreHan.getUnit()),
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(bHan.bonusHan.getHan() - 1, 0), bHan.bonusHan.getUnit())
			);
	};
}
yaku::yakuCalculator::Yaku::KuisagariHan::KuisagariHan(YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan)
{
	hFunc = [=](const MentsuAnalysis* const analysisDat) {
		return analysisDat->MenzenFlag() ? yaku::yakuCalculator::Yaku::YAKU_HAN(cHan, dHan) :
			yaku::yakuCalculator::Yaku::YAKU_HAN(
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(cHan.getHan() - 1, 0), cHan.getUnit()),
			yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(max(dHan.getHan() - 1, 0), dHan.getUnit())
			);
	};
}
#endif /* GUOBIAO */


// -------------------------------------------------------------------------

#ifdef GUOBIAO
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN() { han = 0; }
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(int8_t h) { han = h; }
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(int8_t h, HanUnit) { han = h; } // Compatibility
int8_t yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::getHan() const {return this->han;}
yaku::yakuCalculator::HanUnit yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::getUnit() const {return HanUnit::han;} // Compatibility
#else /* GUOBIAO */
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN() { han = 0; unit = HanUnit::han; }
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(int8_t h) { han = h; unit = HanUnit::han; }
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::HAN(int8_t h, HanUnit u) { han = h; unit = u; }
int8_t yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::getHan() const {return this->han;}
yaku::yakuCalculator::HanUnit yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::getUnit() const {return this->unit;}
#endif /* GUOBIAO */

yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN() {coreHan = HAN(); bonusHan = HAN();}
yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN(HAN han) {coreHan = han; bonusHan = HAN();}
yaku::yakuCalculator::Yaku::YAKU_HAN::YAKU_HAN(HAN han, HAN bonus) {coreHan = han; bonusHan = bonus;}

bool yaku::yakuCalculator::Yaku::checkYaku(const MentsuAnalysis* const mentsu) {
	// 役を判定する
	return this->yakuProc(mentsu);
}
CodeConv::tstring yaku::yakuCalculator::Yaku::getName() {return this->yakuName;} // 役の名前を取得する
yaku::yakuCalculator::Yaku::HANFUNC yaku::yakuCalculator::Yaku::getHan() {return han;}
yaku::yakuCalculator::Yaku::YAKU_HAN yaku::yakuCalculator::Yaku::getHan(const MentsuAnalysis* const mentsu)
{
	return han(mentsu);
}
std::set<CodeConv::tstring> yaku::yakuCalculator::Yaku::getSuppression() {return suppressionList;}

// Constructor
#define YKCLASS_GENERAL_INIT \
	yakuName = CodeConv::tstring(name.begin(), name.end()); han = hanVal; yakuProc = YAKUFUNC(f); \
	suppressionList = std::set<CodeConv::tstring>()
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal, YAKUFUNC f) {
	YKCLASS_GENERAL_INIT;
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
	CodeConv::tstring yk6, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
	CodeConv::tstring yk6, CodeConv::tstring yk7, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
	CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
	CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, CodeConv::tstring yk9, YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
	suppressionList.insert(yk9);
}
yaku::yakuCalculator::Yaku::Yaku(CodeConv::tstring name, HANFUNC hanVal,
	CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
	CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, CodeConv::tstring yk9, CodeConv::tstring yk10,
	YAKUFUNC f)
{
	YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
	suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
	suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
	suppressionList.insert(yk9); suppressionList.insert(yk10);
}
#undef YKCLASS_GENERAL_INIT
