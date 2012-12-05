#pragma once

#include <Windows.h>
#include <dinput.h>

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

class InputManipulator {
private:
	LPDIRECTINPUT8 myInterface;
	Keyboard* myKeyboard;
public:
	InputManipulator(HWND hwnd);
	~InputManipulator();
	Keyboard* kbd() {return myKeyboard;}
};

}
}
