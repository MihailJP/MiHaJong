#include "ruletbl.h"

#include <lua.hpp>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <fstream>
#include <exception>
#include <Windows.h>
#include <cassert>
#include "logging.h"
#include "decomp.h"
#include "reader/readrsrc.h"
#include "../common/strcode.h"
#include "gametbl.h"
#include "../common/nmrules.h"

// -------------------------------------------------------------------------

typedef std::map<std::string, int8_t> RULETBL;

// -------------------------------------------------------------------------

template class ConfigData<RULESIZE, RULES_IN_PAGE, RULE_IN_LINE>;
class RuleConfigData : public ConfigData<RULESIZE, RULES_IN_PAGE, RULE_IN_LINE> {
	friend class ReqChecker;
private:
	void configinit_csv();
	void configinit_ini();
public:
	bool reqFailed(uint16_t ruleID, const int* const ruleStat);
};
namespace {
	RuleConfigData ruleTableData;
}

void RuleConfigData::configinit_csv() { // コンフィグ用CSVを読み込む
	Compressed::file_confitem_csv* csvfile = new Compressed::file_confitem_csv();
	ConfigData::configinit_csv(csvfile);
	delete csvfile;
	assert(inverse_ruletags["game_length"]["east_wind_game"] == 1);
	assert(ruletags["game_length"][1] == "east_wind_game");
}

void RuleConfigData::configinit_ini() { // コンフィグ文字列変換用INIを読み込む
	Compressed::file_confitem_ini* inifile = new Compressed::file_confitem_ini();
	ConfigData::configinit_ini(inifile);
	delete inifile;
}

// -------------------------------------------------------------------------

template class ConfigData<PREFERENCE_ITEMS, RULES_IN_PAGE, RULE_IN_LINE>;
class PreferenceData : public ConfigData<PREFERENCE_ITEMS, RULES_IN_PAGE, RULE_IN_LINE> {
	friend class ReqChecker;
private:
	void configinit_csv();
	void configinit_ini();
public:
	PreferenceData() : ConfigData<PREFERENCE_ITEMS, RULES_IN_PAGE, RULE_IN_LINE>(_T("preferences")) {}
};
namespace {
	PreferenceData preferenceTableData;
}

void PreferenceData::configinit_csv() { // コンフィグ用CSVを読み込む
	Compressed::file_prefitem_csv* csvfile = new Compressed::file_prefitem_csv();
	ConfigData::configinit_csv(csvfile);
	delete csvfile;
}

void PreferenceData::configinit_ini() { // コンフィグ文字列変換用INIを読み込む
	Compressed::file_prefitem_ini* inifile = new Compressed::file_prefitem_ini();
	ConfigData::configinit_ini(inifile);
	delete inifile;
}

// -------------------------------------------------------------------------

class ReqChecker {
private:
	lua_State* myState;
	static int check(lua_State* L);
	static const int* ourRuleStat;
	ReqChecker();
	~ReqChecker();
	ReqChecker(const ReqChecker&) {throw;}
	ReqChecker& operator=(const ReqChecker&) {throw;}
public:
	bool reqFailed (const std::string& expression, const int* const ruleStat);
	static ReqChecker* instantiate();
};

ReqChecker* ReqChecker::instantiate() {
	static ReqChecker reqChecker;
	return &reqChecker;
}

ReqChecker::ReqChecker () {
	myState = luaL_newstate();
	lua_register(myState, "chk", check);
}
ReqChecker::~ReqChecker () {
	lua_close(myState);
}
const int* ReqChecker::ourRuleStat = nullptr;

int ReqChecker::check (lua_State* L) {
	int n = lua_gettop(L);
	if (n < 2) luaL_error(L, "Not enough arguments");
	const char* ruleTag = lua_tostring(L, 1);
	const char* itemTag = lua_tostring(L, 2);
	//lua_pushboolean(L, chkRule(ruleTag, itemTag));
	if (ruleTableData.inverse_nametbl.find(ruleTag) == ruleTableData.inverse_nametbl.end())
		return luaL_error(L, "Unrecognized rule key: %s", ruleTag);
	else if (ruleTableData.inverse_ruletags[ruleTag].find(itemTag) == ruleTableData.inverse_ruletags[ruleTag].end())
		return luaL_error(L, "Unrecognized rule value: %s", itemTag);
	lua_pushboolean(L, ourRuleStat[ruleTableData.inverse_nametbl[ruleTag]] == ruleTableData.inverse_ruletags[ruleTag][itemTag]);
	return 1;
}

bool ReqChecker::reqFailed(const std::string& expression, const int* const ruleStat) {
	if (expression.empty()) {
		return false;
	}
	else {
		ourRuleStat = ruleStat;
		std::string expr = std::string("result = ") + expression;
		if (luaL_dostring(myState, expr.c_str())) return true;
		lua_getglobal(myState, "result");
		bool ans = lua_toboolean(myState, -1);
		lua_pop(myState, 1);
		return ans;
	}
}

// -------------------------------------------------------------------------

bool RuleConfigData::reqFailed(uint16_t ruleID, const int* const ruleStat) {
	return ReqChecker::instantiate()->reqFailed(getRuleMaskExpr(nametbl[ruleID]), ruleStat);
}

// -------------------------------------------------------------------------

__declspec(dllexport) void RuleData::configinit() { // コンフィグ用CSVを読み込む
	ruleTableData.configinit();
	preferenceTableData.configinit();
}

