/*=============================
 * HSP麻雀クライアントMiHaJong
 *  [状態管理用ストラクチャ]
 *=============================
 */

#module gameStruct \
PlayerScore, playerChip, SumaroFlag, YakitoriFlag, \
PlayerID, GameLength, GameRound, LoopRound, Honba, Deposit, AgariChain, LastAgariPlayer, \
Hand, Discard, Meld, MenzenFlag, HandStat, NumberOfQuads, RichiFlag, OpenRichiWait, \
FirstDrawFlag, DoujunFuriten, AgariHouki, FlowerFlag, NorthFlag, \
KangFlag, KangNum, RichiCounter, \
WaremePlayer, DoukasenPlayer, Dice1, Dice2, Dice1Direction, Dice2Direction, \
PaoFlag, Deck, DeadTiles, ExtraRinshan, ShibariFlag, \
DoraFlag, TilePointer, DoraPointer, RinshanPointer, TianHuFlag, \
PreviousMeld, ConnectionLost, \
CurrentPlayer, DeclarationFlag, TsumoAgariFlag, CurrentDiscard

#include "const.as"

/* メンバにアクセスするためのAPI */
#modfunc setScore int Player, int Digit, int value
	PlayerScore(Player, Digit) = value: return
#modfunc addScore int Player, int Digit, int value
	PlayerScore(Player, Digit) += value: return
#modcfunc getScore int Player, int Digit
	return PlayerScore(Player, Digit)
#modfunc exportScore array exportArray
	dim exportArray, NUM_OF_PLAYERS, NUM_OF_DIGIT_GROUPS
	repeat NUM_OF_PLAYERS*NUM_OF_DIGIT_GROUPS
		exportArray(cnt\NUM_OF_PLAYERS, cnt/NUM_OF_PLAYERS) = PlayerScore(cnt\NUM_OF_PLAYERS, cnt/NUM_OF_PLAYERS)
	loop
	return
#modfunc importScore array importArray
	repeat NUM_OF_PLAYERS*NUM_OF_DIGIT_GROUPS
		PlayerScore(cnt\NUM_OF_PLAYERS, cnt/NUM_OF_PLAYERS) = importArray(cnt\NUM_OF_PLAYERS, cnt/NUM_OF_PLAYERS)
	loop
	return

#modfunc setChip int Player, int value
	playerChip(Player) = value: return
#modfunc addChip int Player, int value
	playerChip(Player) += value: return
#modcfunc getChip int Player
	return playerChip(Player)

#modfunc setSumaroFlag int Player, int value
	SumaroFlag(Player) = value: return
#modcfunc getSumaroFlag int Player
	return SumaroFlag(Player)

#modfunc setYakitori int Player, int value
	YakitoriFlag(Player) = value: return
#modcfunc getYakitori int Player
	return YakitoriFlag(Player)

#modfunc setPlayer int value
	PlayerID = value: return
#modcfunc getPlayer
	return PlayerID

#modcfunc getGameLength
	return GameLength

#modfunc setRound int value
	GameRound = value: return
#modfunc incRound
	GameRound++: return
#modcfunc getRound
	return GameRound

#modfunc setRoundLoop int value
	LoopRound = value: return
#modfunc incRoundLoop
	LoopRound++: return
#modcfunc getRoundLoop
	return LoopRound

#modfunc setHonba int value
	Honba = value: return
#modfunc incHonba
	Honba++: return
#modcfunc getHonba
	return Honba

#modfunc setDeposit int value
	Deposit = value: return
#modfunc addDeposit int value
	Deposit += value: return
#modcfunc getDeposit
	return Deposit

#modfunc setAgariChain int value
	AgariChain = value: return
#modfunc incAgariChain
	AgariChain++: return
#modcfunc getAgariChain
	return AgariChain

#modfunc setLastAgariPlayer int value
	LastAgariPlayer = value: return
#modcfunc getLastAgariPlayer
	return LastAgariPlayer

#modfunc setHand int Page, int Index, int Player, int value
	Hand(Index, Player, Page) = value
	return
#modcfunc getHand int Page, int Index, int Player
	return Hand(Index, Player, Page)

