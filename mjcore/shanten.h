#pragma once

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
#endif
#include "except.h"
#include "mjexport.h"
#include "gametbl.h"
#include "tilecode.h"
#include "func.h"
#ifdef MJCORE_EXPORTS
#include "logging.h"
#include "reader/readrsrc.h"
#include "strcode.h"
#include "decomp.h"
#endif

#define SHANTEN_PAGES 10

void LoadFileInResource(int name, int type, DWORD& size, const char*& data);
typedef int8_t SHANTEN; // �������̂��߂̃f�[�^�^�i0=���v�A-1=�a���A127=�����j

#ifdef MJCORE_EXPORTS
MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PLAYER_ID playerID); // External
#endif

class ShantenAnalyzer {
private:
	static uint8_t* mentsuAnalysisDat;

public:
#ifdef MJCORE_EXPORTS
	static __declspec(dllexport) void initMentsuAnalysisDat();
#endif

	static const int8_t SHANTEN_IMPOSSIBLE = SCHAR_MAX;
	enum shantenType : uint8_t { // getShanten�ɓn���X�C�b�`�p
		shantenAll, // ���ׂċ��߂�
		shantenRegular, // ���ʂ̘a���`�̂�
		shantenPairs, // ���Ύq
		shantenOrphans, // ���m���o
		shantenStellar, // ��������
		shantenCivilWar, // ��k�푈
		shantenSyzygy, // �f������
		shantenQuanbukao, // �S�s��
		shantenSevenup, // �Z�u���A�b�v
		shantenZuhelong, // �g����
	};

	static MJCORE SHANTEN calcShanten(const GameTable* const gameStat, PLAYER_ID playerID, shantenType mode);
#ifdef MJCORE_EXPORTS
	static __declspec(dllexport) int calcShanten(const GameTable* const gameStat, int playerID, int mode);
private:
	static void setQixingTilePattern(tileCode* const QixingPai, unsigned int pattern);
	static unsigned int chkMianzi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount, unsigned limit);
	static SHANTEN calcShantenRegular(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenChiitoi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenKokushi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenStellar(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount, bool qixing);
	static SHANTEN calcShantenCivilWar(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenSyzygy(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenSevenup(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
	static SHANTEN calcShantenZuhelong(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount);
#endif

};
