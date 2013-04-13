#include "sprite.h"
#include "geometry.h"

/* スプライト表示処理 */

namespace mihajong_graphic {

std::map<int, SpriteRenderer*> SpriteRenderer::renderer;

/* コンストラクタ */
SpriteRenderer::SpriteRenderer(DevicePtr device) {
	if (FAILED(D3DXCreateSprite(device, &sprite)))
		throw _T("スプライトの生成に失敗しました");
}

/* デストラクタ */
SpriteRenderer::~SpriteRenderer() {
	if (sprite) sprite->Release();
}

/* インスタンス化 */
SpriteRenderer* SpriteRenderer::instantiate(DevicePtr device) {
	if (renderer.find((int)device) != renderer.end()) { // デバイスに対応するスプライトがすでにある
		return renderer[(int)device];
	} else { // デバイスに対応するスプライトは初回の使用(初期化が必要)
		renderer[(int)device] = new SpriteRenderer(device);
		return renderer[(int)device];
	}
}

void SpriteRenderer::delInstance(DevicePtr device) {
	if (renderer.find((int)device) != renderer.end()) {
		delete renderer[(int)device];
		renderer.erase((int)device);
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
	TexturePtr texture, int X, int Y, int Width, int Height,
	ArgbColor color, RECT* rect, int CenterX, int CenterY, TransformMatrix* matrix)
{
	if ((!sprite) || (!texture)) return; // ぬるぽは(・∀・)ｶｴﾚ!!
	RECT defaultRect = {0, 0, Width, Height};
	TransformMatrix defaultMatrix; D3DXMatrixIdentity(&defaultMatrix);
	D3DXMatrixScaling(&defaultMatrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXVECTOR3 Center(CenterX, CenterY, 0);
	D3DXVECTOR3 Pos((float)X, (float)Y, 0);
	sprite->SetTransform(matrix ? matrix : &defaultMatrix);
	sprite->Draw(texture, rect ? rect : &defaultRect, &Center, &Pos, color);
	sprite->Flush();
}

}
