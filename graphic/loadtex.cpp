#include "loadtex.h"
#include "resource.h"
#include <map>
#include <cassert>
#include "../mjcore/strcode.h"

namespace mihajong_graphic {

namespace {
	std::map<int, LPDIRECT3DTEXTURE9> Textures;
}

void LoadTexture(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height) {
	assert(((int)resource & 0xffff0000) == 0); // ��ʃ��[�h��0�Ȃ當����ł͂Ȃ����\�[�X�ԍ��Ƃ݂Ȃ����(Win32API�̎d�l)
	if (Textures.find((int)resource) != Textures.end()) { // ���Ƀ��[�h�ς݂̃e�N�X�`��
		Textures[(int)resource]->AddRef();
		*texture = Textures[(int)resource];
		return;
	} else { // ���[�h����Ă��Ȃ��ꍇ
		HRSRC Resource = FindResource(GraphicDLL, resource, MAKEINTRESOURCE(PNG_FILE));
		HGLOBAL ResourceMem = LoadResource(GraphicDLL, Resource);
		DWORD pngSize = SizeofResource(GraphicDLL, Resource);
		void* pngData = LockResource(ResourceMem);
		Textures[(int)resource] = nullptr;
		HRESULT result = 
			D3DXCreateTextureFromFileInMemoryEx(device, pngData, pngSize, width, height, 0, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER, D3DX_DEFAULT,
			0x00000000, nullptr, nullptr, &(Textures[(int)resource]));
		UnlockResource(ResourceMem);
		switch (result) {
		case D3D_OK:
			Textures[(int)resource]->AddRef();
			*texture = Textures[(int)resource];
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
}

void UnloadAllTextures() {
	for (auto k = Textures.begin(); k != Textures.end(); ++k) {
		ULONG refs = (*k).second->Release();
		CodeConv::tostringstream o;
		o << _T("UnloadAllTextures(): Texture resource #") << (*k).first << _T(": remaining number of refs is ") << refs << std::endl;
		OutputDebugString(o.str().c_str());
	}
	Textures.clear();
}

}
