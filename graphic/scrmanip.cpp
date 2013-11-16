#include "scrmanip.h"
#include "scenes/scenes.h"
#include "sprite.h"
#include "resource.h"

namespace mihajong_graphic {

void ScreenManipulator::InitDevice(bool fullscreen) { // Direct3D �I�u�W�F�N�g������
#if defined(_WIN32) && defined(WITH_DIRECTX)
	/* Direct3D �I�u�W�F�N�g���� */
	pd3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pd3d) // �������s
		throw _T("Direct3D �I�u�W�F�N�g�̐����Ɏ��s���܂���");

	/* D3D�f�o�C�X�I�u�W�F�N�g���� */
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = Geometry::WindowWidth;
	d3dpp.BackBufferHeight = Geometry::WindowHeight;
	d3dpp.BackBufferFormat = fullscreen ? D3DFMT_X8R8G8B8 : D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = !fullscreen;

	if (SUCCEEDED(pd3d->CreateDevice( // HAL
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &pDevice
		)))
		return;
	else if (SUCCEEDED(pd3d->CreateDevice( // HEL
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &pDevice
		)))
		return;
	else if (SUCCEEDED(pd3d->CreateDevice( // REF
		D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp, &pDevice
		)))
		return;
	else // All the four failed
		throw _T("Direct3D �f�o�C�X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
#else
#ifdef _WIN32
	pDevice = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iFormat = ChoosePixelFormat(pDevice, &pfd);
	SetPixelFormat(pDevice, iFormat, &pfd);

	rContext = wglCreateContext(pDevice);
	wglMakeCurrent(pDevice, rContext);
#else
	int numOfElements;
	
	int attribs[] = {
		GLX_RGBA, GLX_DOUBLEBUFFER,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		0 /* sentinel */
	};
	XVisualInfo* vi = glXChooseVisual(disp, DefaultScreen(disp), attribs);
	pDevice = glXCreateContext(disp, vi, nullptr, true);
	XFree(vi);
#if 0
	/* glXCreateContextAttribsARB() not supported */
	GLXFBConfig* fbc = glXChooseFBConfig(disp, DefaultScreen(disp), nullptr, &numOfElements);
	int attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 0,
		0
	};
	pDevice = glXCreateContextAttribsARB(disp, *fbc, 0, true, attribs);
#endif

	glXMakeCurrent(disp, hWnd, pDevice);
#endif

	const intptr_t textureList[] = { // �e�N�X�`���̐�s�ǂݍ���
		IDB_PNG_TBLBAIZE,
		IDB_PNG_TBLBORDER,
		IDB_PNG_SDBAR,
		IDB_PNG_TILE,
		IDB_PNG_FONT,
		IDB_PNG_TITLE,
		IDB_PNG_BUTTON,
		IDB_PNG_FONT_HUGE,
		IDB_PNG_DICE,
		IDB_PNG_FONT_SMALL,
		IDB_PNG_TEXTBOX,
		IDB_PNG_TENBOU,
		IDB_PNG_CHICHAMARK,
		IDB_PNG_SCORE_INDICATOR,
		IDB_PNG_CALL_TEXT,
		IDB_PNG_CALL_DIGITS,
		IDB_PNG_AGARI_WINDOW,
		IDB_PNG_SCORE_DIGITS,
		IDB_PNG_CHECKBOX,
		IDB_PNG_TILE_BLACK,
		IDB_PNG_MOON_CLOCK,
		0, // sentinel
	};
	TexturePtr dummyTexture;
	for (const intptr_t* i = textureList; *i != 0; ++i)
		LoadTexture(pDevice, &dummyTexture, MAKEINTRESOURCE(*i));
#endif
}
#ifdef _WIN32
ScreenManipulator::ScreenManipulator(HWND windowHandle, bool fullscreen) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	redrawFlag = false;
	pDevice = nullptr; hWnd = windowHandle;
	InitDevice(fullscreen);
	myScene = new SplashScreen(this);
	myFPSIndicator = new FPSIndicator(this);
	lastRedrawTime = 0;
	redrawFlag = true;
}
#else /*_WIN32*/
ScreenManipulator::ScreenManipulator(Display* displayPtr, Window windowHandle, bool fullscreen) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	redrawFlag = false;
	pDevice = nullptr; disp = displayPtr; hWnd = windowHandle;
	InitDevice(fullscreen);
	myScene = new SplashScreen(this);
	myFPSIndicator = new FPSIndicator(this);
	lastRedrawTime = 0;
	redrawFlag = true;
}
#endif /*_WIN32*/

void ScreenManipulator::Render() {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	if (redrawFlag) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
		pDevice->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0); // �o�b�t�@�N���A
		if (SUCCEEDED(pDevice->BeginScene())) { // �V�[���J�n
			SpriteRenderer::instantiate(pDevice)->Start(); // �X�v���C�g�`��J�n
			if (myScene) myScene->Render(); // �ĕ`�揈��
			if (myFPSIndicator) myFPSIndicator->Render(); // FPS�\��
			SpriteRenderer::instantiate(pDevice)->End(); // �X�v���C�g�`��I��
			pDevice->EndScene(); // �V�[���I��
			pDevice->Present(nullptr, nullptr, nullptr, nullptr); // ��ʂ̍X�V
		}
#else
#ifdef _WIN32
		wglMakeCurrent(pDevice, rContext);
#else /*_WIN32*/
		glXMakeCurrent(disp, hWnd, pDevice);
#endif /*_WIN32*/
		glClearColor(1, 1, 1, 1); glClear(GL_COLOR_BUFFER_BIT); // �o�b�t�@�N���A
		SpriteRenderer::instantiate(pDevice)->Start(); // �X�v���C�g�`��J�n
		if (myScene) myScene->Render(); // �ĕ`�揈��
		if (myFPSIndicator) myFPSIndicator->Render(); // FPS�\��
		SpriteRenderer::instantiate(pDevice)->End(); // �X�v���C�g�`��I��
		glFlush();
