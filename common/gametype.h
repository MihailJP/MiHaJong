#pragma once

#include <cstdint>

namespace mihajong_structs {

using std::uint8_t;

enum class GameTypeID : uint8_t { // 卓の種類(四麻、三麻)指定用
	yonma     = 0x01, // 四人打ち
	sanma     = 0x02, // 三人打ち
	sanma4    = 0x04, // 四人三麻
	sanmaS    = 0x08, // 数牌三麻
	sanmaSeto = 0x10, // 瀬戸内三麻
	allSanma  = 0x1e, // 三麻全ルール
	sanmaT    = 0x1a, // 四人三麻以外の三麻
	sanmaX    = 0x06, // 二～八萬なしの三麻
	richiMJ   = 0x1f, // 日本ルール
	guobiaoMJ = 0x20, // 中国ルール
};

} /* namespace */
