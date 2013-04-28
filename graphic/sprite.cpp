#include "sprite.h"
#include "geometry.h"
#include "loadtex.h"

/* �X�v���C�g�\������ */

namespace mihajong_graphic {

std::map<int, SpriteRenderer*> SpriteRenderer::renderer;

/* �R���X�g���N�^ */
SpriteRenderer::SpriteRenderer(DevicePtr device) {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (FAILED(D3DXCreateSprite(device, &sprite)))
		throw _T("�X�v���C�g�̐����Ɏ��s���܂���");
#else
	/* TODO: OpenGL�ōĎ��� */
#endif
}

/* �f�X�g���N�^ */
SpriteRenderer::~SpriteRenderer() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (sprite) sprite->Release();
#else
	/* TODO: OpenGL�ōĎ��� */
#endif
}

/* �C���X�^���X�� */
SpriteRenderer* SpriteRenderer::instantiate(DevicePtr device) {
	if (renderer.find((int)device) != renderer.end()) { // �f�o�C�X�ɑΉ�����X�v���C�g�����łɂ���
		return renderer[(int)device];
	} else { // �f�o�C�X�ɑΉ�����X�v���C�g�͏���̎g�p(���������K�v)
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

/* �X�v���C�g�`��J�n */
void SpriteRenderer::Start() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
#else
	/* TODO: OpenGL�ōĎ��� */
#endif
}

/* �X�v���C�g�`��I�� */
void SpriteRenderer::End() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	sprite->End();
#else
	/* TODO: OpenGL�ōĎ��� */
#endif
}

/* �X�v���C�g�`�� */
void SpriteRenderer::ShowSprite(
	TexturePtr texture, int X, int Y, int Width, int Height,
	ArgbColor color, RECT* rect, int CenterX, int CenterY, TransformMatrix* matrix)
{
	if ((!sprite) || (!texture)) return; // �ʂ�ۂ�(�E�́E)���!!
	RECT defaultRect = {0, 0, Width, Height};
#if defined(_WIN32) && defined(WITH_DIRECTX)
	TransformMatrix defaultMatrix; D3DXMatrixIdentity(&defaultMatrix);
	D3DXMatrixScaling(&defaultMatrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXVECTOR3 Center(CenterX, CenterY, 0);
	D3DXVECTOR3 Pos((float)X, (float)Y, 0);
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
	//glFlush(); // ��[20130415]�{�g���l�b�N�ɂȂ邱�Ƃ������B������O������26FPS��40FPS�܂ŉ��P(�f�o�b�O�r���h)�B�����[�X����50FPS��60FPS�B���B(GeForce GT240��)

	glDisable(GL_TEXTURE_2D);
#endif
}

}
