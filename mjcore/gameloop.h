#ifndef GAMELOOP_H
#define GAMELOOP_H

#ifdef MJCORE_EXPORTS
#include "strcode.h"
#include "logging.h"
#include "envtbl.h"
#include "../socket/socket.h"
#include "../sound/sound.h"
#include "bgmid.h"
#include "tileutil.h"
#include "mouda.h"
#include "fuuro.h"
#include "prepare.h"
#include "../graphic/graphic.h"
#endif
#include "gametbl.h"

EndType doTableTurn(GameTable* const gameStat);
bool doTableRound(GameTable* const gameStat);
void startgame(gameTypeID gameType);

#endif
