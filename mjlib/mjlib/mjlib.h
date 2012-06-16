#ifndef MJLIB_H
#define MJLIB_H

#include "ruletbl.h"

extern RULETBL ruleList;

__declspec(dllexport) void setRule(int* ruleListRaw);

#endif
