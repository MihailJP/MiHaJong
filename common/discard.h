#pragma once

#include <cstdint>
#include <type_traits>

namespace mihajong_structs {

enum class DiscardType : uint8_t {
	normal, ankan, kakan, riichi, flower, openRiichi,
	agari, kyuushu, disconnect, meldSel
};
struct DiscardTileNum { /* 打牌コード用 */
	static const int TypeStep = 20;
	DiscardType type;
	uint8_t id;
	int toSingleInt() const;
	static DiscardTileNum fromSingleInt(int);
};
static_assert(std::is_trivially_copyable<DiscardTileNum>::value, "DiscardTileNum is not trivially copyable");
static_assert(std::is_standard_layout<DiscardTileNum>::value, "DiscardTileNum is not standard layout");

} /* namespace */
