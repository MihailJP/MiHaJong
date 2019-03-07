#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include <cstdint>
#include <string>
#include "../common/strcode.h"
#include "exports.h"

namespace mihajong_graphic {
namespace rules {

typedef void (*RetrieveTxt)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint16_t /*RuleID*/);
typedef void (*RetrieveTxtIndex)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint16_t /*RuleID*/, uint8_t /*index*/);
typedef int (*RetrieveInt)(uint16_t /*RuleID*/);
typedef std::size_t (*RetrieveSize)(uint16_t /*RuleID*/);
#ifdef _WIN32
typedef BOOL (*CheckDep)(uint16_t /*RuleID*/, const int* const /* ruleStat */);
#else /*_WIN32*/
typedef bool (*CheckDep)(uint16_t /*RuleID*/, const int* const /* ruleStat */);
#endif /*_WIN32*/
typedef void (*RetrieveCaption)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint8_t /*page*/);
typedef void (*RuleWrite)(const char** /*ruleTxt*/);
typedef void (*RuleRead)(char** /*ruleTxt*/);
typedef int (*ConfSave)(const char* const /*filename*/);
#ifdef _WIN32
typedef BOOL (*CheckRule)(const char* /*RuleTag*/, const char* /*Expectation*/);
#else /*_WIN32*/
typedef bool (*CheckRule)(const char* /*RuleTag*/, const char* /*Expectation*/);
#endif /*_WIN32*/
typedef unsigned int (*RetrieveUInt)(uint16_t /*RuleID*/);
typedef const char* (*RetrieveStr)(uint16_t /*RuleID*/);
typedef void (*SetStr)(uint16_t /*RuleID*/, const char* /*str*/);

#ifdef GRAPHIC_EXPORTS
extern RetrieveTxt getRuleName;
extern RetrieveTxt getRuleDescription;
extern RetrieveTxtIndex getRuleTxt;
extern RetrieveInt getRule;
extern RetrieveSize getRuleSize;
extern CheckDep reqFailed;
extern RetrieveCaption getPageCaption;
extern RuleWrite storeRule;
extern RuleRead exportRule;
extern ConfSave saveConfigFile;
extern CheckRule chkRule;
extern const char* digit;
extern std::string conffile;
#endif

EXPORT void setfunc(
	RetrieveTxt fpGetRuleName, RetrieveTxt fpGetRuleDesc, RetrieveTxtIndex fpGetRuleTxt,
	RetrieveInt fpGetRule, RetrieveSize fpGetRuleSize, CheckDep fpReqFailed,
	RetrieveCaption fpGetPageCaption, RuleWrite fpStoreRule, RuleRead fpExportRule,
	ConfSave fpSaveConfigFile, CheckRule fpChkRule, const char* pDigit);

EXPORT void setconffile(const char* filename);

}

namespace preferences {

#ifdef GRAPHIC_EXPORTS
extern bool blackTile;
extern std::string serverAddress;
std::string serverIP();
#endif

EXPORT void useBlackTile(bool);
EXPORT void setServerAddr(const char*);

}
}
