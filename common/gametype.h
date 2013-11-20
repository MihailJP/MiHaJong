#pragma once

#include <cstdint>

namespace mihajong_structs {

using std::uint8_t;

enum GameTypeID : uint8_t { // 卓の種類(四麻、三麻)指定用
	Yonma = 0x01, Sanma = 0x02, Sanma4 = 0x04, SanmaS = 0x08,
	AllSanma = 0x0e, SanmaT = 0x0a, SanmaX = 0x06, RichiMJ = 0x0f,
	GuobiaoMJ = 0x10,
};

} /* namespace */
