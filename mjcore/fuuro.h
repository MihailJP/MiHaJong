#ifndef FUURO_H
#define FUURO_H

#ifdef MJCORE_EXPORTS
#include <sstream>
#include <cassert>
#include "strcode.h"
#include "logging.h"
#include "haifu.h"
#include "ruletbl.h"
#include "tileutil.h"
#include "discard.h"
#endif
#include "gametbl.h"
#include "endtype.h"
#include "discard.h"

enum FuuroType : unsigned { /* ���I�����̎�� */
	FuuroAnkan,     // �Þ�
	FuuroKakan,     // ����
	FuuroDaiminkan, // �喾��
	FuuroNorth,     // �����k(�O��)
	FuuroFlower,    // �Ԕv(�l��)
	FuuroPon,       // �|��
	FuuroChii,      // �`�[
};

bool fuuroproc(GameTable* const gameStat, EndType* RoundEndType, const DiscardTileNum& DiscardTileIndex, FuuroType Mode);

#endif
