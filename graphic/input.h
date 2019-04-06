#pragma once

#ifdef _WIN32
#include <windows.h>
#include <dinput.h>
#include <utility>
#include <mutex>

namespace mihajong_graphic {
namespace input {

class InputDevice {
protected:
	LPDIRECTINPUTDEVICE8 myInputDevice;
	InputDevice() : myInputDevice(nullptr) {}
public:
	LPDIRECTINPUTDEVICE8 getDevice() {return myInputDevice;}
	InputDevice(const InputDevice&) = delete; // Delete unexpected copy constructor
	InputDevice& operator= (const InputDevice&) = delete; // Delete unexpected assign operator
	virtual ~InputDevice() = 0;
};
class Keyboard : public InputDevice {
public:
	Keyboard(LPDIRECTINPUT8 inputInterface, HWND hwnd);
	Keyboard(const Keyboard&) = delete; // Delete unexpected copy constructor
	Keyboard& operator= (const Keyboard&) = delete; // Delete unexpected assign operator
	~Keyboard();
};
class Mouse : public InputDevice {
private:
	HWND hWnd;
	bool fullScreenFlag;
public:
	Mouse(LPDIRECTINPUT8 inputInterface, HWND hwnd, bool fullscreen);
	Mouse(const Mouse&) = delete; // Delete unexpected copy constructor
	Mouse& operator= (const Mouse&) = delete; // Delete unexpected assign operator
	~Mouse();
	typedef std::pair<int, int> Position;
	Position pos();
};
class Joystick : public InputDevice {
private:
	static std::recursive_mutex myMutex;
	static Joystick* currentInstance;
	static LPDIRECTINPUT8 currentInterface;
	HWND myHWnd;
private:
	static BOOL CALLBACK enumerationCallback(const DIDEVICEINSTANCE *pdidInstance, LPVOID pContext);
	static void init_main();
public:
	Joystick(LPDIRECTINPUT8 inputInterface, HWND hwnd);
	Joystick(const Joystick&) = delete; // Delete unexpected copy constructor
	Joystick& operator= (const Joystick&) = delete; // Delete unexpected assign operator
	~Joystick();
};

class InputManipulator {
private:
	LPDIRECTINPUT8 myInterface;
	Keyboard* myKeyboard;
	Mouse* myMouse;
	Joystick* myJoystick;
public:
	InputManipulator(HWND hwnd, bool fullscreen);
	InputManipulator(const InputManipulator&) = delete; // Delete unexpected copy constructor
	InputManipulator& operator= (const InputManipulator&) = delete; // Delete unexpected assign operator
	~InputManipulator();
	Keyboard* kbd() {return myKeyboard;}
	Mouse* mouse() {return myMouse;}
	Joystick* pad() {return myJoystick;}
};

}
}
#endif /*_WIN32*/
