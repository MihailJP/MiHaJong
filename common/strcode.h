#pragma once

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#else /* _WIN32 */
#include <cstring>
#include <cwchar>
#include <clocale>
#include <climits>
#include <iomanip>
#include "mutex.h"
#ifdef UNICODE
#define UNI_TEXT(str) L##str
#define _T(str) UNI_TEXT(str)
#define _tcsncpy wcsncpy
#define _tcslen wcslen
#define _tcsncat wcsncat
#define _ttoi(str) wcstol(str, nullptr, 10)
#define _sntprintf swprintf
typedef wchar_t TCHAR;
typedef wchar_t* LPTSTR;
typedef const wchar_t* LPCTSTR;
#else
#define UNI_TEXT(str) str
#define _T(str) UNI_TEXT(str)
#define _tcsncpy strncpy
#define _tcslen strlen
#define _tcsncat strncat
#define _ttoi(str) strtol(str, nullptr, 10)
#define _sntprintf snprintf
typedef char TCHAR;
typedef char* LPTSTR;
typedef const char* LPCTSTR;
#endif
#endif /* _WIN32 */
#include <string>
#include <sstream>
#include <fstream>
#include <locale>
#include <vector>
#include <iostream>

#ifdef UNICODE
#define PON L"碰"
constexpr wchar_t PengPengHu[] = L"碰碰和";
#else /* UNICODE */
#define PON "ポン"
constexpr char PengPengHu[] = "ポンポン和";
#endif /* UNICODE */

