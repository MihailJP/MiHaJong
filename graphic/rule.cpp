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
extern const char* digit = nullptr;

EXPORT void setfunc(
	RetrieveTxt fpGetRuleName, RetrieveTxt fpGetRuleDesc, RetrieveTxtIndex fpGetRuleTxt,
	RetrieveInt fpGetRule, RetrieveInt fpGetRuleSize, CheckDep fpReqFailed,
	RetrieveCaption fpGetPageCaption, RuleWrite fpStoreRule, RuleRead fpExportRule,
	const char* pDigit)
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
	digit = pDigit;
}

}
}