__declspec(dllexport) void RuleData::storeRule(const char** ruleTxt) { // UI→Core ルール設定転送
	ruleTableData.storeRule(ruleTxt);
}
__declspec(dllexport) void RuleData::exportRule(char** ruleTxt) { // Core→UI ルール設定転送
	ruleTableData.exportRule(ruleTxt);
}

__declspec(dllexport) void RuleData::storePref(const char** ruleTxt) { // UI→Core 環境設定転送
	preferenceTableData.storeRule(ruleTxt);
}
__declspec(dllexport) void RuleData::exportPref(char** ruleTxt) { // Core→UI 環境設定転送
	preferenceTableData.exportRule(ruleTxt);
}

std::string RuleData::chkRule(std::string RuleTag) { // ルール設定タグを取得する
	return ruleTableData.chkRule(RuleTag);
}
const char* RuleData::chkRule(const char* RuleTag) { // ルール設定タグを取得する
	static std::string ruledat;
	ruledat = chkRule(std::string(RuleTag));
	return ruledat.c_str();
}
bool RuleData::chkRule(std::string RuleTag, std::string Expectation) { // ルール設定
	return ruleTableData.chkRule(RuleTag, Expectation);
}
BOOL RuleData::chkRule(const char* RuleTag, const char* Expectation) { // ルール設定
	return chkRule(std::string(RuleTag), std::string(Expectation)) ? TRUE : FALSE;
}
bool RuleData::chkRuleApplied(std::string RuleTag) { // ルール設定
	return ruleTableData.chkRuleApplied(RuleTag);
}
int RuleData::getRule(std::string RuleTag) {
	return ruleTableData.getRule(RuleTag);
}
int RuleData::getRule(uint16_t RuleID) { // ルール設定を取得する(旧仕様)
	return ruleTableData.getRule(RuleID);
}
int RuleData::getPreference(std::string RuleTag) {
	return preferenceTableData.getRule(RuleTag);
}
int RuleData::getPreference(uint16_t RuleID) { // 環境設定を取得する(旧仕様)
	return preferenceTableData.getRule(RuleID);
}
const char* RuleData::getPreferenceRawStr(uint16_t RuleID) { // 環境設定を取得する(旧仕様)
	static std::string ruletag;
	ruletag = preferenceTableData.chkRule(RuleID);
	return ruletag.c_str();
}
__declspec(dllexport) int RuleData::getRuleSize(uint16_t RuleID) { // ルール項目のアイテム数
	return ruleTableData.getRuleSize(RuleID);
}
__declspec(dllexport) int RuleData::getPreferenceSize(uint16_t RuleID) { // 環境項目のアイテム数
	return preferenceTableData.getRuleSize(RuleID);
}

__declspec(dllexport) void RuleData::getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	ruleTableData.getRuleName(txt, bufsize, RuleID);
}
__declspec(dllexport) void RuleData::getPreferenceName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	preferenceTableData.getRuleName(txt, bufsize, RuleID);
}

__declspec(dllexport) void RuleData::getRuleDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	ruleTableData.getRuleDescription(txt, bufsize, RuleID);
}
__declspec(dllexport) void RuleData::getPreferenceDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	preferenceTableData.getRuleDescription(txt, bufsize, RuleID);
}

std::string RuleData::getRuleItemTag(uint16_t RuleID, int index) {
	return ruleTableData.getRuleItemTag(RuleID, index);
}
std::string RuleData::getRuleItemTag(std::string RuleTag, int index) {
	return ruleTableData.getRuleItemTag(RuleTag, index);
}

__declspec(dllexport) void RuleData::getRuleTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index) {
	ruleTableData.getRuleTxt(txt, bufsize, RuleID, index);
}
__declspec(dllexport) void RuleData::getPreferenceTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index) {
	preferenceTableData.getRuleTxt(txt, bufsize, RuleID, index);
}

__declspec(dllexport) int RuleData::loadConfigFile(const char* const filename) {
	return ruleTableData.loadConfigFile(filename);
}
__declspec(dllexport) int RuleData::saveConfigFile(const char* const filename) {
	return ruleTableData.saveConfigFile(filename);
}

__declspec(dllexport) int RuleData::loadPreferenceFile(const char* const filename) {
	return preferenceTableData.loadConfigFile(filename);
}
__declspec(dllexport) int RuleData::savePreferenceFile(const char* const filename) {
	return preferenceTableData.saveConfigFile(filename);
}

std::string RuleData::getRuleMaskExpr(const std::string& RuleTag) {
	return ruleTableData.getRuleMaskExpr(RuleTag);
}

__declspec(dllexport) BOOL RuleData::reqFailed(uint16_t RuleID, const int* const ruleStat) {
	return ruleTableData.reqFailed(RuleID, ruleStat) ? TRUE : FALSE;
}

void RuleData::forEachRule(std::function<void (std::string, std::string)> f) {
	ruleTableData.forEachRule(f);
}

__declspec(dllexport) void RuleData::getPageCaption(LPTSTR const caption, unsigned bufsize, uint8_t page) {
	ruleTableData.getPageCaption(caption, bufsize, page);
}

const char* RuleData::ruleDigit() {
	return ruleTableData.ruleDigit();
}

__declspec(dllexport) unsigned RuleData::getPreferenceInputSize(uint16_t RuleID) {
	return preferenceTableData.getRuleStrBufLen(RuleID);
}

void RuleData::setPreferenceFreeStr(uint16_t RuleID, const char* str) {
	preferenceTableData.setFreeStr(RuleID, str);
}

// -------------------------------------------------------------------------
