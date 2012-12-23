#pragma once

#include "exports.h"
#include "../mjcore/strcode.h"

namespace mihajong_graphic {
namespace logwnd {

EXPORT void append(LPCTSTR logstr);

#ifdef GRAPHIC_EXPORTS
CodeConv::tstring getlog();
#endif

}
}
