#include "mjlib.h"

static RULETBL ruleList;

// ƒ‹[ƒ‹İ’è‚ğ“ü—Í
__declspec(dllexport) void setRule(int* ruleListRaw) {
	for (int i = 0; i < RULESIZE; i++) ruleList[i] = ruleListRaw[i];
	return;
}
