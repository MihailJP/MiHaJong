/*=============================
 * HSP麻雀クライアントMiHaJong
 *  [状態管理用ストラクチャ]
 *=============================
 */

/* メンバにアクセスするためのAPI */
#uselib "mjlib.dll"
#func setScore "setScore" int, int, int, int
#func addScore "addScore" int, int, int, int
#cfunc getScore "getScore" int, int, int
#func _exportScore "exportScore" int, var
#func importScore "importScore" int, var
#define exportScore(%1,%2) dim %2, NUM_OF_PLAYERS, NUM_OF_DIGIT_GROUPS: _exportScore %1, %2

#func setChip "setChip" int, int, int
#func addChip "addChip" int, int, int
#cfunc getChip "getChip" int, int

#func setSumaroFlag "setSumaroFlag" int, int, int
#cfunc getSumaroFlag "getSumaroFlag" int, int

#func setYakitori "setYakitori" int, int, int
#cfunc getYakitori "getYakitori" int, int

#func setPlayer "setPlayer" int, int
#cfunc getPlayer "getPlayer" int

#cfunc getGameLength "getGameLength" int

#func setRound "setRound" int, int
#func incRound "incRound" int
#cfunc getRound "getRound" int

#func setRoundLoop "setRoundLoop" int, int
#func incRoundLoop "incRoundLoop" int
#cfunc getRoundLoop "getRoundLoop" int

#func setHonba "setHonba" int, int
#func incHonba "incHonba" int
#cfunc getHonba "getHonba" int

#func setTurn "setTurn" int, int
#func incTurn "incTurn" int
#cfunc getTurn "getTurn" int

#func setDeposit "setDeposit" int, int
#func addDeposit "addDeposit" int, int
#cfunc getDeposit "getDeposit" int

#func setAgariChain "setAgariChain" int, int
#func incAgariChain "incAgariChain" int
#cfunc getAgariChain "getAgariChain" int

#func setLastAgariPlayer "setLastAgariPlayer" int, int
#cfunc getLastAgariPlayer "getLastAgariPlayer" int

#func setHand "setHand" int, int, int, int, int
#cfunc getHand "getHand" int, int, int, int

#func setDiscard "setDiscard" int, int, int, int, int
#cfunc getDiscard "getDiscard" int, int, int, int
#cfunc DiscardPointer "DiscardPointer" int, int
#func DiscardPointerIncrement "DiscardPointerIncrement" int, int
#func flagDiscard "flagDiscard" int, int, int, int

#func setMeld "setMeld" int, int, int, int, int
#cfunc getMeld "getMeld" int, int, int, int
#cfunc MeldPointer "MeldPointer" int, int
#func MeldPointerIncrement "MeldPointerIncrement" int, int
#func MeldPointerDecrement "MeldPointerDecrement" int, int
#func flagMeld "flagMeld" int, int, int, int, int

#func setMenzen "setMenzen" int, int, int
#cfunc getMenzen "getMenzen" int, int

#func setHandStat "setHandStat" int, int, int
#cfunc getHandStat "getHandStat" int, int

#func setNumberOfQuads "setNumberOfQuads" int, int, int
#func incNumberOfQuads "incNumberOfQuads" int, int
#cfunc getNumberOfQuads "getNumberOfQuads" int, int

#func setRichiFlag "setRichiFlag" int, int, int, int
#cfunc getRichiFlag "getRichiFlag" int, int, int

#func setOpenWait "setOpenWait" int, int, int
#cfunc getOpenWait "getOpenWait" int, int

#func setFirstDrawFlag "setFirstDrawFlag" int, int, int
#cfunc getFirstDrawFlag "getFirstDrawFlag" int, int

#func setDoujunFuriten "setDoujunFuriten" int, int, int
#cfunc getDoujunFuriten "getDoujunFuriten" int, int

#func setAgariHouki "setAgariHouki" int, int, int
#cfunc getAgariHouki "getAgariHouki" int, int

#func putFlowerFlag "putFlowerFlag" int, int, int
#func setFlowerFlag "setFlowerFlag" int, int, int
#cfunc getFlowerFlag "getFlowerFlag" int, int

#func setNorthFlag "setNorthFlag" int, int, int
#func addNorthFlag "addNorthFlag" int, int
#cfunc getNorthFlag "getNorthFlag" int, int

#func setKangFlag "setKangFlag" int, int, int
#func incKangFlag "incKangFlag" int, int
#cfunc getKangFlag "getKangFlag" int, int

#func setTotalKang "setTotalKang" int, int
#func incTotalKang "incTotalKang" int
#cfunc getTotalKang "getTotalKang" int

#func setRichiCounterFlag "setRichiCounterFlag" int, int
#cfunc getRichiCounterFlag "getRichiCounterFlag" int

#func calcWareme "calcWareme" int
#cfunc getWareme "getWareme" int

#func setDoukasen "setDoukasen" int, int
#cfunc getDoukasen "getDoukasen" int

#cfunc getShibari "getShibari" int

#func setPao "setPao" int, int, int, int
#cfunc getPao "getPao" int, int, int

#func setDice "setDice" int, int, int, int
#cfunc getDice "getDice" int, int, int

#func addDeadWallLength "addDeadWallLength" int, int
#cfunc getDeadWallLength "getDeadWallLength" int

#func setWall "setWall" int, int, int, int
#cfunc getWall "getWall" int, int, int

#cfunc getRinshanExtension "getRinshanExtension" int

#func setDrawPointer "setDrawPointer" int, int
#func incDrawPointer "incDrawPointer" int
#cfunc getDrawPointer "getDrawPointer" int

#func setDoraPointer "setDoraPointer" int, int
#cfunc getDoraPointer "getDoraPointer" int

#func setRinshanPointer "setRinshanPointer" int, int
#func decRinshanPointer "decRinshanPointer" int
#cfunc getRinshanPointer "getRinshanPointer" int

#func setHeavenHandFlag "setHeavenHandFlag" int, int
#cfunc getHeavenHandFlag "getHeavenHandFlag" int

#func setPreviousMeld "setPreviousMeld" int, int, int
#cfunc getPreviousMeld "getPreviousMeld" int, int

#func setDisconnectFlag "setDisconnectFlag" int, int, int
#cfunc getDisconnectFlag "getDisconnectFlag" int, int

#func setDoraFlag "setDoraFlag" int, int, int, int
#func incDoraFlag "incDoraFlag" int, int, int
#cfunc getDoraFlag "getDoraFlag" int, int, int

#func setCurrentDiscard "setCurrentDiscard" int, int, int
#cfunc getCurrentDiscard "getCurrentDiscard" int, int

#func setTsumoAgariFlag "setTsumoAgariFlag" int, int
#cfunc getTsumoAgariFlag "getTsumoAgariFlag" int

#func setDeclarationFlag "setDeclarationFlag" int, int, int, int
#func resetDeclarationFlag "resetDeclarationFlag" int
#cfunc getDeclarationFlag "getDeclarationFlag" int, int, int

#func setCurrentPlayer "setCurrentPlayer" int, int, int
#func resetCurrentPlayer "resetCurrentPlayer" int
#cfunc getCurrentPlayer "getCurrentPlayer" int, int

/* 局単位での初期化 */
#func inittable "inittable" int

/* 初期化ルーチン */
#cfunc initializeGameTable "initializeGameTable" int
#cfunc initializeSandbox "initializeSandbox" int
