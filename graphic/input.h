#pragma once

#ifdef _WIN32
#include <windows.h>
#include <dinput.h>
#include <utility>
#include "../common/mutex.h"

namespace mihajong_graphic {
namespace input {

class InputDevice {
protected:
	LPDIRECTINPUTDEVICE8 myInputDevice;
public:
	LPDIRECTINPUTDEVICE8 getDevice() {return myInputDevice;}
	virtual ~InputDevice() = 0;
};
class Keyboard : public InputDevice {
public:
	Keyboard(LPDIRECTINPUT8 inputInterface, HWND hwnd);
	~Keyboard();
};
class Mouse : public InputDevice {
private:
	HWND hWnd;
	bool fullScreenFlag;
public:
	Mouse(LPDIRECTINPUT8 inputInterface, HWND hwnd, bool fullscreen);
	~Mouse();
	typedef std::pair<int, int> Position;
	Position pos();
};
class Joystick : public InputDevice {
private:
	static MHJMutex myMutex;
	static Joystick* currentInstance;
	static LPDIRECTINPUT8 currentInterface;
	HWND myHWnd;
private:
	static BOOL CALLBACK enumerationCallback(const DIDEVICEINSTANCE *pdidInstance, LPVOID pContext);
	static void init_main();
	static void init_finally();
public:
	Joystick(LPDIRECTINPUT8 inputInterface, HWND hwnd);
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
	~InputManipulator();
	Keyboard* kbd() {return myKeyboard;}
	Mouse* mouse() {return myMouse;}
	Joystick* pad() {return myJoystick;}
};

}
}
#endif /*_WIN32*/
