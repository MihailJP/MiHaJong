#pragma once

#if !defined(_WIN32)
#define EXPORT /* */
#elif defined(GRAPHIC_EXPORTS)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
