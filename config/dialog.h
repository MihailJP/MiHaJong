#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "controls.h"
#include <map>
#include <memory>
#include "../mjcore/conffile/conffile.h"

namespace DialogWrapper {

class Dialog {
private:
	static Dialog* activeInstance;
	static int CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
	const HINSTANCE hInstance;
	const int dialogID;
protected:
	std::map<int, ControlWrapper::DialogControl*> controls;
	virtual void initWrapper(HWND) = 0 {};
	virtual void okButtonPressed() = 0 {}
public:
	Dialog(HINSTANCE hInstance, int dialogID) : hInstance(hInstance), dialogID(dialogID) {};
	Dialog(const Dialog&) = delete;
	Dialog& operator=(const Dialog&) = delete;
	virtual ~Dialog();
	virtual bool show(HWND parent = nullptr);
};

class ConfigDialog : public Dialog {
private:
	ConfigFile::ConfigFile confFile;
protected:
	virtual void initWrapper(HWND) override;
	virtual void okButtonPressed() override;
public:
	explicit ConfigDialog(HINSTANCE hInstance) : Dialog(hInstance, IDD_DIALOG1) {};
};

}
