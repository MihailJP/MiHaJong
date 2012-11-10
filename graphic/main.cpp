#include "main.h"

#ifdef MINGW_UNICODE /* Workaround for MinGW */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	MSG msg;
	
	/* ウィンドウの初期化 */
	MainWindow* myMainWindow = nullptr;
	try {
		myMainWindow = new MainWindow(hInstance, nCmdShow);
	}
	catch (LPTSTR e) {
		MessageBox(nullptr, e, _T("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	
	/* メインループ */
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	/* 終了処理 */
	delete myMainWindow;
	return msg.wParam;
}
