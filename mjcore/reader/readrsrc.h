#pragma once

#include <cstdint>
#include <windows.h>

void LoadFileInResource(int name, int type, DWORD& size, const uint8_t*& data);
