#include "game.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../init.h"

void TableProtoScene::LoadTexture(LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height) {
	HRSRC Resource = FindResource(GraphicDLL, resource, MAKEINTRESOURCE(PNG_FILE));
	HGLOBAL ResourceMem = LoadResource(GraphicDLL, Resource);
	DWORD pngSize = SizeofResource(GraphicDLL, Resource);
	void* pngData = LockResource(ResourceMem);
	HRESULT result = 
		D3DXCreateTextureFromFileInMemoryEx(caller->getDevice(), pngData, pngSize, width, height, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0x00000000, nullptr, nullptr, texture);
	UnlockResource(ResourceMem);
	switch (result) {
	case D3D_OK:
		return; // Congratulations, your texture has been loaded.
	case D3DERR_NOTAVAILABLE:
		throw _T("�e�N�X�`���̐����Ɏ��s���܂����B");
	case D3DERR_OUTOFVIDEOMEMORY:
		throw _T("�e�N�X�`���̐����Ɏ��s���܂����BVRAM������܂���B");
	case D3DERR_INVALIDCALL:
		throw _T("�e�N�X�`���̐����Ɏ��s���܂����B�p�����[�^������������܂���B");
	case D3DXERR_INVALIDDATA:
		throw _T("�e�N�X�`���̐����Ɏ��s���܂����B�f�[�^���ُ�ł��B");
	case E_OUTOFMEMORY:
		throw _T("�e�N�X�`���̐����Ɏ��s���܂����B������������܂���B");
	default: // This may not happen...
		throw _T("�e�N�X�`���̐����Ɏ��s���܂����B�����s���̃G���[�ł��B");
	}
}
void TableProtoScene::InitSprite(LPD3DXSPRITE* sprite) {
	if (FAILED(D3DXCreateSprite(caller->getDevice(), sprite)))
		throw _T("�X�v���C�g�̐����Ɏ��s���܂���");
}

TableProtoScene::TableProtoScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	LoadTexture(&tSideBar, MAKEINTRESOURCE(IDB_PNG_SDBAR), 256, 768); InitSprite(&sSideBar);
}

TableProtoScene::~TableProtoScene() {
	if (tSideBar) tSideBar->Release();
	if (sSideBar) sSideBar->Release();
}

void TableProtoScene::ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height) {
	RECT rect = {0, 0, Width, Height};
	D3DXVECTOR3 Center(0, 0, 0);
	D3DXVECTOR3 Pos((float)X, (float)Y, 0);
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, &rect, &Center, &Pos, 0xffffffff);
	sprite->End();
}

void TableProtoScene::ShowSidebar() {
	ShowSprite(sSideBar, tSideBar, 768, 0, 256, 768);
}
