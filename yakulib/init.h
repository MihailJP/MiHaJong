#ifndef INIT_H
#define INIT_H

#include <list>
#include "ykclass.h"

void createYakuCatalog(std::list<Yaku> yakuCatalog);
__declspec(dllexport) void* initYaku();

#endif
