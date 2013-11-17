#include "window.h"
#include "../common/version.h"
#include "extchar.h"
#include "loadtex.h"
#ifndef _WIN32
#include <X11/Xutil.h>
#include <iostream>
#endif /*_WIN32*/
#include <cstdlib>

namespace mihajong_graphic {

const LPCTSTR MainWindow::myWindowClassName = _T("mihajong_main");
const LPCTSTR MainWindow::WindowCaption = _T("MiHaJong ver. ") _T(MIHAJONG_VER);
unsigned& MainWindow::WindowWidth = Geometry::WindowWidth;
unsigned& MainWindow::WindowHeight = Geometry::WindowHeight;
extern MainWindow* myMainWindow;

#ifdef _WIN32
LRESULT MainWindow::keyev(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (wParam) {
	case VK_LEFT:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(CHARDAT_CURSOR_LEFT, lParam);
		break;
	case VK_RIGHT:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(CHARDAT_CURSOR_RIGHT, lParam);
		break;
	case VK_RETURN:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(CHARDAT_CURSOR_ENTER, lParam);
		break;
	case VK_ESCAPE:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(CHARDAT_CURSOR_ESCAPE, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK MainWindow::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { // �E�B���h�E�v���V�[�W��
	switch (message) {
	case WM_DESTROY: // �E�B���h�E�������
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		return keyev(hWnd, message, wParam, lParam);
	case WM_CHAR:
		if (myMainWindow) myMainWindow->myScreenManipulator->inputProc(wParam, lParam);
		break;
	case WM_INPUTLANGCHANGE: case WM_IME_SETCONTEXT: case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION: case WM_IME_ENDCOMPOSITION: case WM_IME_NOTIFY:
		if (myMainWindow) myMainWindow->myScreenManipulator->IMEvent(message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void MainWindow::initWindowClass(HINSTANCE hThisInst, LPCTSTR icon) { // �E�B���h�E�N���X�̏�����
	WNDCLASSEX myWindowClass;
	
	myWindowClass.hInstance = hThisInst;
	myWindowClass.lpszClassName = myWindowClassName;
	myWindowClass.lpfnWndProc = WinProc;
#ifdef WITH_DIRECTX
	myWindowClass.style = 0;
#else
	myWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
#endif
	myWindowClass.cbSize = sizeof(WNDCLASSEX);
	myWindowClass.hIcon = (HICON)LoadImage(hThisInst, icon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	myWindowClass.hIconSm = (HICON)LoadImage(hThisInst, icon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	myWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	myWindowClass.lpszMenuName = nullptr;
	myWindowClass.cbClsExtra = 0;
	myWindowClass.cbWndExtra = 0;
	myWindowClass.hbrBackground = nullptr;
	
	if (!RegisterClassEx(&myWindowClass))
		throw _T("�E�B���h�E�N���X�̓o�^�Ɏ��s���܂���");
	else return;
}

void MainWindow::initWindow(HINSTANCE hThisInst, int nWinMode, bool fullscreen) {
	RECT WindowRect;
	WindowRect.left = 0; WindowRect.right = WindowWidth;
	WindowRect.top = 0; WindowRect.bottom = WindowHeight;
	DWORD ExStyle = 0;
	DWORD Style;
#ifndef WITH_DIRECTX
	if (fullscreen) {
		Style = WS_POPUP;
	} else {
#endif
		Style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		AdjustWindowRectEx(&WindowRect, Style, FALSE, ExStyle);
#ifndef WITH_DIRECTX
	}
#endif
	hWnd = CreateWindowEx(
		ExStyle, myWindowClassName, WindowCaption, Style,
#ifdef WITH_DIRECTX
		CW_USEDEFAULT, CW_USEDEFAULT,
#else
		fullscreen ? 0 : CW_USEDEFAULT,
		fullscreen ? 0 : CW_USEDEFAULT,
#endif
		WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
		nullptr, nullptr, hThisInst, nullptr);
	if (!hWnd) throw _T("�E�B���h�E�̐����Ɏ��s���܂���");
	ShowWindow(hWnd, nWinMode);
	UpdateWindow(hWnd);

#ifndef WITH_DIRECTX
	/* �t���X�N���[���ɂ��鏈��(WinAPI) */
	/* DirectX�̏ꍇ��DirectX���̐ݒ�łł��邪�AOpenGL�̏ꍇ��WinAPI�Őݒ肪�K�v(GLUT�͎g�p���Ȃ�) */
	if (fullscreen) {
		DEVMODE dMode; ZeroMemory(&dMode, sizeof dMode);
		dMode.dmSize = sizeof dMode;
		dMode.dmPelsWidth = WindowWidth;
		dMode.dmPelsHeight = WindowHeight;
		dMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
		if (FAILED(ChangeDisplaySettings(&dMode, CDS_FULLSCREEN)))
			throw _T("�t���X�N���[���ɂł��܂���ł���");
	}
#endif
	return;
}
#else /*_WIN32*/
bool MainWindow::WinProc(MainWindow* mainWindow) { // �E�B���h�E�v���V�[�W��
	if (XPending(mainWindow->disp)) { // �C�x���g����
		XEvent event;
		XNextEvent(mainWindow->disp, &event); // �C�x���g�ҋ@
		
		switch (event.type) {
		case ClientMessage:
			if (event.xclient.data.l[0] == mainWindow->wmDelMsg) { // �E�B���h�E�������
				XFlush(mainWindow->disp);
				return false;
			}
			break;
		case MotionNotify: // �}�E�X�|�C���^�̈ړ�
		case ButtonPress: // �}�E�X�{�^������������
		case ButtonRelease: // �}�E�X�{�^���𗣂�����
			mainWindow->myScreenManipulator->mouseInputProc(&event);
			break;
		case KeyPress: // �L�[����������
		case KeyRelease: // �L�[�𗣂�����
			mainWindow->myScreenManipulator->kbdInputProc(&event);
			break;
		}
		return true;
	} else { // �C�x���g�Ȃ�
		mainWindow->Render();
		return true;
	}
}

void MainWindow::initWindow(void* hThisInst, int nWinMode, bool fullscreen) {
	XInitThreads();
	disp = XOpenDisplay(nullptr); // �ڑ���f�B�X�v���C�� DISPLAY �Ŏw��
	if (disp == nullptr) throw _T("�f�B�X�v���C�ɐڑ��o���܂���BCannot connect to display.");
	int screen = DefaultScreen(disp);
	hWnd = XCreateSimpleWindow(
		disp,
		RootWindow(disp, screen),
		0, 0,
		WindowWidth, WindowHeight,
		1, BlackPixel(disp, screen),
		WhitePixel(disp, screen));
	
	std::string wName(CodeConv::toANSI(WindowCaption));
	XStoreName(disp, hWnd, wName.c_str()); // �E�B���h�E�L���v�V������ݒ�

	XSizeHints hints;
	hints.flags = PMinSize|PMaxSize; // �E�B���h�E�T�C�Y���Œ�
	hints.min_width = hints.max_width = WindowWidth;
	hints.min_height = hints.max_height = WindowHeight;
	XSetWMNormalHints(disp, hWnd, &hints);
	
	XSelectInput(disp, hWnd,
		PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
		KeyPressMask | KeyReleaseMask
		);

	wmDelMsg = XInternAtom(disp, "WM_DELETE_WINDOW", False);
	XMapWindow(disp, hWnd);
	XSetWMProtocols(disp, hWnd, &wmDelMsg, 1);
	XFlush(disp);
	return;
}
#endif /*_WIN32*/

#ifdef _WIN32
MainWindow::MainWindow(HINSTANCE hThisInst, int nWinMode, LPCTSTR icon, unsigned width, unsigned height, bool fullscreen) {
	Geometry::WindowWidth = width; Geometry::WindowHeight = height;
	initWindowClass(hThisInst, icon);
	initWindow(hThisInst, nWinMode, fullscreen);
	myScreenManipulator = new ScreenManipulator(hWnd, fullscreen);
	myInputManipulator = new input::InputManipulator(hWnd, fullscreen);
}
#else /*_WIN32*/
MainWindow::MainWindow(void* hThisInst, int nWinMode, LPCTSTR icon, unsigned width, unsigned height, bool fullscreen) {
	Geometry::WindowWidth = width; Geometry::WindowHeight = height;
	initWindow(hThisInst, nWinMode, fullscreen);
	myScreenManipulator = new ScreenManipulator(disp, hWnd, fullscreen);
}
#endif /*_WIN32*/

MainWindow::~MainWindow() {
	delete myScreenManipulator; myScreenManipulator = nullptr;
	UnloadAllTextures();
#ifndef _WIN32
	XDestroyWindow(disp, hWnd);
	XCloseDisplay(disp);
	std::exit(0);
#endif /*_WIN32*/
}

void MainWindow::Render() { // �E�B���h�E�̍ĕ`��
	if (myScreenManipulator) {
		myScreenManipulator->Render();
#ifdef _WIN32
		// Windows�̂݁BLinux�ł͕ʂ̉ӏ��œ��̓C�x���g�������̂ŕs�v
		ValidateRect(hWnd, nullptr);
		if (myInputManipulator)
			myScreenManipulator->inputProc(myInputManipulator);
#endif /*_WIN32*/
	}
	return;
}

void MainWindow::transit(sceneID scene) { // �V�[���؂�ւ�
	if (myScreenManipulator)
		myScreenManipulator->transit(scene);
}

void MainWindow::subscene(unsigned int subsceneID) { // �T�u�V�[���؂�ւ�
	if (myScreenManipulator)
		myScreenManipulator->subscene(subsceneID);
}

}
