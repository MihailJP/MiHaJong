#pragma once

#include "class.h"

class aiscript::table::playertable {
private:
	class pltable;
public:
	static inline void PlayerTbl(lua_State* const L, const PlayerTable* const plStat, PlayerID player);
};

class aiscript::table::playertable::pltable {
public:
	static inline void PlayerHand(lua_State* const L, const HandTiles* const plHand);
	static inline void PlayerDiscard(lua_State* const L, const DiscardBuf* const plDiscard, uint8_t DiscardPointer);
	static inline void PlayerMeld(lua_State* const L, const MeldBuf* const plMeld, uint8_t MeldPointer);
	static inline void PlayerRichiStat(lua_State* const L, const RichiStat* const plstat);
	static inline void PlayerFlower(lua_State* const L, const Flowers* const plstat);
	static inline void PlayerDeclFlag(lua_State* const L, const DeclFlag* const plstat);
};
