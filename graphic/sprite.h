#ifndef GRAPHIC_SPRITE_H
#define GRAPHIC_SPRITE_H

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>

namespace mihajong_graphic {

class SpriteRenderer {
public:
	static void ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height,
		D3DCOLOR color = 0xffffffff, RECT* rect = nullptr, int CenterX = 0, int CenterY = 0, LPD3DXMATRIX matrix = nullptr);
};

}
#endif
