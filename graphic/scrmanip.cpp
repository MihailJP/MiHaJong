#include "scrmanip.h"
#include "scenes/scenes.h"
#include "sprite.h"
#include "resource.h"
#include "except.h"

namespace mihajong_graphic {

#ifndef WITH_DIRECTX
#ifdef _WIN32
HGLRC ScreenManipulator::getContext() {
	HGLRC context = wglCreateContext(pDevice);
	wglShareLists(rContext, context);
	return context;
}
void ScreenManipulator::discardContext(HGLRC context) {
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(context);
}
#else /* _WIN32 */
GLXContext ScreenManipulator::getContext(bool shared) {
	//int numOfElements;
#ifdef __linux__
	if (shared) return pDevice;
#endif /* __linux__ */

	int attribs[] = {
		GLX_RGBA, GLX_DOUBLEBUFFER,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		0 /* sentinel */
	};
	XVisualInfo* vi = glXChooseVisual(disp, DefaultScreen(disp), attribs);
	GLXContext context = glXCreateContext(disp, vi, shared ? pDevice : nullptr, True);
	XFree(vi);
#if 0
	/* glXCreateContextAttribsARB() not supported */
	GLXFBConfig* fbc = glXChooseFBConfig(disp, DefaultScreen(disp), nullptr, &numOfElements);
	int attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 0,
		0
	};
	GLXContext context = glXCreateContextAttribsARB(disp, *fbc, shared ? pDevice : nullptr, True, attribs);
#endif
	return context;
}
void ScreenManipulator::discardContext(GLXContext context) {
#ifndef __linux__
	glXMakeCurrent(disp, hWnd, nullptr);
	glXDestroyContext(disp, context);
#endif /* __linux__ */
}
#endif /* _WIN32 */
#endif /* WITH_DIRECTX */

void ScreenManipulator::InitDevice(bool fullscreen) { // Direct3D オブジェクト初期化
#if defined(_WIN32) && defined(WITH_DIRECTX)
	/* Direct3D オブジェクト生成 */
	pd3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pd3d) // 生成失敗
		throw ModuleInitializationError("Direct3D オブジェクトの生成に失敗しました");

	/* D3Dデバイスオブジェクト生成 */
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
		throw ModuleInitializationError("Direct3D デバイスオブジェクトの生成に失敗しました");
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
	pDevice = getContext(false);
	glXMakeCurrent(disp, hWnd, pDevice);
#endif
	
	TexturePtr dummyTexture;
	LoadTexture(pDevice, &dummyTexture, MAKEINTRESOURCE(IDB_PNG_FONT));
	LoadTexture(pDevice, &dummyTexture, MAKEINTRESOURCE(IDB_PNG_SPLASH_SCREEN));
}

void ScreenManipulator::preloadTextures() { // テクスチャの先行読み込み
	constexpr intptr_t textureList[] = {
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
		IDB_PNG_SPLASH_SCREEN,
		IDB_PNG_TITLE_BACKGROUND,
		0, // sentinel
	};
	TexturePtr dummyTexture;
	for (const intptr_t* i = textureList; *i != 0; ++i)
		LoadTexture(pDevice, &dummyTexture, MAKEINTRESOURCE(*i));
}

void ScreenManipulator::disposeTextures() {
	UnloadAllTextures();
#endif
}
#ifdef _WIN32
ScreenManipulator::ScreenManipulator(HWND windowHandle, bool fullscreen) {
	pd3d = 0;
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	redrawFlag = false;
	pDevice = nullptr; hWnd = windowHandle;
	InitDevice(fullscreen);
	SplashScreen::LoadTexture(pDevice);
	myScene = new SplashScreen(this);
	myFPSIndicator = new FPSIndicator(this);
	lastRedrawTime = 0;
	redrawFlag = true;
}
#else /*_WIN32*/
ScreenManipulator::ScreenManipulator(Display* displayPtr, Window windowHandle, bool fullscreen) {
	pd3d = 0;
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
			D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0); // バッファクリア
		if (SUCCEEDED(pDevice->BeginScene())) { // シーン開始
			SpriteRenderer::instantiate(pDevice)->Start(); // スプライト描画開始
			if (myScene) myScene->Render(); // 再描画処理
			if (myFPSIndicator) myFPSIndicator->Render(); // FPS表示
			SpriteRenderer::instantiate(pDevice)->End(); // スプライト描画終了
			pDevice->EndScene(); // シーン終了
			pDevice->Present(nullptr, nullptr, nullptr, nullptr); // 画面の更新
		}
