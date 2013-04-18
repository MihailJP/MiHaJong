#include "main.h"

#include <tuple>
#include "logging.h"
#include "init.h"
#include "func.h"
#include "../graphic/graphic.h"
#include "gameloop.h"

GameThread* gameThread = nullptr;

#ifdef _WIN32
GameThread::GameThread(GameTypeID gameType, HWND hwnd) {
	myGameType = gameType;
	hWnd = hwnd;
	hThread = CreateThread(nullptr, 0, ThreadMain, this, 0, nullptr);
}
#else /*_WIN32*/
GameThread::GameThread(GameTypeID gameType, void* hwnd) {
	/* TODO: 未実装箇所 */
}
#endif /*_WIN32*/

GameThread::~GameThread() {
	/* WE ASSUME THAT THIS DESTRUCTOR GETS CALLED ONLY WHEN THE PROGRAM IS ABOUT TO END. */
	/* DO NOT CALL THIS DESTRUCTOR ELSEWHEN, OR IT MAY CAUSE SOME LEAKS!!! */
	/* プログラム終了時にのみ呼び出されることを想定しています */
	/* それ以外の時には絶対に呼び出さないこと！！！ */
#ifdef _WIN32
	DWORD exitCode; GetExitCodeThread(hThread, &exitCode);
	if (exitCode == STILL_ACTIVE) {
		warn(_T("スレッドを強制終了します！"));
		TerminateThread(hThread, S_OK);
	}
#else /*_WIN32*/
	/* TODO: 未実装箇所 */
#endif /*_WIN32*/
}

#ifdef _WIN32
DWORD WINAPI GameThread::ThreadMain(LPVOID lpParam) {
	GameTypeID gameType = reinterpret_cast<GameThread*>(lpParam)->myGameType;
	HWND hwnd = reinterpret_cast<GameThread*>(lpParam)->hWnd;
	initapp(gameType, hwnd);
	startgame(gameType);
	cleanup();
	SendMessage(hwnd, WM_CLOSE, 0, 0);
	return S_OK;
}
#else /*_WIN32*/
/* TODO: 未実装箇所 */
#endif /*_WIN32*/

#ifdef _WIN32
MJCORE void StartGame (GameTypeID gameType, HWND hwnd) {
	gameThread = new GameThread(gameType, hwnd);
}
#else /*_WIN32*/
MJCORE void StartGame (GameTypeID gameType, void* hwnd) {
	/* TODO: 暫定実装 */
	gameThread = new GameThread(gameType, nullptr);
}
#endif /*_WIN32*/

MJCORE void TerminateGame () {
	delete gameThread;
	mihajong_graphic::CleanupWindow();
}
