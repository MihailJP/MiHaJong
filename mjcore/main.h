#pragma once

#include "mjexport.h"
#include "gametbl.h"
#include <Windows.h>

#ifdef MJCORE_EXPORTS

class GameThread {
private:
	gameTypeID myGameType;
	HANDLE hThread;
	HWND hWnd;
	static DWORD WINAPI ThreadMain(LPVOID lpParam);
public:
	explicit GameThread(gameTypeID gameType, HWND hWnd);
	~GameThread();
};

extern GameThread* gameThread;
#endif

MJCORE void StartGame (gameTypeID gameType, HWND hwnd);
MJCORE void TerminateGame ();
