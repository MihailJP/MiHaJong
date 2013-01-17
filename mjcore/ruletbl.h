#pragma once

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <lua.hpp>
#endif
#include <string>
#include <map>
#include <set>
#include <array>
#ifdef MJCORE_EXPORTS
#include "reader/csv2arry.h"
#include "reader/ini2map.h"
#endif
#include "mjexport.h"
#include "nmrules.h"

#ifdef MJCORE_EXPORTS
typedef std::map<std::string, int8_t> RULETBL;
#endif

enum gameTypeID : uint8_t { // 卓の種類(四麻、三麻)指定用
	Yonma = 0x01, Sanma = 0x02, Sanma4 = 0x04, SanmaS = 0x08,
	AllSanma = 0x0e, SanmaT = 0x0a, SanmaX = 0x06
};

#ifdef MJCORE_EXPORTS

class RuleData {
private:
	class ReqChecker;
	static char ruleConf[RULE_LINES][RULE_IN_LINE + 1];
	static RULETBL Rules;
	static std::array<std::string, RULESIZE> nametbl;
	static CSVReader::CsvVecVec confdat;
	static INIParser::IniMapMap confdict;
	static void parseRule();
	static void configinit_csv();
	static void configinit_ini();
	static std::map<std::string, unsigned int> inverse_nametbl;
	static std::map<std::string, std::vector<std::string> > ruletags;
	static std::map<std::string, std::map<std::string, unsigned int> > inverse_ruletags;
	static std::set<std::string> nonapplicable;
	static std::array<CodeConv::tstring, RULE_PAGES> pageCaption;
	static const char digit[];
public:
	__declspec(dllexport) static void configinit();
	__declspec(dllexport) static void storeRule(const char** ruleTxt);
	__declspec(dllexport) static void exportRule(char** ruleTxt);
	__declspec(dllexport) static void getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID);
	__declspec(dllexport) static void getRuleDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID);
	__declspec(dllexport) static void getRuleTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index);
	static std::string getRuleItemTag(uint16_t RuleID, int index);
	static std::string getRuleItemTag(std::string RuleTag, int index);
	static std::string chkRule(std::string RuleTag);
	static bool chkRule(std::string RuleTag, std::string Expectation);
	static BOOL chkRule(const char* RuleTag, const char* Expectation);
	static bool chkRuleApplied(std::string RuleTag);
	inline static int getRule(std::string RuleTag) {return Rules[RuleTag];}
	static int getRule(uint16_t RuleID);
	__declspec(dllexport) static int getRuleSize(uint16_t RuleID);
	__declspec(dllexport) static int loadConfigFile(const char* const filename);
	__declspec(dllexport) static int saveConfigFile(const char* const filename);
	static std::string getRuleMaskExpr(const std::string& RuleTag);
	__declspec(dllexport) static BOOL reqFailed(uint16_t ruleID, const int* const ruleStat);
	__declspec(dllexport) static void getPageCaption(LPTSTR const caption, unsigned bufsize, uint8_t page);
	static const char* ruleDigit() {return digit;}
};

class RuleData::ReqChecker {
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

__declspec(dllexport) int getRule(int RuleID);
#endif
