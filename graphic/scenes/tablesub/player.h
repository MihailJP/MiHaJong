#pragma once

#include "tablesub.h"

namespace mihajong_graphic {

class TableSubscenePlayerProto : public TableSubscene {
public:
	TableSubscenePlayerProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubscenePlayerProto();
};

class TableSubscenePlayerDahai : public TableSubscenePlayerProto {
public:
	TableSubscenePlayerDahai(LPDIRECT3DDEVICE9 device);
	~TableSubscenePlayerDahai();
	void Render();
};

}
