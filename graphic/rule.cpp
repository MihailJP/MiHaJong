#include "rule.h"

namespace mihajong_graphic {
namespace rules {

RetrieveTxt getRuleName = nullptr, getPreferenceName = nullptr;
RetrieveTxt getRuleDescription = nullptr, getPreferenceDescription = nullptr;
RetrieveTxtIndex getRuleTxt = nullptr, getPreferenceTxt = nullptr;
RetrieveInt getRule = nullptr, getPreference = nullptr;
RetrieveInt getRuleSize = nullptr, getPreferenceSize = nullptr;
CheckDep reqFailed = nullptr;
RetrieveCaption getPageCaption = nullptr;
RuleWrite storeRule = nullptr, storePref = nullptr;
RuleRead exportRule = nullptr, exportPref = nullptr;
ConfSave saveConfigFile = nullptr, savePreferenceFile = nullptr;
CheckRule chkRule = nullptr;
extern const char* digit = nullptr;
std::string conffile = "", preffile = "";

EXPORT void setfunc(
	RetrieveTxt fpGetRuleName, RetrieveTxt fpGetRuleDesc, RetrieveTxtIndex fpGetRuleTxt,
	RetrieveInt fpGetRule, RetrieveInt fpGetRuleSize, CheckDep fpReqFailed,
	RetrieveCaption fpGetPageCaption, RuleWrite fpStoreRule, RuleRead fpExportRule,
	ConfSave fpSaveConfigFile, CheckRule fpChkRule, const char* pDigit,
	RuleWrite fpStorePref, RuleRead fpExportPref, ConfSave fpSavePreferenceFile,
	RetrieveTxt fpGetPreferenceName, RetrieveTxt fpGetPreferenceDesc, RetrieveTxtIndex fpGetPreferenceTxt,
	RetrieveInt fpGetPreference, RetrieveInt fpGetPreferenceSize)
{
	getRuleName = fpGetRuleName;
	getRuleDescription = fpGetRuleDesc;
	getRuleTxt = fpGetRuleTxt;
	getRule = fpGetRule;
	getRuleSize = fpGetRuleSize;
	reqFailed = fpReqFailed;
	getPageCaption = fpGetPageCaption;
	storeRule = fpStoreRule;
	exportRule = fpExportRule;
	saveConfigFile = fpSaveConfigFile;
	chkRule = fpChkRule;
	digit = pDigit;

	storePref = fpStorePref;
	exportPref = fpExportPref;
	savePreferenceFile = fpSavePreferenceFile;
	getPreferenceName = fpGetPreferenceName;
	getPreferenceDescription = fpGetPreferenceDesc;
	getPreferenceTxt = fpGetPreferenceTxt;
	getPreference = fpGetPreference;
	getPreferenceSize = fpGetPreferenceSize;
}

EXPORT void setconffile(const char* filename, const char* filename2) {
	conffile = std::string(filename);
	preffile = std::string(filename2);
}

}
}
