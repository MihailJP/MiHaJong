#include "dialog.h"

namespace DialogWrapper {

Dialog* Dialog::activeInstance = nullptr;

int CALLBACK Dialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		activeInstance->initWrapper(hwndDlg);
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hwndDlg, IDCANCEL);
			return TRUE;
		case IDOK:
			activeInstance->okButtonPressed();
			EndDialog(hwndDlg, IDOK);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		EndDialog(hwndDlg, IDCANCEL);
		return TRUE;
	}
	return FALSE;
}

bool Dialog::show(HWND parent) {
	activeInstance = this;
	DialogBox(hInstance, reinterpret_cast<LPCTSTR>(dialogID), parent, DialogProc);
	activeInstance = nullptr;
	return false;
}

Dialog::~Dialog() {
	for (auto i : controls) {
		delete i.second;
	}
}

void ConfigDialog::initWrapper(HWND hWnd) {
	using namespace ControlWrapper;

	const std::vector<tstring> resolutionList({
		_T("SVGA (800 x 600)"),
		_T("XGA (1024 x 768)"),
		_T("FWXGA (1366 x 768)"),
		_T("SXGA (1280 x 1024)"),
		_T("UXGA (1600 x 1200)"),
		_T("Full HD (1920 x 1080)"),
		_T("WUXGA (1920 x 1200)"),
		});

	controls.emplace(IDC_EDIT2, new TextBox(hWnd, IDC_EDIT2, 32));
	controls.emplace(IDC_RADIO1, new RadioButton(hWnd, IDC_RADIO1));
	controls.emplace(IDC_RADIO2, new RadioButton(hWnd, IDC_RADIO2));
	controls.emplace(IDC_RADIO3, new RadioButton(hWnd, IDC_RADIO3));
	controls.emplace(IDC_RADIO4, new RadioButton(hWnd, IDC_RADIO4));
	controls.emplace(IDC_SLIDER1, new Slider(hWnd, IDC_SLIDER1, 0, 20, 5));
	controls.emplace(IDC_SLIDER2, new Slider(hWnd, IDC_SLIDER2, 0, 20, 5));
	controls.emplace(IDC_COMBO1, new ComboBox(hWnd, IDC_COMBO1, resolutionList));
	controls.emplace(IDC_IPADDRESS1, new IPaddress(hWnd, IDC_IPADDRESS1));

	dynamic_cast<RadioButton*>(controls[IDC_RADIO1])->set(confFile.fullScreen());
	dynamic_cast<RadioButton*>(controls[IDC_RADIO2])->set(!confFile.fullScreen());
	dynamic_cast<RadioButton*>(controls[IDC_RADIO3])->set(!confFile.blackTile());
	dynamic_cast<RadioButton*>(controls[IDC_RADIO4])->set(confFile.blackTile());
	dynamic_cast<TextBox*>(controls[IDC_EDIT2])->set(confFile.playerName());
	dynamic_cast<Slider*>(controls[IDC_SLIDER1])->set(confFile.bgmVolume() / 5);
	dynamic_cast<Slider*>(controls[IDC_SLIDER2])->set(confFile.soundVolume() / 5);
	dynamic_cast<ComboBox*>(controls[IDC_COMBO1])->set(confFile.screenResolution());
	if (confFile.serverAddress())
		dynamic_cast<IPaddress*>(controls[IDC_IPADDRESS1])->set(confFile.serverAddress());
}

void ConfigDialog::okButtonPressed() {
	using namespace ControlWrapper;

	confFile.fullScreen(dynamic_cast<RadioButton*>(controls[IDC_RADIO1])->get());
	confFile.blackTile(dynamic_cast<RadioButton*>(controls[IDC_RADIO4])->get());
	confFile.playerName(dynamic_cast<TextBox*>(controls[IDC_EDIT2])->get());
	confFile.bgmVolume(dynamic_cast<Slider*>(controls[IDC_SLIDER1])->get() * 5);
	confFile.soundVolume(dynamic_cast<Slider*>(controls[IDC_SLIDER2])->get() * 5);
	confFile.screenResolution(static_cast<ConfigFile::ScreenConfig>(dynamic_cast<ComboBox*>(controls[IDC_COMBO1])->get()));
	confFile.serverAddress(dynamic_cast<IPaddress*>(controls[IDC_IPADDRESS1])->get());

	confFile.save();
}

}