#else
#ifdef _WIN32
		wglMakeCurrent(pDevice, rContext);
#else /*_WIN32*/
		glXMakeCurrent(disp, hWnd, pDevice);
#endif /*_WIN32*/
		glClearColor(1, 1, 1, 1); glClear(GL_COLOR_BUFFER_BIT); // バッファクリア
		SpriteRenderer::instantiate(pDevice)->Start(); // スプライト描画開始
		if (myScene) myScene->Render(); // 再描画処理
		if (myFPSIndicator) myFPSIndicator->Render(); // FPS表示
		SpriteRenderer::instantiate(pDevice)->End(); // スプライト描画終了
		glFlush();
#ifdef _WIN32
		SwapBuffers(pDevice); // 画面の更新
#else /*_WIN32*/
		glXSwapBuffers(disp, hWnd); // 画面の更新
#endif /*_WIN32*/
#endif
	}
	return;
}

void ScreenManipulator::transit(SceneID scene) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
#ifdef _WIN32
	HGLRC context = getContext();
	wglMakeCurrent(pDevice, context);
#else /*_WIN32*/
	GLXContext context = getContext(true);
	glXMakeCurrent(disp, hWnd, context);
#endif /*_WIN32*/
#endif
	redrawFlag = false;
	delete myScene; myScene = nullptr;
	if ((scene != SceneID::null) && (!myFPSIndicator))
		myFPSIndicator = new FPSIndicator(this);
	switch (scene) {
	case SceneID::null:
		delete myFPSIndicator, myFPSIndicator = nullptr;
		break;
	case SceneID::splash:
		myScene = new SplashScreen(this); redrawFlag = true;
		break;
	case SceneID::title:
		myScene = new TitleScreen(this); redrawFlag = true;
		break;
	case SceneID::config:
		myScene = new RuleConfigScene(this); redrawFlag = true;
		break;
	case SceneID::serverWaiting:
		myScene = new ServerWait(this); redrawFlag = true;
		break;
	case SceneID::clientWaiting:
		myScene = new ClientWait(this); redrawFlag = true;
		break;
	case SceneID::waitingError:
		myScene = new ConnectionWaitFailed(this); redrawFlag = true;
		break;
	case SceneID::gameTable:
		myScene = new GameTableScreen(this); redrawFlag = true;
		break;
	case SceneID::result:
		myScene = new ResultScreen(this); redrawFlag = true;
		break;
	default:
#ifndef WITH_DIRECTX
		discardContext(context);
#endif /* WITH_DIRECTX */
		throw InvalidScene("正しくないシーン番号が指定されました");
	}
#ifndef WITH_DIRECTX
	discardContext(context);
#endif /* WITH_DIRECTX */
}

void ScreenManipulator::subscene(SubSceneID subsceneID) {
#ifdef __linux__
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
#endif /* __linux__ */
#ifndef WITH_DIRECTX
#ifdef _WIN32
	HGLRC context = getContext();
	wglMakeCurrent(pDevice, context);
#else /*_WIN32*/
	GLXContext context = getContext(true);
	glXMakeCurrent(disp, hWnd, context);
#endif /*_WIN32*/
#endif /* WITH_DIRECTX */
	if (myScene)
		myScene->SetSubscene(subsceneID);
#ifndef WITH_DIRECTX
	discardContext(context);
#endif /* WITH_DIRECTX */
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
	disposeTextures();
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(rContext);
	ReleaseDC(hWnd, pDevice);
#endif
#else /*_WIN32*/
	disposeTextures();
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

void ScreenManipulator::kbdInputProc(const XEvent* event) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	if (myScene) myScene->KeyboardInput(event);
}

void ScreenManipulator::mouseInputProc(const XEvent* event) {
	std::unique_lock<std::recursive_mutex> lock(CS_SceneAccess);
	Window rtw, chw; // ←取得して捨てる
	int rtx, rty; // ←取得して捨てる
	unsigned mask; // ←取得して捨てる
	int x, y; // ←これだけ使う
	XQueryPointer(disp, hWnd, &rtw, &chw, &rtx, &rty, &x, &y, &mask);
	if (myScene) myScene->MouseInput(event, x, y);
}
#endif /*_WIN32*/

}
