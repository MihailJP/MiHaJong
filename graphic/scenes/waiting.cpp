#include "waiting.h"

#include "../../common/strcode.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

ConnectionWaitingProto::ConnectionWaitingProto(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	myTextRenderer = new TextRenderer(manipulator->getDevice());
}
ConnectionWaitingProto::~ConnectionWaitingProto() {
	delete myTextRenderer;
}
void ConnectionWaitingProto::waiting_title() {
	const CodeConv::tstring titletxt = _T("Ú‘±‘Ò‹@’†");
	const float sizeRate = 3.0f;
	const float widthRate = (float)Geometry::WindowWidth / (float)Geometry::WindowHeight * 0.75f;
	myTextRenderer->NewText(0, titletxt,
		(Geometry::WindowWidth / Geometry::WindowScale() - myTextRenderer->strWidthByPix(titletxt) * sizeRate) / 2,
		200, sizeRate, widthRate);
}

// -------------------------------------------------------------------------

ServerWait::ServerWait(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
}
ServerWait::~ServerWait() {
}
void ServerWait::Render() {
	clearWithGameTypeColor();
	waiting_title();
	myTextRenderer->Render();
}

// -------------------------------------------------------------------------

ClientWait::ClientWait(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
}
ClientWait::~ClientWait() {
}
void ClientWait::Render() {
	clearWithGameTypeColor();
	waiting_title();
	myTextRenderer->Render();
}

// -------------------------------------------------------------------------

}
