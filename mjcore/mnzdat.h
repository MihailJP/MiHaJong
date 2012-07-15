#ifndef MNZDAT_H
#define MNZDAT_H

#include <windows.h>
#ifdef MJCORE_EXPORTS
#include <cstdio>
#endif
#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include "resource.h"
#include "lzma/LzmaLib.h"
#include "lzma/Sha256.h"
#endif
#include "except.h"
#include "mjexport.h"
#include "gametbl.h"
#include "tilecode.h"
#include "func.h"
#ifdef MJCORE_EXPORTS
#include "logging.h"
#include "dllmain.h"
#endif

#define SHANTEN_IMPOSSIBLE SCHAR_MAX

#ifdef MJCORE_EXPORTS
MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PLAYER_ID playerID); // External

void LoadFileInResource(int name, int type, DWORD& size, const char*& data);
size_t decompressMentsuAnalysisDat();
void calcSHA256(uint8_t* digest, const uint8_t* buf, size_t bufSize);
std::string bytesToHexString(std::vector<std::uint8_t> byteStr);
void verifyMentsuAnalysisDat(size_t bufSize);
__declspec(dllexport) void initMentsuAnalysisDat();
#endif

typedef int8_t SHANTEN; // �������̂��߂̃f�[�^�^�i0=���v�A-1=�a���A999=�����j
enum shantenType : uint8_t { // getShanten�ɓn���X�C�b�`�p
	shantenAll, // ���ׂċ��߂�
	shantenRegular, // ���ʂ̘a���`�̂�
	shantenPairs, // ���Ύq
	shantenOrphans, // ���m���o
	shantenStellar, // ��������
	shantenCivilWar, // ��k�푈
	shantenSyzygy, // �f������
	shantenQuanbukao // �S�s��
};
#define SHANTEN_PAGES 8

MJCORE SHANTEN calcShanten(const GameTable* const gameStat, PLAYER_ID playerID, shantenType mode);
#ifdef MJCORE_EXPORTS
__declspec(dllexport) int calcShanten(const GameTable* const gameStat, int playerID, int mode);
SHANTEN calcShantenRegular(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
SHANTEN calcShantenChiitoi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
SHANTEN calcShantenKokushi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
SHANTEN calcShantenStellar(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount, bool qixing);
SHANTEN calcShantenCivilWar(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
SHANTEN calcShantenSyzygy(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
#endif

#endif
