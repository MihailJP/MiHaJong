#ifndef EXCEPT_H
#define EXCEPT_H

#ifdef MJCORE_EXPORTS
#define WINDOWS_LEAN_AND_MEAN
#define WINDOWS_EXTRA_MEAN
#include <Windows.h>
#include <string.h>
#endif
#include "mjexport.h"

#define STRINGBUF 1024u
EXPORT_STRUCT ErrorInfo {
	char msg[STRINGBUF];
	char file[STRINGBUF];
	int line;
	char func[STRINGBUF];
};

#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<ErrorInfo>::value, "ErrorInfo is not POD");
#endif

#ifdef MJCORE_EXPORTS
extern ErrorInfo errorInfo;
extern const ULONG_PTR errorInfoPtr[1];
#define Raise(exceptionCode,message) strcpy_s(errorInfo.msg, STRINGBUF, message); \
	strcpy_s(errorInfo.file, STRINGBUF, __FILE__); errorInfo.line = __LINE__; \
	strcpy_s(errorInfo.func, STRINGBUF, __FUNCTION__);  \
	RaiseException(exceptionCode, EXCEPTION_NONCONTINUABLE, 1, errorInfoPtr)
#endif

#define EXCEPTION_MJCORE 0xc8000000
#define EXCEPTION_MJCORE_INVALID_ARGUMENT 0xc800005
#define EXCEPTION_MJCORE_OVERFLOW 0xc800011
#define EXCEPTION_MJCORE_DECOMPRESSION_FAILURE 0xc800101
#define EXCEPTION_MJCORE_HASH_MISMATCH 0xc800102

#endif
