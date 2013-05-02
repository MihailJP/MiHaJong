#pragma once

#include "mjexport.h"
#include "../common/strcode.h"

#ifdef MJCORE_EXPORTS

class RuleData {
public:
	static void configinit();
	static void preference_init();
	static void storeRule(const char** ruleTxt);
	static void exportRule(char** ruleTxt);
	static void getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID);
	static void getRuleDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID);
	static void getRuleTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index);
	static std::string getRuleItemTag(uint16_t RuleID, int index);
	static std::string getRuleItemTag(std::string RuleTag, int index);
	static std::string chkRule(std::string RuleTag);
	static const char* chkRule(const char* RuleTag);
	static bool chkRule(std::string RuleTag, std::string Expectation);
#ifdef _WIN32
	static BOOL chkRule(const char* RuleTag, const char* Expectation);
#else /*_WIN32*/
	static bool chkRule(const char* RuleTag, const char* Expectation);
#endif /*_WIN32*/
	static bool chkRuleApplied(std::string RuleTag);
	static int getRule(std::string RuleTag);
	static int getRule(uint16_t RuleID);
	static int getRuleSize(uint16_t RuleID);
	static int loadConfigFile(const char* const filename);
	static int saveConfigFile(const char* const filename);
	static std::string getRuleMaskExpr(const std::string& RuleTag);
#ifdef _WIN32
	static BOOL reqFailed(uint16_t ruleID, const int* const ruleStat);
#else /*_WIN32*/
	static bool reqFailed(uint16_t ruleID, const int* const ruleStat);
#endif /*_WIN32*/
	static void getPageCaption(LPTSTR const caption, unsigned bufsize, uint8_t page);
	static const char* ruleDigit();
	static void forEachRule(std::function<void (std::string, std::string)> f);
public:
	static void storePref(const char** ruleTxt);
	static void exportPref(char** ruleTxt);
	static int loadPreferenceFile(const char* const filename);
	static int savePreferenceFile(const char* const filename);
	static void getPreferenceName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID);
	static void getPreferenceDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID);
	static void getPreferenceTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index);
	static int getPreference(std::string RuleTag);
	static int getPreference(uint16_t RuleID);
	static std::string chkPreference(std::string RuleTag);
	static const char* chkPreference(const char* RuleTag);
	static const char* getPreferenceRawStr(uint16_t RuleID);
	static int getPreferenceSize(uint16_t RuleID);
	static unsigned getPreferenceInputSize(uint16_t RuleID);
	static void setPreferenceFreeStr(uint16_t RuleID, const char* str);
	static void applyPreference();
	static bool chkPreference(std::string RuleTag, std::string Expectation);
#ifdef _WIN32
	static BOOL chkPreference(const char* RuleTag, const char* Expectation);
#else /*_WIN32*/
	static bool chkPreference(const char* RuleTag, const char* Expectation);
#endif /*_WIN32*/
};

int getRule(int RuleID);
#endif

MJCORE void preferenceInit();
MJCORE void getWindowSize(unsigned* width, unsigned* height, bool* fullscreen);

