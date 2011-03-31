/*=============================
 * HSP麻雀クライアントMiHaJong
 *   [卓環境用ストラクチャ]
 *=============================
 */

#module envStruct PlayerName, RemotePlayerFlag, GameMode, WatchModeFlag

#include "const.as"

#modfunc setPlayerName int PlayerNum, str NameStr
	PlayerName(PlayerNum) = NameStr
	return
#modcfunc getPlayerName int PlayerNum
	return PlayerName(PlayerNum)

#modfunc setRemotePlayer int PlayerNum, int Value
	RemotePlayerFlag(PlayerNum) = Value
	return
#modcfunc isRemotePlayer int PlayerNum
	return RemotePlayerFlag(PlayerNum)

#modfunc setGameMode int Value
	GameMode = Value
	return
#modcfunc GetGameMode int PlayerNum
	return GameMode

#modfunc setWatchModeFlag int Value
	WatchModeFlag = Value
	return
#modcfunc GetWatchModeFlag int PlayerNum
	return WatchModeFlag

#modinit
	sdim PlayerName, 20, NUM_OF_PLAYERS
	dim RemotePlayerFlag, NUM_OF_PLAYERS
	GameMode = -1
	WatchModeFlag = 0
	return
#global
