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
		throw CodeConv::tstring(_T("DirectInput8Create失敗！！"));
	myKeyboard = new Keyboard(myInterface, hwnd);
	myMouse = new Mouse(myInterface, hwnd, fullscreen);
	try {
		myJoystick = new Joystick(myInterface, hwnd);
	} catch (...) {
		myJoystick = nullptr;
	}
}

InputManipulator::~InputManipulator() {
	if (myKeyboard) delete myKeyboard;
	if (myMouse) delete myMouse;
	if (myJoystick) delete myJoystick;
	myInterface->Release();
}

// -------------------------------------------------------------------------

InputDevice::~InputDevice() {}

// -------------------------------------------------------------------------

Keyboard::Keyboard(LPDIRECTINPUT8 inputInterface, HWND hwnd) {
	if (FAILED(inputInterface->CreateDevice(GUID_SysKeyboard, &myInputDevice, nullptr)))
		throw CodeConv::tstring(_T("CreateDevice失敗！！"));
	if (FAILED(myInputDevice->SetDataFormat(&c_dfDIKeyboard)))
		throw CodeConv::tstring(_T("SetDataFormat失敗！！"));
	if (FAILED(myInputDevice->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		throw CodeConv::tstring(_T("SetCooperativeLevel失敗！！"));
	DIPROPDWORD diProp;
	diProp.diph.dwSize = sizeof(diProp);
	diProp.diph.dwHeaderSize = sizeof(diProp.diph);
	diProp.diph.dwObj = 0;
	diProp.diph.dwHow = DIPH_DEVICE;
	diProp.dwData = 1000;
	if (FAILED(myInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProp.diph)))
		throw CodeConv::tstring(_T("SetProperty失敗！！"));
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
		throw CodeConv::tstring(_T("CreateDevice失敗！！"));
	if (FAILED(myInputDevice->SetDataFormat(&c_dfDIMouse2)))
		throw CodeConv::tstring(_T("SetDataFormat失敗！！"));
	if (FAILED(myInputDevice->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		throw CodeConv::tstring(_T("SetCooperativeLevel失敗！！"));
	DIPROPDWORD diProp;
	diProp.diph.dwSize = sizeof(diProp);
	diProp.diph.dwHeaderSize = sizeof(diProp.diph);
	diProp.diph.dwObj = 0;
	diProp.diph.dwHow = DIPH_DEVICE;
	diProp.dwData = 1000;
	if (FAILED(myInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProp.diph)))
		throw CodeConv::tstring(_T("SetProperty失敗！！"));
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

MUTEXLIB::recursive_mutex Joystick::myMutex;
Joystick* Joystick::currentInstance;
LPDIRECTINPUT8 Joystick::currentInterface;

BOOL CALLBACK Joystick::enumerationCallback(const DIDEVICEINSTANCE *pdidInstance, LPVOID pContext) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(myMutex);
	return (FAILED(currentInterface->CreateDevice(pdidInstance->guidInstance, &(currentInstance->myInputDevice), nullptr)))
		? DIENUM_CONTINUE : DIENUM_STOP;
}

void Joystick::init_main() {
	currentInterface->EnumDevices(DI8DEVCLASS_GAMECTRL, enumerationCallback, nullptr, DIEDFL_ATTACHEDONLY);
	if (!currentInstance->myInputDevice)
		throw CodeConv::tstring(_T("ジョイスティックがありません"));
	if (FAILED(currentInstance->myInputDevice->SetDataFormat(&c_dfDIJoystick)))
		throw CodeConv::tstring(_T("SetDataFormat失敗！！"));
	if (FAILED(currentInstance->myInputDevice->SetCooperativeLevel(currentInstance->myHWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		throw CodeConv::tstring(_T("SetCooperativeLevel失敗！！"));
	DIPROPDWORD diProp;
	diProp.diph.dwSize = sizeof(diProp);
	diProp.diph.dwHeaderSize = sizeof(diProp.diph);
	diProp.diph.dwObj = 0;
	diProp.diph.dwHow = DIPH_DEVICE;
	diProp.dwData = 1000;
	if (FAILED(reinterpret_cast<Joystick*>(currentInstance)->myInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProp.diph)))
		throw CodeConv::tstring(_T("SetProperty(DIPROP_BUFFERSIZE)失敗！！"));
	DIPROPRANGE diRange;
	diRange.diph.dwSize = sizeof(diRange);
	diRange.diph.dwHeaderSize = sizeof(diRange.diph);
	diRange.diph.dwObj = 0;
	diRange.diph.dwHow = DIPH_DEVICE;
	diRange.lMin = -32767;
	diRange.lMax = 32767;
	if (FAILED(reinterpret_cast<Joystick*>(currentInstance)->myInputDevice->SetProperty(DIPROP_RANGE, &diRange.diph)))
		throw CodeConv::tstring(_T("SetProperty(DIPROP_RANGE)失敗！！"));
	reinterpret_cast<Joystick*>(currentInstance)->myInputDevice->Acquire();
}
Joystick::Joystick(LPDIRECTINPUT8 inputInterface, HWND hwnd) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(myMutex);
	myInputDevice = nullptr;
	currentInstance = this;
	currentInterface = inputInterface;
	myHWnd = hwnd;

	init_main();
}

Joystick::~Joystick() {
	if (myInputDevice) {
		myInputDevice->Unacquire();
		myInputDevice->Release();
	}
}

// -------------------------------------------------------------------------

}
}
#endif /*_WIN32*/
