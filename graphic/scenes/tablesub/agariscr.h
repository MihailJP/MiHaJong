#pragma once

#include "tablesub.h"

namespace mihajong_graphic {

class TableSubsceneAgariScreenProto : public TableSubscene {
protected:
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 windowTexture;
protected:
	void renderWindow();
public:
	TableSubsceneAgariScreenProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubsceneAgariScreenProto();
};

class TableSubsceneAgariScreen : public TableSubsceneAgariScreenProto {
public:
	TableSubsceneAgariScreen(LPDIRECT3DDEVICE9 device);
	~TableSubsceneAgariScreen();
	void Render();
};

}
