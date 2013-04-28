#pragma once

#include "sysscr.h"
#include "../text.h"
#include "scene_id.h"
#include "../../common/strcode.h"

namespace mihajong_graphic {

class ConnectionWaitingProto : public SystemScreen {
protected:
	TextRenderer* myTextRenderer;
	void showCentered(unsigned id, CodeConv::tstring txt, int y, float sizeRate, bool blink);
	void waiting_title();
	virtual CodeConv::tstring waiting_desc_str() = 0;
	virtual void waiting_desc();
public:
	ConnectionWaitingProto(ScreenManipulator* const manipulator);
	virtual ~ConnectionWaitingProto();
};

class ServerWait : public ConnectionWaitingProto {
private:
	ServerWaitingSubsceneID subsceneID;
	CodeConv::tstring waiting_desc_str();
public:
	ServerWait(ScreenManipulator* const manipulator);
	~ServerWait();
	void Render();
	void KeyboardInput(LPDIDEVICEOBJECTDATA od);
	void SetSubscene(unsigned int scene_ID);
};

class ClientWait : public ConnectionWaitingProto {
private:
	ClientWaitingSubsceneID subsceneID;
	CodeConv::tstring waiting_desc_str();
public:
	ClientWait(ScreenManipulator* const manipulator);
	~ClientWait();
	void Render();
	void SetSubscene(unsigned int scene_ID);
};

class ConnectionWaitFailed : public ConnectionWaitingProto {
private:
	CodeConv::tstring waiting_desc_str();
public:
	ConnectionWaitFailed(ScreenManipulator* const manipulator);
	~ConnectionWaitFailed();
	void Render();
};

}
