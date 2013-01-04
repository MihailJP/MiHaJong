#pragma once

#include "../mjcore/mjimport.h"
#include "exports.h"

namespace mihajong_graphic {

class GameStatus {
#ifdef GRAPHIC_EXPORTS
private:
	class GStatModFlag {
	public:
		CRITICAL_SECTION myCriticalSection;
		bool myModificationFlag;
		GStatModFlag();
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
};

}
