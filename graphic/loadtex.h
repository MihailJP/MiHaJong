#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "directx.h"
#include "init.h"

namespace mihajong_graphic {
	void LoadTexture(DevicePtr device, TexturePtr* texture, LPCTSTR resource); // �e�N�X�`���ǂݍ���
	void UnloadAllTextures(); // �e�N�X�`���̉��(�A�v���P�[�V�����I�����ɂ����ĂԂ��ƁI)
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	int getTextureWidth(DevicePtr device, TexturePtr texture);
	int getTextureHeight(DevicePtr device, TexturePtr texture);
#endif
}
