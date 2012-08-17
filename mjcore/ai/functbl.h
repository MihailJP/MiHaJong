#ifndef FUNCTBL_H
#define FUNCTBL_H

#include <lua.hpp>
#include "class.h"

enum MeldCallID {
	meldNone, meldRon, meldKan, meldPon, meldChiiLower, meldChiiMiddle, meldChiiUpper
};

class aiscript::table::functable {
private:
	static const char* tblname;
	static inline void discardTileCode(lua_State* const L);
	static inline void meldCallCode(lua_State* const L);
public:
	static void inittable(lua_State* const L);
};

#endif
