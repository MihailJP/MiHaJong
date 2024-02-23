#include "loadtex.h"
#include "resource.h"
#if defined(_WIN32) && !defined(WITH_DIRECTX)
#include <gdiplus.h>
#elif !defined(_WIN32)
#include "filenum.h"
#include <png.h>
#include <cstdio>
#include <csetjmp>
#endif
#include <map>
#include <cassert>
#include "../common/strcode.h"
#include "except.h"

namespace mihajong_graphic {

//namespace {
	std::map<intptr_t, std::map<intptr_t, TexturePtr> > Textures;
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	std::map<TexturePtr, unsigned> TextureWidth;
	std::map<TexturePtr, unsigned> TextureHeight;
#endif
//}

void LoadTexture(DevicePtr device, TexturePtr* texture, LPCTSTR resource) {
#if defined(_WIN32) && !defined(WITH_DIRECTX)
	using namespace Gdiplus;
#endif
	auto resourceID = reinterpret_cast<intptr_t>(resource);
	assert((resourceID & 0xffff0000) == 0); // 上位ワードが0なら文字列ではなくリソース番号とみなされる(Win32APIの仕様)
	if (Textures[reinterpret_cast<intptr_t>(device)].find(resourceID) != Textures[reinterpret_cast<intptr_t>(device)].end()) { // 既にロード済みのテクスチャ
#if defined(_WIN32) && defined(WITH_DIRECTX)
		Textures[resourceID]->AddRef();
#endif
		*texture = Textures[reinterpret_cast<intptr_t>(device)][resourceID];
		return;
	} else { // ロードされていない場合
#ifdef _WIN32
		HRSRC Resource = FindResource(GraphicDLL, resource, MAKEINTRESOURCE(PNG_FILE));
		if (Resource == nullptr)
			throw TextureLoadError(GetLastError(), resourceID);
		HGLOBAL ResourceMem = LoadResource(GraphicDLL, Resource);
		if (ResourceMem == 0)
			throw TextureLoadError(GetLastError(), resourceID);
		DWORD pngSize = SizeofResource(GraphicDLL, Resource);
		void* pngData = LockResource(ResourceMem);
#if defined(_WIN32) && defined(WITH_DIRECTX)
		Textures[reinterpret_cast<intptr_t>(device)][resourceID] = nullptr;
		HRESULT result = 
			D3DXCreateTextureFromFileInMemoryEx(device, pngData, pngSize, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER, D3DX_DEFAULT,
			0x00000000, nullptr, nullptr, &(Textures[reinterpret_cast<intptr_t>(device)][resourceID]));
		UnlockResource(ResourceMem);
		switch (result) {
		case D3D_OK:
			Textures[reinterpret_cast<intptr_t>(device)][resourceID]->AddRef();
			*texture = Textures[reinterpret_cast<intptr_t>(device)][resourceID];
			return; // Congratulations, your texture has been loaded.
		case D3DERR_NOTAVAILABLE:
			throw TextureCreationError("", resourceID);
		case D3DERR_OUTOFVIDEOMEMORY:
			throw TextureCreationError("VRAMが足りません。", resourceID);
		case D3DERR_INVALIDCALL:
			throw TextureCreationError("パラメータが正しくありません。", resourceID);
		case D3DXERR_INVALIDDATA:
			throw TextureCreationError("データが異常です。", resourceID);
		case E_OUTOFMEMORY:
			throw TextureCreationError("メモリが足りません。", resourceID);
		default: // This may not happen...
			throw TextureCreationError("原因不明のエラーです。", resourceID);
		}
#else
		Textures[reinterpret_cast<intptr_t>(device)][resourceID] = 0;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &Textures[reinterpret_cast<intptr_t>(device)][resourceID]);
		TextureWidth[Textures[reinterpret_cast<intptr_t>(device)][resourceID]] =
		TextureHeight[Textures[reinterpret_cast<intptr_t>(device)][resourceID]] = 0;
		glBindTexture(GL_TEXTURE_2D, Textures[reinterpret_cast<intptr_t>(device)][resourceID]);
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
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.Width, data.Height, 0,
					GL_BGRA_EXT, GL_UNSIGNED_BYTE, data.Scan0);
				bitmap->UnlockBits(&data);
				TextureWidth[Textures[reinterpret_cast<intptr_t>(device)][resourceID]] = data.Width;
				TextureHeight[Textures[reinterpret_cast<intptr_t>(device)][resourceID]] = data.Height;
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		GlobalUnlock(resBuf);
		GlobalFree(resBuf);
		UnlockResource(ResourceMem);
		if (stream) stream->Release();
		delete bitmap;
		*texture = Textures[reinterpret_cast<intptr_t>(device)][resourceID];
		return;
#endif
#else /* _WIN32 */
		/* テクスチャの仮初期化 */
		Textures[reinterpret_cast<intptr_t>(device)][resourceID] = 0;
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &Textures[reinterpret_cast<intptr_t>(device)][resourceID]);
		TextureWidth[Textures[reinterpret_cast<intptr_t>(device)][resourceID]] =
		TextureHeight[Textures[reinterpret_cast<intptr_t>(device)][resourceID]] = 0;
		glBindTexture(GL_TEXTURE_2D, Textures[reinterpret_cast<intptr_t>(device)][resourceID]);
		/* ファイルをオープン */
		const std::string fileName = dataFileName(resourceID);
		FILE* pngFile = fopen(fileName.c_str(), "rb");
		if (!pngFile)
			throw TextureCreationError(std::string("fopen()失敗。テクスチャ画像 ") + fileName + std::string(" が読み込めませんでした。"));
		/* シグネチャの確認 */
		unsigned char header[8];
		fread(header, 1, 8, pngFile);
		if (png_sig_cmp(header, 0, 8)) {
			fclose(pngFile);
			throw TextureCreationError(fileName + std::string(" はPNGファイルではありません！"));
		}
		/* 読み込み準備 */
		png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!pngPtr) {
			fclose(pngFile);
			throw TextureCreationError("png_create_read_struct失敗！！");
		}
		png_infop infoPtr = png_create_info_struct(pngPtr);
		if (!infoPtr) {
			png_destroy_read_struct(&pngPtr, nullptr, nullptr);
			fclose(pngFile);
			throw TextureCreationError("png_create_info_struct失敗！！");
		}
		if (setjmp(png_jmpbuf(pngPtr))) { /* C++なのにsetjmpとか勘弁してくれ。 */
			png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
			fclose(pngFile);
			throw TextureCreationError(fileName + std::string(" の読み込みに失敗しました"));
		}
		/* ファイルポインタを渡す */
		png_init_io(pngPtr, pngFile);
		png_set_sig_bytes(pngPtr, 8);
		/* 読み込み */
		png_read_png(pngPtr, infoPtr,
			PNG_TRANSFORM_PACKING | PNG_TRANSFORM_STRIP_16,
			nullptr);
		png_byte** rows = png_get_rows(pngPtr, infoPtr);
		const unsigned pngWidth = png_get_image_width(pngPtr, infoPtr);
		const unsigned pngHeight = png_get_image_height(pngPtr, infoPtr);
		unsigned pngChannels = png_get_channels(pngPtr, infoPtr);
		char* imageDat = new char[pngWidth * pngHeight * 4];
		for (int y = 0; y < pngHeight; ++y) {
			switch (pngChannels) {
			case 4: /* RGBAカラー */
				memcpy(
					imageDat + y * pngWidth * pngChannels,
					rows[y /* Linux/libpngだと逆にする必要がない？ */],
					pngWidth * pngChannels);
				break;
			case 2: /* アルファ付きグレースケール */
				for (int x = 0; x < pngWidth; ++x) {
					/* RGBAの順に格納 */
					imageDat[    (y * pngWidth + x) * 4] =
						imageDat[(y * pngWidth + x) * 4 + 1] =
						imageDat[(y * pngWidth + x) * 4 + 2] = rows[y][x * 2];
					imageDat[    (y * pngWidth + x) * 4 + 3] = rows[y][x * 2 + 1];
				}
				break;
			default:
				// アルファチャンネル無きものスプライト画像の資格なし！
				throw TextureCreationError(fileName + std::string(" にアルファチャンネルがありません"));
			}
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pngWidth, pngHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, imageDat);
		TextureWidth[Textures[reinterpret_cast<intptr_t>(device)][resourceID]] = pngWidth;
		TextureHeight[Textures[reinterpret_cast<intptr_t>(device)][resourceID]] = pngHeight;
		/* 解放 */
		delete[] imageDat;
		png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
		fclose(pngFile);
		return;
