#pragma once

#include "sysscr.h"
#include "../text.h"

namespace mihajong_graphic {

class ConnectionWaitingProto : public SystemScreen {
protected:
	TextRenderer* myTextRenderer;
	void waiting_title();
public:
	ConnectionWaitingProto(ScreenManipulator* const manipulator);
	virtual ~ConnectionWaitingProto();
};

class ServerWait : public ConnectionWaitingProto {
public:
	ServerWait(ScreenManipulator* const manipulator);
	~ServerWait();
	void Render();
};

class ClientWait : public ConnectionWaitingProto {
public:
	ClientWait(ScreenManipulator* const manipulator);
	~ClientWait();
	void Render();
};

}
