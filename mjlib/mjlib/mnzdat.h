#ifndef MNZDAT_H
#define MNZDAT_H

#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>
#include "resource.h"
#include "lzma/LzmaLib.h"
#include "lzma/Sha256.h"
#include "except.h"
#include "gametbl.h"
#include "tilecode.h"
#include "func.h"

void LoadFileInResource(int name, int type, DWORD& size, const char*& data);
size_t decompressMentsuAnalysisDat();
void calcSHA256(uint8_t* digest, const uint8_t* buf, size_t bufSize);
std::string bytesToHexString(std::vector<std::uint8_t> byteStr);
void verifyMentsuAnalysisDat(size_t bufSize);
__declspec(dllexport) void initMentsuAnalysisDat();

typedef int SHANTEN; // 向聴数のためのデータ型（0=聴牌、-1=和了、999=無効）
enum shantenType { // getShantenに渡すスイッチ用
	shantenAll, // すべて求める
	shantenRegular, // 普通の和了形のみ
	shantenPairs, // 七対子
	shantenOrphans, // 国士無双
	shantenStellar, // 七星無靠
	shantenCivilWar, // 南北戦争
	shantenSyzygy, // 惑星直列
	shantenQuanbukao // 全不靠
};

SHANTEN calcShanten(GameTable gameStat, PLAYER_ID playerID, shantenType mode);
SHANTEN calcShantenRegular(GameTable gameStat, PLAYER_ID playerID, TileCount tileCount);

#endif
