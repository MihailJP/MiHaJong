#pragma once

#include <windows.h>
#include "directx.h"
#include "init.h"

namespace mihajong_graphic {
	void LoadTexture(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9* texture, LPCTSTR resource); // �e�N�X�`���ǂݍ���
	void UnloadAllTextures(); // �e�N�X�`���̉��(�A�v���P�[�V�����I�����ɂ����ĂԂ��ƁI)
}
