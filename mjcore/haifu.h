#ifndef HAIFU_H
#define HAIFU_H

#include <sstream>
#include <string>
#include "gametbl.h"
#include "largenum.h"
#include "func.h"

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
}

#endif
