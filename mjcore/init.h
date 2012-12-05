#pragma once

#include "mjexport.h"
#include "gametbl.h"
#ifdef MJCORE_EXPORTS
#include "logging.h"
#include "func.h"
#include <string>
#include "strcode.h"
#include "../mihajong/version.h"
#include "envtbl.h"
#include "../sound/sound.h"
#include "bgmid.h"
#include "sound.h"
#include "shanten.h"
#include "random.h"
#include "../graphic/graphic.h"
#endif

MJCORE void initapp(gameTypeID gameType, HWND hwnd);
