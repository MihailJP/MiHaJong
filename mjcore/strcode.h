#ifndef STRCODE_H
#define STRCODE_H

#include <tchar.h>

namespace CodeConv {

inline std::wstring NarrowToWide(UINT CodePage, std::string& str) {
	int bufsize = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* buf = new wchar_t[bufsize];
	MultiByteToWideChar(CodePage, 0, str.c_str(), -1, buf, bufsize);
	std::wstring ans(buf); delete[] buf;
	return ans;
}
inline std::wstring ANSItoWIDE(std::string& str) {
	return NarrowToWide(CP_ACP, str);
}
inline std::string WIDEtoUTF8(std::wstring& str) {
	int bufsize = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* buf = new char[bufsize];
	WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, buf, bufsize, nullptr, nullptr);
	std::string ans(buf); delete[] buf;
	return ans;
}
#ifdef UNICODE
typedef std::wstring tstring;
typedef std::wostringstream tostringstream;
inline std::string toUTF8(std::wstring& str) {
	return WIDEtoUTF8(str);
}
inline std::string EncodeStr(std::wstring& str) {
	return WIDEtoUTF8(str);
}
#else
typedef std::string tstring;
typedef std::ostringstream tostringstream;
inline std::string toUTF8(std::string& str) {
	return WIDEtoUTF8(ANSItoWIDE(str));
}
inline std::string EncodeStr(std::string& str) {
	return str;
}
#endif

#endif

}
