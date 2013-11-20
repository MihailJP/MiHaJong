#pragma once

#include "mjexport.h"
#include "gametbl.h"
#ifdef _WIN32
#include <windows.h>
#else /*_WIN32*/
#include <X11/Xlib.h>
#endif /*_WIN32*/
#include "../common/thread.h"

#ifdef MJCORE_EXPORTS

class GameThread {
private:
	GameTypeID myGameType;
	THREADLIB::thread myThread;
#ifdef _WIN32
	HWND hWnd;
#else /*_WIN32*/
	Window hWnd;
#endif /*_WIN32*/
	static void ThreadMain(GameThread* lpParam);
public:
#ifdef _WIN32
	explicit GameThread(GameTypeID gameType, HWND hWnd);
#else /*_WIN32*/
	explicit GameThread(GameTypeID gameType, Window hWnd);
#endif /*_WIN32*/
	GameThread(const GameThread&) = delete; // Delete unexpected copy constructor
	GameThread& operator= (const GameThread&) = delete; // Delete unexpected assign operator
	~GameThread();
};

extern GameThread* gameThread;
#endif

#ifdef _WIN32
MJCORE void StartGame (GameTypeID gameType, HWND hwnd);
#else /*_WIN32*/
MJCORE void StartGame (GameTypeID gameType, Window hWnd);
#endif /*_WIN32*/
MJCORE void TerminateGame ();