#ifdef _WIN32
		SwapBuffers(pDevice); // ��ʂ̍X�V
#else /*_WIN32*/
		glXSwapBuffers(disp, hWnd); // ��ʂ̍X�V
#endif /*_WIN32*/
#endif
	}
	return;
}

void ScreenManipulator::transit(sceneID scene) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
#ifdef _WIN32
	HGLRC context = wglCreateContext(pDevice);
	wglShareLists(rContext, context);
	wglMakeCurrent(pDevice, context);
#else /*_WIN32*/
	// Linux�ł͕ʂ̃R���e�L�X�g�ɂ���K�v�Ȃ��H
	glXMakeCurrent(disp, hWnd, pDevice);
#endif /*_WIN32*/
#endif
	redrawFlag = false;
	delete myScene; myScene = nullptr;
	switch (scene) {
	case sceneSplash:
		myScene = new SplashScreen(this); redrawFlag = true;
		break;
	case sceneTitle:
		myScene = new TitleScreen(this); redrawFlag = true;
		break;
	case sceneConfig:
		myScene = new RuleConfigScene(this); redrawFlag = true;
		break;
	case sceneSetting:
		myScene = new PreferenceConfigScene(this); redrawFlag = true;
		break;
	case sceneServerWaiting:
		myScene = new ServerWait(this); redrawFlag = true;
		break;
	case sceneClientWaiting:
		myScene = new ClientWait(this); redrawFlag = true;
		break;
	case sceneWaitingError:
		myScene = new ConnectionWaitFailed(this); redrawFlag = true;
		break;
	case sceneGameTable:
		myScene = new GameTableScreen(this); redrawFlag = true;
		break;
	case sceneResult:
		myScene = new ResultScreen(this); redrawFlag = true;
		break;
	default:
#if defined(_WIN32) && !defined(WITH_DIRECTX)
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(context);
#endif
		throw _T("�������Ȃ��V�[���ԍ����w�肳��܂���");
	}
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(context);
#endif
}

void ScreenManipulator::subscene(unsigned int subsceneID) {
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
#ifdef _WIN32
	HGLRC context = wglCreateContext(pDevice);
	wglShareLists(rContext, context);
	wglMakeCurrent(pDevice, context);
#else /*_WIN32*/
	
	glXMakeCurrent(disp, hWnd, pDevice);
#endif /*_WIN32*/
#endif
	if (myScene)
		myScene->SetSubscene(subsceneID);
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(context);
#endif
}

ScreenManipulator::~ScreenManipulator() {
#ifdef _WIN32
	if (myScene) delete myScene;
	if (myFPSIndicator) delete myFPSIndicator;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (pd3d) {pd3d->Release(); pd3d = nullptr;}
#endif
	SpriteRenderer::delInstance(pDevice);
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (pDevice) {pDevice->Release(); pDevice = nullptr;}
#else
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(rContext);
	ReleaseDC(hWnd, pDevice);
#endif
#else /*_WIN32*/
	glXMakeCurrent(disp, 0, nullptr);
	glXDestroyContext(disp, pDevice);
#endif /*_WIN32*/
}

#ifdef _WIN32
void ScreenManipulator::inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f) {
	if (inputDev == nullptr) return;
	while (true) {
		DIDEVICEOBJECTDATA objDat; DWORD items = 1;
		HRESULT hr = inputDev->getDevice()->GetDeviceData(
			sizeof(DIDEVICEOBJECTDATA), &objDat, &items, 0);
		if (FAILED(hr)) /*(hr == DIERR_INPUTLOST)*/ {
			inputDev->getDevice()->Acquire();
			break;
		}
		else if (SUCCEEDED(hr) && (items > 0)) {
			f(myScene, &objDat);
		}
		else {
			break;
		}
	}
}
void ScreenManipulator::inputProc(input::InputManipulator* iManip) {
	if (iManip) {
		std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
		input::InputManipulator* iManip_ = iManip;
		inputProc(iManip->kbd(), [](Scene* sc, LPDIDEVICEOBJECTDATA od) -> void {
			if (sc) sc->KeyboardInput(od);
		});
		inputProc(iManip->pad(), [](Scene* sc, LPDIDEVICEOBJECTDATA od) -> void {
			if (sc) sc->PadInput(od);
		});
		inputProc(iManip->mouse(), [iManip_](Scene* sc, LPDIDEVICEOBJECTDATA od) -> void {
			input::Mouse::Position mousepos = iManip_->mouse()->pos();
			if (sc) sc->MouseInput(od, mousepos.first, mousepos.second);
		});
	}
}

void ScreenManipulator::inputProc(WPARAM wParam, LPARAM lParam) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	if (myScene) myScene->KeyboardInput(wParam, lParam);
}

void ScreenManipulator::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	if (myScene) myScene->IMEvent(message, wParam, lParam);
}
#else /*_WIN32*/
/* TODO: Linux�ł͓��{����͂������� */

void ScreenManipulator::kbdInputProc(const XEvent* event) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	if (myScene) myScene->KeyboardInput(event);
}

void ScreenManipulator::mouseInputProc(const XEvent* event) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	Window rtw, chw; // ���擾���Ď̂Ă�
	int rtx, rty; // ���擾���Ď̂Ă�
	unsigned mask; // ���擾���Ď̂Ă�
	int x, y; // �����ꂾ���g��
	XQueryPointer(disp, hWnd, &rtw, &chw, &rtx, &rty, &x, &y, &mask);
	if (myScene) myScene->MouseInput(event, x, y);
}
#endif /*_WIN32*/

}
