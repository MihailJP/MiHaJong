#pragma once

#ifdef USE_LIBSAFEC
#define restrict /**/
#include <cstddef>
#include <libsafec/safe_lib.h>
#include <libsafec/safe_str_lib.h>
#endif /* USE_LIBSAFEC */

#include <algorithm>
#include "strcode.h"

inline void tcsCat(LPTSTR dest, size_t destsz, LPCTSTR src) {
#if defined(_MSC_VER) || defined(USE_LIBSAFEC) || (!defined(_UNICODE) && defined(HAVE_STRCAT_S)) || (defined(_UNICODE) && defined(HAVE_WCSCAT_S))
	_tcscat_s(dest, destsz, src);
#else
	_tcsncat(dest, src, destsz - _tcslen(dest));
	dest[destsz - 1] = _T('\0');
#endif
}

inline void tcsCpy(LPTSTR dest, size_t destsz, LPCTSTR src) {
#if defined(_MSC_VER) || defined(USE_LIBSAFEC) || (!defined(_UNICODE) && defined(HAVE_STRCPY_S)) || (defined(_UNICODE) && defined(HAVE_WCSCPY_S))
	_tcscpy_s(dest, destsz, src);
#else
	_tcsncpy(dest, src, destsz);
	dest[destsz - 1] = _T('\0');
#endif
}

inline void strCpy(char* dest, size_t destsz, const char* src) {
#if defined(_MSC_VER) || defined(USE_LIBSAFEC) || defined(HAVE_STRCPY_S)
	strcpy_s(dest, destsz, src);
#else
	strncpy(dest, src, destsz);
	dest[destsz - 1] = _T('\0');
#endif
}

inline void memCpy(void* dest, size_t destsz, const void* src, size_t count) {
#if defined(_MSC_VER) || defined(USE_LIBSAFEC) || defined(HAVE_MEMCPY_S)
	memcpy_s(dest, destsz, src, count);
#else
	memcpy(dest, src, std::min(count, destsz));
#endif
}

inline void getEnv(size_t* len, char* buffer, size_t bufSize, const char* varName) {
	size_t lenBuf = 0;
#if defined(_MSC_VER) || defined(USE_LIBSAFEC) || defined(HAVE_GETENV_S)
	getenv_s(len ? len : &lenBuf, buffer, bufSize, varName);
#else
	if (getenv(varName)) {
		if (len) *len = strlen(getenv(varName)) + 1;
		strCpy(buffer, bufSize, getenv(varName));
	}
	buffer[bufSize - 1] = '\0';
#endif
}
