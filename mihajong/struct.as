/*=============================
 * HSP麻雀クライアントMiHaJong
 *  [状態管理用ストラクチャ]
 *=============================
 */

#module

/* メンバにアクセスするためのAPI */
#uselib "mjlib.dll"
#func global setScore "setScore" int, int, int, int
#func global addScore "addScore" int, int, int, int
#cfunc global getScore "getScore" int, int, int
#func global exportScore "exportScore" int, var
#func global importScore "importScore" int, var

#func global setChip "setChip" int, int, int
#func global addChip "addChip" int, int, int
#cfunc global getChip "getChip" int, int

#func global setSumaroFlag "setSumaroFlag" int, int, int
#cfunc global getSumaroFlag "getSumaroFlag" int, int

#func global setYakitori "setYakitori" int, int, int
#cfunc global getYakitori "getYakitori" int, int

#func global setPlayer "setPlayer" int, int
#cfunc global getPlayer "getPlayer" int

#cfunc global getGameLength "getGameLength" int

#func global setRound "setRound" int, int
#func global incRound "incRound" int
#cfunc global getRound "getRound" int

#func global setRoundLoop "setRoundLoop" int, int
#func global incRoundLoop "incRoundLoop" int
#cfunc global getRoundLoop "getRoundLoop" int

#func global setHonba "setHonba" int, int
#func global incHonba "incHonba" int
#cfunc global getHonba "getHonba" int

#func global setTurn "setTurn" int, int
#func global incTurn "incTurn" int
#cfunc global getTurn "getTurn" int

#func global setDeposit "setDeposit" int, int
#func global addDeposit "addDeposit" int, int
#cfunc global getDeposit "getDeposit" int

#func global setAgariChain "setAgariChain" int, int
#func global incAgariChain "incAgariChain" int
#cfunc global getAgariChain "getAgariChain" int

#func global setLastAgariPlayer "setLastAgariPlayer" int, int
#cfunc global getLastAgariPlayer "getLastAgariPlayer" int

#func global setHand "setHand" int, int, int, int, int
#cfunc global getHand "getHand" int, int, int, int

#func global setDiscard "setDiscard" int, int, int, int, int
#cfunc global getDiscard "getDiscard" int, int, int, int
#cfunc global DiscardPointer "DiscardPointer" int, int
#func global DiscardPointerIncrement "DiscardPointerIncrement" int, int
#func global flagDiscard "flagDiscard" int, int, int, int

#func global setMeld "setMeld" int, int, int, int, int
#cfunc global getMeld "getMeld" int, int, int, int
#cfunc global MeldPointer "MeldPointer" int, int
#func global MeldPointerIncrement "MeldPointerIncrement" int, int
#func global MeldPointerDecrement "MeldPointerDecrement" int, int
#func global flagMeld "flagMeld" int, int, int, int, int

#func global setMenzen "setMenzen" int, int, int
#cfunc global getMenzen "getMenzen" int, int

#func global setHandStat "setHandStat" int, int, int
#cfunc global getHandStat "getHandStat" int, int

#func global setNumberOfQuads "setNumberOfQuads" int, int, int
#func global incNumberOfQuads "incNumberOfQuads" int, int
#cfunc global getNumberOfQuads "getNumberOfQuads" int, int

#func global setRichiFlag "setRichiFlag" int, int, int, int
#cfunc global getRichiFlag "getRichiFlag" int, int, int

#func global setOpenWait "setOpenWait" int, int, int
#cfunc global getOpenWait "getOpenWait" int, int

#func global setFirstDrawFlag "setFirstDrawFlag" int, int, int
#cfunc global getFirstDrawFlag "getFirstDrawFlag" int, int

#func global setDoujunFuriten "setDoujunFuriten" int, int, int
#cfunc global getDoujunFuriten "getDoujunFuriten" int, int

#func global setAgariHouki "setAgariHouki" int, int, int
#cfunc global getAgariHouki "getAgariHouki" int, int

#func global putFlowerFlag "putFlowerFlag" int, int, int
#func global setFlowerFlag "setFlowerFlag" int, int, int
#cfunc global getFlowerFlag "getFlowerFlag" int, int

#func global setNorthFlag "setNorthFlag" int, int, int
#func global addNorthFlag "addNorthFlag" int, int
#cfunc global getNorthFlag "getNorthFlag" int, int

#func global setKangFlag "setKangFlag" int, int, int
#func global incKangFlag "incKangFlag" int, int
#cfunc global getKangFlag "getKangFlag" int, int

#func global setTotalKang "setTotalKang" int, int
#func global incTotalKang "incTotalKang" int
#cfunc global getTotalKang "getTotalKang" int

#func global setRichiCounterFlag "setRichiCounterFlag" int, int
#cfunc global getRichiCounterFlag "getRichiCounterFlag" int

#func global calcWareme "calcWareme" int
#cfunc global getWareme "getWareme" int

#func global setDoukasen "setDoukasen" int, int
#cfunc global getDoukasen "getDoukasen" int

#cfunc global getShibari "getShibari" int

#func global setPao "setPao" int, int, int, int
#cfunc global getPao "getPao" int, int, int

#func global setDice "setDice" int, int, int, int
#cfunc global getDice "getDice" int, int, int

#func global addDeadWallLength "addDeadWallLength" int, int
#cfunc global getDeadWallLength "getDeadWallLength" int

#func global setWall "setWall" int, int, int, int
#cfunc global getWall "getWall" int, int, int

#cfunc global getRinshanExtension "getRinshanExtension" int

#func global setDrawPointer "setDrawPointer" int, int
#func global incDrawPointer "incDrawPointer" int
#cfunc global getDrawPointer "getDrawPointer" int

#func global setDoraPointer "setDoraPointer" int, int
#cfunc global getDoraPointer "getDoraPointer" int

#func global setRinshanPointer "setRinshanPointer" int, int
#func global decRinshanPointer "decRinshanPointer" int
#cfunc global getRinshanPointer "getRinshanPointer" int

#func global setHeavenHandFlag "setHeavenHandFlag" int, int
#cfunc global getHeavenHandFlag "getHeavenHandFlag" int

#func global setPreviousMeld "setPreviousMeld" int, int, int
#cfunc global getPreviousMeld "getPreviousMeld" int, int

#func global setDisconnectFlag "setDisconnectFlag" int, int, int
#cfunc global getDisconnectFlag "getDisconnectFlag" int, int

#func global setDoraFlag "setDoraFlag" int, int, int, int
#func global incDoraFlag "incDoraFlag" int, int, int
#cfunc global getDoraFlag "getDoraFlag" int, int, int

#func global setCurrentDiscard "setCurrentDiscard" int, int, int
#cfunc global getCurrentDiscard "getCurrentDiscard" int, int

#func global setTsumoAgariFlag "setTsumoAgariFlag" int, int
#cfunc global getTsumoAgariFlag "getTsumoAgariFlag" int

#func global setDeclarationFlag "setDeclarationFlag" int, int, int, int
#func global resetDeclarationFlag "resetDeclarationFlag" int
#cfunc global getDeclarationFlag "getDeclarationFlag" int, int, int

#func global setCurrentPlayer "setCurrentPlayer" int, int, int
#func global resetCurrentPlayer "resetCurrentPlayer" int
#cfunc global getCurrentPlayer "getCurrentPlayer" int, int

/* 局単位での初期化 */
#func global inittable "inittable" int

/* 初期化ルーチン */
#cfunc global initializeGameTable "initializeGameTable" int
#cfunc global initializeSandbox "initializeSandbox" int

#global
