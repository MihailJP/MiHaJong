#include "ruletbl.h"

#include <lua.hpp>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <fstream>
#include <exception>
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include <cassert>
#ifdef WITH_BOOST_REGEX
#include <boost/regex.hpp>
#define REGEX boost
#else /*WITH_BOOST_REGEX*/
#include <regex>
#define REGEX std
#endif /*WITH_BOOST_REGEX*/
#include "logging.h"
#include "decomp.h"
#include "reader/readrsrc.h"
#include "../common/strcode.h"
#include "gametbl.h"
#include "../common/nmrules.h"
#include "sound.h"
#include "func.h"

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

void RuleConfigData::configinit_csv() { // �R���t�B�O�pCSV��ǂݍ���
	Compressed::file_confitem_csv* csvfile = new Compressed::file_confitem_csv();
	ConfigData::configinit_csv(csvfile);
	delete csvfile;
	assert(inverse_ruletags["game_length"]["east_wind_game"] == 1);
	assert(ruletags["game_length"][1] == "east_wind_game");
}

void RuleConfigData::configinit_ini() { // �R���t�B�O������ϊ��pINI��ǂݍ���
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

void PreferenceData::configinit_csv() { // �R���t�B�O�pCSV��ǂݍ���
	Compressed::file_prefitem_csv* csvfile = new Compressed::file_prefitem_csv();
	ConfigData::configinit_csv(csvfile);
	delete csvfile;
}

void PreferenceData::configinit_ini() { // �R���t�B�O������ϊ��pINI��ǂݍ���
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

void RuleData::configinit() { // �R���t�B�O�pCSV��ǂݍ���
	ruleTableData.configinit();
}

void RuleData::preference_init() { // �R���t�B�O�pCSV��ǂݍ���
	preferenceTableData.configinit();
}

void RuleData::storeRule(const char** ruleTxt) { // UI��Core ���[���ݒ�]��
	ruleTableData.storeRule(ruleTxt);
}
void RuleData::exportRule(char** ruleTxt) { // Core��UI ���[���ݒ�]��
	ruleTableData.exportRule(ruleTxt);
}

void RuleData::storePref(const char** ruleTxt) { // UI��Core ���ݒ�]��
	preferenceTableData.storeRule(ruleTxt);
}
void RuleData::exportPref(char** ruleTxt) { // Core��UI ���ݒ�]��
	preferenceTableData.exportRule(ruleTxt);
}

std::string RuleData::chkRule(std::string RuleTag) { // ���[���ݒ�^�O���擾����
	return ruleTableData.chkRule(RuleTag);
}
const char* RuleData::chkRule(const char* RuleTag) { // ���[���ݒ�^�O���擾����
	static std::string ruledat;
	ruledat = chkRule(std::string(RuleTag));
	return ruledat.c_str();
}
std::string RuleData::chkPreference(std::string RuleTag) { // ���[���ݒ�^�O���擾����
	return preferenceTableData.chkRule(RuleTag);
}
const char* RuleData::chkPreference(const char* RuleTag) { // ���[���ݒ�^�O���擾����
	static std::string ruledat;
	ruledat = chkPreference(std::string(RuleTag));
	return ruledat.c_str();
}
bool RuleData::chkRule(std::string RuleTag, std::string Expectation) { // ���[���ݒ�
	return ruleTableData.chkRule(RuleTag, Expectation);
}
#ifdef _WIN32
BOOL RuleData::chkRule(const char* RuleTag, const char* Expectation) { // ���[���ݒ�
	return chkRule(std::string(RuleTag), std::string(Expectation)) ? TRUE : FALSE;
}
#else /*_WIN32*/
bool RuleData::chkRule(const char* RuleTag, const char* Expectation) { // ���[���ݒ�
	return chkRule(std::string(RuleTag), std::string(Expectation));
}
#endif /*_WIN32*/
bool RuleData::chkPreference(std::string RuleTag, std::string Expectation) { // ���[���ݒ�
	return preferenceTableData.chkRule(RuleTag, Expectation);
}
#ifdef _WIN32
BOOL RuleData::chkPreference(const char* RuleTag, const char* Expectation) { // ���[���ݒ�
	return chkPreference(std::string(RuleTag), std::string(Expectation)) ? TRUE : FALSE;
}
#else /*_WIN32*/
bool RuleData::chkPreference(const char* RuleTag, const char* Expectation) { // ���[���ݒ�
	return chkPreference(std::string(RuleTag), std::string(Expectation));
}
#endif /*_WIN32*/
bool RuleData::chkRuleApplied(std::string RuleTag) { // ���[���ݒ�
	return ruleTableData.chkRuleApplied(RuleTag);
}
int RuleData::getRule(std::string RuleTag) {
	return ruleTableData.getRule(RuleTag);
}
int RuleData::getRule(uint16_t RuleID) { // ���[���ݒ���擾����(���d�l)
	return ruleTableData.getRule(RuleID);
}
int RuleData::getPreference(std::string RuleTag) {
	return preferenceTableData.getRule(RuleTag);
}
int RuleData::getPreference(uint16_t RuleID) { // ���ݒ���擾����(���d�l)
	return preferenceTableData.getRule(RuleID);
}
const char* RuleData::getPreferenceRawStr(uint16_t RuleID) { // ���ݒ���擾����(���d�l)
	static std::string ruletag;
	ruletag = preferenceTableData.chkRule(RuleID);
	return ruletag.c_str();
}
int RuleData::getRuleSize(uint16_t RuleID) { // ���[�����ڂ̃A�C�e����
	return ruleTableData.getRuleSize(RuleID);
}
int RuleData::getPreferenceSize(uint16_t RuleID) { // �����ڂ̃A�C�e����
	return preferenceTableData.getRuleSize(RuleID);
}

void RuleData::getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	ruleTableData.getRuleName(txt, bufsize, RuleID);
}
void RuleData::getPreferenceName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	preferenceTableData.getRuleName(txt, bufsize, RuleID);
}

