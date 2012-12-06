#include "rule.h"

namespace mihajong_graphic {
namespace rules {

RetrieveTxt getRuleName = nullptr;
RetrieveTxt getRuleDescription = nullptr;
RetrieveTxtIndex getRuleTxt = nullptr;
RetrieveInt getRule = nullptr;
RetrieveInt getRuleSize = nullptr;
CheckDep reqFailed = nullptr;
RetrieveCaption getPageCaption = nullptr;
RuleWrite storeRule = nullptr;
RuleRead exportRule = nullptr;
ConfSave saveConfigFile = nullptr;
extern const char* digit = nullptr;
std::string conffile = "";

EXPORT void setfunc(
	RetrieveTxt fpGetRuleName, RetrieveTxt fpGetRuleDesc, RetrieveTxtIndex fpGetRuleTxt,
	RetrieveInt fpGetRule, RetrieveInt fpGetRuleSize, CheckDep fpReqFailed,
	RetrieveCaption fpGetPageCaption, RuleWrite fpStoreRule, RuleRead fpExportRule,
	ConfSave fpSaveConfigFile, const char* pDigit)
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
	digit = pDigit;
}

EXPORT void setconffile(const char* filename) {
	conffile = std::string(filename);
}

}
}
