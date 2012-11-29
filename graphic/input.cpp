#include "input.h"
#include "init.h"
#include "../mjcore/strcode.h"

namespace mihajong_graphic {
namespace input {

// -------------------------------------------------------------------------

InputManipulator::InputManipulator(HWND hwnd) {
	if (FAILED(DirectInput8Create(
		GraphicDLL, 0x0800, IID_IDirectInput8, reinterpret_cast<void**>(&myInterface), nullptr)))
		throw CodeConv::tstring(_T("DirectInput8Create���s�I�I"));
	myKeyboard = new Keyboard(myInterface, hwnd);
}

InputManipulator::~InputManipulator() {
	if (myKeyboard) delete myKeyboard;
	myInterface->Release();
}

// -------------------------------------------------------------------------

InputDevice::~InputDevice() {}

// -------------------------------------------------------------------------

Keyboard::Keyboard(LPDIRECTINPUT8 inputInterface, HWND hwnd) {
	if (FAILED(inputInterface->CreateDevice(GUID_SysKeyboard, &myInputDevice, nullptr)))
		throw CodeConv::tstring(_T("CreateDevice���s�I�I"));
	if (FAILED(myInputDevice->SetDataFormat(&c_dfDIKeyboard)))
		throw CodeConv::tstring(_T("SetDataFormat���s�I�I"));
	if (FAILED(myInputDevice->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		throw CodeConv::tstring(_T("SetCooperativeLevel���s�I�I"));
	DIPROPDWORD diProp;
	diProp.diph.dwSize = sizeof(diProp);
	diProp.diph.dwHeaderSize = sizeof(diProp.diph);
	diProp.diph.dwObj = 0;
	diProp.diph.dwHow = DIPH_DEVICE;
	diProp.dwData = 1000;
	if (FAILED(myInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProp.diph)))
		throw CodeConv::tstring(_T("SetProperty���s�I�I"));
	myInputDevice->Acquire();
}

Keyboard::~Keyboard() {
	if (myInputDevice) {
		myInputDevice->Unacquire();
		myInputDevice->Release();
	}
}

// -------------------------------------------------------------------------

}
}
