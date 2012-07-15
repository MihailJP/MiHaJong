#ifndef ENVTBL_H
#define ENVTBL_H

#include <cstdint>
#include <string>
#include "gametbl.h"

class EnvTable { // 卓には直接関係ないデータ [singleton]
private: // singleton
	EnvTable(){}
	EnvTable(const EnvTable&);
	EnvTable& operator=(const EnvTable&);

	// 内部クラスとかを定義する
	class PlayerLabel {
	public:
		std::string PlayerName;
		int8_t RemotePlayerFlag;
	};
	enum ClientType : int32_t {
		Unavailable = (-1),
		Standalone, Server, Client,
	};
public:
	static EnvTable* Instantiate(); // Singleton instance accessor

	// データ
	InfoByPlayer<PlayerLabel> PlayerDat;
	ClientType GameMode;
	bool WatchModeFlag;
	uint8_t bgColorR, bgColorG, bgColorB;

	// HSPからのアクセサ
	__declspec(dllexport) static void setPlayerName(void *, int PlayerNum, const char* const NameStr);
	__declspec(dllexport) static void getPlayerName(void *, int PlayerNum, char* const NameStr, int bufsize);
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

#endif
