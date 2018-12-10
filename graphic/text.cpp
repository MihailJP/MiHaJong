#include "text.h"
#include "resource.h"
#include "geometry.h"
#include "../common/strcode.h"
#include "sprite.h"

namespace mihajong_graphic {

ITextRenderer::ITextRenderer(DevicePtr device) {
	myDevice = device;
}
TextRenderer::TextRenderer(DevicePtr device) : ITextRenderer(device) {
	fontmap = FontMap::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT));
}
HugeTextRenderer::HugeTextRenderer(DevicePtr device) : ITextRenderer(device) {
	fontmap = FontMapLargeChr::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT_HUGE));
}
SmallTextRenderer::SmallTextRenderer(DevicePtr device) : ITextRenderer(device) {
	fontmap = FontMapSmallChr::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT_SMALL));
}
CallDigitRenderer::CallDigitRenderer(DevicePtr device) : ITextRenderer(device) {
	fontmap = FontMapCallDigits::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_CALL_DIGITS));
}
ScoreDigitRenderer::ScoreDigitRenderer(DevicePtr device) : ITextRenderer(device) {
	fontmap = FontMapScoreDigits::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_SCORE_DIGITS));
}

ITextRenderer::~ITextRenderer() {
	deleteSprite();
	for (auto& k : StringData) {
		if (k) {
			delete k; k = nullptr;
		}
	}
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (font) font->Release();
#endif
}
TextRenderer::~TextRenderer() {
}
HugeTextRenderer::~HugeTextRenderer() {
}
SmallTextRenderer::~SmallTextRenderer() {
}
CallDigitRenderer::~CallDigitRenderer() {
}
ScoreDigitRenderer::~ScoreDigitRenderer() {
}

/* 新規の文字列オブジェクトを作成する */
void ITextRenderer::NewText(unsigned int ID, const std::wstring& str, int x, int y, float scale, float width, ArgbColor color) {
	if (StringData.size() <= ID) StringData.resize(ID + 1, nullptr); // 配列の拡張
	bool TextChanged = (!StringData[ID]) || (StringData[ID]->str != str);
	if (StringData[ID] && TextChanged) delete StringData[ID]; // 既に存在した場合
	if (TextChanged) StringData[ID] = new StringAttr;
	StringData[ID]->X = x; StringData[ID]->Y = y;
	StringData[ID]->scale = scale; StringData[ID]->width = width;
	StringData[ID]->color = color;
	if (TextChanged) {
		StringData[ID]->str = std::wstring(str);
		reconstruct(ID, true);
	} else {
		reconstruct(ID, false);
	}
}
void ITextRenderer::NewText(unsigned int ID, const std::string& str, int x, int y, float scale, float width, ArgbColor color) {
	NewText(ID, CodeConv::ANSItoWIDE(str), x, y, scale, width, color);
}

/* 文字列オブジェクトの後始末 */
void ITextRenderer::DelText(unsigned int ID) {
	if (StringData.size() <= ID) return;
	delete StringData[ID]; StringData[ID] = nullptr;
	deleteSprite(ID);
}

