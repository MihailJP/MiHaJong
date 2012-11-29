#ifndef GRAPHIC_LOADTEX_H
#define GRAPHIC_LOADTEX_H

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include "init.h"

namespace mihajong_graphic {
	void LoadTexture(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height); // テクスチャ読み込み
}

#endif
