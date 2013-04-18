#pragma once

#include "mjexport.h"
#include "gametbl.h"

#ifdef _WIN32
MJCORE void initapp(GameTypeID gameType, HWND hwnd);
#else /*_WIN32*/
MJCORE void initapp(GameTypeID gameType, void* hwnd);
#endif /*_WIN32*/
