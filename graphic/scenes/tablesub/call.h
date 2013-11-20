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
	TableSubsceneCallProto(const TableSubsceneCallProto&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallProto& operator= (const TableSubsceneCallProto&) = delete; // Delete unexpected assign operator
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
	TableSubsceneCallZoomProto(const TableSubsceneCallZoomProto&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallZoomProto& operator= (const TableSubsceneCallZoomProto&) = delete; // Delete unexpected assign operator
	virtual ~TableSubsceneCallZoomProto();
};

class TableSubsceneCall : public TableSubsceneCallZoomProto {
	friend class TableSubsceneCallChankanRon;
public:
	TableSubsceneCall(DevicePtr device);
	TableSubsceneCall(const TableSubsceneCall&) = delete; // Delete unexpected copy constructor
	TableSubsceneCall& operator= (const TableSubsceneCall&) = delete; // Delete unexpected assign operator
	~TableSubsceneCall();
	void Render();
};

class TableSubsceneCallFadeProto : public TableSubsceneCallProto {
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
public:
	TableSubsceneCallFadeProto(DevicePtr device);
	TableSubsceneCallFadeProto(const TableSubsceneCallFadeProto&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallFadeProto& operator= (const TableSubsceneCallFadeProto&) = delete; // Delete unexpected assign operator
	virtual ~TableSubsceneCallFadeProto();
};

class TableSubsceneCallFade : public TableSubsceneCallFadeProto {
public:
	TableSubsceneCallFade(DevicePtr device);
	TableSubsceneCallFade(const TableSubsceneCallFade&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallFade& operator= (const TableSubsceneCallFade&) = delete; // Delete unexpected assign operator
	~TableSubsceneCallFade();
	void Render();
};

class TableSubsceneCallCutProto : public TableSubsceneCallProto {
protected:
	virtual void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
	void ShowCall(PlayerID player, int x, int y);
public:
	TableSubsceneCallCutProto(DevicePtr device);
	TableSubsceneCallCutProto(const TableSubsceneCallCutProto&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallCutProto& operator= (const TableSubsceneCallCutProto&) = delete; // Delete unexpected assign operator
	virtual ~TableSubsceneCallCutProto();
};

class TableSubsceneCallCut : public TableSubsceneCallCutProto {
	friend class TableSubsceneCallChankanRon;
public:
	TableSubsceneCallCut(DevicePtr device);
	TableSubsceneCallCut(const TableSubsceneCallCut&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallCut& operator= (const TableSubsceneCallCut&) = delete; // Delete unexpected assign operator
	~TableSubsceneCallCut();
	void Render();
};

class TableSubsceneCallChankanPre : public TableSubsceneCallCutProto {
protected:
	void ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y);
public:
	TableSubsceneCallChankanPre(DevicePtr device);
	TableSubsceneCallChankanPre(const TableSubsceneCallChankanPre&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallChankanPre& operator= (const TableSubsceneCallChankanPre&) = delete; // Delete unexpected assign operator
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
	TableSubsceneCallChankanRon(const TableSubsceneCallChankanRon&) = delete; // Delete unexpected copy constructor
	TableSubsceneCallChankanRon& operator= (const TableSubsceneCallChankanRon&) = delete; // Delete unexpected assign operator
	~TableSubsceneCallChankanRon();
	void Render();
};

}
