#pragma once

#include <cstdint>
#include <type_traits>
#include "tilecode.h"

namespace mihajong_structs {

struct MachihaiTileInfo {
	bool MachihaiFlag; // �҂��v�ɂȂ��Ă��邩�̃t���O
	int8_t MachihaiCount; // �҂��v�̎c�薇��
};
static_assert(std::is_pod<MachihaiTileInfo>::value, "MachihaiTileInfo is not POD");

template struct InfoByTile<MachihaiTileInfo>;
struct MachihaiInfo { // �҂��v�Ƃ��̏����i�[(chkFuriten�֐��p)
	bool FuritenFlag; // �t���e�����ǂ����̃t���O
	InfoByTile<MachihaiTileInfo> Machihai; // �҂��v���
	int8_t MachihaiTotal; // �҂��v�̍��v����
	int8_t MachiMen; // ���ʑ҂��H
};
static_assert(std::is_pod<MachihaiInfo>::value, "MachihaiInfo is not POD");

struct TileStatus { // gettilestatus�p
	bool isExistent, canFormQuad,
		seqMidWait, seqDoubleSideWait, seqSingleSideWait,
		formsPair, formsSequence, formsTriplet;
};
static_assert(std::is_pod<TileStatus>::value, "TileStatus is not POD");

} /* namespace */
