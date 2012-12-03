#ifndef MAIN_H
#define MAIN_H

#include "mjexport.h"
#include "gametbl.h"
#include <Windows.h>

#ifdef MJCORE_EXPORTS
#include <tuple>
#include "logging.h"
#include "init.h"
#include "func.h"
#include "../graphic/graphic.h"
#include "gameloop.h"

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

#endif