#modfunc setDiscard int Page, int Index, int Player, int value
	Discard(Index, Player, Page) = value
	return
#modcfunc getDiscard int Page, int Index, int Player
	return Discard(Index, Player, Page)
#modcfunc DiscardPointer int Player
	return Discard(0, Player, DISCARD_TILECODE)
#modfunc DiscardPointerIncrement int Player
	Discard(0, Player, DISCARD_TILECODE)++
	return
#modfunc flagDiscard int Index, int Player, int value
	Discard(Index, Player, DISCARD_TILECODE) += value
	return

#modfunc setMeld int Page, int Index, int Player, int value
	Meld(Index, Player, Page) = value
	return
#modcfunc getMeld int Page, int Index, int Player
	return Meld(Index, Player, Page)
#modcfunc MeldPointer int Player
	return Meld(0, Player, MELD_TILECODE)
#modfunc MeldPointerIncrement int Player
	Meld(0, Player, MELD_TILECODE)++
	return
#modfunc MeldPointerDecrement int Player
	Meld(0, Player, MELD_TILECODE)--
	return
#modfunc flagMeld int Page, int Index, int Player, int value
	Meld(Index, Player, Page) += value
	return

#modfunc setMenzen int Player, int value
	MenzenFlag(Player) = value
	return
#modcfunc getMenzen int Player
	return MenzenFlag(Player)

#modfunc setHandStat int Player, int value
	HandStat(Player) = value
	return
#modcfunc getHandStat int Player
	return HandStat(Player)

#modfunc setNumberOfQuads int Player, int value
	NumberOfQuads(Player) = value
	return
#modfunc incNumberOfQuads int Player
	NumberOfQuads(Player)++
	return
#modcfunc getNumberOfQuads int Player
	return NumberOfQuads(Player)

#modfunc setRichiFlag int Page, int Player, int value
	RichiFlag(Player, Page) = value
	return
#modcfunc getRichiFlag int Page, int Player
	return RichiFlag(Player, Page)

#modfunc setOpenWait int Tile, int value
	OpenRichiWait(Tile) = value
	return
#modcfunc getOpenWait int Tile
	return OpenRichiWait(Tile)

#modfunc setFirstDrawFlag int Player, int value
	FirstDrawFlag(Player) = value
	return
#modcfunc getFirstDrawFlag int Player
	return FirstDrawFlag(Player)

#modfunc setDoujunFuriten int Player, int value
	DoujunFuriten(Player) = value
	return
#modcfunc getDoujunFuriten int Player
	return DoujunFuriten(Player)

#modfunc setAgariHouki int Player, int value
	AgariHouki(Player) = value
	return
#modcfunc getAgariHouki int Player
	return AgariHouki(Player)

#modfunc putFlowerFlag int Player, int value
	FlowerFlag(Player) = value
	return
#modfunc setFlowerFlag int Player, int value
	FlowerFlag(Player) |= value
	return
#modcfunc getFlowerFlag int Player
	return FlowerFlag(Player)

#modfunc setNorthFlag int Player, int value
	NorthFlag(Player) = value
	return
#modfunc addNorthFlag int Player
	NorthFlag(Player)++
	return
#modcfunc getNorthFlag int Player
	return NorthFlag(Player)

#modfunc setKangFlag int Page, int value
	KangFlag(Page) = value
	return
#modfunc incKangFlag int Page
	KangFlag(Page)++
	return
#modcfunc getKangFlag int Page
	return KangFlag(Page)

#modfunc setTotalKang int value
	KangNum = value
	return
#modfunc incTotalKang
	KangNum++
	return
#modcfunc getTotalKang
	return KangNum

#modfunc setRichiCounterFlag int value
	RichiCounter = value
	return
#modcfunc getRichiCounterFlag
	return RichiCounter

#modfunc calcWareme
#ifdef ALLSANMA
	if ((getRule(RULE_WAREME) != 0)||(getRule(RULE_KAIMENKAZE) != 0)) {
		WaremePlayer = ((GameRound-(GameRound/4))+24+(Dice1+Dice2)-1)\3
#ifdef SANMA4
		WaremePlayer = ((0)+24+(Dice1+Dice2)-1)\3
		switch (GameRound\NUM_OF_PLAYERS)
			case 0: tobePlayed = 0, 1, 2: swbreak
			case 1: tobePlayed = 1, 2, 3: swbreak
			case 2: tobePlayed = 2, 3, 0: swbreak
			case 3: tobePlayed = 3, 0, 1: swbreak
		swend
		WaremePlayer = tobePlayed(WaremePlayer)
#endif
	}
