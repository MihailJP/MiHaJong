#pragma once

#include <cstdint>
#include <functional>
#include "mjexport.h"
#include "../common/strcode.h"
#include "conffile/conffile.h"
#include "../common/scrmode.h"

#ifdef MJCORE_EXPORTS

class RuleData {
public:
	static ConfigFile::ConfigFile confFile;
	static void configinit();
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
	static std::size_t getRuleSize(uint16_t RuleID);
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
public: /* Monostate class: cannot be instantiated */
	RuleData() = delete;
	RuleData(const RuleData&) = delete;
	RuleData& operator= (const RuleData&) = delete;
	~RuleData() = delete;
};

int getRule(int RuleID);
#endif

MJCORE void getWindowSize(unsigned* width, unsigned* height, ScreenMode::ScreenMode* fullscreen, unsigned* monitor);
