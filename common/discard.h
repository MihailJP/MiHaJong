#pragma once

#include <cstdint>
#include <type_traits>

namespace mihajong_structs {

struct DiscardTileNum { /* 打牌コード用 */
	enum discardType : uint8_t {
		Normal, Ankan, Kakan, Riichi, Flower, OpenRiichi,
		Agari, Kyuushu, Disconnect, MeldSel
	};
	static const int TypeStep = 20;
	discardType type;
	uint8_t id;
	int toSingleInt() const;
	static DiscardTileNum fromSingleInt(int);
};
static_assert(std::is_trivially_copyable<DiscardTileNum>::value, "DiscardTileNum is not trivially copyable");
static_assert(std::is_standard_layout<DiscardTileNum>::value, "DiscardTileNum is not standard layout");

} /* namespace */
