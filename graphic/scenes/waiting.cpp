#include "waiting.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

ConnectionWaitingProto::ConnectionWaitingProto(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	myTextRenderer = new TextRenderer(manipulator->getDevice());
}
ConnectionWaitingProto::~ConnectionWaitingProto() {
	delete myTextRenderer;
}
void ConnectionWaitingProto::showCentered(unsigned id, CodeConv::tstring txt, int y, float sizeRate, bool blink) {
	const double TwoTimesPi = atan2(1.0, 1.0) * 8;
	const float widthRate = (float)Geometry::WindowWidth / (float)Geometry::WindowHeight * 0.75f;
	if (txt.empty())
		myTextRenderer->DelText(id);
	else
		myTextRenderer->NewText(id, txt,
			(Geometry::WindowWidth / Geometry::WindowScale() - myTextRenderer->strWidthByPix(txt) * sizeRate) / 2,
			y, sizeRate, widthRate,
			blink ? ((255 + (int)(75.0 * (cos((double)myTimer.elapsed() / 1000000.0 * TwoTimesPi) - 1.0))) << 24) | 0x00ffffff : 0xffffffff);
}
void ConnectionWaitingProto::waiting_title() {
	showCentered(0, _T("Ú‘±‘Ò‹@’†"), 300, 3.0f, false);
}
void ConnectionWaitingProto::waiting_desc() {
	const CodeConv::tstring txt(waiting_desc_str());
	showCentered(1, txt, 660, 1.5f, true);
}

// -------------------------------------------------------------------------

ServerWait::ServerWait(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
	subsceneID = srvwSubsceneNone;
}
ServerWait::~ServerWait() {
}
void ServerWait::Render() {
	clearWithGameTypeColor();
	waiting_title();
	waiting_desc();
	myTextRenderer->Render();
}
void ServerWait::SetSubscene(unsigned int scene_ID) {
	subsceneID = static_cast<ServerWaitingSubsceneID>(scene_ID);
};
CodeConv::tstring ServerWait::waiting_desc_str() {
	switch (subsceneID) {
	case srvwSubscene1of4:
		return _T("Œ»Ý‚Ì‘Ò‹@l”F‚P/‚Sl");
	case srvwSubscene1of3:
		return _T("Œ»Ý‚Ì‘Ò‹@l”F‚P/‚Rl");
	case srvwSubscene2of4:
		return _T("Œ»Ý‚Ì‘Ò‹@l”F‚Q/‚Sl");
	case srvwSubscene2of3:
		return _T("Œ»Ý‚Ì‘Ò‹@l”F‚Q/‚Rl");
	case srvwSubscene3of4:
		return _T("Œ»Ý‚Ì‘Ò‹@l”F‚R/‚Sl");
	case srvwSubscene3of3:
		return _T("Œ»Ý‚Ì‘Ò‹@l”F‚R/‚Rl");
	case srvwSubscene4of4:
		return _T("Œ»Ý‚Ì‘Ò‹@l”F‚S/‚Sl");
	default:
		return _T("‚µ‚Î‚ç‚­‚¨‘Ò‚¿‚­‚¾‚³‚¢");
	}
}

// -------------------------------------------------------------------------

ClientWait::ClientWait(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
}
ClientWait::~ClientWait() {
}
void ClientWait::Render() {
	clearWithGameTypeColor();
	waiting_title();
	waiting_desc();
	myTextRenderer->Render();
}
CodeConv::tstring ClientWait::waiting_desc_str() {
	return _T("");
}

// -------------------------------------------------------------------------

}
