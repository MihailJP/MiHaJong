#include "main.h"

#include <tuple>
#include "logging.h"
#include "init.h"
#include "func.h"
#include "../graphic/graphic.h"
#include "gameloop.h"
#ifndef _WIN32
#include <sys/types.h>
#include <unistd.h>
#endif /*_WIN32*/

GameThread* gameThread = nullptr;

#ifdef _WIN32
GameThread::GameThread(GameTypeID gameType, HWND hwnd)
#else /*_WIN32*/
GameThread::GameThread(GameTypeID gameType, Window hwnd)
#endif /*_WIN32*/
{
	myGameType = gameType;
	hWnd = hwnd;
	myThread = THREADLIB::thread(ThreadMain, this);
}

GameThread::~GameThread() {
#ifdef WITH_BOOST_THREAD
	myThread.interrupt();
	myThread.join();
	cleanup();
#ifdef _WIN32
	SendMessage(hWnd, WM_CLOSE, 0, 0);
#endif /*_WIN32*/
#else
	cleanup();
	exit(0);
#endif
}

void GameThread::ThreadMain(GameThread* lpParam) {
	GameTypeID gameType = lpParam->myGameType;
#ifdef _WIN32
	HWND hwnd = lpParam->hWnd;
#else /*_WIN32*/
	Window hwnd = lpParam->hWnd;
#endif /*_WIN32*/
	initapp(gameType, hwnd);
	startgame(gameType);
	cleanup();
	exit(0);
}

#ifdef _WIN32
MJCORE void StartGame (GameTypeID gameType, HWND hwnd)
#else /*_WIN32*/
MJCORE void StartGame (GameTypeID gameType, Window hwnd)
#endif /*_WIN32*/
{
	gameThread = new GameThread(gameType, hwnd);
}

MJCORE void TerminateGame () {
	delete gameThread;
	mihajong_graphic::CleanupWindow();
}
