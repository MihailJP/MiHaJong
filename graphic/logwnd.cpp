#include "logwnd.h"

namespace mihajong_graphic {
namespace logwnd {

namespace {
	CodeConv::tostringstream logdata;
}

EXPORT void append(LPCTSTR logstr) {
	logdata << logstr; logdata.flush();
}

CodeConv::tstring getlog() {
	return logdata.str();
}

}
}
