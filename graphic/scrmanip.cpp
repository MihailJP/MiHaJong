#include "scrmanip.h"

void ScreenManipulator::InitDevice() { // Direct3D �I�u�W�F�N�g������
	/* Direct3D �I�u�W�F�N�g���� */
	pd3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pd3d) // �������s
		throw _T("Direct3D �I�u�W�F�N�g�̐����Ɏ��s���܂���");

	/* D3D�f�o�C�X�I�u�W�F�N�g���� */
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
		throw _T("Direct3D �f�o�C�X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
}
ScreenManipulator::ScreenManipulator(HWND windowHandle) {
	pDevice = nullptr; hWnd = windowHandle;
	InitDevice();
	myScene = new RuleConfigScene(this);
	myFPSIndicator = new FPSIndicator(this);
	lastRedrawTime = 0;
}

void ScreenManipulator::Render() {
	pDevice->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0); // �o�b�t�@�N���A
	if (SUCCEEDED(pDevice->BeginScene())) { // �V�[���J�n
		if (myScene) myScene->Render(); // �ĕ`�揈��
		if (myFPSIndicator) myFPSIndicator->Render(); // FPS�\��
		pDevice->EndScene(); // �V�[���I��
	}
	pDevice->Present(nullptr, nullptr, nullptr, nullptr); // ��ʂ̍X�V
	return;
}

ScreenManipulator::~ScreenManipulator() {
	if (myScene) delete myScene;
	if (myFPSIndicator) delete myFPSIndicator;
	if (pd3d) {pd3d->Release(); pd3d = nullptr;}
	if (pDevice) {pDevice->Release(); pDevice = nullptr;}
}
