#pragma once

#include "exports.h"
#include <mutex>
#include "../common/gametbl.h"

namespace mihajong_graphic {

using namespace mihajong_structs;

class GameStatus {
#ifdef GRAPHIC_EXPORTS
private:
	class GStatModFlag {
	public:
		std::recursive_mutex myCriticalSection;
		bool myModificationFlag;
		GStatModFlag();
		GStatModFlag(const GStatModFlag&) = delete; // Delete unexpected copy constructor
		GStatModFlag& operator= (const GStatModFlag&) = delete; // Delete unexpected assign operator
		~GStatModFlag();
	};
	static GStatModFlag myModFlag;
	static GameTable myGameStat, myGameStat1;
	static bool isInitialized;
#endif
public:
#ifdef GRAPHIC_EXPORTS
	static GameTable* gameStat();
	static GameTable* retrGameStat();
	static bool isModified();
#endif
	EXPORT static void updateGameStat(const GameTable* const gameStat);
public: /* Monostate class: cannot be instantiated */
	GameStatus() = delete;
	GameStatus(const GameStatus&) = delete;
	GameStatus& operator= (const GameStatus&) = delete;
	~GameStatus() = delete;
};

}
