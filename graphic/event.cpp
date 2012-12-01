#include "event.h"

namespace mihajong_graphic {
namespace ui {

UI_Event* UIEvent = nullptr;

UI_Event::UI_Event() {
	myEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (!myEvent) throw _T("�C�x���g�I�u�W�F�N�g�̏������Ɏ��s���܂���");
}

UI_Event::~UI_Event() {
	CloseHandle(myEvent);
}

void UI_Event::set() {
	SetEvent(myEvent);
}

void UI_Event::wait() {
	WaitForSingleObject(myEvent, INFINITE);
	ResetEvent(myEvent);
}

EXPORT void WaitUI() {
	if (UIEvent) UIEvent->wait();
}

}
}
