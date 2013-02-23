#pragma once

#include "tablesub.h"
#include "../../gametbl.h"
#include "../../calltext.h"

namespace mihajong_graphic {

class TableSubsceneCallProto : public TableSubscene {
protected:
	LPDIRECT3DTEXTURE9 tCall;
	virtual void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) = 0;
	virtual void ShowCall(PlayerID player, int x, int y) = 0;
	virtual void ShowAllCall();
public:
	TableSubsceneCallProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubsceneCallProto();
};

class TableSubsceneCallChankanRon;

class TableSubsceneCallZoomProto : public TableSubsceneCallProto {
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
public:
	TableSubsceneCallZoomProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubsceneCallZoomProto();
};

class TableSubsceneCall : public TableSubsceneCallZoomProto {
	friend class TableSubsceneCallChankanRon;
public:
	TableSubsceneCall(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCall();
	void Render();
};

class TableSubsceneCallFadeProto : public TableSubsceneCallProto {
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
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

class TableSubsceneCallCutProto : public TableSubsceneCallProto {
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
public:
	TableSubsceneCallCutProto(LPDIRECT3DDEVICE9 device);
	virtual ~TableSubsceneCallCutProto();
};

class TableSubsceneCallCut : public TableSubsceneCallCutProto {
	friend class TableSubsceneCallChankanRon;
public:
	TableSubsceneCallCut(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCallCut();
	void Render();
};

class TableSubsceneCallChankanRon : public TableSubsceneCallProto {
private:
	TableSubsceneCall* zoomCall;
	TableSubsceneCallCut* cutCall;
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
public:
	TableSubsceneCallChankanRon(LPDIRECT3DDEVICE9 device);
	~TableSubsceneCallChankanRon();
	void Render();
};

}
