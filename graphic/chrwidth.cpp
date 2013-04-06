#include "chrwidth.h"

#include <Windows.h>
#include <stdexcept>
#include "../common/strcode.h"

namespace mihajong_graphic {
namespace character_width {

bool isFullWidth(wchar_t chr) {
	return !(((chr >= L' ') && (chr <= L'~')) || ((chr >= L'\uff61') && (chr <= L'\uff9f')));
}
bool isFullWidth(char) {
	return false;
}

bool isLeadingByte(wchar_t chr) {
	static_assert((sizeof (wchar_t) == 2) || (sizeof (wchar_t) == 4), "sizeof (wchar_t) is invalid");
	if (sizeof (wchar_t) == 2) // assume UTF-16
		return (chr >= 0xd800) && (chr <= 0xdbff);
	else // assume UTF-32
		return false;
}
bool isLeadingByte(char chr) {
	if (GetACP() == 932) // CP932 aka SJIS
		return ((chr >= (char)0x81) && (chr <= (char)0x9f)) || ((chr >= (char)0xe0) && (chr <= (char)0xfc));
	else return false; // Other codepages are not supported
}
bool isLeadingByte(const CodeConv::tstring& str, unsigned pos) {
	bool flag = false;
	for (unsigned i = 0; i <= pos; i++) {
		try {
			if (!flag) flag = isLeadingByte(str.at(i));
			else flag = false;
		} catch (std::out_of_range&) {
			return false;
		}
	}
	return flag;
}

}
}
