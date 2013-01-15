#include "sprite.h"
#include "geometry.h"
#include <map>

/* スプライト表示処理 */

namespace mihajong_graphic {

/* コンストラクタ */
SpriteRenderer::SpriteRenderer(LPDIRECT3DDEVICE9 device) {
	if (FAILED(D3DXCreateSprite(device, &sprite)))
		throw _T("スプライトの生成に失敗しました");
}

/* デストラクタ */
SpriteRenderer::~SpriteRenderer() {
	if (sprite) sprite->Release();
}

/* インスタンス化 */
SpriteRenderer* SpriteRenderer::instantiate(LPDIRECT3DDEVICE9 device) {
	static std::map<int, SpriteRenderer*> renderer;
	if (renderer.find((int)device) != renderer.end()) { // デバイスに対応するスプライトがすでにある
		return renderer[(int)device];
	} else { // デバイスに対応するスプライトは初回の使用(初期化が必要)
		renderer[(int)device] = new SpriteRenderer(device);
		return renderer[(int)device];
	}
}

/* スプライト描画開始 */
void SpriteRenderer::Start() {
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

/* スプライト描画終了 */
void SpriteRenderer::End() {
	sprite->End();
}

/* スプライト描画 */
void SpriteRenderer::ShowSprite(
	LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height,
	D3DCOLOR color, RECT* rect, int CenterX, int CenterY, LPD3DXMATRIX matrix)
{ /* FIXME: Zオーダーが乱れることがあるんで要修正。特に対面の鳴き面子 */
	if ((!sprite) || (!texture)) return; // ぬるぽは(・∀・)ｶｴﾚ!!
	RECT defaultRect = {0, 0, Width, Height};
	D3DXMATRIX defaultMatrix; D3DXMatrixIdentity(&defaultMatrix);
	D3DXMatrixScaling(&defaultMatrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXVECTOR3 Center(CenterX, CenterY, 0);
	D3DXVECTOR3 Pos((float)X, (float)Y, 0);
	sprite->SetTransform(matrix ? matrix : &defaultMatrix);
	sprite->Draw(texture, rect ? rect : &defaultRect, &Center, &Pos, color);
	sprite->Flush();
}

}
