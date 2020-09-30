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
	Background* background;
public:
	ConnectionWaitingProto(ScreenManipulator* const manipulator);
	ConnectionWaitingProto(const ConnectionWaitingProto&) = delete; // Delete unexpected copy constructor
	ConnectionWaitingProto& operator= (const ConnectionWaitingProto&) = delete; // Delete unexpected assign operator
	virtual ~ConnectionWaitingProto();
};

class ServerWait : public ConnectionWaitingProto {
private:
	ServerWaitingSubsceneID subsceneID;
	CodeConv::tstring waiting_desc_str();
public:
	ServerWait(ScreenManipulator* const manipulator);
	ServerWait(const ServerWait&) = delete; // Delete unexpected copy constructor
	ServerWait& operator= (const ServerWait&) = delete; // Delete unexpected assign operator
	~ServerWait();
	void Render();
#ifdef _WIN32
	void KeyboardInput(LPDIDEVICEOBJECTDATA od);
#else /*_WIN32*/
	void KeyboardInput(const XEvent* od);
#endif /*_WIN32*/
	void SetSubscene(SubSceneID scene_ID) override;
};

class ClientWait : public ConnectionWaitingProto {
private:
	ClientWaitingSubsceneID subsceneID;
	CodeConv::tstring waiting_desc_str();
public:
	ClientWait(ScreenManipulator* const manipulator);
	ClientWait(const ClientWait&) = delete; // Delete unexpected copy constructor
	ClientWait& operator= (const ClientWait&) = delete; // Delete unexpected assign operator
	~ClientWait();
	void Render();
	void SetSubscene(SubSceneID scene_ID) override;
};

class ConnectionWaitFailed : public ConnectionWaitingProto {
private:
	CodeConv::tstring waiting_desc_str();
public:
	ConnectionWaitFailed(ScreenManipulator* const manipulator);
	ConnectionWaitFailed(const ConnectionWaitFailed&) = delete; // Delete unexpected copy constructor
	ConnectionWaitFailed& operator= (const ConnectionWaitFailed&) = delete; // Delete unexpected assign operator
	~ConnectionWaitFailed();
	void Render();
};

}
