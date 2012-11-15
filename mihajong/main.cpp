#include "main.h"

#ifdef MINGW_UNICODE /* Workaround for MinGW */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	MSG msg;

	/* �E�B���h�E������������ */
	if (!InitWindow(hInstance, nCmdShow)) // ���s������I��
		exit(1);
	
	/* ���C�����[�v */
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W������΃��b�Z�[�W�̏���
			if (!GetMessage(&msg, nullptr, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// �A�C�h�����ɍĕ`��
			RefreshWindow();
			Sleep(1);
		}
	}
	
	/* �I������ */
	CleanupWindow();
	return msg.wParam;
}
