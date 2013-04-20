#include "main.h"

#include <cstdlib>
#ifndef _WIN32
#include <X11/Xlib.h>
#include <unistd.h>
#endif /*_WIN32*/
#include "../mjcore/mjimport.h"
#include "../graphic/graphic.h"
#include "resource.h"

extern const GameTypeID myGameType;

#ifdef _WIN32

#ifdef MINGW_UNICODE /* Workaround for MinGW */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	MSG msg; HWND hwnd;
	unsigned width, height; bool fullscreen;

	/* �E�B���h�E�ݒ�����s�ǂݍ��� */
	preferenceInit();
	getWindowSize(&width, &height, &fullscreen);

	/* �E�B���h�E������������ */
	if (!mihajong_graphic::InitWindow(hInstance, nCmdShow, MAKEINTRESOURCE(IDI_ICON1), &hwnd, width, height, fullscreen))
		exit(1); // ���s������I��
	
	/* �X�^�[�g */
	StartGame(myGameType, hwnd);

	/* ���C�����[�v */
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W������΃��b�Z�[�W�̏���
			if (!GetMessage(&msg, nullptr, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// �A�C�h�����ɍĕ`��
			mihajong_graphic::RefreshWindow();
			Sleep(1);
		}
	}
	
	/* �I������ */
	TerminateGame();
	ExitProcess(0);
	return msg.wParam;
}


#else /*_WIN32*/

int main(int argc, char* argv) {
	/* TODO: �������ӏ� */
	//MSG msg; HWND hwnd;
	Window hwnd;
	unsigned width, height; bool fullscreen;

	/* �E�B���h�E�ݒ�����s�ǂݍ��� */
	//preferenceInit();
	//getWindowSize(&width, &height, &fullscreen);
	width=800; height=600; fullscreen=false;

	/* �E�B���h�E������������ */
	if (!mihajong_graphic::InitWindow(nullptr, 0, MAKEINTRESOURCE(IDI_ICON1), &hwnd, width, height, fullscreen))
		exit(1); // ���s������I��

	/* �X�^�[�g */
	StartGame(myGameType, hwnd);

	/* ���C�����[�v */
	while (true) {
		if (!mihajong_graphic::Event()) // �C�x���g����
			break; // �I���Ȃ�false���Ԃ��Ă���
		usleep(100);
	}
	
	/* �I������ */
	mihajong_graphic::CleanupWindow(); // TODO: �b������BTerminateGame()�ł�邱��
	//TerminateGame();
	//ExitProcess(0);
	//return msg.wParam;
}

#endif /*_WIN32*/