#include "init.h"

MainWindow* myMainWindow = nullptr;

EXPORT BOOL InitWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR icon) {
	/* ウィンドウの初期化 */
	try {
		myMainWindow = new MainWindow(hInstance, nCmdShow, icon);
	}
	catch (LPTSTR e) {
		MessageBox(nullptr, e, _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

EXPORT void RefreshWindow() {
}

EXPORT void CleanupWindow() {
	delete myMainWindow;
}
