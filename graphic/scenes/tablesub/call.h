#pragma once

#include "tablesub.h"
#include "../../gametbl.h"
#include "../../calltext.h"

namespace mihajong_graphic {

class TableSubsceneCallProto : public TableSubscene {
protected:
	TexturePtr tCall;
	virtual void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) = 0;
	virtual void ShowCall(PlayerID player, int x, int y) = 0;
	virtual void ShowAllCall();
public:
	TableSubsceneCallProto(DevicePtr device);
	virtual ~TableSubsceneCallProto();
	virtual void skipEvent();
};

class TableSubsceneCallChankanRon;

class TableSubsceneCallZoomProto : public TableSubsceneCallProto {
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
public:
	TableSubsceneCallZoomProto(DevicePtr device);
	virtual ~TableSubsceneCallZoomProto();
};

class TableSubsceneCall : public TableSubsceneCallZoomProto {
	friend class TableSubsceneCallChankanRon;
public:
	TableSubsceneCall(DevicePtr device);
	~TableSubsceneCall();
	void Render();
};

class TableSubsceneCallFadeProto : public TableSubsceneCallProto {
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
public:
	TableSubsceneCallFadeProto(DevicePtr device);
	virtual ~TableSubsceneCallFadeProto();
};

class TableSubsceneCallFade : public TableSubsceneCallFadeProto {
public:
	TableSubsceneCallFade(DevicePtr device);
	~TableSubsceneCallFade();
	void Render();
};

class TableSubsceneCallCutProto : public TableSubsceneCallProto {
protected:
	virtual void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
public:
	TableSubsceneCallCutProto(DevicePtr device);
	virtual ~TableSubsceneCallCutProto();
};

class TableSubsceneCallCut : public TableSubsceneCallCutProto {
	friend class TableSubsceneCallChankanRon;
public:
	TableSubsceneCallCut(DevicePtr device);
	~TableSubsceneCallCut();
	void Render();
};

class TableSubsceneCallChankanPre : public TableSubsceneCallCutProto {
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
public:
	TableSubsceneCallChankanPre(DevicePtr device);
	~TableSubsceneCallChankanPre();
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
	TableSubsceneCallChankanRon(DevicePtr device);
	~TableSubsceneCallChankanRon();
	void Render();
};

}
