#include "tablesub.h"
#include "tblnorm.h"

namespace mihajong_graphic {

TableSubscene::TableSubscene(DevicePtr device) {
	myDevice = device;
};

TableSubsceneNormal::TableSubsceneNormal(DevicePtr device) : TableSubscene(device) {
}
TableSubsceneNormal::~TableSubsceneNormal() {
}
void TableSubsceneNormal::Render() {
}

}
