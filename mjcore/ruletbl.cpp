#include "ruletbl.h"

char RuleData::ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
uint8_t RuleData::Rules[RULESIZE];
std::vector<std::vector<std::string> > RuleData::confdat;

__declspec(dllexport) void RuleData::configinit() { // コンフィグ用CSVを読み込む
	DWORD size = 0; const uint8_t* csv = NULL;
	LoadFileInResource(IDR_CSV_TABL1, CSV_TABLE, size, csv);
	CSVReader::parsecsv(confdat, reinterpret_cast<const char*>(csv));
}

void RuleData::parseRule() { // ルール設定を数値に変換
	debug("ルール設定を数値配列に変換します。");
	for (int i = 0; i < RULESIZE; i++) {
		std::string::size_type idx = std::string("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").find(
			ruleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] );
		Rules[i] = ((idx == std::string::npos) ? 0 : (uint8_t)idx);
	}
}

__declspec(dllexport) void RuleData::storeRule(const char** ruleTxt) { // HSP→C++ ルール設定転送
	debug("HSP→C++ ルール転送");
	for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++)
		memcpy(ruleConf[i], ruleTxt[i], RULE_IN_LINE);
	parseRule();
	info("設定がロードされました。");
}

__declspec(dllexport) void RuleData::exportRule(char** ruleTxt) { // C++→HSP ルール設定転送
	debug("C++→HSP ルール転送");
	for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++)
		memcpy(ruleTxt[i], ruleConf[i], RULE_IN_LINE);
}

uint8_t RuleData::getRule(RuleCode RuleID) { // ルール設定を取得する
	return Rules[RuleID];
}
__declspec(dllexport) int getRule(int RuleID) { // ルール設定を取得する
	return (int)RuleData::getRule((RuleCode)RuleID);
}
extern "C" inline MJCORE uint8_t getRule(RuleCode RuleID) { // ルール設定を取得する
	return RuleData::getRule(RuleID);
}
