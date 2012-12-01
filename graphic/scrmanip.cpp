#include "scrmanip.h"

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
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice
		)))
		return;
	else if (SUCCEEDED(pd3d->CreateDevice( // HEL
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice
		)))
		return;
	else if (SUCCEEDED(pd3d->CreateDevice( // REF
		D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice
		)))
		return;
	else // All the four failed
		throw _T("Direct3D デバイスオブジェクトの生成に失敗しました");
}
ScreenManipulator::ScreenManipulator(HWND windowHandle) {
	pDevice = nullptr; hWnd = windowHandle;
	InitDevice();
	myScene = new TitleScreen(this);
	myFPSIndicator = new FPSIndicator(this);
	lastRedrawTime = 0;
}

void ScreenManipulator::Render() {
	pDevice->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0); // バッファクリア
	if (SUCCEEDED(pDevice->BeginScene())) { // シーン開始
		if (myScene) myScene->Render(); // 再描画処理
		if (myFPSIndicator) myFPSIndicator->Render(); // FPS表示
		pDevice->EndScene(); // シーン終了
	}
	pDevice->Present(nullptr, nullptr, nullptr, nullptr); // 画面の更新
	return;
}

void ScreenManipulator::transit(sceneID scene) {
	delete myScene; myScene = nullptr;
	switch (scene) {
	case sceneSplash:
		break;
	case sceneTitle:
		break;
	case sceneConfig:
		break;
	case sceneGameTable:
		myScene = new GameTableScreen(this);
		break;
	default:
		throw _T("正しくないシーン番号が指定されました");
	}
}

ScreenManipulator::~ScreenManipulator() {
	if (myScene) delete myScene;
	if (myFPSIndicator) delete myFPSIndicator;
	if (pd3d) {pd3d->Release(); pd3d = nullptr;}
	if (pDevice) {pDevice->Release(); pDevice = nullptr;}
}

void ScreenManipulator::inputProc(input::InputDevice* inputDev, std::function<void (Scene*, LPDIDEVICEOBJECTDATA)> f) {
	DIDEVICEOBJECTDATA objDat; DWORD items = 1;
	HRESULT hr = inputDev->getDevice()->GetDeviceData(
		sizeof(DIDEVICEOBJECTDATA), &objDat, &items, 0);
	if (FAILED(hr)) /*(hr == DIERR_INPUTLOST)*/
		inputDev->getDevice()->Acquire();
	else if (SUCCEEDED(hr) && (items > 0))
		f(myScene, &objDat);
}
void ScreenManipulator::inputProc(input::InputManipulator* iManip) {
	if (iManip) {
		inputProc(iManip->kbd(), [](Scene* sc, LPDIDEVICEOBJECTDATA od) -> void {sc->KeyboardInput(od);});
	}
}

}
