#include "ruletbl.h"

static char ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
static uint8_t Rules[RULESIZE];

void parseRule() { // ���[���ݒ�𐔒l�ɕϊ�
	debug("���[���ݒ�𐔒l�z��ɕϊ����܂��B");
	for (int i = 0; i < RULESIZE; i++) {
		std::string::size_type idx = std::string("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").find(
			ruleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] );
		Rules[i] = ((idx == std::string::npos) ? 0 : (uint8_t)idx);
	}
}

__declspec(dllexport) void storeRule(const char** ruleTxt) { // HSP��C++ ���[���ݒ�]��
	debug("HSP��C++ ���[���]��");
	for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++)
		memcpy(ruleConf[i], ruleTxt[i], RULE_IN_LINE);
	parseRule();
	info("�ݒ肪���[�h����܂����B");
}

__declspec(dllexport) void exportRule(char** ruleTxt) { // C++��HSP ���[���ݒ�]��
	debug("C++��HSP ���[���]��");
	for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++)
		memcpy(ruleTxt[i], ruleConf[i], RULE_IN_LINE);
}

__declspec(dllexport) int getRule(int RuleID) { // ���[���ݒ���擾����
	return (int)Rules[RuleID];
}
extern "C" inline MJCORE uint8_t getRule(RuleCode RuleID) { // ���[���ݒ���擾����
	return Rules[(int)RuleID];
}
