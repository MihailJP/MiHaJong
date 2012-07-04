#ifndef TILEUTIL_H
#define TILEUTIL_H

#ifdef MJCORE_EXPORTS
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <type_traits>
#endif
#include <cstdint>
#include "mjexport.h"
#include "gametbl.h"
#ifdef MJCORE_EXPORTS
#include "logging.h"
#include "mnzdat.h"
#include "func.h"
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT MachihaiTileInfo {
	bool MachihaiFlag; // �҂��v�ɂȂ��Ă��邩�̃t���O
	int8_t MachihaiCount; // �҂��v�̎c�薇��
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<MachihaiTileInfo>::value, "MachihaiTileInfo is not POD");
#endif

EXPORT_STRUCT MachihaiInfo { // �҂��v�Ƃ��̏����i�[(chkFuriten�֐��p)
	bool FuritenFlag; // �t���e�����ǂ����̃t���O
	MachihaiTileInfo Machihai[TILE_NONFLOWER_MAX]; // �҂��v���
	int8_t MachihaiTotal; // �҂��v�̍��v����
	int8_t MachiMen; // ���ʑ҂��H
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<MachihaiInfo>::value, "MachihaiInfo is not POD");
#endif

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
void lipai(GameTable* const gameStat, PLAYER_ID targetPlayer);
__declspec(dllexport) void lipai(GameTable* const gameStat, int targetPlayer);

void chkOpenMachi(GameTable* const gameStat, PLAYER_ID targetPlayer);
__declspec(dllexport) void chkOpenMachi(GameTable* const gameStat, int targetPlayer);
#endif

#endif
