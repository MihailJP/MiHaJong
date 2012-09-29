#ifndef RULETBL_H
#define RULETBL_H

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <array>
#include <sstream>
#include <fstream>
#include <exception>
#include <Windows.h>
#include "logging.h"
#include "resource.h"
#include "reader/readrsrc.h"
#include "reader/csv2arry.h"
#include "reader/ini2map.h"
#endif
#include "mjexport.h"
#include "nmrules.h"

#ifdef MJCORE_EXPORTS
typedef std::map<std::string, int8_t> RULETBL;
#endif

#ifdef MJCORE_EXPORTS

enum gameTypeID : uint8_t { // 卓の種類(四麻、三麻)指定用
	Yonma = 0x01, Sanma = 0x02, Sanma4 = 0x04, SanmaS = 0x08,
	AllSanma = 0x0e, SanmaT = 0x0a, SanmaX = 0x06
};

class RuleData {
private:
	static char ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
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
	static const char digit[];
public:
	__declspec(dllexport) static void configinit();
	__declspec(dllexport) static void storeRule(const char** ruleTxt);
	__declspec(dllexport) static void exportRule(char** ruleTxt);
	__declspec(dllexport) static void getRuleName(char* const txt, int bufsize, int RuleID);
	__declspec(dllexport) static void getRuleDescription(char* const txt, int bufsize, int RuleID);
	__declspec(dllexport) static void getRuleTxt(char* const txt, int bufsize, int RuleID, int index);
	static std::string getRuleItemTag(int RuleID, int index);
	static std::string getRuleItemTag(std::string RuleTag, int index);
	static std::string chkRule(std::string RuleTag);
	static bool chkRule(std::string RuleTag, std::string Expectation);
	__declspec(dllexport) static int chkRule_hsp(const char* const RuleTag, const char* const Expectation);
	static bool chkRuleApplied(std::string RuleTag);
	inline static uint8_t getRule(std::string RuleTag) {return Rules[RuleTag];}
	static uint8_t getRule(int RuleID);
	__declspec(dllexport) static int getRuleSize(int RuleID);
	__declspec(dllexport) static int loadConfigFile(const char* const filename);
	__declspec(dllexport) static int saveConfigFile(const char* const filename);
};

__declspec(dllexport) int getRule(int RuleID);
#endif

#endif
