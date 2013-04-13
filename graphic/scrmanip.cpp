#include "scrmanip.h"
#include "scenes/scenes.h"
#include "sprite.h"

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
	/* TODO: OpenGL�ōĎ��� */
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

	HGLRC rContext = wglCreateContext(pDevice);
	wglMakeCurrent(pDevice, rContext);
#endif
}
ScreenManipulator::ScreenManipulator(HWND windowHandle, bool fullscreen) {
	CS_SceneAccess.syncDo<void>([this, windowHandle, fullscreen]() -> void {
		redrawFlag = false;
		pDevice = nullptr; hWnd = windowHandle;
		InitDevice(fullscreen);
		myScene = new SplashScreen(this);
		myFPSIndicator = new FPSIndicator(this);
		lastRedrawTime = 0;
		redrawFlag = true;
	});
}

void ScreenManipulator::Render() {
	CS_SceneAccess.syncDo<void>([this]() -> void {
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
			/* TODO: OpenGL�ōĎ��� */
			//HGLRC rContext = wglCreateContext(pDevice);
			//wglMakeCurrent(pDevice, rContext);

			glClearColor(1, 1, 1, 1); glClear(GL_COLOR_BUFFER_BIT); // �o�b�t�@�N���A
			SpriteRenderer::instantiate(pDevice)->Start(); // �X�v���C�g�`��J�n
			if (myScene) myScene->Render(); // �ĕ`�揈��
			if (myFPSIndicator) myFPSIndicator->Render(); // FPS�\��
			SpriteRenderer::instantiate(pDevice)->End(); // �X�v���C�g�`��I��
			glFlush();
			SwapBuffers(pDevice); // ��ʂ̍X�V

			//wglMakeCurrent(nullptr, nullptr);
			//wglDeleteContext(rContext);
#endif
		}
	});
	return;
}

void ScreenManipulator::transit(sceneID scene) {
	CS_SceneAccess.syncDo<void>([this, scene]() -> void {
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
			throw _T("�������Ȃ��V�[���ԍ����w�肳��܂���");
		}
	});
}

void ScreenManipulator::subscene(unsigned int subsceneID) {
	if (myScene)
		myScene->SetSubscene(subsceneID);
}

ScreenManipulator::~ScreenManipulator() {
	if (myScene) delete myScene;
	if (myFPSIndicator) delete myFPSIndicator;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (pd3d) {pd3d->Release(); pd3d = nullptr;}
#endif
	SpriteRenderer::delInstance(pDevice);
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (pDevice) {pDevice->Release(); pDevice = nullptr;}
#else
	ReleaseDC(hWnd, pDevice);
#endif
}

void ScreenManipulator::inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f) {
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
		CS_SceneAccess.syncDo<void>([this, iManip]() -> void {
			input::InputManipulator* iManip_ = iManip;
			inputProc(iManip->kbd(), [](Scene* sc, LPDIDEVICEOBJECTDATA od) -> void {
				if (sc) sc->KeyboardInput(od);
			});
			inputProc(iManip->mouse(), [iManip_](Scene* sc, LPDIDEVICEOBJECTDATA od) -> void {
				input::Mouse::Position mousepos = iManip_->mouse()->pos();
				if (sc) sc->MouseInput(od, mousepos.first, mousepos.second);
			});
		});
	}
}

void ScreenManipulator::inputProc(WPARAM wParam, LPARAM lParam) {
	CS_SceneAccess.syncDo<void>([this, wParam, lParam]() -> void {
		if (myScene) myScene->KeyboardInput(wParam, lParam);
	});
}

void ScreenManipulator::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	CS_SceneAccess.syncDo<void>([this, message, wParam, lParam]() -> void {
		if (myScene) myScene->IMEvent(message, wParam, lParam);
	});
}

}