#else
	if ((getRule(RULE_WAREME) != 0)||(getRule(RULE_KAIMENKAZE) != 0)) {
		WaremePlayer = ((GameRound\4)+32+(Dice1+Dice2)-1)\4
	}
#endif
	return
#modcfunc getWareme
	return WaremePlayer

#modfunc setDoukasen int value
	DoukasenPlayer = value
	return
#modcfunc getDoukasen
	return DoukasenPlayer

#modcfunc getShibari
	return ShibariFlag

#modfunc setPao int Page, int Yaku, int value
	PaoFlag(Yaku, Page) = value
	return
#modcfunc getPao int Page, int Yaku
	return PaoFlag(Yaku, Page)

#modfunc setDice int ID, int Direction, int value
	switch ID+Direction*2
		case 0: Dice1 = value: swbreak
		case 1: Dice2 = value: swbreak
		case 2: Dice1Direction = value: swbreak
		case 3: Dice2Direction = value: swbreak
	swend
	return
#modcfunc getDice int ID, int Direction
	switch ID+Direction*2
		case 0: return Dice1: swbreak
		case 1: return Dice2: swbreak
		case 2: return Dice1Direction: swbreak
		case 3: return Dice2Direction: swbreak
	swend
	return 0x7fffffff

#modfunc addDeadWallLength int value
	DeadTiles += value
	return
#modcfunc getDeadWallLength
	return DeadTiles

#modfunc setWall int Page, int Index, int value
	Deck(Index, Page) = value
	return
#modcfunc getWall int Page, int Index
	return Deck(Index, Page)

#modcfunc getRinshanExtension
	return ExtraRinshan

#modfunc setDrawPointer int value
	TilePointer = value: return
#modfunc incDrawPointer
	TilePointer++: return
#modcfunc getDrawPointer
	return TilePointer

#modfunc setDoraPointer int value
	DoraPointer = value: return
#modcfunc getDoraPointer
	return DoraPointer

#modfunc setRinshanPointer int value
	RinshanPointer = value: return
#modfunc decRinshanPointer
	RinshanPointer--: return
#modcfunc getRinshanPointer
	return RinshanPointer

#modfunc setHeavenHandFlag int value
	TianHuFlag = value
	return
#modcfunc getHeavenHandFlag
	return TianHuFlag

#modfunc setPreviousMeld int Page, int value
	PreviousMeld(Page) = value
	return
#modcfunc getPreviousMeld int Page
	return PreviousMeld(Page)

#modfunc setDisconnectFlag int Player, int value
	ConnectionLost(Player) = value
	return
#modcfunc getDisconnectFlag int Player
	return ConnectionLost(Player)

#modfunc setDoraFlag int Page, int Tile, int value
	DoraFlag(Tile, Page) = value
	return
#modfunc incDoraFlag int Page, int Tile
	DoraFlag(Tile, Page)++
	return
#modcfunc getDoraFlag int Page, int Tile
	return DoraFlag(Tile, Page)

#modfunc setCurrentDiscard int Page, int value
	CurrentDiscard(Page) = value
	return
#modcfunc getCurrentDiscard int Page
	return CurrentDiscard(Page)

#modfunc setTsumoAgariFlag int value
	TsumoAgariFlag = value
	return
#modcfunc getTsumoAgariFlag
	return TsumoAgariFlag

#modfunc setDeclarationFlag int Page, int Player, int value
	DeclarationFlag(Player, Page) = value
	return
#modfunc resetDeclarationFlag
	dim DeclarationFlag, NUM_OF_PLAYERS, DECLARATIONFLAG_PAGES
	return
#modcfunc getDeclarationFlag int Page, int Player
	return DeclarationFlag(Player, Page)

#modfunc setCurrentPlayer int Page, int value
	CurrentPlayer(Page) = value
	return
