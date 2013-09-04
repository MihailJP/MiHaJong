#include "input.h"
#ifdef _WIN32
#include "init.h"
#include "../common/strcode.h"

namespace mihajong_graphic {
namespace input {

// -------------------------------------------------------------------------

InputManipulator::InputManipulator(HWND hwnd, bool fullscreen) {
	if (FAILED(DirectInput8Create(
		GraphicDLL, 0x0800, IID_IDirectInput8, reinterpret_cast<void**>(&myInterface), nullptr)))
		throw CodeConv::tstring(_T("DirectInput8CreateŽ¸”sII"));
	myKeyboard = new Keyboard(myInterface, hwnd);
	myMouse = new Mouse(myInterface, hwnd, fullscreen);
}

InputManipulator::~InputManipulator() {
	if (myKeyboard) delete myKeyboard;
	if (myMouse) delete myMouse;
	myInterface->Release();
}

// -------------------------------------------------------------------------

InputDevice::~InputDevice() {}

// -------------------------------------------------------------------------

Keyboard::Keyboard(LPDIRECTINPUT8 inputInterface, HWND hwnd) {
	if (FAILED(inputInterface->CreateDevice(GUID_SysKeyboard, &myInputDevice, nullptr)))
		throw CodeConv::tstring(_T("CreateDeviceŽ¸”sII"));
	if (FAILED(myInputDevice->SetDataFormat(&c_dfDIKeyboard)))
		throw CodeConv::tstring(_T("SetDataFormatŽ¸”sII"));
	if (FAILED(myInputDevice->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		throw CodeConv::tstring(_T("SetCooperativeLevelŽ¸”sII"));
	DIPROPDWORD diProp;
	diProp.diph.dwSize = sizeof(diProp);
	diProp.diph.dwHeaderSize = sizeof(diProp.diph);
	diProp.diph.dwObj = 0;
	diProp.diph.dwHow = DIPH_DEVICE;
	diProp.dwData = 1000;
	if (FAILED(myInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProp.diph)))
		throw CodeConv::tstring(_T("SetPropertyŽ¸”sII"));
	myInputDevice->Acquire();
}

Keyboard::~Keyboard() {
	if (myInputDevice) {
		myInputDevice->Unacquire();
		myInputDevice->Release();
	}
}

// -------------------------------------------------------------------------

Mouse::Mouse(LPDIRECTINPUT8 inputInterface, HWND hwnd, bool fullscreen) {
	hWnd = hwnd; fullScreenFlag = fullscreen;
	if (FAILED(inputInterface->CreateDevice(GUID_SysMouse, &myInputDevice, nullptr)))
		throw CodeConv::tstring(_T("CreateDeviceŽ¸”sII"));
	if (FAILED(myInputDevice->SetDataFormat(&c_dfDIMouse2)))
		throw CodeConv::tstring(_T("SetDataFormatŽ¸”sII"));
	if (FAILED(myInputDevice->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		throw CodeConv::tstring(_T("SetCooperativeLevelŽ¸”sII"));
	DIPROPDWORD diProp;
	diProp.diph.dwSize = sizeof(diProp);
	diProp.diph.dwHeaderSize = sizeof(diProp.diph);
	diProp.diph.dwObj = 0;
	diProp.diph.dwHow = DIPH_DEVICE;
	diProp.dwData = 1000;
	if (FAILED(myInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProp.diph)))
		throw CodeConv::tstring(_T("SetPropertyŽ¸”sII"));
	myInputDevice->Acquire();
}

Mouse::~Mouse() {
	if (myInputDevice) {
		myInputDevice->Unacquire();
		myInputDevice->Release();
	}
}

Mouse::Position Mouse::pos() {
	POINT mpos;
	GetCursorPos(&mpos);
	if (!fullScreenFlag) ScreenToClient(hWnd, &mpos);
	return Position(mpos.x, mpos.y);
}

// -------------------------------------------------------------------------

}
}
#endif /*_WIN32*/
