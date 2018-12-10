#include "sprite.h"
#include "geometry.h"
#include "loadtex.h"

/* スプライト表示処理 */

namespace mihajong_graphic {

std::map<intptr_t, SpriteRenderer*> SpriteRenderer::renderer;

/* コンストラクタ */
SpriteRenderer::SpriteRenderer(DevicePtr device) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	// Direct3Dのみ。OpenGLでは不要？
	if (FAILED(D3DXCreateSprite(device, &sprite)))
		throw _T("スプライトの生成に失敗しました");
#endif
}

/* デストラクタ */
SpriteRenderer::~SpriteRenderer() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	// Direct3Dのみ。OpenGLでは不要？
	if (sprite) sprite->Release();
#endif
}

/* インスタンス化 */
SpriteRenderer* SpriteRenderer::instantiate(DevicePtr device) {
	if (renderer.find((intptr_t)device) != renderer.end()) { // デバイスに対応するスプライトがすでにある
		return renderer[(intptr_t)device];
	} else { // デバイスに対応するスプライトは初回の使用(初期化が必要)
		renderer[(intptr_t)device] = new SpriteRenderer(device);
		return renderer[(intptr_t)device];
	}
}

void SpriteRenderer::delInstance(DevicePtr device) {
	if (renderer.find((intptr_t)device) != renderer.end()) {
		delete renderer[(intptr_t)device];
		renderer.erase((intptr_t)device);
	}
}

/* スプライト描画開始 */
void SpriteRenderer::Start() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	// Direct3Dのみ。OpenGLでは不要？
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
#endif
}

/* スプライト描画終了 */
void SpriteRenderer::End() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	// Direct3Dのみ。OpenGLでは不要？
	sprite->End();
#endif
}

/* スプライト描画 */
void SpriteRenderer::ShowSprite(
	TexturePtr texture, int X, int Y, int Width, int Height,
	ArgbColor color, RECT* rect, int CenterX, int CenterY, TransformMatrix* matrix)
{
	if ((!sprite) || (!texture)) return; // ぬるぽは(・∀・)ｶｴﾚ!!
	RECT defaultRect = {0, 0, Width, Height};
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix defaultMatrix; D3DXMatrixIdentity(&defaultMatrix);
	D3DXMatrixScaling(&defaultMatrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXVECTOR3 Center(static_cast<float>(CenterX), static_cast<float>(CenterY), 0.0f);
	D3DXVECTOR3 Pos(static_cast<float>(X), static_cast<float>(Y), 0.0f);
	sprite->SetTransform(matrix ? matrix : &defaultMatrix);
	sprite->Draw(texture, rect ? rect : &defaultRect, &Center, &Pos, color);
	sprite->Flush();
#else
	const TransformMatrix defaultMatrix = {
		Geometry::WindowScale(), 0,                                                         0, 0,
		0,                       Geometry::WindowScale(),                                   0, 0,
		0,                       0,                                                         1, 0,
		0,                       Geometry::WindowHeight * (1.0f - Geometry::WindowScale()), 0, 1,
	};

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, Geometry::WindowWidth, 0, Geometry::WindowHeight, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(matrix ? &((*matrix)[0]) : &defaultMatrix[0]);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	RECT* txRect = rect ? rect : &defaultRect;

	glBegin(GL_QUADS);
	glColor4d(
		(double)((color & 0x00ff0000) >> 16) / 255.0,
		(double)((color & 0x0000ff00) >>  8) / 255.0,
		(double)((color & 0x000000ff)      ) / 255.0,
		(double)((color & 0xff000000) >> 24) / 255.0);
	const double lpos = (double)txRect->left   / (double)getTextureWidth (nullptr, texture);
	const double rpos = (double)txRect->right  / (double)getTextureWidth (nullptr, texture);
	const double tpos = (double)txRect->top    / (double)getTextureHeight(nullptr, texture);
	const double bpos = (double)txRect->bottom / (double)getTextureHeight(nullptr, texture);
	glTexCoord2d(lpos, bpos); glVertex2i(X         - CenterX, Geometry::WindowHeight - (Y + Height - CenterY));
	glTexCoord2d(rpos, bpos); glVertex2i(X + Width - CenterX, Geometry::WindowHeight - (Y + Height - CenterY));
	glTexCoord2d(rpos, tpos); glVertex2i(X + Width - CenterX, Geometry::WindowHeight - (Y          - CenterY));
	glTexCoord2d(lpos, tpos); glVertex2i(X         - CenterX, Geometry::WindowHeight - (Y          - CenterY));
	glEnd();
	//glFlush(); // ←[20130415]ボトルネックになることが判明。これを外したら26FPSが40FPSまで改善(デバッグビルド)。リリースだと50FPS→60FPS達成。(GeForce GT240で)

	glDisable(GL_TEXTURE_2D);
#endif
}

}
