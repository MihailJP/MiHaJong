/*=============================
 * HSP麻雀クライアントMiHaJong
 *   [卓環境用ストラクチャ]
 *=============================
 */

#module envStruct PlayerName, RemotePlayerFlag

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

#modinit
	sdim PlayerName, 20, NUM_OF_PLAYERS
	dim RemotePlayerFlag, NUM_OF_PLAYERS
	return
#global
