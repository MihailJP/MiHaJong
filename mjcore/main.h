#pragma once

#include "mjexport.h"
#include "gametbl.h"
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/

#ifdef MJCORE_EXPORTS

class GameThread {
private:
	GameTypeID myGameType;
#ifdef _WIN32
	HANDLE hThread;
	HWND hWnd;
	static DWORD WINAPI ThreadMain(LPVOID lpParam);
#else /*_WIN32*/
	/* TODO: �������ӏ� */
#endif /*_WIN32*/
public:
#ifdef _WIN32
	explicit GameThread(GameTypeID gameType, HWND hWnd);
#else /*_WIN32*/
	explicit GameThread(GameTypeID gameType, void* hWnd); // TODO: �b��
#endif /*_WIN32*/
	~GameThread();
};

extern GameThread* gameThread;
#endif

#ifdef _WIN32
MJCORE void StartGame (GameTypeID gameType, HWND hwnd);
#else /*_WIN32*/
MJCORE void StartGame (GameTypeID gameType, void* hwnd); // TODO: �b��
#endif /*_WIN32*/
MJCORE void TerminateGame ();
