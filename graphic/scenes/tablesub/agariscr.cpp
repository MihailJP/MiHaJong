#include "agariscr.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubsceneAgariScreenProto::TableSubsceneAgariScreenProto(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
}
TableSubsceneAgariScreenProto::~TableSubsceneAgariScreenProto() {
}

// -------------------------------------------------------------------------

TableSubsceneAgariScreen::TableSubsceneAgariScreen(LPDIRECT3DDEVICE9 device) : TableSubsceneAgariScreenProto(device) {
}
TableSubsceneAgariScreen::~TableSubsceneAgariScreen() {
}
void TableSubsceneAgariScreen::Render() {
}

// -------------------------------------------------------------------------

}
