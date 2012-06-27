#ifndef MJCORE_H
#define MJCORE_H

#include "ruletbl.h"

extern RULETBL ruleList;

__declspec(dllexport) void setRule(int* ruleListRaw);

#endif
