#include "mjlib.h"

static RULETBL ruleList;

// ���[���ݒ�����
__declspec(dllexport) void setRule(int* ruleListRaw) {
	for (int i = 0; i < RULESIZE; i++) ruleList[i] = ruleListRaw[i];
	return;
}