#endif /* _WIN32 */
	}
}

void PreloadTextures(DevicePtr device) { // テクスチャの先行読み込み
	constexpr intptr_t textureList[] = {
		IDB_PNG_TBLBAIZE,
		IDB_PNG_TBLBORDER,
		IDB_PNG_SDBAR,
		IDB_PNG_TILE,
		IDB_PNG_FONT,
		IDB_PNG_TITLE,
		IDB_PNG_BUTTON,
		IDB_PNG_FONT_HUGE,
		IDB_PNG_DICE,
		IDB_PNG_FONT_SMALL,
		IDB_PNG_TENBOU,
		IDB_PNG_CHICHAMARK,
		IDB_PNG_SCORE_INDICATOR,
		IDB_PNG_CALL_TEXT,
		IDB_PNG_CALL_DIGITS,
		IDB_PNG_AGARI_WINDOW,
		IDB_PNG_SCORE_DIGITS,
		IDB_PNG_CHECKBOX,
		IDB_PNG_TILE_BLACK,
		IDB_PNG_MOON_CLOCK,
		IDB_PNG_SPLASH_SCREEN,
		IDB_PNG_TITLE_BACKGROUND,
		0, // sentinel
	};
	TexturePtr dummyTexture;
	for (const intptr_t* i = textureList; *i != 0; ++i)
		LoadTexture(device, &dummyTexture, MAKEINTRESOURCE(*i));
}

void UnloadAllTextures() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	for (const auto& d : Textures) {
		for (const auto& k : d.second) {
			ULONG refs = k.second->Release();
			CodeConv::tostringstream o;
			o << _T("UnloadAllTextures(): Texture resource #") << k.first << _T(": remaining number of refs is ") << refs << std::endl;
			OutputDebugString(o.str().c_str());
		}
	}
#else
	for (const auto& d : Textures) {
		for (const auto& k : d.second) {
			const GLuint tx[1] = {k.second};
			glDeleteTextures(1, tx);
		}
	}
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
