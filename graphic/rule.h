#ifndef GRAPHIC_RULE_H
#define GRAPHIC_RULE_H

#include <Windows.h>
#include <cstdint>
#include "../mjcore/strcode.h"
#include "exports.h"

namespace mihajong_graphic {
namespace rules {

typedef void (*RetrieveTxt)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint16_t /*RuleID*/);
typedef void (*RetrieveTxtIndex)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint16_t /*RuleID*/, uint8_t /*index*/);
typedef int (*RetrieveInt)(uint16_t /*RuleID*/);
typedef BOOL (*CheckDep)(uint16_t /*RuleID*/, const int* const /* ruleStat */);
typedef void (*RetrieveCaption)(LPTSTR const /*txt*/, unsigned /*bufsize*/, uint8_t /*page*/);

#ifdef GRAPHIC_EXPORTS
extern RetrieveTxt getRuleName;
extern RetrieveTxt getRuleDescription;
extern RetrieveTxtIndex getRuleTxt;
extern RetrieveInt getRule;
extern RetrieveInt getRuleSize;
extern CheckDep reqFailed;
extern RetrieveCaption getPageCaption;
#endif

EXPORT void setfunc(
	RetrieveTxt fpGetRuleName, RetrieveTxt fpGetRuleDesc, RetrieveTxtIndex fpGetRuleTxt,
	RetrieveInt fpGetRule, RetrieveInt fpGetRuleSize, CheckDep fpReqFailed,
	RetrieveCaption fpGetPageCaption);

}
}

#endif
