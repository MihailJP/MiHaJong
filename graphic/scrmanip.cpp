#include "scrmanip.h"
#include "scenes/scenes.h"

namespace mihajong_graphic {

void ScreenManipulator::InitDevice() { // Direct3D オブジェクト初期化
	/* Direct3D オブジェクト生成 */
	pd3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pd3d) // 生成失敗
		throw _T("Direct3D オブジェクトの生成に失敗しました");

	/* D3Dデバイスオブジェクト生成 */
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

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
		throw _T("Direct3D デバイスオブジェクトの生成に失敗しました");
}
ScreenManipulator::ScreenManipulator(HWND windowHandle) {
	InitializeCriticalSection(&CS_SceneAccess);
	EnterCriticalSection(&CS_SceneAccess);
	redrawFlag = false;
	pDevice = nullptr; hWnd = windowHandle;
	InitDevice();
	myScene = new SplashScreen(this);
	myFPSIndicator = new FPSIndicator(this);
	lastRedrawTime = 0;
	redrawFlag = true;
	LeaveCriticalSection(&CS_SceneAccess);
}

void ScreenManipulator::Render() {
	EnterCriticalSection(&CS_SceneAccess);
	if (redrawFlag) {
		pDevice->Clear(0, nullptr, D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0); // バッファクリア
		if (SUCCEEDED(pDevice->BeginScene())) { // シーン開始
			if (myScene) myScene->Render(); // 再描画処理
			if (myFPSIndicator) myFPSIndicator->Render(); // FPS表示
			pDevice->EndScene(); // シーン終了
			pDevice->Present(nullptr, nullptr, nullptr, nullptr); // 画面の更新
		}
	}
	LeaveCriticalSection(&CS_SceneAccess);
	return;
}

void ScreenManipulator::transit(sceneID scene) {
	EnterCriticalSection(&CS_SceneAccess);
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
	case sceneGameTable:
		myScene = new GameTableScreen(this); redrawFlag = true;
		break;
	case sceneGameTableCall:
		myScene = new GameTableCall(this); redrawFlag = true;
		break;
	default:
		LeaveCriticalSection(&CS_SceneAccess);
		throw _T("正しくないシーン番号が指定されました");
	}
	LeaveCriticalSection(&CS_SceneAccess);
}

ScreenManipulator::~ScreenManipulator() {
	if (myScene) delete myScene;
	if (myFPSIndicator) delete myFPSIndicator;
	if (pd3d) {pd3d->Release(); pd3d = nullptr;}
	if (pDevice) {pDevice->Release(); pDevice = nullptr;}
	DeleteCriticalSection(&CS_SceneAccess);
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
		EnterCriticalSection(&CS_SceneAccess);
		inputProc(iManip->kbd(), [](Scene* sc, LPDIDEVICEOBJECTDATA od) -> void {
			if (sc) sc->KeyboardInput(od);
		});
		inputProc(iManip->mouse(), [iManip](Scene* sc, LPDIDEVICEOBJECTDATA od) -> void {
			input::Mouse::Position mousepos = iManip->mouse()->pos();
			if (sc) sc->MouseInput(od, mousepos.first, mousepos.second);
		});
		LeaveCriticalSection(&CS_SceneAccess);
	}
}

void ScreenManipulator::inputProc(WPARAM wParam, LPARAM lParam) {
	EnterCriticalSection(&CS_SceneAccess);
	if (myScene) myScene->KeyboardInput(wParam, lParam);
	LeaveCriticalSection(&CS_SceneAccess);
}

void ScreenManipulator::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	EnterCriticalSection(&CS_SceneAccess);
	if (myScene) myScene->IMEvent(message, wParam, lParam);
	LeaveCriticalSection(&CS_SceneAccess);
}

}
