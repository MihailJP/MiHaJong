#include "waiting.h"
#include <cmath>
#include "../event.h"
#include "../rule.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"

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
	showCentered(0, _T("�ڑ��ҋ@��"), 300, 3.0f, false);
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
	showCentered(2, _T("X�L�[�܂���ESC�L�[�������ƌ��݂̖ʎq��COM�v���C���[�����ĊJ�n���܂�"), 900, 1.0f, false);
	myTextRenderer->Render();
}
void ServerWait::SetSubscene(unsigned int scene_ID) {
	subsceneID = static_cast<ServerWaitingSubsceneID>(scene_ID);
};
#ifdef _WIN32
void ServerWait::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	switch (od->dwOfs) {
	case DIK_ESCAPE: case DIK_X: // �L�����Z��
		if (od->dwData) {
			ui::cancellableWait->set(1);
		}
		break;
	}
}
#else /*_WIN32*/
/* TODO: �������ӏ� */
#endif /*_WIN32*/
CodeConv::tstring ServerWait::waiting_desc_str() {
	switch (subsceneID) {
	case srvwSubscene1of4:
		return _T("���݂̑ҋ@�l���F�P/�S�l");
	case srvwSubscene1of3:
		return _T("���݂̑ҋ@�l���F�P/�R�l");
	case srvwSubscene2of4:
		return _T("���݂̑ҋ@�l���F�Q/�S�l");
	case srvwSubscene2of3:
		return _T("���݂̑ҋ@�l���F�Q/�R�l");
	case srvwSubscene3of4:
		return _T("���݂̑ҋ@�l���F�R/�S�l");
	case srvwSubscene3of3:
		return _T("���݂̑ҋ@�l���F�R/�R�l");
	case srvwSubscene4of4:
		return _T("���݂̑ҋ@�l���F�S/�S�l");
	default:
		return _T("���΂炭���҂���������");
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
	showCentered(2, _T("���΂炭���҂���������"), 900, 1.0f, false);
	myTextRenderer->Render();
}
void ClientWait::SetSubscene(unsigned int scene_ID) {
	subsceneID = static_cast<ClientWaitingSubsceneID>(scene_ID);
};
CodeConv::tstring ClientWait::waiting_desc_str() {
	switch (subsceneID) {
	case cliwSubsceneConnecting:
		{
			const std::string addr(rules::getPreferenceRawStr(1 /*hardcoded*/));
			const CodeConv::tstring msg(CodeConv::EnsureTStr(addr) + _T("�ɐڑ����Ă��܂�"));
			return msg;
		}
	case cliwSubsceneWaiting:
		return _T("�ʎq�������̂�҂��Ă��܂�");
	default:
		return _T("�������ł�");
	}
}

// -------------------------------------------------------------------------

ConnectionWaitFailed::ConnectionWaitFailed(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
	sound::Play(sound::IDs::sndCuohu);
}
ConnectionWaitFailed::~ConnectionWaitFailed() {
}
void ConnectionWaitFailed::Render() {
	const std::string addr(rules::getPreferenceRawStr(1 /*hardcoded*/));
	const CodeConv::tstring errmsg(CodeConv::EnsureTStr(addr) + _T("�ɐڑ��ł��܂���ł���"));
	clearWithGameTypeColor();
	showCentered(0, _T("�ڑ����s"), 300, 3.0f, false);
	showCentered(1, errmsg, 660, 1.5f, false);
	myTextRenderer->Render();
}
CodeConv::tstring ConnectionWaitFailed::waiting_desc_str() {
	return _T("");
}

// -------------------------------------------------------------------------

}
