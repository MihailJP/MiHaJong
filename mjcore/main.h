#pragma once

#include "mjexport.h"
#include "gametbl.h"
#include <windows.h>

#ifdef MJCORE_EXPORTS

class GameThread {
private:
	GameTypeID myGameType;
	HANDLE hThread;
	HWND hWnd;
	static DWORD WINAPI ThreadMain(LPVOID lpParam);
public:
	explicit GameThread(GameTypeID gameType, HWND hWnd);
	~GameThread();
};

extern GameThread* gameThread;
#endif

MJCORE void StartGame (GameTypeID gameType, HWND hwnd);
MJCORE void TerminateGame ();
