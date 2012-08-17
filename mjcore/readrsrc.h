#ifndef READRSRC_H
#define READRSRC_H

#include <cstdint>
#include <Windows.h>
#include <cassert>
#include "dllmain.h"

void LoadFileInResource(int name, int type, DWORD& size, const uint8_t*& data);

#endif
