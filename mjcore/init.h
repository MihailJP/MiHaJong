#pragma once

#include "mjexport.h"
#include "gametbl.h"
#ifndef _WIN32
#include <X11/Xlib.h>
#endif /*_WIN32*/

#ifdef _WIN32
MJCORE void initapp(GameTypeID gameType, HWND hwnd);
#else /*_WIN32*/
MJCORE void initapp(GameTypeID gameType, Window hwnd);
#endif /*_WIN32*/
