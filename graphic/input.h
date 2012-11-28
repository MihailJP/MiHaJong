#ifndef GRAPHIC_INPUT_H
#define GRAPHIC_INPUT_H

#include <Windows.h>
#include <dinput.h>
#include "exports.h"

namespace mihajong_graphic {
namespace input {
#ifdef GRAPHIC_EXPORTS
class InputManipulator {
private:
	LPDIRECTINPUT8 myInterface;
public:
	InputManipulator(HWND hwnd);
	~InputManipulator();
};

class InputDevice {
protected:
	LPDIRECTINPUTDEVICE8 myInputDevice;
};
class Keyboard : public InputDevice {
public:
	Keyboard(LPDIRECTINPUT8 inputInterface, HWND hwnd);
	~Keyboard();
};
#endif

EXPORT void InitInputManipulator(HWND hwnd);
EXPORT void DisposeInputManipulator();

}
}

#endif
