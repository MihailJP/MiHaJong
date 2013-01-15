#include "sprite.h"
#include "geometry.h"
#include <map>

/* �X�v���C�g�\������ */

namespace mihajong_graphic {

/* �R���X�g���N�^ */
SpriteRenderer::SpriteRenderer(LPDIRECT3DDEVICE9 device) {
	if (FAILED(D3DXCreateSprite(device, &sprite)))
		throw _T("�X�v���C�g�̐����Ɏ��s���܂���");
}

/* �f�X�g���N�^ */
SpriteRenderer::~SpriteRenderer() {
	if (sprite) sprite->Release();
}

/* �C���X�^���X�� */
SpriteRenderer* SpriteRenderer::instantiate(LPDIRECT3DDEVICE9 device) {
	static std::map<int, SpriteRenderer*> renderer;
	if (renderer.find((int)device) != renderer.end()) { // �f�o�C�X�ɑΉ�����X�v���C�g�����łɂ���
		return renderer[(int)device];
	} else { // �f�o�C�X�ɑΉ�����X�v���C�g�͏���̎g�p(���������K�v)
		renderer[(int)device] = new SpriteRenderer(device);
		return renderer[(int)device];
	}
}

/* �X�v���C�g�`��J�n */
void SpriteRenderer::Start() {
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

/* �X�v���C�g�`��I�� */
void SpriteRenderer::End() {
	sprite->End();
}

/* �X�v���C�g�`�� */
void SpriteRenderer::ShowSprite(
	LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height,
	D3DCOLOR color, RECT* rect, int CenterX, int CenterY, LPD3DXMATRIX matrix)
{ /* FIXME: Z�I�[�_�[������邱�Ƃ������ŗv�C���B���ɑΖʂ̖��ʎq */
	if ((!sprite) || (!texture)) return; // �ʂ�ۂ�(�E�́E)���!!
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
