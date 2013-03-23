#include "main.h"

extern const GameTypeID myGameType;

#ifdef MINGW_UNICODE /* Workaround for MinGW */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	MSG msg; HWND hwnd;

	/* �E�B���h�E������������ */
	if (!mihajong_graphic::InitWindow(hInstance, nCmdShow, MAKEINTRESOURCE(IDI_ICON1), &hwnd))
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
	return msg.wParam;
}
