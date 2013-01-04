#pragma once

#include "tablesub.h"
#include "../../gametbl.h"

namespace mihajong_graphic {

class TableSubsceneCallProto : public TableSubscene {
protected:
	LPDIRECT3DTEXTURE9 tCall; LPD3DXSPRITE sCall;
	virtual void ShowCall(PLAYER_ID player, int x, int y) = 0;
	virtual void ShowAllCall() = 0;
public:
	TableSubsceneCallProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubsceneCallProto();
};

class TableSubsceneCallZoomProto : public TableSubsceneCallProto {
protected:
	void ShowCall(PLAYER_ID player, int x, int y);
	void ShowAllCall();
public:
	TableSubsceneCallZoomProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubsceneCallZoomProto();
};

class TableSubsceneCall : public TableSubsceneCallZoomProto {
public:
	TableSubsceneCall(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCall();
	void Render();
};

class TableSubsceneCallFadeProto : public TableSubsceneCallProto {
protected:
	void ShowCall(PLAYER_ID player, int x, int y);
	void ShowAllCall();
public:
	TableSubsceneCallFadeProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubsceneCallFadeProto();
};

class TableSubsceneCallFade : public TableSubsceneCallFadeProto {
public:
	TableSubsceneCallFade(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCallFade();
	void Render();
};

}
