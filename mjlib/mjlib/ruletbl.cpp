#include "ruletbl.h"

static char ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
using namespace std;

__declspec(dllexport) void getRule(char** ruleTxt) { // HSP→C++ ルール設定転送
	for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++)
		memcpy(ruleConf[i], ruleTxt[i], RULE_IN_LINE);
	info("設定がロードされました。");
}

__declspec(dllexport) void exportRule(char** ruleTxt) { // C++→HSP ルール設定転送
	for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++)
		memcpy(ruleTxt[i], ruleConf[i], RULE_IN_LINE);
}

__declspec(dllexport) int getRule(int RuleID) { // ルール設定を取得する
	string::size_type i = string("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").find(
		ruleConf[RuleID / RULE_IN_LINE][RuleID % RULE_IN_LINE]
		);
	if (i == string::npos) return 0;
	else return (int)i;
}
