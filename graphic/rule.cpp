#include "rule.h"

namespace mihajong_graphic {
namespace rules {

RetrieveTxt getRuleName = nullptr;
RetrieveTxt getRuleDescription = nullptr;
RetrieveTxtIndex getRuleTxt = nullptr;
RetrieveInt getRule = nullptr;
RetrieveSize getRuleSize = nullptr;
CheckDep reqFailed = nullptr;
RetrieveCaption getPageCaption = nullptr;
RuleWrite storeRule = nullptr;
RuleRead exportRule = nullptr;
ConfSave saveConfigFile = nullptr;
CheckRule chkRule = nullptr;
const char* digit = nullptr;
std::string conffile = "";

EXPORT void setfunc(
	RetrieveTxt fpGetRuleName, RetrieveTxt fpGetRuleDesc, RetrieveTxtIndex fpGetRuleTxt,
	RetrieveInt fpGetRule, RetrieveSize fpGetRuleSize, CheckDep fpReqFailed,
	RetrieveCaption fpGetPageCaption, RuleWrite fpStoreRule, RuleRead fpExportRule,
	ConfSave fpSaveConfigFile, CheckRule fpChkRule, const char* pDigit)
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
}

EXPORT void setconffile(const char* filename) {
	conffile = std::string(filename);
}

}

namespace preferences {

bool blackTile = false;
std::string serverAddress;

std::string serverIP() {
	return serverAddress;
}

EXPORT void useBlackTile(bool val) {
	blackTile = val;
}
EXPORT void setServerAddr(const char* addr) {
	serverAddress = addr;
}


}

}
