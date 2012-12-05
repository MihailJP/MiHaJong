#pragma once

#include <cstdint>
#include <string>
#include "gametbl.h"

class EnvTable { // 卓には直接関係ないデータ [singleton]
private: // singleton
	EnvTable(){}
	EnvTable(const EnvTable&);
	EnvTable& operator=(const EnvTable&);

public:
	// 内部クラスとかを定義する
	class PlayerLabel {
	public:
		CodeConv::tstring PlayerName;
		int8_t RemotePlayerFlag;
	};

	enum ClientType : int32_t {
		Unavailable = (-1),
		Standalone, Server, Client,
	};
	static EnvTable* Instantiate(); // Singleton instance accessor

	// データ
	InfoByPlayer<PlayerLabel> PlayerDat;
	ClientType GameMode;
	bool WatchModeFlag;
	uint8_t bgColorR, bgColorG, bgColorB;

	static void TableEnvInit();
	// HSPからのアクセサ
	__declspec(dllexport) static void setPlayerName(void *, int PlayerNum, LPCTSTR const NameStr);
	__declspec(dllexport) static void getPlayerName(void *, int PlayerNum, LPTSTR const NameStr, int bufsize);
	__declspec(dllexport) static void setRemotePlayer(void *, int PlayerNum, int Value);
	__declspec(dllexport) static int isRemotePlayer(void *, int PlayerNum);
	__declspec(dllexport) static void setGameMode(void *, int Value);
	__declspec(dllexport) static int getGameMode(void *);
	__declspec(dllexport) static void setWatchModeFlag(void *, int Value);
	__declspec(dllexport) static int getWatchModeFlag(void *);
	__declspec(dllexport) static void setTableColor(void *, int R, int G, int B);
	__declspec(dllexport) static int getTableColorR(void *);
	__declspec(dllexport) static int getTableColorG(void *);
	__declspec(dllexport) static int getTableColorB(void *);
	__declspec(dllexport) static void TableEnvInit(void *);
	__declspec(dllexport) static void TableEnvInitAll(void *);
};
