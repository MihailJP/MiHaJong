#pragma once

#include <windows.h>
#include "directx.h"
#include "init.h"

namespace mihajong_graphic {
	void LoadTexture(DevicePtr device, TexturePtr* texture, LPCTSTR resource); // テクスチャ読み込み
	void UnloadAllTextures(); // テクスチャの解放(アプリケーション終了時にだけ呼ぶこと！)
}
