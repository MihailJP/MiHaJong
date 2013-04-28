#pragma once

#include "../common/strcode.h"

namespace mihajong_graphic {
namespace character_width {

bool isFullWidth(wchar_t chr);
bool isFullWidth(char);

bool isLeadingByte(wchar_t chr);
bool isLeadingByte(char chr);
bool isLeadingByte(const CodeConv::tstring& str, unsigned pos);

}
}
