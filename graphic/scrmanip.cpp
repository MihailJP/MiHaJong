#include "scrmanip.h"

HRESULT ScreenManipulator::InitDevice() { // Direct3D オブジェクト初期化
	/* Direct3D オブジェクト生成 */
	LPDIRECT3D9 pd3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pd3d) { // 生成失敗
		MessageBox(hWnd, _T("Direct3D オブジェクトの生成に失敗しました"), _T("Error"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	/* D3Dデバイスオブジェクト生成 */
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;

	if (SUCCEEDED(pd3d->CreateDevice( // HAL, with hardware vertex processing
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice
		)))
		return S_OK;
	else if (SUCCEEDED(pd3d->CreateDevice( // HAL, with software vertex processing
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice
		)))
		return S_OK;
	else if (SUCCEEDED(pd3d->CreateDevice( // REF, with hardware vertex processing
		D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice
		)))
		return S_OK;
	else if (SUCCEEDED(pd3d->CreateDevice( // REF, with software vertex processing
		D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice
		)))
		return S_OK;
	else { // All the four failed
		MessageBox(nullptr, _T("Direct3D デバイスオブジェクトの生成に失敗しました"), _T("Error"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
}
ScreenManipulator::ScreenManipulator(HWND windowHandle) {
	pDevice = nullptr; hWnd = windowHandle;
	if (FAILED(InitDevice())) throw FALSE; /* 例外仮置き */
}

