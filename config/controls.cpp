#include "controls.h"
#include <CommCtrl.h>

namespace ControlWrapper {

void DialogControl::enable(bool val) {
	EnableWindow(hWnd, val);
}



bool RadioButton::get() {
	return (bool)SendMessage(hWnd, BM_GETCHECK, 0, 0);
}

void RadioButton::set(bool val) {
	SendMessage(hWnd, BM_SETCHECK, val ? BST_CHECKED : BST_UNCHECKED, 0);
}



TextBox::TextBox(HWND hWndDlg, int nIDDlgItem, size_t limit) : DialogControl(hWndDlg, nIDDlgItem) {
	SendMessage(hWnd, EM_SETLIMITTEXT, limit, 0);
}

tstring TextBox::get() {
	LPTSTR buf = new TCHAR[getLimit() + 1];
	GetWindowText(hWnd, buf, getLimit());
	tstring ret(buf);
	delete[] buf, buf = nullptr;
	return ret;
}

void TextBox::set(const tstring& text) {
	SetWindowText(hWnd, text.c_str());
}

size_t TextBox::getLimit() {
	return SendMessage(hWnd, EM_GETLIMITTEXT, 0, 0);
}



Slider::Slider(HWND hWndDlg, int nIDDlgItem, LONG_PTR min, LONG_PTR max, UINT_PTR ticFreq) : DialogControl(hWndDlg, nIDDlgItem) {
	SendMessage(hWnd, TBM_SETRANGEMIN, TRUE, min);
	SendMessage(hWnd, TBM_SETRANGEMAX, TRUE, max);
	SendMessage(hWnd, TBM_SETTICFREQ, ticFreq, 0);
}

LONG Slider::get() {
	return SendMessage(hWnd, TBM_GETPOS, 0, 0);
}

void Slider::set(LONG val) {
	SendMessage(hWnd, TBM_SETPOS, TRUE, val);
}



ComboBox::ComboBox(HWND hWnd, int nIDDlgItem, const std::vector<tstring>& list) : ComboBox(hWnd, nIDDlgItem) {
	for (auto i : list) {
		SendDlgItemMessage(hWnd, nIDDlgItem, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(i.c_str()));
	}
}

int ComboBox::get() {
	return SendMessage(hWnd, CB_GETCURSEL, 0, 0);
}

void ComboBox::set(int val) {
	SendMessage(hWnd, CB_SETCURSEL, val, 0);
}



DWORD IPaddress::get() {
	DWORD ip;
	SendMessage(hWnd, IPM_GETADDRESS, 0, reinterpret_cast<LPARAM>(&ip));
	return ip;
}
void IPaddress::set(DWORD ip) {
	SendMessage(hWnd, IPM_SETADDRESS, 0, (LPARAM)ip);
}
void IPaddress::set(BYTE first, BYTE second, BYTE third, BYTE fourth) {
	set(MAKEIPADDRESS(first, second, third, fourth));
}

}
