#ifndef HAIFU_H
#define HAIFU_H

#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "gametbl.h"
#include "largenum.h"
#include "func.h"
#include "envtbl.h"

// 今のところここでしか使ってないから仮置き。
// 移植が進んで別のところでも使うようなら移動します

/* 局終了コード */
enum EndType : int { // 局の終了理由
	Ryuukyoku,       // 普通の流局
	Agari,           // 誰かの和了
	Chonbo,          // 錯和で終了
	KyuushuKyuuhai,  // 九種九牌
	Suukaikan,       // 四開槓
	TripleRon,       // 三家和
	SuufonRenda,     // 四風連打
	SuuchaRiichi,    // 四人立直
	NagashiMangan,   // 流し満貫
	Disconnect,      // 通信途絶(ネットワーク対戦時のみ)
	Uukaikan,        // 五回目の槓で流局した場合(誰かが四槓子聴牌の場合のみ)
};

// 牌譜関係のコードはそれ用の名前空間に分けておきましょうか。
namespace haifu {
	__declspec(dllexport) void haifubufinit();
	__declspec(dllexport) void haifuinit();

	__declspec(dllexport) void haifurechaipai(const GameTable* const gameStat);

	void haifurecdora(tileCode tmpDora);
	__declspec(dllexport) void haifurecdora(int tmpDora);

	void haifurecuradora(tileCode tmpDora);
	__declspec(dllexport) void haifurecuradora(int tmpDora);

	void haifurecalicedora(tileCode tmpDora);
	__declspec(dllexport) void haifurecalicedora(int tmpDora);

	__declspec(dllexport) void haifualicedoraupd();

	__declspec(dllexport) void haifuresetalicedora();

	__declspec(dllexport) void haifurecdorap();

	__declspec(dllexport) void haifurecmota(const GameTable* const gameStat, int DiscardTileIndex);

	__declspec(dllexport) void haifurecfurikomi(const GameTable* const gameStat);

	__declspec(dllexport) void haifurecchi(const GameTable* const gameStat);
	__declspec(dllexport) void haifurecpon(const GameTable* const gameStat);
	__declspec(dllexport) void haifurectsumo(const GameTable* const gameStat);
	__declspec(dllexport) void haifurecminkan(const GameTable* const gameStat);

	__declspec(dllexport) void haifurecnorth(const GameTable* const gameStat, int DiscardTileIndex);
	__declspec(dllexport) void haifurecchanpei(const GameTable* const gameStat);
	__declspec(dllexport) void haifurecnorthproc(const GameTable* const gameStat);

	__declspec(dllexport) void haifurecankan(const GameTable* const gameStat, int DiscardTileIndex);
	__declspec(dllexport) void haifurecchankan(const GameTable* const gameStat);
	__declspec(dllexport) void haifureckanproc(const GameTable* const gameStat);

	__declspec(dllexport) void haifurecflower(const GameTable* const gameStat, int DiscardTileIndex);

	__declspec(dllexport) void haifuwritebuffer(
		const GameTable* const gameStat, void *,
		int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
		const char* ResultDesc, int RoundEndType);

	void haifusave(const GameTable* const gameStat);
	__declspec(dllexport) void haifusave();
}

#endif
