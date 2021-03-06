﻿#define NOMINMAX
#include "dialog.h"
#include <WinUser.h>
#include <cassert>
#include <Windows.h>

#pragma comment(lib, "Winmm.lib")

namespace DialogWrapper {

Dialog* Dialog::activeInstance = nullptr;

intptr_t CALLBACK Dialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
		default:
			return activeInstance->controlPressed(LOWORD(wParam));
		}
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

std::vector<tstring> monitors() {
	std::vector<tstring> monitorList;

	DISPLAY_DEVICE device, monitor;
	memset(&device,  0, sizeof device );
	memset(&monitor, 0, sizeof monitor);
	device .cb = sizeof(DISPLAY_DEVICE);
	monitor.cb = sizeof(DISPLAY_DEVICE);

	for (int i = 0; EnumDisplayDevices(nullptr, i, &device, 0) != 0; ++i) {
		if (device.StateFlags & DISPLAY_DEVICE_ACTIVE) {
			HDC hdc = CreateDC(device.DeviceName, device.DeviceName, nullptr, nullptr);
			if (hdc) {
				EnumDisplayDevices(device.DeviceName, 0, &monitor, 0);
				int width = GetDeviceCaps(hdc, HORZRES), height = GetDeviceCaps(hdc, VERTRES);
				monitorList.push_back(
					to_tstring(i + 1) + _T(" : ") + tstring(monitor.DeviceString)
					+ _T(", ") + to_tstring(width) + _T("x") + to_tstring(height));
				DeleteDC(hdc);
			}
		}
	}

	return monitorList;
}

std::vector<tstring> midiDevices() {
	std::vector<tstring> deviceList{_T("DirectSound")};
	const UINT numOfDevs(midiOutGetNumDevs());
	MIDIOUTCAPS midiOutCaps{};

	for (unsigned int i = 0; i < numOfDevs; ++i) {
		if (midiOutGetDevCaps(i, &midiOutCaps, sizeof midiOutCaps) == MMSYSERR_NOERROR) {
			deviceList.push_back(midiOutCaps.szPname);
		}
	}

	return deviceList;
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
#ifdef WITH_4K_PICS
		_T("WQHD (2560 x 1440)"),
		_T("4K (3840 x 2160)"),
#endif /* WITH_4K_PICS */
		});

	const auto monitorList(monitors());

	const auto midiDeviceList(midiDevices());

	controls.emplace(IDC_EDIT2, new TextBox(hWnd, IDC_EDIT2, 32));
	controls.emplace(IDC_EDIT3, new TextBox(hWnd, IDC_EDIT3, 64));
	controls.emplace(IDC_RADIO1, new RadioButton(hWnd, IDC_RADIO1));
	controls.emplace(IDC_RADIO2, new RadioButton(hWnd, IDC_RADIO2));
	controls.emplace(IDC_RADIO3, new RadioButton(hWnd, IDC_RADIO3));
	controls.emplace(IDC_RADIO4, new RadioButton(hWnd, IDC_RADIO4));
	controls.emplace(IDC_RADIO5, new RadioButton(hWnd, IDC_RADIO5));
	controls.emplace(IDC_SLIDER1, new Slider(hWnd, IDC_SLIDER1, 0, 20, 5));
	controls.emplace(IDC_SLIDER2, new Slider(hWnd, IDC_SLIDER2, 0, 20, 5));
	controls.emplace(IDC_COMBO1, new ComboBox(hWnd, IDC_COMBO1, resolutionList));
	controls.emplace(IDC_COMBO2, new ComboBox(hWnd, IDC_COMBO2, monitorList));
	controls.emplace(IDC_COMBO3, new ComboBox(hWnd, IDC_COMBO3, midiDeviceList));

	dynamic_cast<RadioButton*>(controls[IDC_RADIO1])->set(confFile.scrMode() == Screen_Mode::ScreenMode::fullscreen);
	dynamic_cast<RadioButton*>(controls[IDC_RADIO2])->set(confFile.scrMode() == Screen_Mode::ScreenMode::windowed);
	dynamic_cast<RadioButton*>(controls[IDC_RADIO3])->set(!confFile.blackTile());
	dynamic_cast<RadioButton*>(controls[IDC_RADIO4])->set(confFile.blackTile());
	dynamic_cast<RadioButton*>(controls[IDC_RADIO5])->set(confFile.scrMode() == Screen_Mode::ScreenMode::borderless);
	dynamic_cast<TextBox*>(controls[IDC_EDIT2])->set(confFile.playerName());
	dynamic_cast<Slider*>(controls[IDC_SLIDER1])->set(confFile.bgmVolume() / 5);
	dynamic_cast<Slider*>(controls[IDC_SLIDER2])->set(confFile.soundVolume() / 5);
	if (confFile.screenResolution() == ConfigFile::ScreenConfig::invalid)
		dynamic_cast<ComboBox*>(controls[IDC_COMBO1])->set(static_cast<int>(ConfigFile::ScreenConfig::xga));
	else
		dynamic_cast<ComboBox*>(controls[IDC_COMBO1])->set(static_cast<int>(confFile.screenResolution()));
	if (!confFile.serverAddress().empty())
		dynamic_cast<TextBox*>(controls[IDC_EDIT3])->set(confFile.serverAddress());
	dynamic_cast<ComboBox*>(controls[IDC_COMBO2])->set(confFile.monitorNumber() - 1);

	controls[IDC_COMBO1]->enable(confFile.scrMode() != Screen_Mode::ScreenMode::borderless);
	controls[IDC_COMBO2]->enable(confFile.scrMode() != Screen_Mode::ScreenMode::fullscreen);

	dynamic_cast<ComboBox*>(controls[IDC_COMBO3])->set(0);
	for (size_t i = 0; i < midiDeviceList.size(); ++i) {
		if (confFile.midiDevice() == CodeConv::lower(midiDeviceList.at(i)))
			dynamic_cast<ComboBox*>(controls[IDC_COMBO3])->set(i);
	}
}

