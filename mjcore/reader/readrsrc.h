#pragma once

#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#else /*_WIN32*/
#include <cstddef>
#endif /*_WIN32*/

#ifdef _WIN32
void LoadFileInResource(int name, int type, DWORD& size, const uint8_t*& data);
#else /*_WIN32*/
void LoadFileInResource(int name, int type, size_t& size, const uint8_t*& data);
#endif /*_WIN32*/
