#ifndef RULETBL_H
#define RULETBL_H

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <array>
#include "logging.h"
#include "resource.h"
#include "reader/readrsrc.h"
#include "reader/csv2arry.h"
#endif
#include "mjexport.h"

#define RULESIZE 640U
#define RULES_IN_PAGE 40U
#define RULE_IN_LINE 80U
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
	static std::vector<std::vector<std::string> > confdat;
	static void parseRule();
public:
	__declspec(dllexport) static void configinit();
	__declspec(dllexport) static void storeRule(const char** ruleTxt);
	__declspec(dllexport) static void exportRule(char** ruleTxt);
	__declspec(dllexport) static void getRuleName(char* const txt, int bufsize, int RuleID);
	__declspec(dllexport) static void getRuleDescription(char* const txt, int bufsize, int RuleID);
	__declspec(dllexport) static void getRuleTxt(char* const txt, int bufsize, int RuleID, int index);
	inline static uint8_t getRule(std::string RuleTag) {return Rules[RuleTag];}
	static uint8_t getRule(int RuleID);
};

__declspec(dllexport) int getRule(int RuleID);
#endif

#endif
