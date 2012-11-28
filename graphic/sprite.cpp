#include "sprite.h"
#include "geometry.h"

void SpriteRenderer::ShowSprite(
	LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height,
	D3DCOLOR color, RECT* rect, int CenterX, int CenterY, LPD3DXMATRIX matrix)
{
	RECT defaultRect = {0, 0, Width, Height};
	D3DXMATRIX defaultMatrix; D3DXMatrixIdentity(&defaultMatrix);
	D3DXMatrixScaling(&defaultMatrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXVECTOR3 Center(CenterX, CenterY, 0);
	D3DXVECTOR3 Pos((float)X, (float)Y, 0);
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->SetTransform(matrix ? matrix : &defaultMatrix);
	sprite->Draw(texture, rect ? rect : &defaultRect, &Center, &Pos, color);
	sprite->End();
}
