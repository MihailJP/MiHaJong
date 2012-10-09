#ifndef PREPARE_H
#define PREPARE_H

#include <algorithm>
#include "../socket/socket.h"
#include "gametbl.h"
#include "envtbl.h"
#include "random.h"
#include "haifu.h"
#include "tileutil.h"

__declspec(dllexport) void shuffle(GameTable* const gameStat); // �v���o�b�t�@�ɕ��ׂāA���v
__declspec(dllexport) void initdora(GameTable* const gameStat); // �h���̐ݒ�
#endif
