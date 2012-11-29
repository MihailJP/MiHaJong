#include "main.h"

#ifdef MINGW_UNICODE /* Workaround for MinGW */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	GameTable* const gameStat = setGameType(Yonma);
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
	MSG msg;

	/* ウィンドウを初期化する */
	if (!mihajong_graphic::InitWindow(hInstance, nCmdShow, MAKEINTRESOURCE(IDI_ICON1)))
		exit(1); // 失敗したら終了
	
	/* メインループ */
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
			// メッセージがあればメッセージの処理
			if (!GetMessage(&msg, nullptr, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// アイドル時に再描画
			mihajong_graphic::RefreshWindow();
			Sleep(1);
		}
	}
	
	/* 終了処理 */
	mihajong_graphic::CleanupWindow();
	return msg.wParam;
}
