#pragma once

#include <windows.h>
#include "directx.h"
#include "init.h"

namespace mihajong_graphic {
	void LoadTexture(DevicePtr device, TexturePtr* texture, LPCTSTR resource); // �e�N�X�`���ǂݍ���
	void UnloadAllTextures(); // �e�N�X�`���̉��(�A�v���P�[�V�����I�����ɂ����ĂԂ��ƁI)
}
