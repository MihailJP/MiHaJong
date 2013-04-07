#pragma once

#include <windows.h>
#include "directx.h"
#include "init.h"

namespace mihajong_graphic {
	void LoadTexture(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9* texture, LPCTSTR resource); // テクスチャ読み込み
	void UnloadAllTextures(); // テクスチャの解放(アプリケーション終了時にだけ呼ぶこと！)
}
