﻿#include "main.h"

#include <cstdlib>
#ifndef _WIN32
#include <X11/Xlib.h>
#include <unistd.h>
#endif /*_WIN32*/
#include "../mjcore/mjimport.h"
#include "../graphic/graphic.h"
#include "resource.h"
#include "../common/thread.h"
#include "../common/sleep.h"

extern const GameTypeID myGameType;

#ifdef _WIN32

#ifdef MINGW_UNICODE /* Workaround for MinGW */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	MSG msg; HWND hwnd;
	unsigned width, height; bool fullscreen;

	/* ウィンドウ設定情報を先行読み込み */
	preferenceInit();
	getWindowSize(&width, &height, &fullscreen);

	/* ウィンドウを初期化する */
	if (!mihajong_graphic::InitWindow(hInstance, nCmdShow, MAKEINTRESOURCE(IDI_ICON1), &hwnd, width, height, fullscreen))
		exit(1); // 失敗したら終了
	
	/* スタート */
	StartGame(myGameType, hwnd);

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
			threadYield();
		}
	}
	
	/* 終了処理 */
	TerminateGame();
	ExitProcess(0);
	return msg.wParam;
}


#else /*_WIN32*/

int main(int argc, char** argv) {
	Window hwnd;
	unsigned width, height; bool fullscreen;

	/* ウィンドウ設定情報を先行読み込み */
	preferenceInit();
	getWindowSize(&width, &height, &fullscreen);

	/* ウィンドウを初期化する */
	if (!mihajong_graphic::InitWindow(nullptr, 0, MAKEINTRESOURCE(IDI_ICON1), &hwnd, width, height, fullscreen))
		exit(1); // 失敗したら終了

	/* スタート */
	StartGame(myGameType, hwnd);

	/* メインループ */
	while (true) {
		if (!mihajong_graphic::Event()) // イベント処理
			break; // 終了ならfalseが返ってくる
		threadYield();
	}
	
	/* 終了処理 */
	TerminateGame();
	exit(0);
}

#endif /*_WIN32*/