void RuleData::getRuleDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	ruleTableData.getRuleDescription(txt, bufsize, RuleID);
}
void RuleData::getPreferenceDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	preferenceTableData.getRuleDescription(txt, bufsize, RuleID);
}

std::string RuleData::getRuleItemTag(uint16_t RuleID, int index) {
	return ruleTableData.getRuleItemTag(RuleID, index);
}
std::string RuleData::getRuleItemTag(std::string RuleTag, int index) {
	return ruleTableData.getRuleItemTag(RuleTag, index);
}

void RuleData::getRuleTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index) {
	ruleTableData.getRuleTxt(txt, bufsize, RuleID, index);
}
void RuleData::getPreferenceTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index) {
	preferenceTableData.getRuleTxt(txt, bufsize, RuleID, index);
}

int RuleData::loadConfigFile(const char* const filename) {
	return ruleTableData.loadConfigFile(filename);
}
int RuleData::saveConfigFile(const char* const filename) {
	return ruleTableData.saveConfigFile(filename);
}

int RuleData::loadPreferenceFile(const char* const filename) {
	return preferenceTableData.loadConfigFile(filename);
}
int RuleData::savePreferenceFile(const char* const filename) {
	return preferenceTableData.saveConfigFile(filename);
}

std::string RuleData::getRuleMaskExpr(const std::string& RuleTag) {
	return ruleTableData.getRuleMaskExpr(RuleTag);
}

#ifdef _WIN32
BOOL RuleData::reqFailed(uint16_t RuleID, const int* const ruleStat) {
	return ruleTableData.reqFailed(RuleID, ruleStat) ? TRUE : FALSE;
}
#else /*_WIN32*/
bool RuleData::reqFailed(uint16_t RuleID, const int* const ruleStat) {
	return ruleTableData.reqFailed(RuleID, ruleStat);
}
#endif /*_WIN32*/

void RuleData::forEachRule(std::function<void (std::string, std::string)> f) {
	ruleTableData.forEachRule(f);
}

void RuleData::getPageCaption(LPTSTR const caption, unsigned bufsize, uint8_t page) {
	ruleTableData.getPageCaption(caption, bufsize, page);
}

const char* RuleData::ruleDigit() {
	return ruleTableData.ruleDigit();
}

unsigned RuleData::getPreferenceInputSize(uint16_t RuleID) {
	return preferenceTableData.getRuleStrBufLen(RuleID);
}

void RuleData::setPreferenceFreeStr(uint16_t RuleID, const char* str) {
	preferenceTableData.setFreeStr(RuleID, str);
}

void RuleData::applyPreference() {
	sound::util::setvolume();
}

// -------------------------------------------------------------------------

MJCORE void preferenceInit() {
	std::string preferenceFile = confpath::confPath() + "config.ini";
	RuleData::preference_init();
	const bool prefFileExists = exist(preferenceFile.c_str()); // �ݒ�t�@�C�������邩�ǂ������ׂ�
	if (prefFileExists) {
		RuleData::loadPreferenceFile(preferenceFile.c_str()); // �ݒ�t�@�C���ǂݍ���
	}
}

MJCORE void getWindowSize(unsigned* width, unsigned* height, bool* fullscreen) {
	std::string sizeConf(RuleData::chkPreference("scrsize"));
	REGEX::smatch matchDat;
	if (REGEX::regex_match(sizeConf, matchDat, REGEX::regex("scr_(\\d+)_(\\d+)"))) {
		*width = atoi(matchDat[1].str().c_str());
		*height = atoi(matchDat[2].str().c_str());
	} else {
		*width = 1024; *height = 768;
	}
	std::string fsConf(RuleData::chkPreference((std::string)"screen"));
	*fullscreen = (fsConf == std::string("fullscreen"));
}