namespace CodeConv {

#ifndef _WIN32
constexpr unsigned CP_UTF8 = 65001u;
constexpr unsigned CP_ACP = 932u;
#endif /* _WIN32 */

template <typename T> inline void setStreamLocale(T& file) {
	const auto setLoc = [](T& file, const char* locale) {
		try {
			(void)file.imbue(std::locale(locale));
		} catch (std::runtime_error&) {
			return false;
		}
		return true;
	};
#ifdef UNICODE
	if (setLoc(file, ".65001")) return;
	if (setLoc(file, "ja_JP.utf8")) return;
	if (setLoc(file, "ja_JP.UTF-8")) return;
#else /* UNICODE */
	if (setLoc(file, ".932")) return;
	if (setLoc(file, "ja_JP.cp932")) return;
	if (setLoc(file, "ja_JP.shiftjisx0213")) return;
	if (setLoc(file, "ja_JP.sjis-8")) return;
#endif /* UNICODE */
	std::cerr << "Failed to set locale for stream" << std::endl;
}

#ifndef _WIN32
inline void setCP(unsigned int CodePage) {
	char teststr[MB_CUR_MAX];
	if (CodePage == CP_ACP) { /* Japanese SJIS code page */
		if (setlocale(LC_CTYPE, "ja_JP.cp932")) { /* Linux */
			if (wctomb(teststr, L'あ') != -1) return;
		}
		if (setlocale(LC_CTYPE, "ja_JP.shiftjisx0213")) { /* Linux */
			if (wctomb(teststr, L'あ') != -1) return;
		}
		if (setlocale(LC_CTYPE, "ja_JP.sjis")) { /* returns nullptr if failed */
			if (wctomb(teststr, L'あ') != -1) return;
		}
	}
	else if (CodePage == CP_UTF8) { /* UTF-8 code page */
		if (setlocale(LC_CTYPE, "ja_JP.utf8")) return; /* Linux */
		if (setlocale(LC_CTYPE, "ja_JP.UTF-8")) return; /* Mac OS */
	}
	else { /* Unsupported code page */
		std::cerr << "Unsupported code page " << CodePage << " ignored" << std::endl;
		return;
	}
	std::cerr << "Failed to set code page to " << CodePage << std::endl;
}

inline MUTEXLIB::recursive_mutex& conversionMutex() {
	static MUTEXLIB::recursive_mutex myMutex;
	return myMutex;
}

#endif /* _WIN32 */

inline std::wstring NarrowToWide(unsigned int CodePage, std::string str) {
#ifdef _WIN32
	int bufsize = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* buf = new wchar_t[bufsize];
	MultiByteToWideChar(CodePage, 0, str.c_str(), -1, buf, bufsize);
#else /* _WIN32 */
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(conversionMutex());
	const std::string origLocale(setlocale(LC_CTYPE, nullptr)); /* backup locale */
	setCP(CodePage);
	mbstate_t mbStat {};
	char* srcBuf = new char[str.length() + 1] {}; /* source buffer */
	strncpy(srcBuf, str.c_str(), str.length());
	const char* srcPtr = const_cast<char*>(srcBuf);
	const size_t bufsize = mbsrtowcs(nullptr, &srcPtr, 0, &mbStat);
	if (bufsize == (size_t)-1) {
		setlocale(LC_CTYPE, origLocale.c_str()); /* restore locale */
		delete[] srcBuf;
		std::cerr << "Failed to convert into wide string" << std::endl;
		throw _T("ワイド文字への変換に失敗しました");
	}
	wchar_t* buf = new wchar_t[bufsize + 1 /* Do not forget the trailing null */] {};
	srcPtr = const_cast<char*>(srcBuf);
	mbsrtowcs(buf, &srcPtr, bufsize, &mbStat);
	setlocale(LC_CTYPE, origLocale.c_str()); /* restore locale */
#endif /* _WIN32 */
	std::wstring ans(buf); delete[] buf;
#ifndef _WIN32
	delete[] srcBuf;
#endif /* _WIN32 */
	return ans;
}
inline std::string WideToNarrow(unsigned int CodePage, std::wstring str) {
#ifdef _WIN32
	int bufsize = WideCharToMultiByte(CodePage, 0, str.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* buf = new char[bufsize];
	WideCharToMultiByte(CodePage, 0, str.c_str(), -1, buf, bufsize, nullptr, nullptr);
#else /* _WIN32 */
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(conversionMutex());
	const std::string origLocale(setlocale(LC_CTYPE, nullptr)); /* backup locale */
	setCP(CodePage);
	mbstate_t mbStat {};
	wchar_t* srcBuf = new wchar_t[str.length() + 1] {}; /* source buffer */
	wcsncpy(srcBuf, str.c_str(), str.length());

	std::string buf;
	for (int pos = 0; pos < str.size(); ++pos) {
		wctomb(nullptr, str[pos]);
		char tmpstr[MB_LEN_MAX] {};
		int rslt = wctomb(tmpstr, str[pos]);
		if (str[pos] <= 0x007f) { // Keep 7bit ASCII
			tmpstr[0] = static_cast<char>((int)(str[pos])); tmpstr[1] = 0;
			buf += std::string(tmpstr);
		} else if (rslt != -1) {
			buf += std::string(tmpstr);
		} else if (CodePage == 932) {
			switch (str[pos]) {
			case 0xff5e: // FULLWIDTH TILDE
				tmpstr[0] = 0x81; tmpstr[1] = 0x60; tmpstr[2] = 0; break;
			case 0x2460: // CIRCLED DIGIT ONE
				tmpstr[0] = 0x87; tmpstr[1] = 0x40; tmpstr[2] = 0; break;
			case 0x2461: // CIRCLED DIGIT TWO
				tmpstr[0] = 0x87; tmpstr[1] = 0x41; tmpstr[2] = 0; break;
			case 0x2462: // CIRCLED DIGIT THREE
				tmpstr[0] = 0x87; tmpstr[1] = 0x42; tmpstr[2] = 0; break;
			case 0x2463: // CIRCLED DIGIT FOUR
				tmpstr[0] = 0x87; tmpstr[1] = 0x43; tmpstr[2] = 0; break;
			case 0x2464: // CIRCLED DIGIT FIVE
				tmpstr[0] = 0x87; tmpstr[1] = 0x44; tmpstr[2] = 0; break;
			case 0x2465: // CIRCLED DIGIT SIX
				tmpstr[0] = 0x87; tmpstr[1] = 0x45; tmpstr[2] = 0; break;
			case 0x2466: // CIRCLED DIGIT SEVEN
				tmpstr[0] = 0x87; tmpstr[1] = 0x46; tmpstr[2] = 0; break;
			case 0x2467: // CIRCLED DIGIT EIGHT
				tmpstr[0] = 0x87; tmpstr[1] = 0x47; tmpstr[2] = 0; break;
			case 0x2468: // CIRCLED DIGIT NINE
				tmpstr[0] = 0x87; tmpstr[1] = 0x48; tmpstr[2] = 0; break;
			case 0x2469: // CIRCLED DIGIT TEN
				tmpstr[0] = 0x87; tmpstr[1] = 0x49; tmpstr[2] = 0; break;
			default:
				tmpstr[0] = '?'; tmpstr[1] = '\0'; break;
			}
			buf += std::string(tmpstr);
		} else {
			buf += std::string("?");
		}
	}

	setlocale(LC_CTYPE, origLocale.c_str()); /* restore locale */
#endif /* _WIN32 */
	std::string ans(buf);
#ifdef _WIN32
	delete[] buf;
#else /* _WIN32 */
	delete[] srcBuf;
#endif /* _WIN32 */
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
typedef std::basic_istringstream<TCHAR> tistringstream;
typedef std::basic_ifstream<TCHAR> tifstream;
typedef std::basic_ofstream<TCHAR> tofstream;
#ifdef UNICODE
inline std::string toANSI(std::wstring str) {return WIDEtoANSI(str);}
inline std::string toUTF8(std::wstring str) {return WIDEtoUTF8(str);}
inline std::wstring fromUTF8(std::string str) {return UTF8toWIDE(str);}
inline std::wstring DecodeStr(std::string str) {return UTF8toWIDE(str);}
inline std::string EncodeStr(std::wstring str) {return WIDEtoUTF8(str);}
inline std::wstring EnsureTStr(std::string str) {return ANSItoWIDE(str);}
inline std::wstring EnsureTStr(std::wstring str) {return str;}
#define to_tstring std::to_wstring
#else
inline std::string toANSI(std::string str) {return str;}
inline std::string toUTF8(std::string str) {return WIDEtoUTF8(ANSItoWIDE(str));}
inline std::string fromUTF8(std::string str) {return WIDEtoANSI(UTF8toWIDE(str));}
inline std::string DecodeStr(std::string str) {return str;}
inline std::string EncodeStr(std::string str) {return str;}
inline std::string EnsureTStr(std::string str) {return str;}
inline std::string EnsureTStr(std::wstring str) {return WIDEtoANSI(str);}
#define to_tstring std::to_string
#endif

template<class CharT, class Traits = std::char_traits<CharT> >
auto split(const std::basic_string<CharT>& str, CharT delimiter) {
	std::basic_istringstream<CharT, Traits> stream(str);
	std::basic_string<CharT, Traits> row;
	std::vector<std::basic_string<CharT, Traits> > result;
	while (std::getline(stream, row, delimiter)) {
		if ((delimiter == stream.widen('\n')) && (row.back() == stream.widen('\r'))) row.pop_back();
		result.push_back(row);
	}
	return result;
}
template<class CharT> auto split(const CharT* str, CharT delimiter) {
	return split(std::basic_string<CharT>(str), delimiter);
}

inline tstring lower(const CodeConv::tstring& str) {
	tstring result;
	for (auto i = str.begin(); i != str.end(); ++i) {
		result += std::tolower(*i, std::locale::classic());
	}
	return result;
}

}
