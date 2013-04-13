#include "sprite.h"
#include "geometry.h"

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
	/* TODO: OpenGL�ōĎ��� */
#endif
}

}
