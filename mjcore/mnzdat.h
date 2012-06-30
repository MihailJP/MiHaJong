#ifndef MNZDAT_H
#define MNZDAT_H

#include <windows.h>
#ifndef IMPORT_MJCORE
#include <cstdio>
#endif
#include <cstdint>
#ifndef IMPORT_MJCORE
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>
#include "resource.h"
#include "lzma/LzmaLib.h"
#include "lzma/Sha256.h"
#include "except.h"
#endif
#include "mjexport.h"
#include "gametbl.h"
#include "tilecode.h"
#include "func.h"
#ifndef IMPORT_MJCORE
#include "logging.h"
#include "dllmain.h"
#endif

#define SHANTEN_IMPOSSIBLE 999

#ifndef IMPORT_MJCORE
void LoadFileInResource(int name, int type, DWORD& size, const char*& data);
size_t decompressMentsuAnalysisDat();
void calcSHA256(uint8_t* digest, const uint8_t* buf, size_t bufSize);
std::string bytesToHexString(std::vector<std::uint8_t> byteStr);
void verifyMentsuAnalysisDat(size_t bufSize);
__declspec(dllexport) void initMentsuAnalysisDat();
#endif

typedef int SHANTEN; // �������̂��߂̃f�[�^�^�i0=���v�A-1=�a���A999=�����j
enum shantenType { // getShanten�ɓn���X�C�b�`�p
	shantenAll, // ���ׂċ��߂�
	shantenRegular, // ���ʂ̘a���`�̂�
	shantenPairs, // ���Ύq
	shantenOrphans, // ���m���o
	shantenStellar, // ��������
	shantenCivilWar, // ��k�푈
	shantenSyzygy, // �f������
	shantenQuanbukao // �S�s��
};

MJCORE SHANTEN calcShanten(GameTable* gameStat, PLAYER_ID playerID, shantenType mode);
#ifndef IMPORT_MJCORE
__declspec(dllexport) int calcShanten(GameTable* gameStat, int playerID, int mode);
SHANTEN calcShantenRegular(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount);
SHANTEN calcShantenChiitoi(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount);
SHANTEN calcShantenKokushi(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount);
SHANTEN calcShantenStellar(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount, bool qixing);
SHANTEN calcShantenCivilWar(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount);
SHANTEN calcShantenSyzygy(GameTable* gameStat, PLAYER_ID playerID, TileCount& tileCount);
#endif

#endif
