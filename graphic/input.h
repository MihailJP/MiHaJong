#pragma once

#ifdef _WIN32
#include <windows.h>
#include <dinput.h>
#include <utility>

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

class InputManipulator {
private:
	LPDIRECTINPUT8 myInterface;
	Keyboard* myKeyboard;
	Mouse* myMouse;
public:
	InputManipulator(HWND hwnd, bool fullscreen);
	~InputManipulator();
	Keyboard* kbd() {return myKeyboard;}
	Mouse* mouse() {return myMouse;}
};

}
}
#endif /*_WIN32*/
