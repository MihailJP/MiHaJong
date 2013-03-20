#include "tablesub.h"
#include "tblnorm.h"

namespace mihajong_graphic {

TableSubscene::TableSubscene(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
};

TableSubsceneNormal::TableSubsceneNormal(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
}
TableSubsceneNormal::~TableSubsceneNormal() {
}
void TableSubsceneNormal::Render() {
}

}
