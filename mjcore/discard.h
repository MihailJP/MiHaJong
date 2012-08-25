#ifndef DISCARD_H
#define DISCARD_H

#include <cstdint>
#include <type_traits>
#include "mjexport.h"
#include "gametbl.h"

EXPORT_STRUCT DiscardTileNum { /* 打牌コード用 */
	enum discardType : uint8_t {
		Normal, Ankan, Kakan, Riichi, Flower, OpenRiichi,
		Agari, Kyuushu, Disconnect
	};
	discardType type;
	uint8_t id;
	int toSingleInt() const;
	static DiscardTileNum fromSingleInt(int);
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<DiscardTileNum>::value, "DiscardTileNum is not POD");
#endif
#endif
