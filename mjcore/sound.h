#pragma once

#include "../sound/sound.h"

namespace sound {
namespace util {

void bgmload(unsigned ID, const char* filename, bool looped); /* BGM�ǂݍ��� */
void soundload(unsigned ID, const char* filename, bool looped); /* �T�E���h�ǂݍ��� */
void bgmstop(); /* BGM��~ */
void bgmplay(unsigned ID); /* �a�f�l�Đ� */
void setvolume(); /* ���ʐݒ�𔽉f */

}
}
