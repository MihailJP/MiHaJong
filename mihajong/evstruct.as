/*=============================
 * HSP麻雀クライアントMiHaJong
 *   [卓環境用ストラクチャ]
 *=============================
 */

#module envStruct PlayerName, RemotePlayerFlag, GameMode, WatchModeFlag, \
bgColorR, bgColorG, bgColorB

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
#modcfunc GetGameMode
	return GameMode

#modfunc setWatchModeFlag int Value
	WatchModeFlag = Value
	return
#modcfunc GetWatchModeFlag
	return WatchModeFlag

#modfunc SetTableColor int r, int g, int b
	bgColorR = r: bgColorG = g: bgColorB = b
	return
#modcfunc GetTableColorR
	return bgColorR
#modcfunc GetTableColorG
	return bgColorG
#modcfunc GetTableColorB
	return bgColorB

#modfunc TableEnvInit
	sdim PlayerName, 20, NUM_OF_PLAYERS
	dim RemotePlayerFlag, NUM_OF_PLAYERS
	GameMode = GAMEMODE_UNAVAILABLE
	WatchModeFlag = 0
	return

#modinit
	TableEnvInit thismod
	bgColorR = 0: bgColorG = 0: bgColorB = 0
	return
#global
