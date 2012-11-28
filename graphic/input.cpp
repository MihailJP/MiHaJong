#include "input.h"
#include "init.h"
#include "../mjcore/strcode.h"

namespace mihajong_graphic {
namespace input {

// -------------------------------------------------------------------------

namespace {
	InputManipulator* iManip = nullptr;
}

InputManipulator::InputManipulator(HWND hwnd) {
	if (FAILED(DirectInput8Create(
		GraphicDLL, 0x0800, IID_IDirectInput8, reinterpret_cast<void**>(&myInterface), nullptr)))
		throw CodeConv::tstring(_T("DirectInput8CreateŽ¸”sII"));
}

InputManipulator::~InputManipulator() {
	myInterface->Release();
}

// -------------------------------------------------------------------------

Keyboard::Keyboard(LPDIRECTINPUT8 inputInterface, HWND hwnd) {
	if (FAILED(inputInterface->CreateDevice(GUID_SysKeyboard, &myInputDevice, nullptr)))
		throw CodeConv::tstring(_T("CreateDeviceŽ¸”sII"));
	if (FAILED(myInputDevice->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		throw CodeConv::tstring(_T("SetCooperativeLevelŽ¸”sII"));
}

Keyboard::~Keyboard() {
	if (myInputDevice) {
		myInputDevice->Unacquire();
		myInputDevice->Release();
	}
}

// -------------------------------------------------------------------------

EXPORT void InitInputManipulator(HWND hwnd) {
	iManip = new InputManipulator(hwnd);
}
EXPORT void DisposeInputManipulator() {
	if (iManip) delete iManip;
	iManip = nullptr;
}

// -------------------------------------------------------------------------

}
}
