#pragma once

#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <string>

#ifdef UNICODE
typedef std::wstring tstring;
#define to_tstring std::to_wstring
#else /* UNICODE */
typedef std::string tstring;
#define to_tstring std::to_string
#endif /* UNICODE */

namespace ControlWrapper {

class DialogControl {
protected:
	const HWND hWnd;
public:
	DialogControl(HWND hWnd, int nIDDlgItem) : hWnd(GetDlgItem(hWnd, nIDDlgItem)) {}
	DialogControl(const DialogControl&) = delete;
	DialogControl& operator=(const DialogControl&) = delete;
	virtual ~DialogControl() = 0 {};
	void enable(bool);
};

class TextBox : public DialogControl {
public:
	TextBox(HWND hWnd, int nIDDlgItem, size_t limit = 30000);
	tstring get();
	void set(const tstring&);
	size_t getLimit();
};

class RadioButton : public DialogControl {
public:
	RadioButton(HWND hWnd, int nIDDlgItem) : DialogControl(hWnd, nIDDlgItem) {}
	bool get();
	void set(bool);
};

class ComboBox : public DialogControl {
public:
	ComboBox(HWND hWnd, int nIDDlgItem) : DialogControl(hWnd, nIDDlgItem) {}
	ComboBox(HWND hWnd, int nIDDlgItem, const std::vector<tstring>&);
	int get();
	void set(int);
};

class Slider : public DialogControl {
public:
	Slider(HWND hWnd, int nIDDlgItem, LONG_PTR min, LONG_PTR max, UINT_PTR ticFreq);
	LONG get();
	void set(LONG);
};

class IPaddress : public DialogControl {
public:
	IPaddress(HWND hWnd, int nIDDlgItem) : DialogControl(hWnd, nIDDlgItem) {}
	DWORD get();
	void set(BYTE, BYTE, BYTE, BYTE);
	void set(DWORD);
};


}
