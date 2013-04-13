#include "loadtex.h"
#include "resource.h"
#if defined(_WIN32) && !defined(WITH_DIRECTX)
#include <gdiplus.h>
#endif
#include <map>
#include <cassert>
#include "../common/strcode.h"

namespace mihajong_graphic {

namespace {
	std::map<int, TexturePtr> Textures;
}

void LoadTexture(DevicePtr device, TexturePtr* texture, LPCTSTR resource) {
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	using namespace Gdiplus;
#endif
	assert(((int)resource & 0xffff0000) == 0); // ��ʃ��[�h��0�Ȃ當����ł͂Ȃ����\�[�X�ԍ��Ƃ݂Ȃ����(Win32API�̎d�l)
	if (Textures.find((int)resource) != Textures.end()) { // ���Ƀ��[�h�ς݂̃e�N�X�`��
#if defined(_WIN32) && defined(WITH_DIRECTX)
		Textures[(int)resource]->AddRef();
#endif
		*texture = Textures[(int)resource];
		return;
	} else { // ���[�h����Ă��Ȃ��ꍇ
		HRSRC Resource = FindResource(GraphicDLL, resource, MAKEINTRESOURCE(PNG_FILE));
		HGLOBAL ResourceMem = LoadResource(GraphicDLL, Resource);
		DWORD pngSize = SizeofResource(GraphicDLL, Resource);
		void* pngData = LockResource(ResourceMem);
#if defined(_WIN32) && defined(WITH_DIRECTX)
		Textures[(int)resource] = nullptr;
		HRESULT result = 
			D3DXCreateTextureFromFileInMemoryEx(device, pngData, pngSize, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0,
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
#else
		/* TODO: OpenGL�ōĎ��� */
		Textures[(int)resource] = 0;
		glGenTextures(1, &Textures[(int)resource]);
		glBindTexture(GL_TEXTURE_2D, Textures[(int)resource]);
		HGLOBAL resBuf = GlobalAlloc(GMEM_MOVEABLE, pngSize);
		void* pResBuf = GlobalLock(resBuf);
		Bitmap* bitmap = nullptr;
		BitmapData data;
		if (pResBuf) {
			CopyMemory(pResBuf, pngData, pngSize);
			IStream* stream = nullptr;
			if (SUCCEEDED(CreateStreamOnHGlobal(resBuf, false, &stream))) {
				bitmap = Bitmap::FromStream(stream);
				Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
				bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &data);
				stream->Release();
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->GetWidth(), bitmap->GetHeight(), 0,
					GL_RGBA, GL_UNSIGNED_INT, data.Scan0);
				bitmap->UnlockBits(&data);
			}
		}
		GlobalUnlock(resBuf);
		GlobalFree(resBuf);
		UnlockResource(ResourceMem);
		delete bitmap;
#endif
	}
}

void UnloadAllTextures() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	for (auto k = Textures.begin(); k != Textures.end(); ++k) {
		ULONG refs = (*k).second->Release();
		CodeConv::tostringstream o;
		o << _T("UnloadAllTextures(): Texture resource #") << (*k).first << _T(": remaining number of refs is ") << refs << std::endl;
		OutputDebugString(o.str().c_str());
	}
#else
	/* TODO: OpenGL�ōĎ��� */
#endif
	Textures.clear();
}

}
