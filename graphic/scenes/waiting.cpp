﻿#include "waiting.h"
#include <cmath>
#include "../event.h"
#include "../rule.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#ifndef _WIN32
#include "../keycode.h"
#endif /*_WIN32*/

constexpr double TwoTimesPi = 6.283185307179586476;

namespace mihajong_graphic {

// -------------------------------------------------------------------------

ConnectionWaitingProto::ConnectionWaitingProto(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	background = new Background(this);
	myTextRenderer = new TextRenderer(manipulator->getDevice());
}
ConnectionWaitingProto::~ConnectionWaitingProto() {
	delete myTextRenderer;
	delete background;
}
void ConnectionWaitingProto::showCentered(unsigned id, CodeConv::tstring txt, int y, float sizeRate, bool blink) {
	if (txt.empty())
		myTextRenderer->DelText(id);
	else
		myTextRenderer->NewText(id, txt,
			static_cast<int>(static_cast<float>(Geometry::WindowWidth) / Geometry::WindowScale() - static_cast<float>(myTextRenderer->strWidthByPix(txt)) * sizeRate) / 2,
			y, sizeRate, WidthRate(),
			blink ? ((255 + static_cast<int>(75.0 * (cos(static_cast<double>(myTimer.elapsed()) / 1000000.0 * TwoTimesPi) - 1.0))) << 24) | 0x00ffffff : 0xffffffff);
}
void ConnectionWaitingProto::waiting_title() {
	showCentered(0, _T("接続待機中"), 300, 3.0f, false);
}
void ConnectionWaitingProto::waiting_desc() {
	const CodeConv::tstring txt(waiting_desc_str());
	showCentered(1, txt, 660, 1.5f, true);
}

// -------------------------------------------------------------------------

ServerWait::ServerWait(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
	subsceneID = ServerWaitingSubsceneID::none;
}
ServerWait::~ServerWait() {
}
void ServerWait::Render() {
	clearWithGameTypeColor();
	background->show();
	waiting_title();
	waiting_desc();
	showCentered(2, _T("XキーまたはESCキーを押すと現在の面子にCOMプレイヤーを入れて開始します"), 900, 1.0f, false);
	myTextRenderer->Render();
}
void ServerWait::SetSubscene(SubSceneID scene_ID) {
	subsceneID = scene_ID.serverWaitingSubsceneID;
};
#ifdef _WIN32
void ServerWait::KeyboardInput(LPDIDEVICEOBJECTDATA od)
#else /*_WIN32*/
void ServerWait::KeyboardInput(const XEvent* od)
#endif /*_WIN32*/
{
#ifdef _WIN32
	switch (od->dwOfs)
#else /*_WIN32*/
	switch (od->xkey.keycode)
#endif /*_WIN32*/
	{
	case DIK_ESCAPE: case DIK_X: // キャンセル
#ifdef _WIN32
		if (od->dwData)
#else /*_WIN32*/
		if (od->type == KeyPress)
#endif /*_WIN32*/
		{
			ui::cancellableWait->set(1);
		}
		break;
	}
}
CodeConv::tstring ServerWait::waiting_desc_str() {
	switch (subsceneID) {
	case ServerWaitingSubsceneID::oneOfFour:
		return _T("現在の待機人数：１/４人");
	case ServerWaitingSubsceneID::oneOfThree:
		return _T("現在の待機人数：１/３人");
	case ServerWaitingSubsceneID::twoOfFour:
		return _T("現在の待機人数：２/４人");
	case ServerWaitingSubsceneID::twoOfThree:
		return _T("現在の待機人数：２/３人");
	case ServerWaitingSubsceneID::threeOfFour:
		return _T("現在の待機人数：３/４人");
	case ServerWaitingSubsceneID::threeOfThree:
		return _T("現在の待機人数：３/３人");
	case ServerWaitingSubsceneID::fourOfFour:
		return _T("現在の待機人数：４/４人");
	default:
		return _T("しばらくお待ちください");
	}
}

// -------------------------------------------------------------------------

ClientWait::ClientWait(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
	subsceneID = ClientWaitingSubsceneID::none;
}
ClientWait::~ClientWait() {
}
void ClientWait::Render() {
	clearWithGameTypeColor();
	background->show();
	waiting_title();
	waiting_desc();
	showCentered(2, _T("しばらくお待ちください"), 900, 1.0f, false);
	myTextRenderer->Render();
}
void ClientWait::SetSubscene(SubSceneID scene_ID) {
	subsceneID = scene_ID.clientWaitingSubsceneID;
};
CodeConv::tstring ClientWait::waiting_desc_str() {
	switch (subsceneID) {
	case ClientWaitingSubsceneID::connecting:
		{
			const std::string addr(preferences::serverIP());
			const CodeConv::tstring msg(CodeConv::EnsureTStr(addr) + _T("に接続しています"));
			return msg;
		}
	case ClientWaitingSubsceneID::waiting:
		return _T("面子が揃うのを待っています");
	default:
		return _T("処理中です");
	}
}

// -------------------------------------------------------------------------

ConnectionWaitFailed::ConnectionWaitFailed(ScreenManipulator* const manipulator) : ConnectionWaitingProto(manipulator) {
	sound::Play(sound::IDs::sndCuohu);
}
ConnectionWaitFailed::~ConnectionWaitFailed() {
}
void ConnectionWaitFailed::Render() {
	const std::string addr(preferences::serverIP());
	const CodeConv::tstring errmsg(CodeConv::EnsureTStr(addr) + _T("に接続できませんでした"));
	clearWithGameTypeColor();
	background->show();
	showCentered(0, _T("接続失敗"), 300, 3.0f, false);
	showCentered(1, errmsg, 660, 1.5f, false);
	myTextRenderer->Render();
}
CodeConv::tstring ConnectionWaitFailed::waiting_desc_str() {
	return _T("");
}

// -------------------------------------------------------------------------

}
