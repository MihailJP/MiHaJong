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

#define SHANTEN_PAGES 8

void LoadFileInResource(int name, int type, DWORD& size, const char*& data);
typedef int8_t SHANTEN; // �������̂��߂̃f�[�^�^�i0=���v�A-1=�a���A127=�����j

#ifdef MJCORE_EXPORTS
MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PLAYER_ID playerID); // External
#endif

class ShantenAnalyzer {
private:
	static uint8_t* mentsuAnalysisDat;
#ifdef MJCORE_EXPORTS
	static size_t decompressMentsuAnalysisDat();
	static void calcSHA256(uint8_t* digest, const uint8_t* buf, size_t bufSize);
	static std::string bytesToHexString(std::vector<std::uint8_t> byteStr);
	static void verifyMentsuAnalysisDat(size_t bufSize);
public:
	static __declspec(dllexport) void initMentsuAnalysisDat();
#endif

public:
	static const int8_t SHANTEN_IMPOSSIBLE = SCHAR_MAX;
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

	static MJCORE SHANTEN calcShanten(const GameTable* const gameStat, PLAYER_ID playerID, shantenType mode);
#ifdef MJCORE_EXPORTS
	static __declspec(dllexport) int calcShanten(const GameTable* const gameStat, int playerID, int mode);
private:
	static SHANTEN calcShantenRegular(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenChiitoi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenKokushi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenStellar(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount, bool qixing);
	static SHANTEN calcShantenCivilWar(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenSyzygy(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
#endif

};

#endif