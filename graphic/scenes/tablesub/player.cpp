#include "player.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubscenePlayerProto::TableSubscenePlayerProto(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
}

TableSubscenePlayerProto::~TableSubscenePlayerProto() {
}

// -------------------------------------------------------------------------

TableSubscenePlayerDahai::TableSubscenePlayerDahai(LPDIRECT3DDEVICE9 device) : TableSubscenePlayerProto(device) {
}

TableSubscenePlayerDahai::~TableSubscenePlayerDahai() {
}

void TableSubscenePlayerDahai::Render() {
}

// -------------------------------------------------------------------------

TableSubscenePlayerNaki::TableSubscenePlayerNaki(LPDIRECT3DDEVICE9 device) : TableSubscenePlayerProto(device) {
}

TableSubscenePlayerNaki::~TableSubscenePlayerNaki() {
}

void TableSubscenePlayerNaki::Render() {
}

// -------------------------------------------------------------------------

}
