#include "sprite.h"
#include "geometry.h"
#include "loadtex.h"
#include "matrix.h"
#include "except.h"

/* スプライト表示処理 */

namespace mihajong_graphic {

std::map<intptr_t, SpriteRenderer*> SpriteRenderer::renderer;

/* コンストラクタ */
SpriteRenderer::SpriteRenderer(DevicePtr device) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	// Direct3Dのみ。OpenGLでは不要？
	if (FAILED(D3DXCreateSprite(device, &sprite)))
		throw SpriteCreationError("スプライトの生成に失敗しました");
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
	if (renderer.find(reinterpret_cast<intptr_t>(device)) != renderer.end()) { // デバイスに対応するスプライトがすでにある
		return renderer[reinterpret_cast<intptr_t>(device)];
	} else { // デバイスに対応するスプライトは初回の使用(初期化が必要)
		renderer[reinterpret_cast<intptr_t>(device)] = new SpriteRenderer(device);
		return renderer[reinterpret_cast<intptr_t>(device)];
	}
}

void SpriteRenderer::delInstance(DevicePtr device) {
	if (renderer.find(reinterpret_cast<intptr_t>(device)) != renderer.end()) {
		delete renderer[reinterpret_cast<intptr_t>(device)];
		renderer.erase(reinterpret_cast<intptr_t>(device));
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
	ArgbColor color, const RECT* rect, int CenterX, int CenterY, const TransformMatrix* matrix)
{
	if ((!sprite) || (!texture)) return; // ぬるぽは(・∀・)ｶｴﾚ!!
	const TransformMatrix defaultMatrix(getMatrix());
	const RECT txRect = {
		static_cast<int>(static_cast<float>(rect ? rect->left : 0) * Geometry::DataScale),
		static_cast<int>(static_cast<float>(rect ? rect->top : 0) * Geometry::DataScale),
		static_cast<int>(static_cast<float>(rect ? rect->right : Width) * Geometry::DataScale),
		static_cast<int>(static_cast<float>(rect ? rect->bottom : Height) * Geometry::DataScale)
	};
#if defined(_WIN32) && defined(WITH_DIRECTX)
	const float dataScale = matrix ? 1.0f : Geometry::DataScale;
	D3DXVECTOR3 Center(static_cast<float>(CenterX) * Geometry::DataScale, static_cast<float>(CenterY) * Geometry::DataScale, 0.0f);
	D3DXVECTOR3 Pos(static_cast<float>(X) * dataScale, static_cast<float>(Y) * dataScale, 0.0f);
	sprite->SetTransform(matrix ? matrix : &defaultMatrix);
	sprite->Draw(texture, &txRect, &Center, &Pos, color);
	sprite->Flush();
#else
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

	glBegin(GL_QUADS);
	glColor4d(
		static_cast<double>((color & 0x00ff0000) >> 16) / 255.0,
		static_cast<double>((color & 0x0000ff00) >>  8) / 255.0,
		static_cast<double>((color & 0x000000ff)      ) / 255.0,
		static_cast<double>((color & 0xff000000) >> 24) / 255.0);
	const double lpos = static_cast<double>(txRect.left  ) / static_cast<double>(getTextureWidth (nullptr, texture));
	const double rpos = static_cast<double>(txRect.right ) / static_cast<double>(getTextureWidth (nullptr, texture));
	const double tpos = static_cast<double>(txRect.top   ) / static_cast<double>(getTextureHeight(nullptr, texture));
	const double bpos = static_cast<double>(txRect.bottom) / static_cast<double>(getTextureHeight(nullptr, texture));
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
