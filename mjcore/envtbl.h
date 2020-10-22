#pragma once

#include <cstdint>
#include <string>
#include "gametbl.h"

enum class ClientType : int32_t {
	unavailable = (-1),
	standalone, server, client,
};

class EnvTable { // 卓には直接関係ないデータ [singleton]
private: // singleton
	EnvTable();
	EnvTable(const EnvTable&) = delete; // Delete unexpected copy constructor
	EnvTable& operator= (const EnvTable&) = delete; // Delete unexpected assign operator

public:
	// 内部クラスとかを定義する
	class PlayerLabel {
	public:
		CodeConv::tstring PlayerName;
		int8_t RemotePlayerFlag;
		PlayerLabel() : RemotePlayerFlag(0) {}
	};

	static EnvTable* Instantiate(); // Singleton instance accessor

	// データ
	InfoByPlayer<PlayerLabel> PlayerDat;
	ClientType GameMode;
	bool WatchModeFlag;
	uint8_t bgColorR, bgColorG, bgColorB;
};

LPCTSTR getName(PlayerID playerID);
bool isStandAlone();
bool isWatchMode();