void ConfigDialog::okButtonPressed() {
	using namespace ControlWrapper;

	const auto midiDeviceList(midiDevices());

	if (dynamic_cast<RadioButton*>(controls[IDC_RADIO1])->get())
		confFile.scrMode(Screen_Mode::ScreenMode::fullscreen);
	else if (dynamic_cast<RadioButton*>(controls[IDC_RADIO5])->get())
		confFile.scrMode(Screen_Mode::ScreenMode::borderless);
	else
		confFile.scrMode(Screen_Mode::ScreenMode::windowed);
	confFile.blackTile(dynamic_cast<RadioButton*>(controls[IDC_RADIO4])->get());
	confFile.playerName(dynamic_cast<TextBox*>(controls[IDC_EDIT2])->get());
	try {
		confFile.midiDevice(midiDeviceList.at(dynamic_cast<ComboBox*>(controls[IDC_COMBO3])->get()));
	} catch (std::out_of_range&) {
		confFile.midiDevice(_T(""));
	}
	confFile.bgmVolume(dynamic_cast<Slider*>(controls[IDC_SLIDER1])->get() * 5);
	confFile.soundVolume(dynamic_cast<Slider*>(controls[IDC_SLIDER2])->get() * 5);
	confFile.screenResolution(static_cast<ConfigFile::ScreenConfig>(dynamic_cast<ComboBox*>(controls[IDC_COMBO1])->get()));
	confFile.monitorNumber(dynamic_cast<ComboBox*>(controls[IDC_COMBO2])->get() + 1);
	confFile.serverAddress(dynamic_cast<TextBox*>(controls[IDC_EDIT3])->get());

	confFile.save();
}

BOOL ConfigDialog::controlPressed(WORD id) {
	switch (id) {
	case IDC_RADIO1:
		controls[IDC_COMBO1]->enable(true);
		controls[IDC_COMBO2]->enable(false);
		return TRUE;
	case IDC_RADIO2:
		controls[IDC_COMBO1]->enable(true);
		controls[IDC_COMBO2]->enable(true);
		return TRUE;
	case IDC_RADIO5:
		controls[IDC_COMBO1]->enable(false);
		controls[IDC_COMBO2]->enable(true);
		return TRUE;
	default:
		return FALSE;
	}
}

}