#modfunc resetCurrentPlayer
	dim CurrentPlayer, CURRENTPLAYER_PAGES
	return
#modcfunc getCurrentPlayer int Page
	return CurrentPlayer(Page)


/* 局単位での初期化 */
#modfunc inittable
	dim ConnectionLost, NUM_OF_PLAYERS // 回線切断による和了り放棄
	if (((Honba >= 5)&&(getRule(RULE_RYANSHIBA) == 1))||((Honba >= 4)&&(getRule(RULE_RYANSHIBA) == 2))) {
		ShibariFlag = 1 //二飜縛り
	} else {
		ShibariFlag = 0
	}
	// 包フラグ（-1…なし、0～3…該当プレイヤー）
	dim PaoFlag, PAO_YAKU_PAGES, PAO_PLAYER_PAGES
	repeat PAO_YAKU_PAGES*PAO_PLAYER_PAGES
		PaoFlag(cnt\PAO_YAKU_PAGES, cnt/PAO_YAKU_PAGES) = -1
	loop
#ifdef ALLSANMA
	DeadTiles = 14 // 王牌の数
	dim Deck, 108, WALL_PAGES // 壁牌の配列
	if (getRule(RULE_FLOWER_TILES) != 0) {
		ExtraRinshan = 4
	} else {
		ExtraRinshan = 0
	}
#else
	if (getRule(RULE_FLOWER_TILES) != 0) {
		if (getRule(RULE_FLOWER_TILES) == 3) {
			DeadTiles = 22 // 王牌の数(花牌を入れる時は特別に２２枚残しとする)
			dim Deck, 144, WALL_PAGES // 壁牌の配列
		} else {
			DeadTiles = 18 // 王牌の数
			dim Deck, 140, WALL_PAGES // 壁牌の配列
		}
	} else {
		DeadTiles = 14 // 王牌の数
		dim Deck, 136, WALL_PAGES // 壁牌の配列
	}
#endif
	dim Hand, NUM_OF_TILES_IN_HAND, NUM_OF_PLAYERS, HAND_PAGES // 手牌の配列(４人分)
	dim Discard, SIZE_OF_DISCARD_BUFFER, NUM_OF_PLAYERS, DISCARD_PAGES // 捨牌の配列(４人分)
	dim MenzenFlag, NUM_OF_PLAYERS // 門前フラグ
	dim HandStat, NUM_OF_PLAYERS // 手牌の状態（立てる・見せる・伏せる）
	dim Meld, SIZE_OF_MELD_BUFFER, NUM_OF_PLAYERS, MELD_PAGES // 鳴き面子を格納
	dim NumberOfQuads, NUM_OF_PLAYERS // 槓子の数（四槓流局、三槓子、四槓子などの判定に使う）
	dim RichiFlag, NUM_OF_PLAYERS, RICHI_PAGES // リーチしているかどうか（０…不聴・ダマ・副露、１…リーチ、２…リー即圏内、３…ダブリー、４…ダブリー即圏内）
	dim OpenRichiWait, TILE_NONFLOWER_MAX // プンリーの待ち牌(ＣＯＭに意図的な放銃を起こさせないために使用)
	dim FirstDrawFlag, NUM_OF_PLAYERS // １巡目である（地和、ダブル立直の判定に使う）
	dim DoujunFuriten, NUM_OF_PLAYERS // 同順振聴である
	dim AgariHouki, NUM_OF_PLAYERS // 和了り放棄の罰則中かどうか
	dim FlowerFlag, NUM_OF_PLAYERS // 晒している花牌を格納するフラグ
#ifdef SANMAX
	dim NorthFlag, NUM_OF_PLAYERS // 晒している北風牌を格納するフラグ
#endif
	dim KangFlag, KANG_PAGES // 嶺上開花；連開花と槓振り；頭槓和；搶槓の判定に使う
	KangNum = 0 // 四槓流局、四槓子などの判定に使う
	RichiCounter = 0 // リーチをカウンター(宣言牌をロン)
	WaremePlayer = -1 // 割れ目の位置(-1で割れ目なし)
	DoukasenPlayer = -1 // 導火線の位置(-1で導火線なし)
	DoraPointer = 999
	Dice1 = 0: Dice2 = 0
	Dice1direction = 0: Dice2direction = 0
	TilePointer = 0 // ツモ牌のポインタ
