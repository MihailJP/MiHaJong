#include "loadtex.h"
#include "resource.h"
#if defined(_WIN32) && !defined(WITH_DIRECTX)
#include <gdiplus.h>
#endif
#include <map>
#include <cassert>
#include "../common/strcode.h"

namespace mihajong_graphic {

//namespace {
	std::map<intptr_t, TexturePtr> Textures;
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	std::map<TexturePtr, unsigned> TextureWidth;
	std::map<TexturePtr, unsigned> TextureHeight;
#endif
//}

void LoadTexture(DevicePtr device, TexturePtr* texture, LPCTSTR resource) {
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	using namespace Gdiplus;
#endif
	assert(((intptr_t)resource & 0xffff0000) == 0); // ��ʃ��[�h��0�Ȃ當����ł͂Ȃ����\�[�X�ԍ��Ƃ݂Ȃ����(Win32API�̎d�l)
	if (Textures.find((intptr_t)resource) != Textures.end()) { // ���Ƀ��[�h�ς݂̃e�N�X�`��
#if defined(_WIN32) && defined(WITH_DIRECTX)
		Textures[(intptr_t)resource]->AddRef();
#endif
		*texture = Textures[(intptr_t)resource];
		return;
	} else { // ���[�h����Ă��Ȃ��ꍇ
#ifdef _WIN32
		HRSRC Resource = FindResource(GraphicDLL, resource, MAKEINTRESOURCE(PNG_FILE));
		HGLOBAL ResourceMem = LoadResource(GraphicDLL, Resource);
		DWORD pngSize = SizeofResource(GraphicDLL, Resource);
		void* pngData = LockResource(ResourceMem);
#if defined(_WIN32) && defined(WITH_DIRECTX)
		Textures[(intptr_t)resource] = nullptr;
		HRESULT result = 
			D3DXCreateTextureFromFileInMemoryEx(device, pngData, pngSize, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER, D3DX_DEFAULT,
			0x00000000, nullptr, nullptr, &(Textures[(intptr_t)resource]));
		UnlockResource(ResourceMem);
		switch (result) {
		case D3D_OK:
			Textures[(intptr_t)resource]->AddRef();
			*texture = Textures[(intptr_t)resource];
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
		Textures[(intptr_t)resource] = 0;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &Textures[(intptr_t)resource]);
		TextureWidth[Textures[(intptr_t)resource]] =
		TextureHeight[Textures[(intptr_t)resource]] = 0;
		glBindTexture(GL_TEXTURE_2D, Textures[(intptr_t)resource]);
		HGLOBAL resBuf = GlobalAlloc(GMEM_MOVEABLE, pngSize);
		void* pResBuf = GlobalLock(resBuf);
		Bitmap* bitmap = nullptr;
		BitmapData data;
		IStream* stream = nullptr;
		if (pResBuf) {
			CopyMemory(pResBuf, pngData, pngSize);
			if (SUCCEEDED(CreateStreamOnHGlobal(resBuf, false, &stream))) {
				bitmap = Bitmap::FromStream(stream);
				Rect rect(0, 0, bitmap->GetWidth(), bitmap->GetHeight());
				bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &data);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.Width, data.Height, 0,
					GL_BGRA_EXT, GL_UNSIGNED_BYTE, data.Scan0);
				bitmap->UnlockBits(&data);
				TextureWidth[Textures[(int)resource]] = data.Width;
				TextureHeight[Textures[(int)resource]] = data.Height;
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		GlobalUnlock(resBuf);
		GlobalFree(resBuf);
		UnlockResource(ResourceMem);
		if (stream) stream->Release();
		delete bitmap;
		*texture = Textures[(int)resource];
		return;
#endif
#else /* _WIN32 */
		/* TODO: �����̎����Blibpng���K�v�Ǝv���� */
#endif /* _WIN32 */
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
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	TextureWidth.clear();
	TextureHeight.clear();
#endif
}

#if !defined(_WIN32) || !defined(WITH_DIRECTX)
int getTextureWidth(DevicePtr device, TexturePtr texture) {
	if (TextureWidth.find(texture) != TextureWidth.end())
		return TextureWidth[texture];
	else
		return 0;
}
int getTextureHeight(DevicePtr device, TexturePtr texture) {
	if (TextureHeight.find(texture) != TextureHeight.end())
		return TextureHeight[texture];
	else
		return 0;
}
#endif

}
