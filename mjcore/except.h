#pragma once

#ifdef MJCORE_EXPORTS
#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#define WINDOWS_EXTRA_MEAN
#include <windows.h>
#endif /*_WIN32*/
#include <iostream>
#include <string.h>
#include "logging.h"
#endif
#include "mjexport.h"
#include "../common/strcode.h"

#define STRINGBUF 1024u
#define ADDRBUF 256u
EXPORT_STRUCT ErrorInfo {
	char msg[STRINGBUF];
	char file[STRINGBUF];
	int line;
	char func[STRINGBUF];
	int traceBack[ADDRBUF];
};

#ifdef MJCORE_EXPORTS
static_assert(std::is_trivially_copyable<ErrorInfo>::value, "ErrorInfo is not trivially copyable");
static_assert(std::is_standard_layout<ErrorInfo>::value, "ErrorInfo is not standard layout");
#endif

void StackTraceToArray();
#ifdef MJCORE_EXPORTS
extern ErrorInfo errorInfo;
#ifdef _WIN32
extern const ULONG_PTR errorInfoPtr[1];
#else /*_WIN32*/
extern const uintptr_t errorInfoPtr[1];
#endif /*_WIN32*/
#if defined(_MSC_VER)
#define setStruct(message) {\
	strcpy_s(errorInfo.msg, STRINGBUF, CodeConv::toANSI(message).c_str()); \
	strcpy_s(errorInfo.file, STRINGBUF, __FILE__); errorInfo.line = __LINE__; \
	strcpy_s(errorInfo.func, STRINGBUF, __FUNCTION__); \
	StackTraceToArray();}
#else
#define setStruct(message) {\
	strncpy(errorInfo.msg, CodeConv::toANSI(message).c_str(), STRINGBUF); \
	strncpy(errorInfo.file, __FILE__, STRINGBUF); errorInfo.line = __LINE__; \
	strncpy(errorInfo.func, __FUNCTION__, STRINGBUF);}
#endif
#ifdef _WIN32
#define Raise(exceptionCode,message) {fatal(message); \
	setStruct(message); \
	RaiseException(exceptionCode, EXCEPTION_NONCONTINUABLE, 1, errorInfoPtr);}
#define RaiseTolerant(exceptionCode,message) {error(message); \
	setStruct(message); \
	RaiseException(exceptionCode, 0, 1, errorInfoPtr);}
#else /*_WIN32*/
#define Raise(exceptionCode,message) {fatal(message); \
	setStruct(message); \
	std::cerr << __FILE__ << ' ' << __LINE__ << ':' << CodeConv::toANSI(message) << std::endl; \
	throw (unsigned long)exceptionCode;}
#define RaiseTolerant(exceptionCode,message) {error(message); \
	setStruct(message); \
	std::cerr << __FILE__ << ' ' << __LINE__ << ':' << CodeConv::toANSI(message) << std::endl; \
	throw (unsigned long)exceptionCode;}
#endif /*_WIN32*/
#endif


#define EXCEPTION_MJCORE                        0xc2580000
#define EXCEPTION_MJCORE_SUBSCRIPT_OUT_OF_RANGE 0xc2580004
#define EXCEPTION_MJCORE_INVALID_ARGUMENT       0xc2580005
#define EXCEPTION_MJCORE_INVALID_DATA           0xc2580007
#define EXCEPTION_MJCORE_OVERFLOW               0xc2580011
#define EXCEPTION_MJCORE_DECOMPRESSION_FAILURE  0xc2580101
#define EXCEPTION_MJCORE_HASH_MISMATCH          0xc2580102
