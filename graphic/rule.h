#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>
#include "../mjcore/strcode.h"
#include "exports.h"

namespace mihajong_graphic {
namespace rules {

typedef void (*RetrieveTxt)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint16_t /*RuleID*/);
typedef void (*RetrieveTxtIndex)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint16_t /*RuleID*/, uint8_t /*index*/);
typedef int (*RetrieveInt)(uint16_t /*RuleID*/);
typedef BOOL (*CheckDep)(uint16_t /*RuleID*/, const int* const /* ruleStat */);
typedef void (*RetrieveCaption)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint8_t /*page*/);
typedef void (*RuleWrite)(const char** /*ruleTxt*/);
typedef void (*RuleRead)(char** /*ruleTxt*/);
typedef int (*ConfSave)(const char* const /*filename*/);

#ifdef GRAPHIC_EXPORTS
extern RetrieveTxt getRuleName;
extern RetrieveTxt getRuleDescription;
extern RetrieveTxtIndex getRuleTxt;
extern RetrieveInt getRule;
extern RetrieveInt getRuleSize;
extern CheckDep reqFailed;
extern RetrieveCaption getPageCaption;
extern RuleWrite storeRule;
extern RuleRead exportRule;
extern ConfSave saveConfigFile;
extern const char* digit;
extern std::string conffile;
#endif

EXPORT void setfunc(
	RetrieveTxt fpGetRuleName, RetrieveTxt fpGetRuleDesc, RetrieveTxtIndex fpGetRuleTxt,
	RetrieveInt fpGetRule, RetrieveInt fpGetRuleSize, CheckDep fpReqFailed,
	RetrieveCaption fpGetPageCaption, RuleWrite fpStoreRule, RuleRead fpExportRule,
	ConfSave fpSaveConfigFile, const char* pDigit);

EXPORT void setconffile(const char* filename);

}
}
