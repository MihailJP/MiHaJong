#include "ruletbl.h"

struct GameTable;
extern GameTable GameStat;
inline bool chkGameType(const GameTable* const gameStat, gameTypeID gameType);

char RuleData::ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
RULETBL RuleData::Rules;
std::array<std::string, RULESIZE> RuleData::nametbl;
std::vector<std::vector<std::string> > RuleData::confdat;

__declspec(dllexport) void RuleData::configinit() { // コンフィグ用CSVを読み込む
	DWORD size = 0; const uint8_t* csv = NULL;
	LoadFileInResource(IDR_CSV_TABL1, CSV_TABLE, size, csv);
	CSVReader::parsecsv(confdat, reinterpret_cast<const char*>(csv));

	for (auto k = confdat.begin(); k != confdat.end(); k++) // 名前テーブル
		nametbl[std::atoi((*k)[0].c_str())] = (*k)[8];
}

void RuleData::parseRule() { // ルール設定を数値に変換
	debug("ルール設定を連想配列に変換します。");
	for (int i = 0; i < RULESIZE; i++) {
		std::string::size_type idx = std::string("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").find(
			ruleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] );
		if (!(nametbl[i].empty()))
			Rules[nametbl[i]] = ((idx == std::string::npos) ? 0 : (uint8_t)idx);
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

uint8_t RuleData::getRule(std::string RuleTag) { // ルール設定を取得する
	return Rules[RuleTag];
}
uint8_t RuleData::getRule(RuleCode RuleID) { // ルール設定を取得する
	return Rules[nametbl[RuleID]];
}
__declspec(dllexport) int getRule(int RuleID) { // ルール設定を取得する
	return (int)RuleData::getRule((RuleCode)RuleID);
}

__declspec(dllexport) void RuleData::getRuleName(char* const txt, int bufsize, int RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (std::atoi((*k)[0].c_str()) != RuleID) continue;
		if ((chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) ||
			(chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))))
			strcpy_s(txt, bufsize, ((*k)[9]).c_str()); return;
	}
	strcpy_s(txt, bufsize, "");
}

__declspec(dllexport) void RuleData::getRuleDescription(char* const txt, int bufsize, int RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (std::atoi((*k)[0].c_str()) != RuleID) continue;
		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) {
			strcpy_s(txt, bufsize, ((*k)[10]).c_str()); return;
		}
		else if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))) {
			if (chkGameType(&GameStat, SanmaS)) strcpy_s(txt, bufsize, "数牌三麻では設定できません");
			else if (chkGameType(&GameStat, SanmaX)) strcpy_s(txt, bufsize, "三人打ちでは設定できません");
			else if (chkGameType(&GameStat, Yonma)) strcpy_s(txt, bufsize, "四人打ちでは設定できません");
			else strcpy_s(txt, bufsize, "");
			return;
		}
	}
	strcpy_s(txt, bufsize, "");
}

__declspec(dllexport) void RuleData::getRuleTxt(char* const txt, int bufsize, int RuleID, int index) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (std::atoi((*k)[0].c_str()) != RuleID) continue;
		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) {
			if ((*k).size() > (unsigned int)(11 + index))
				strcpy_s(txt, bufsize, ((*k)[11 + index]).c_str());
			else strcpy_s(txt, bufsize, "");
			return;
		}
		else if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))) {
			strcpy_s(txt, bufsize, "Ｎ／Ａ");
			return;
		}
	}
	strcpy_s(txt, bufsize, "");
}
