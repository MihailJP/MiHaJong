#pragma once

#include <cstdint>
#include <string>
#include "gametbl.h"

class EnvTable { // ��ɂ͒��ڊ֌W�Ȃ��f�[�^ [singleton]
private: // singleton
	EnvTable();
	EnvTable(const EnvTable&);
	EnvTable& operator=(const EnvTable&);

public:
	// �����N���X�Ƃ����`����
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

	// �f�[�^
	InfoByPlayer<PlayerLabel> PlayerDat;
	ClientType GameMode;
	bool WatchModeFlag;
	uint8_t bgColorR, bgColorG, bgColorB;
};

LPCTSTR getName(PlayerID playerID);