/* スプライト再構築 */
void ITextRenderer::spriteRecalc(unsigned int ID, SpriteAttr* sprite, float chrAdvance, float cursorPos) {
	sprite->X = static_cast<int>(static_cast<float>(StringData[ID]->X) + chrAdvance * cursorPos - static_cast<float>(FontPadding()));
	sprite->Y = StringData[ID]->Y - FontPadding();
	sprite->widthScale = StringData[ID]->scale * StringData[ID]->width;
	sprite->heightScale = StringData[ID]->scale;
	sprite->color = StringData[ID]->color;
	/* 行列を計算する */
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix m; D3DXMatrixIdentity(&m);
	D3DXMatrixIdentity(&sprite->matrix);
	D3DXMatrixTranslation(&m, (float)-(sprite->X), (float)-(sprite->Y), 0);
	D3DXMatrixMultiply(&sprite->matrix, &sprite->matrix, &m);
	D3DXMatrixScaling(&m, sprite->widthScale, sprite->heightScale, 0.0f);
	D3DXMatrixMultiply(&sprite->matrix, &sprite->matrix, &m);
	D3DXMatrixTranslation(&m, (float)sprite->X, (float)sprite->Y, 0);
	D3DXMatrixMultiply(&sprite->matrix, &sprite->matrix, &m);
	D3DXMatrixScaling(&m, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(&sprite->matrix, &sprite->matrix, &m);
#else
	/* DirectXとOpenGLだと座標原点が違う */
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); glLoadIdentity();
	glTranslatef(0.0f, (float)Geometry::WindowHeight, 0.0f);
	glTranslatef((float)(sprite->X) * Geometry::WindowScale(), -(float)(sprite->Y) * Geometry::WindowScale(), 0.0f);
	glScalef(sprite->widthScale, sprite->heightScale, 1.0f);
	glTranslatef(-(float)(sprite->X) * Geometry::WindowScale(), (float)(sprite->Y) * Geometry::WindowScale(), 0.0f);
	glScalef(Geometry::WindowScale(), Geometry::WindowScale(), 1.0f);
	glTranslatef(0.0f, -(float)Geometry::WindowHeight, 0.0f);
	TransformMatrix matrix; glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
	glPopMatrix();
	sprite->matrix = matrix;
#endif
	/* ここまで */
}
void ITextRenderer::reconstruct(unsigned int ID, bool rescanStr) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(SpriteMutex);
	if (SpriteData.size() <= ID) SpriteData.resize(ID + 1, std::vector<SpriteAttr*>()); // 配列の拡張
	if ((!SpriteData[ID].empty()) && rescanStr) deleteSprite(ID); // 既に存在した場合
	if (!StringData[ID]) /* ぬるぽ */
		return; /* ガッ */
	float chrAdvance = (FontWidth() - FontPadding() * 2) * StringData[ID]->scale * StringData[ID]->width;
	float cursorPos = 0;
	if (rescanStr) {
		for (const auto& k : StringData[ID]->str) {
			SpriteData[ID].push_back(new SpriteAttr);
			SpriteData[ID].back()->isFullWidth = fontmap->map(k).first;
			SpriteData[ID].back()->chr_id = fontmap->map(k).second;
			spriteRecalc(ID, SpriteData[ID].back(), chrAdvance, cursorPos);
			if (SpriteData[ID].back()->isFullWidth) cursorPos += 1.0f;
			else cursorPos += .5f;
		}
	} else {
		for (const auto& k : SpriteData[ID]) {
			spriteRecalc(ID, k, chrAdvance, cursorPos);
			if (k->isFullWidth) cursorPos += 1.0f;
			else cursorPos += .5f;
		}
	}
}
void ITextRenderer::reconstruct() {
	// VERY SLOW. DO NOT USE.
	deleteSprite();
	for (unsigned i = 0; i < StringData.size(); i++)
		reconstruct(i);
}

/* スプライトを削除する */
void ITextRenderer::deleteSprite(unsigned int ID) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(SpriteMutex);
	for (const auto& k : SpriteData[ID])
		delete k;
	SpriteData[ID].clear();
}
void ITextRenderer::deleteSprite() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(SpriteMutex);
	for (unsigned int i = 0; i < SpriteData.size(); i++)
		deleteSprite(i);
	SpriteData.clear();
}

/* レンダリング */
void ITextRenderer::Render() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(SpriteMutex);
	for (const auto& i : SpriteData) {
		for (const auto& k : i) {
			if (!k) continue;
			RECT rect = {
				static_cast<int32_t>((k->chr_id % FontCols()) * FontWidth()),
				static_cast<int32_t>((k->chr_id / FontCols()) * FontBaseSize()),
				static_cast<int32_t>((k->chr_id % FontCols() + 1) * FontWidth()),
				static_cast<int32_t>((k->chr_id / FontCols() + 1) * FontBaseSize()),
			};
			SpriteRenderer::instantiate(myDevice)->ShowSprite(
				font, k->X, k->Y, FontWidth(), FontBaseSize(),
				k->color, &rect, 0, 0, &(k->matrix));
		}
	}
}

/* 文字列の幅を計算 */
unsigned ITextRenderer::strWidthByCols(const std::wstring& str) {
	unsigned cols = 0;
	for (const auto& k : str)
		cols += (fontmap->map(k).first) ? /* 全角 */ 2 : /* 半角 */ 1;
	return cols;
}
unsigned ITextRenderer::strWidthByCols(const std::string& str) {
	const std::wstring wstr(CodeConv::ANSItoWIDE(str));
	return strWidthByCols(wstr);
}
unsigned ITextRenderer::strWidthByPix(const std::wstring& str) {
	return strWidthByCols(str) * (FontWidth() - FontPadding() * 2) / 2;
}
unsigned ITextRenderer::strWidthByPix(const std::string& str) {
	return strWidthByCols(str) * (FontWidth() - FontPadding() * 2) / 2;
}

}
