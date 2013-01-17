#pragma once

#include <cstdint>
#include <Windows.h>

void LoadFileInResource(int name, int type, DWORD& size, const uint8_t*& data);
