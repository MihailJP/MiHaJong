#include "tablesub.h"
#include "tblnorm.h"

namespace mihajong_graphic {

TableSubscene::TableSubscene(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	startTime = currTime();
};

std::uint64_t TableSubscene::currTime() { /* åªç›éûçè(WindowsÇ≈ÇÕ100nsíPà ) */
	FILETIME Zeit; GetSystemTimeAsFileTime(&Zeit);
	return ((std::uint64_t)Zeit.dwHighDateTime << 32) | Zeit.dwLowDateTime;
}

TableSubsceneNormal::TableSubsceneNormal(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
}
TableSubsceneNormal::~TableSubsceneNormal() {
}
void TableSubsceneNormal::Render() {
}

}
