#ifndef FUNCTBL_H
#define FUNCTBL_H

#include <lua.hpp>
#include "class.h"

class aiscript::table::functable {
private:
	static const char* tblname;
	static inline void discardTileCode(lua_State* const L);
public:
	static void inittable(lua_State* const L);
};

#endif
