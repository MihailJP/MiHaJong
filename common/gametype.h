#pragma once

#include <cstdint>

namespace mihajong_structs {

using std::uint8_t;

enum GameTypeID : uint8_t { // 卓の種類(四麻、三麻)指定用
	Yonma     = 0x01, // 四人打ち
	Sanma     = 0x02, // 三人打ち
	Sanma4    = 0x04, // 四人三麻
	SanmaS    = 0x08, // 数牌三麻
	SanmaSeto = 0x10, // 瀬戸内三麻
	AllSanma  = 0x1e, // 三麻全ルール
	SanmaT    = 0x1a, // 四人三麻以外の三麻
	SanmaX    = 0x06, // 二～八萬なしの三麻
	RichiMJ   = 0x1f, // 日本ルール
	GuobiaoMJ = 0x20, // 中国ルール
};

} /* namespace */
