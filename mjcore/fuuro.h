#pragma once

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
void checkpao(GameTable* const gameStat);
EndType ronhuproc(GameTable* const gameStat);
bool executeFuuro(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);