#ifdef ALLSANMA
	RinshanPointer = 107 // 嶺上牌のポインタ
#else
	switch getRule(RULE_FLOWER_TILES) // 嶺上牌のポインタ
		case 1: case 2:
			RinshanPointer = 139
			swbreak
		case 3:
			RinshanPointer = 143
			swbreak
		default:
			RinshanPointer = 135
	swend
#endif
	TianHuFlag = 1 // 親の第一打牌がまだ（天和の判定などに使う）
	repeat NUM_OF_PLAYERS: MenzenFlag(cnt) = 1: loop
	repeat NUM_OF_PLAYERS: FirstDrawFlag(cnt) = 1: loop
	dim PreviousMeld, PREVMELD_PAGES // 先ほど鳴いた牌（喰い替えの判定に使う）
	repeat PREVMELD_PAGES: PreviousMeld(cnt) = -999: loop
	repeat NUM_OF_PLAYERS: HandStat(cnt) = 0: loop
	dim DoraFlag, TILE_NONFLOWER_MAX, DORA_PAGES // ドラ判定の配列
	
	TsumoAgariFlag = 0
	resetDeclarationFlag thismod
	dim CurrentDiscard, CURRENTDISCARD_PAGES
	resetCurrentPlayer thismod
	return

/* 初期化ルーチン */
#modinit
	dim PlayerScore, NUM_OF_PLAYERS, NUM_OF_DIGIT_GROUPS
#ifdef SANMAT
	repeat NUM_OF_ACTUAL_PLAYERS
		switch getRule(RULE_STARTING_POINT)
			case 0: PlayerScore(cnt,0) = 350: swbreak
			case 1: PlayerScore(cnt,0) = 400: swbreak
			case 2: PlayerScore(cnt,0) = 450: swbreak
			case 3: PlayerScore(cnt,0) = 500: swbreak
			case 4: PlayerScore(cnt,0) = 250: swbreak
			case 5: PlayerScore(cnt,0) = 300: swbreak
		swend
	loop
	switch getRule(RULE_GAME_LENGTH)
		case 0: GameLength = 6: swbreak
		case 1: case 7: GameLength = 2: swbreak
		case 2: case 3: GameLength = 14: swbreak
		case 4: GameLength = 0: swbreak
		case 5: GameLength = 18: swbreak
		case 6: GameLength = 10: swbreak
	swend
#else
	repeat NUM_OF_ACTUAL_PLAYERS
		switch getRule(RULE_STARTING_POINT)
			case 0: PlayerScore(cnt,0) = 250: swbreak
			case 1: PlayerScore(cnt,0) = 270: swbreak
			case 2: PlayerScore(cnt,0) = 300: swbreak
			case 3: PlayerScore(cnt,0) = 350: swbreak
			case 4: PlayerScore(cnt,0) = 400: swbreak
			case 5: PlayerScore(cnt,0) = 200: swbreak
		swend
	loop
	switch getRule(RULE_GAME_LENGTH)
		case 0: GameLength = 7: swbreak
		case 1: case 7: GameLength = 3: swbreak
		case 2: case 3: GameLength = 15: swbreak
		case 4: GameLength = 0: swbreak
		case 5: GameLength = 19: swbreak
		case 6: GameLength = 11: swbreak
	swend
#endif
	GameRound = 0: Honba = 0
	PlayerID = 0
	Deposit = 0 // 供託点棒（立直棒）の数
	LoopRound = 0 // 帰り東以降、フラグがセットされる
	AgariChain = 0 // 八連荘の判定に使うカウンタ
	LastAgariPlayer = -1 // 八連荘の判定に使う
	dim SumaroFlag, NUM_OF_PLAYERS // 四馬路解禁フラグ
	dim YakitoriFlag, NUM_OF_PLAYERS // 焼き鳥フラグ
	if (getRule(RULE_YAKITORI) != 0) {repeat NUM_OF_PLAYERS: YakitoriFlag(cnt) = 1: loop}
	dim playerChip, NUM_OF_PLAYERS // チップの収支
	inittable thismod
	return
#global
