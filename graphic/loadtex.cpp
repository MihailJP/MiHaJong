#include "loadtex.h"
#include "resource.h"

void LoadTexture(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height) {
	HRSRC Resource = FindResource(GraphicDLL, resource, MAKEINTRESOURCE(PNG_FILE));
	HGLOBAL ResourceMem = LoadResource(GraphicDLL, Resource);
	DWORD pngSize = SizeofResource(GraphicDLL, Resource);
	void* pngData = LockResource(ResourceMem);
	HRESULT result = 
		D3DXCreateTextureFromFileInMemoryEx(device, pngData, pngSize, width, height, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER, D3DX_DEFAULT,
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
