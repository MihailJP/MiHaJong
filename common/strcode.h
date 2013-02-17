#pragma once

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <sstream>

namespace CodeConv {

inline std::wstring NarrowToWide(unsigned int CodePage, const std::string& str) {
	int bufsize = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* buf = new wchar_t[bufsize];
	MultiByteToWideChar(CodePage, 0, str.c_str(), -1, buf, bufsize);
	std::wstring ans(buf); delete[] buf;
	return ans;
}
inline std::string WideToNarrow(unsigned int CodePage, const std::wstring& str) {
	int bufsize = WideCharToMultiByte(CodePage, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* buf = new char[bufsize];
	WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, buf, bufsize, nullptr, nullptr);
	std::string ans(buf); delete[] buf;
	return ans;
}
inline std::wstring UTF8toWIDE(const std::string& str) {
	return NarrowToWide(CP_UTF8, str);
}
inline std::wstring ANSItoWIDE(const std::string& str) {
	return NarrowToWide(CP_ACP, str);
}
inline std::string WIDEtoUTF8(const std::wstring& str) {
	return WideToNarrow(CP_UTF8, str);
}
inline std::string WIDEtoANSI(const std::wstring& str) {
	return WideToNarrow(CP_ACP, str);
}
typedef std::basic_string<TCHAR> tstring;
typedef std::basic_ostringstream<TCHAR> tostringstream;
#ifdef UNICODE
inline std::string toANSI(const std::wstring& str) {return WIDEtoANSI(str);}
inline std::string toUTF8(const std::wstring& str) {return WIDEtoUTF8(str);}
inline std::wstring fromUTF8(const std::string& str) {return UTF8toWIDE(str);}
inline std::wstring DecodeStr(const std::string& str) {return UTF8toWIDE(str);}
inline std::string EncodeStr(const std::wstring& str) {return WIDEtoUTF8(str);}
inline std::wstring EnsureTStr(const std::string& str) {return ANSItoWIDE(str);}
inline std::wstring EnsureTStr(const std::wstring& str) {return str;}
#else
inline std::string toANSI(const std::string& str) {return str;}
inline std::string toUTF8(const std::string& str) {return WIDEtoUTF8(ANSItoWIDE(str));}
inline std::string fromUTF8(const std::string& str) {return WIDEtoANSI(UTF8toWIDE(str));}
inline std::string DecodeStr(const std::string& str) {return str;}
inline std::string EncodeStr(const std::string& str) {return str;}
inline std::string EnsureTStr(const std::string& str) {return str;}
inline std::string EnsureTStr(const std::wstring& str) {return WIDEtoANSI(str);}
#endif

}
