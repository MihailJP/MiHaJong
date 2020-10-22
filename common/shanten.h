#pragma once

#include <cstdint>

namespace mihajong_structs {

typedef int8_t Shanten; // 向聴数のためのデータ型（0=聴牌、-1=和了、127=無効）

static const Shanten ShantenImpossible = SCHAR_MAX;
enum class ShantenType : uint8_t { // getShantenに渡すスイッチ用
	all, // すべて求める
	regular, // 普通の和了形のみ
	pairs, // 七対子
	orphans, // 国士無双
	stellar, // 七星無靠
	civilWar, // 南北戦争
	tohokuGreen, // 東北新幹線グリーン車
	syzygy, // 惑星直列
	quanbukao, // 全不靠
	sevenup, // セブンアップ
	zuhelong, // 組合龍
	ninnaji, // 仁和寺
	pages,
};
constexpr auto ShantenPages = static_cast<unsigned int>(ShantenType::pages);

} /* namespace */
