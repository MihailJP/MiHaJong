#pragma once

#ifdef _WIN32
#include <windows.h>
#include <dinput.h>
#endif /*_WIN32*/
#include <utility>

namespace mihajong_graphic {
namespace input {

/* TODO: Linux‚Å“™‰¿‚Ì‚à‚Ì‚ðŽÀ‘•‚·‚é‚±‚Æ */
#ifdef _WIN32
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
public:
	Mouse(LPDIRECTINPUT8 inputInterface, HWND hwnd);
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
	InputManipulator(HWND hwnd);
	~InputManipulator();
	Keyboard* kbd() {return myKeyboard;}
	Mouse* mouse() {return myMouse;}
};
#endif /*_WIN32*/

}
}
