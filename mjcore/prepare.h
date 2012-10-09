#ifndef PREPARE_H
#define PREPARE_H

#include <algorithm>
#include "../socket/socket.h"
#include "gametbl.h"
#include "envtbl.h"
#include "random.h"
#include "haifu.h"
#include "tileutil.h"

__declspec(dllexport) void shuffle(GameTable* const gameStat); // 牌をバッファに並べて、洗牌
__declspec(dllexport) void initdora(GameTable* const gameStat); // ドラの設定

class SeatShuffler { // 場決め処理
private:
	static volatile bool finished;
	static unsigned ClientNumber;
	static void shuffleSeat ();
	static DWORD WINAPI shuffleSeat_start (LPVOID param);
public:
	__declspec(dllexport) static void shuffle (unsigned cNumber);
	static bool isFinished() {return finished;}
	__declspec(dllexport) static int isFinished_hsp() {return finished ? 1 : 0;}
};
#endif
