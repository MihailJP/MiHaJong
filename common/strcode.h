#pragma once

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#else /* _WIN32 */
#include <cstring>
#include <cwchar>
#include <clocale>
#ifdef UNICODE
#define _T(str) L##str
typedef wchar_t TCHAR;
typedef const wchar_t* LPCTSTR;
#else
#define _T(str) str
typedef char TCHAR;
typedef char* LPTSTR;
typedef const char* LPCTSTR;
#endif
#endif /* _WIN32 */
#include <string>
#include <sstream>

namespace CodeConv {

#ifndef _WIN32
inline void setCP(unsigned int CodePage) {
	if (CodePage == 932u) /* Japanese SJIS code page */
		setlocale(LC_CTYPE, "ja_JP.cp932") || /* Linux */
		setlocale(LC_CTYPE, "ja_JP.sjis"); /* returns nullptr if failed */
	else if (CodePage == 65001u) /* UTF-8 code page */
		setlocale(LC_CTYPE, "ja_JP.utf8"); /* Linux */
}

const unsigned CP_UTF8 = 65501u;
const unsigned CP_ACP = 932u;
#endif /* _WIN32 */

inline std::wstring NarrowToWide(unsigned int CodePage, std::string str) {
#ifdef _WIN32
	int bufsize = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* buf = new wchar_t[bufsize];
	MultiByteToWideChar(CodePage, 0, str.c_str(), -1, buf, bufsize);
#else /* _WIN32 */
	const std::string origLocale(setlocale(LC_CTYPE, nullptr)); /* backup locale */
	setCP(CodePage);
	mbstate_t mbStat; memset(&mbStat, 0, sizeof mbStat);
	const char* srcPtr = str.c_str();
	const size_t bufsize = mbsrtowcs(nullptr, &srcPtr, 0, &mbStat);
	wchar_t* buf = new wchar_t[bufsize + 1 /* Do not forget the trailing null */];
	srcPtr = str.c_str();
	mbsrtowcs(buf, &srcPtr, bufsize, &mbStat);
	setlocale(LC_CTYPE, origLocale.c_str()); /* restore locale */
#endif /* _WIN32 */
	std::wstring ans(buf); delete[] buf;
	return ans;
}
inline std::string WideToNarrow(unsigned int CodePage, std::wstring str) {
#ifdef _WIN32
	int bufsize = WideCharToMultiByte(CodePage, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* buf = new char[bufsize];
	WideCharToMultiByte(CodePage, 0, str.c_str(), -1, buf, bufsize, nullptr, nullptr);
#else /* _WIN32 */
	const std::string origLocale(setlocale(LC_CTYPE, nullptr)); /* backup locale */
	setCP(CodePage);
	mbstate_t mbStat; memset(&mbStat, 0, sizeof mbStat);
	const wchar_t* srcPtr = str.c_str();
	const size_t bufsize = wcsrtombs(nullptr, &srcPtr, 0, &mbStat);
	char* buf = new char[bufsize + 1 /* Do not forget the trailing null */];
	srcPtr = str.c_str();
	wcsrtombs(buf, &srcPtr, bufsize, &mbStat);
	setlocale(LC_CTYPE, origLocale.c_str()); /* restore locale */
#endif /* _WIN32 */
	std::string ans(buf); delete[] buf;
	return ans;
}
inline std::wstring UTF8toWIDE(std::string str) {
	return NarrowToWide(CP_UTF8, str);
}
inline std::wstring ANSItoWIDE(std::string str) {
	return NarrowToWide(CP_ACP, str);
}
inline std::string WIDEtoUTF8(std::wstring str) {
	return WideToNarrow(CP_UTF8, str);
}
inline std::string WIDEtoANSI(std::wstring str) {
	return WideToNarrow(CP_ACP, str);
}
typedef std::basic_string<TCHAR> tstring;
typedef std::basic_ostringstream<TCHAR> tostringstream;
#ifdef UNICODE
inline std::string toANSI(std::wstring str) {return WIDEtoANSI(str);}
inline std::string toUTF8(std::wstring str) {return WIDEtoUTF8(str);}
inline std::wstring fromUTF8(std::string str) {return UTF8toWIDE(str);}
inline std::wstring DecodeStr(std::string str) {return UTF8toWIDE(str);}
inline std::string EncodeStr(std::wstring str) {return WIDEtoUTF8(str);}
inline std::wstring EnsureTStr(std::string str) {return ANSItoWIDE(str);}
inline std::wstring EnsureTStr(std::wstring str) {return str;}
#else
inline std::string toANSI(std::string str) {return str;}
inline std::string toUTF8(std::string str) {return WIDEtoUTF8(ANSItoWIDE(str));}
inline std::string fromUTF8(std::string str) {return WIDEtoANSI(UTF8toWIDE(str));}
inline std::string DecodeStr(std::string str) {return str;}
inline std::string EncodeStr(std::string str) {return str;}
inline std::string EnsureTStr(std::string str) {return str;}
inline std::string EnsureTStr(std::wstring str) {return WIDEtoANSI(str);}
#endif

}
