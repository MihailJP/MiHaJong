#include "main.h"

#include <tuple>
#include "logging.h"
#include "init.h"
#include "func.h"
#include "../graphic/graphic.h"
#include "gameloop.h"

GameThread* gameThread = nullptr;

GameThread::GameThread(gameTypeID gameType, HWND hwnd) {
	myGameType = gameType;
	hWnd = hwnd;
	hThread = CreateThread(nullptr, 0, ThreadMain, this, 0, nullptr);
}

GameThread::~GameThread() {
	/* WE ASSUME THAT THIS DESTRUCTOR GETS CALLED ONLY WHEN THE PROGRAM IS ABOUT TO END. */
	/* DO NOT CALL THIS DESTRUCTOR ELSEWHEN, OR IT MAY CAUSE SOME LEAKS!!! */
	/* �v���O�����I�����ɂ̂݌Ăяo����邱�Ƃ�z�肵�Ă��܂� */
	/* ����ȊO�̎��ɂ͐�΂ɌĂяo���Ȃ����ƁI�I�I */
	DWORD exitCode; GetExitCodeThread(hThread, &exitCode);
	if (exitCode == STILL_ACTIVE) {
		warn(_T("�X���b�h�������I�����܂��I"));
		TerminateThread(hThread, S_OK);
	}
}

DWORD WINAPI GameThread::ThreadMain(LPVOID lpParam) {
	gameTypeID gameType = reinterpret_cast<GameThread*>(lpParam)->myGameType;
	HWND hwnd = reinterpret_cast<GameThread*>(lpParam)->hWnd;
	initapp(gameType, hwnd);
	startgame(gameType);
	cleanup();
	SendMessage(hwnd, WM_CLOSE, 0, 0);
	return S_OK;
}

MJCORE void StartGame (gameTypeID gameType, HWND hwnd) {
	gameThread = new GameThread(gameType, hwnd);
}

MJCORE void TerminateGame () {
	delete gameThread;
	mihajong_graphic::CleanupWindow();
}
