#pragma once

#include <cstdint>

namespace mihajong_structs {

typedef int8_t Shanten; // 向聴数のためのデータ型（0=聴牌、-1=和了、127=無効）

static const Shanten ShantenImpossible = SCHAR_MAX;
enum ShantenType : uint8_t { // getShantenに渡すスイッチ用
	shantenAll, // すべて求める
	shantenRegular, // 普通の和了形のみ
	shantenPairs, // 七対子
	shantenOrphans, // 国士無双
	shantenStellar, // 七星無靠
	shantenCivilWar, // 南北戦争
	shantenTohokuGreen, // 東北新幹線グリーン車
	shantenSyzygy, // 惑星直列
	shantenQuanbukao, // 全不靠
	shantenSevenup, // セブンアップ
	shantenZuhelong, // 組合龍
	ShantenPages,
};

} /* namespace */
