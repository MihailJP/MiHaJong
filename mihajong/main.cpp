#include "main.h"

#ifdef MINGW_UNICODE /* Workaround for MinGW */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#else
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
#endif
	GameTable* const gameStat = setGameType(Yonma);
	GameStatus::updateGameStat(gameStat);
	MSG msg;

	/* �E�B���h�E������������ */
	if (!InitWindow(hInstance, nCmdShow, MAKEINTRESOURCE(IDI_ICON1)))
		exit(1); // ���s������I��
	
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
