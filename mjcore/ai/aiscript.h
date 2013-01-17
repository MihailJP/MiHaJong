#pragma once

#include "class.h"

class aiscript::table::playertable {
private:
	class pltable;
public:
	static inline void PlayerTbl(lua_State* const L, const PlayerTable* const plStat, PLAYER_ID player);
};

class aiscript::table::playertable::pltable {
public:
	static inline void PlayerHand(lua_State* const L, const HAND_TILES* const plHand);
	static inline void PlayerDiscard(lua_State* const L, const DISCARD_BUF* const plDiscard, uint8_t DiscardPointer);
	static inline void PlayerMeld(lua_State* const L, const MELD_BUF* const plMeld, uint8_t MeldPointer);
	static inline void PlayerRichiStat(lua_State* const L, const RichiStat* const plstat);
	static inline void PlayerFlower(lua_State* const L, const FLOWERS* const plstat);
	static inline void PlayerDeclFlag(lua_State* const L, const DECLFLAG* const plstat);
};
