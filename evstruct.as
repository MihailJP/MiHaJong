/*=============================
 * HSP麻雀クライアントMiHaJong
 *   [卓環境用ストラクチャ]
 *=============================
 */

#module envStruct PlayerName

#include "const.as"

#modfunc setPlayerName int PlayerNum, str NameStr
	PlayerName(PlayerNum) = NameStr
	return
#modcfunc getPlayerName int PlayerNum
	return PlayerName(PlayerNum)

#modinit
	sdim PlayerName, 20, NUM_OF_PLAYERS
	return
#global
