#ifndef MOUDA_H
#define MOUDA_H

#ifdef MJCORE_EXPORTS
#include <cassert>
#include "logging.h"
#include "envtbl.h"
#include "ai/class.h"
#include "remote.h"
#include "../socket/socket.h"
#endif
#include "gametbl.h"
#include "discard.h"

DiscardTileNum getdahai(GameTable* const gameStat);

#endif
