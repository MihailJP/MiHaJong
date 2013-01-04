#include "prepare.h"

#define settile(tilecode, pos) {tilepos[tilecode] = pos; for (unsigned int i = 0; i < 4u; ++i) {gameStat->Deck[pos++].tile = tilecode;}}
inline unsigned int inittiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 牌を並べる
	unsigned int p = 0; // 牌の位置ID
	if (chkGameType(gameStat, SanmaX)) {
		settile(CharacterOne, p); // 萬子
		settile(CharacterNine, p); // 萬子
	} else {
		for (unsigned int k = 1u; k <= 9u; ++k)
			settile((tileCode)(TILE_SUIT_CHARACTERS + k), p); // 萬子
	}
	for (unsigned int k = 1u; k <= 9u; ++k)
		settile((tileCode)(TILE_SUIT_CIRCLES + k), p); // 筒子
	for (unsigned int k = 1u; k <= 9u; ++k)
		settile((tileCode)(TILE_SUIT_BAMBOOS + k), p); // 索子
	if (!chkGameType(gameStat, SanmaS)) {
		for (unsigned int k = 1u; k <= 7u; ++k)
			settile((tileCode)(TILE_SUIT_HONORS + k), p); // 字牌
		if (RuleData::chkRule("flower_tiles", "seasons") || RuleData::chkRule("flower_tiles", "8tiles")) {
			gameStat->Deck[p++].tile = Spring; gameStat->Deck[p++].tile = Summer;
			gameStat->Deck[p++].tile = Autumn; gameStat->Deck[p++].tile = Winter;
		}
		if (RuleData::chkRule("flower_tiles", "flowers") || RuleData::chkRule("flower_tiles", "8tiles")) {
			gameStat->Deck[p++].tile = Plum; gameStat->Deck[p++].tile = Orchid;
			gameStat->Deck[p++].tile = Chrysanthemum; gameStat->Deck[p++].tile = Bamboo;
		}
	}
	return p;
}

inline void redtiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 赤ドラを設定する
	{
		const char tileRules[9][16] = {
			"red_one", "red_two", "red_three", "red_four", "red_five",
			"red_six", "red_seven", "red_eight", "red_nine",
		};
		for (unsigned int i = 0; i < 9; ++i) { // 数牌の赤ドラ
			if (RuleData::chkRule(tileRules[i], "character_suit") || // 萬子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "circle_suit") || // 筒子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CIRCLES + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "bamboo_suit") || // 索子
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_BAMBOOS + i + 1]].red = AkaDora;
		}
	}
	// 5のドラは特殊
	if (chkGameType(gameStat, SanmaX)) { // 三麻
		if (RuleData::chkRule("red_five", "2tiles")) {
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "4tiles")) {
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive] + 1].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "2_circle_tiles")) {
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
		}
	} else { // 四麻・数牌三麻
		if (RuleData::chkRule("red_five", "3tiles")) {
			gameStat->Deck[tilepos[CharacterFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "4tiles")) {
			gameStat->Deck[tilepos[CharacterFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "6tiles")) {
			gameStat->Deck[tilepos[CharacterFive]].red = AkaDora;
			gameStat->Deck[tilepos[CharacterFive] + 1].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive] + 1].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "2_circle_tiles")) {
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
		}
	}
	{
		const char tileRules[2][16] = {"red_west", "red_north",};
		const tileCode tileCodes[2] = {WestWind, NorthWind,};
		for (unsigned int i = 0; i < 2; ++i) { // 字牌の赤ドラ
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles") ||
				RuleData::chkRule(tileRules[i], "2tiles") || RuleData::chkRule(tileRules[i], "1tile"))
				gameStat->Deck[tilepos[tileCodes[i]]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles") ||
				RuleData::chkRule(tileRules[i], "2tiles"))
				gameStat->Deck[tilepos[tileCodes[i] + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles"))
				gameStat->Deck[tilepos[tileCodes[i] + 2]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles"))
				gameStat->Deck[tilepos[tileCodes[i] + 3]].red = AkaDora;
		}
	}
	{
		if (RuleData::chkRule("haku_potchi", "1tile_dora")) // オールマイティー扱いはせず、赤ドラと同じ扱い
			gameStat->Deck[tilepos[WhiteDragon]].red = AkaDora; // 白ポッチ
	}
}

inline void bluetiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 青ドラを設定する
	{
		const char tileRules[3][16] = {"blue_one", "blue_five", "blue_nine",};
		const unsigned int tileNum[3] = {1, 5, 9,};
		for (unsigned int i = 0; i < 3; ++i) { // 数牌の青ドラ
			if (RuleData::chkRule(tileRules[i], "character_suit") || // 萬子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + tileNum[i]] + 3].red = AoDora;
			if (RuleData::chkRule(tileRules[i], "circle_suit") || // 筒子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CIRCLES + tileNum[i]] + 3].red = AoDora;
			if (RuleData::chkRule(tileRules[i], "bamboo_suit") || // 索子
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_BAMBOOS + tileNum[i]] + 3].red = AoDora;
		}
	}
	{
		if (RuleData::chkRule("blue_lvfa", "yes"))
			gameStat->Deck[tilepos[GreenDragon] + 3].red = AoDora;
	}
}

inline void shuffletiles(GameTable* const gameStat, UInt8ByTile& tilepos, unsigned int tiles) { // 洗牌する
	if (EnvTable::Instantiate()->GameMode != EnvTable::Client) {
		for (unsigned int i = 0; i < tiles; ++i) { // 洗牌する
			unsigned int tmppos = i + RndNum::rnd(tiles - i);
			TILE tmptile = gameStat->Deck[tmppos];
			gameStat->Deck[tmppos] = gameStat->Deck[i];
			gameStat->Deck[i] = tmptile;
		}
	}
}

__declspec(dllexport) void shuffle(GameTable* const gameStat) { // 牌をバッファに並べて、洗牌
	UInt8ByTile tilepos;
	unsigned int tiles = inittiles(gameStat, tilepos);
	redtiles(gameStat, tilepos);
	bluetiles(gameStat, tilepos);
	shuffletiles(gameStat, tilepos, tiles);
}

#define nagatadora(tilecode) {++gameStat->DoraFlag.Omote[tilecode]; haifu::haifurecdora(tilecode);}
inline void DoraAdding(GameTable* const gameStat) {
	setdora(gameStat, 0); // 表ドラを設定する
	if (RuleData::chkRuleApplied("uradora"))
		setdora(gameStat, 1); // 裏ドラを設定する
	haifu::haifurecdorap();
}

__declspec(dllexport) void initdora(GameTable* const gameStat) { // ドラの設定
	if (chkGameType(gameStat, AllSanma))
		gameStat->DoraPointer = 102 - gameStat->ExtraRinshan; // ドラ表示牌のポインタ
	else gameStat->DoraPointer = 130; // ドラ表示牌のポインタ
	if (RuleData::chkRuleApplied("nagatacho")) { // 永田町ルール
		nagatadora(BambooSeven); // 七索は常にドラ
		unsigned int dice = gameStat->Dice[0].Number + gameStat->Dice[1].Number;
		if (dice <= 8) { // 2～8はその数牌がドラ　三麻では萬子がないので別処理
			if (!chkGameType(gameStat, SanmaX)) nagatadora(TILE_SUIT_CHARACTERS + dice);
			nagatadora(TILE_SUIT_CIRCLES + dice); nagatadora(TILE_SUIT_BAMBOOS + dice);
		} else if (dice == 9) { // 9はそのまま9がドラ
			nagatadora(CharacterNine); nagatadora(CircleNine); nagatadora(BambooNine);
		} else if (dice == 10) { // 10は三元牌がドラ
			nagatadora(WhiteDragon); nagatadora(GreenDragon); nagatadora(RedDragon);
		} else if (dice == 11) { // 11は数牌の1がドラ
			nagatadora(CharacterOne); nagatadora(CircleOne); nagatadora(BambooOne);
		} else if (dice == 12) { // 12は風牌全てドラ
			nagatadora(EastWind); nagatadora(SouthWind); nagatadora(WestWind); nagatadora(NorthWind);
		}
		haifu::haifurecdorap();
	}
	DoraAdding(gameStat);
	if (RuleData::chkRule("dora_twice", "yes") ||
		(RuleData::chkRule("dora_twice", "only_when_doublets") && (gameStat->Dice[0].Number == gameStat->Dice[1].Number))) {
			gameStat->DoraPointer -= 2; /*ドラドラ卓*/
			DoraAdding(gameStat);
	}
}

// -------------------------------------------------------------------------

void SeatShuffler::shuffleSeat () {
	if (EnvTable::Instantiate()->GameMode == EnvTable::Server)
		for (PLAYER_ID i = 0; i < PLAYERS; ++i)
			if (EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag > 1)
				mihajong_socket::listen(SOCK_CHAT - 1 + EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag,
				PORT_CHAT - 1 + EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag);
	// 退避
	InfoByPlayer<EnvTable::PlayerLabel> TmpPlayerDat;
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		TmpPlayerDat[i].PlayerName = EnvTable::Instantiate()->PlayerDat[i].PlayerName;
		TmpPlayerDat[i].RemotePlayerFlag = EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag;
	}
	std::vector<PLAYER_ID> TmpPosition;
	for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; i++) TmpPosition.push_back(i);
	// 場決め
	if (EnvTable::Instantiate()->GameMode != EnvTable::Client) {
		// 場決め処理
		std::random_shuffle(TmpPosition.begin(), TmpPosition.end(),
			[] (unsigned max) {return RndNum::rnd(max);});
		// サーバーであれば結果を送信
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server)
			for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; i++)
				mihajong_socket::server::send(TmpPosition[i]);
	} else {
		// クライアントであれば受信する
		for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; i++) {
			int receivedByte;
			while ((receivedByte = mihajong_socket::getc(0)) == -1) Sleep(0); // 受信待ち
			TmpPosition[i] = receivedByte;
		}
	}
	// シャッフル結果を書き込み
	for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; i++) {
		EnvTable::Instantiate()->PlayerDat[TmpPosition[i]].PlayerName = TmpPlayerDat[i].PlayerName;
		EnvTable::Instantiate()->PlayerDat[TmpPosition[i]].RemotePlayerFlag = TmpPlayerDat[i].RemotePlayerFlag;
		posarry[i] = TmpPosition[i];
	}

	// リモートとしてマーク
	/*PLAYER_ID tmpPlayer = TmpPosition[ClientNumber];
	if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
		for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; i++)
			EnvTable::Instantiate()->PlayerDat[TmpPosition[i]].RemotePlayerFlag =
			(i != tmpPlayer) ? 1 : 0;*/

	{
		CodeConv::tostringstream o;
		o << _T("ClientNumber [") << (int)ClientNumber << _T("]");
		debug(o.str().c_str());
	}
	{
		CodeConv::tostringstream o; o << _T("TmpPosition ");
		for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; i++)
			o << (i ? _T(" ") : _T("[")) << (int)TmpPosition[i];
		o << _T("]"); debug(o.str().c_str());
	}
	{
		CodeConv::tostringstream o; o << _T("Remote? ");
		for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; i++)
			o << (i ? _T(" ") : _T("[")) << (int)EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag;
		o << _T("]"); debug(o.str().c_str());
	}
	{
		CodeConv::tostringstream o; o << _T("Name ");
		for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; i++)
			o << (i ? _T(" [") : _T("[")) << EnvTable::Instantiate()->PlayerDat[i].PlayerName << _T("]");
		debug(o.str().c_str());
	}

	return;
}
unsigned SeatShuffler::ClientNumber;
int* SeatShuffler::posarry = nullptr;
std::array<int, PLAYERS> SeatShuffler::shuffle (unsigned cNumber) {
	std::array<int, PLAYERS> posArry;
	posarry = &posArry[0];
	ClientNumber = cNumber;
	shuffleSeat();
	return posArry;
}

// -------------------------------------------------------------------------

/* 半荘の初期化 */
void gameinit(GameTable* gameStat, gameTypeID gameType, const std::string& ServerAddress, const std::array<int, 4>& PositionArray, unsigned ClientNumber) {
	/* TODO: チャットウィンドウ初期化 closingchat */
	gameStat = initializeGameTable(gameType);
	gameStat->PlayerID = PositionArray[ClientNumber];
	/* TODO: 多分これは不要。詳細を確認すること dim MachihaiCount, TILE_NONFLOWER_STRICT_MAX+1 */ // 起家でのバグ防止用に仮初期化
	haifu::haifubufinit();
	/* TODO: statmes "" */ // 情報窓への表示
	chat::initchat(ServerAddress.c_str(), ClientNumber);
	yaku::yakuCalculator::init(); // 役カタログの初期化
	aiscript::initscript(); // AIの初期化
	return;
}

/* 卓の初期化 */
namespace {
	void init_ai(const GameTable* const gameStat) {
		aiscript::initephemeral(); // AIのephemeralテーブルを初期化
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			aiscript::initcall(makesandBox(gameStat, i), i);
	}
	void statsync(GameTable* const gameStat, std::uint8_t serverMsg, std::function<bool (GameTable* const, std::uint8_t)> f) {
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
			mihajong_socket::server::send(serverMsg);
		}
		else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
			volatile int ClientReceived = 0;
			int ReceivedMsg = 1023;
			while (true) {
				mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
				if (ClientReceived == 1)
					if (f(gameStat, ReceivedMsg))
						break;
				Sleep(1);
			}
		}
	}
	void syncTableStat(GameTable* const gameStat) {
		// 場風のデータを送信。とりあえず１６周目の北四局まで対応
		statsync(gameStat, gameStat->LoopRound * roundLoopRate() + gameStat->GameRound,
			[](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
				gameStat->LoopRound = ReceivedMsg / roundLoopRate();
				gameStat->GameRound = ReceivedMsg % roundLoopRate();
				return true;
			});
		// 積み棒のデータを送信。とりあえず２５５本場まで対応
		statsync(gameStat, gameStat->Honba,
			[](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
				gameStat->Honba = ReceivedMsg;
				return true;
			});
		// 供託本数のデータを送信。とりあえず２５５０００点まで対応
		statsync(gameStat, gameStat->Deposit,
			[](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
				gameStat->Deposit = ReceivedMsg;
				return true;
			});
		// 八連荘の判定に使うデータを送信
		statsync(gameStat, ((gameStat->LastAgariPlayer + 1) << 4) + gameStat->AgariChain,
			[](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
				gameStat->LastAgariPlayer = (ReceivedMsg + 1) / 16 - 1;
				gameStat->AgariChain = (ReceivedMsg + 1) % 16 - 1;
				return true;
			});
		// 持ち点のデータを送信。符号込みで33バイトずつ・スモールエンディアン
		for (PLAYER_ID player = 0; player < PLAYERS; player++) {
			for (int i = 0; i < DIGIT_GROUPS; i++)
				for (int j = 0; j < 4; j++)
					statsync(gameStat, (gameStat->Player[player].PlayerScore.digitGroup[i] >> (j * 8)) & 0xff,
						[player, i, j](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
							if (!j) gameStat->Player[player].PlayerScore.digitGroup[i] = 0;
							gameStat->Player[player].PlayerScore.digitGroup[i] |= (int)ReceivedMsg << (j * 8);
							return true;
						});
			statsync(gameStat, (gameStat->Player[player].PlayerScore < LargeNum::fromInt(0)) ? 0x01 : 0x00,
				[player](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
					if (ReceivedMsg) gameStat->Player[player].PlayerScore *= -1;
					return true;
				});
		}
		// 焼鳥のデータを送信
		statsync(gameStat, (gameStat->Player[0].YakitoriFlag ? 8 : 0) + (gameStat->Player[1].YakitoriFlag ? 4 : 0) + (gameStat->Player[2].YakitoriFlag ? 2 : 0) + (gameStat->Player[3].YakitoriFlag ? 1 : 0),
			[](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
				gameStat->Player[0].YakitoriFlag = ReceivedMsg & 0x08;
				gameStat->Player[1].YakitoriFlag = ReceivedMsg & 0x04;
				gameStat->Player[2].YakitoriFlag = ReceivedMsg & 0x02;
				gameStat->Player[3].YakitoriFlag = ReceivedMsg & 0x01;
				return true;
			});
		// チップのデータを送信。一応±１２７まで対応
		for (PLAYER_ID player = 0; player < PLAYERS; player++)
			/* Excess-128 */
			statsync(gameStat, gameStat->Player[player].playerChip + 128,
				[player](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
					gameStat->Player[player].playerChip = (int)ReceivedMsg - 128;
					return true;
				});
		// 四馬路解禁フラグを送信
		statsync(gameStat, (gameStat->Player[0].SumaroFlag ? 8 : 0) + (gameStat->Player[1].SumaroFlag ? 4 : 0) + (gameStat->Player[2].SumaroFlag ? 2 : 0) + (gameStat->Player[3].SumaroFlag ? 1 : 0),
			[](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
				gameStat->Player[0].SumaroFlag = ReceivedMsg & 0x08;
				gameStat->Player[1].SumaroFlag = ReceivedMsg & 0x04;
				gameStat->Player[2].SumaroFlag = ReceivedMsg & 0x02;
				gameStat->Player[3].SumaroFlag = ReceivedMsg & 0x01;
				return true;
			});
	}
	void choosebgm(const GameTable* const gameStat) { // BGMを鳴らす
		if (((gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) == gameStat->GameLength) &&
			(gameStat->GameLength > 0))
			sound::Play(sound::IDs::musFinal); // オーラスだけ特別
		else if ((gameStat->Honba >= 5) && RuleData::chkRule("ryanshiba", "from_5honba"))
			sound::Play(sound::IDs::musShibari); // リャンシバ時専用BGM
		else if ((gameStat->Honba >= 4) && RuleData::chkRule("ryanshiba", "from_4honba"))
			sound::Play(sound::IDs::musShibari); // リャンシバ時専用BGM
		else
			sound::Play(sound::IDs::BgmStart + gameStat->GameRound);
	}
	void screen(const GameTable* const gameStat) {
		CodeConv::tstring tmpStatus;
		if (gameStat->Honba) {
			CodeConv::tostringstream o;
			o << roundName(gameStat->GameRound, gameStat) << gameStat->Honba << _T("本場");
			tmpStatus = o.str();
		} else {
			tmpStatus = roundName(gameStat->GameRound, gameStat);
		}
		chat::appendchat((CodeConv::tstring(_T("-------------\n*** ")) + tmpStatus + CodeConv::tstring(_T("\n"))).c_str());
		/* TODO: これを移植する。画面表示関係
		if (GetWatchModeFlag(GameEnv) == 1) {
			tmpStatus += " Watch Mode"
		} else {
			switch playerWind(getPlayer(GameStat), getRound(GameStat))
				case PLAYER_EAST: tmpStatus += " あなたが親です": swbreak
				case PLAYER_SOUTH: tmpStatus += " あなたは南家です": swbreak
				case PLAYER_WEST: tmpStatus += " あなたは西家です": swbreak
	#ifdef SANMA4
				case PLAYER_NORTH: tmpStatus += " あなたは抜け番です": swbreak
	#else
				case PLAYER_NORTH: tmpStatus += " あなたは北家です": swbreak
	#endif
			swend
		}
		statmes tmpStatus
		await 100
		setCenterTitle "" // 画面中央に大書する文字列
		*/
		for (PLAYER_ID i = 0; i < PLAYERS; ++i)
			mihajong_graphic::calltext::setCall(i, mihajong_graphic::calltext::None); /* 発声文字列を消去 */
		EnvTable* env = EnvTable::Instantiate();
		if (gameStat->LoopRound % 2 == 0) {
			switch (gameStat->GameRound / PLAYERS) {
			case 0:
				if (gameStat->LoopRound == 0) {
					env->bgColorR =   0; env->bgColorG = 160; env->bgColorB =   0;
				} else {
					env->bgColorR =  80; env->bgColorG = 160; env->bgColorB =   0;
				}
				break;
			case 1:
				if (gameStat->LoopRound == 0) {
					env->bgColorR =   0; env->bgColorG = 160; env->bgColorB = 160;
				} else {
					env->bgColorR = 160; env->bgColorG =  80; env->bgColorB =   0;
				}
				break;
			case 2:
				env->bgColorR =   0; env->bgColorG =  80; env->bgColorB = 160;
				break;
			case 3:
				env->bgColorR = 120; env->bgColorG =   0; env->bgColorB = 160;
				break;
			}
		} else {
			switch (gameStat->GameRound / PLAYERS) {
			case 0:
				env->bgColorR = 160; env->bgColorG = 160; env->bgColorB = 160;
				break;
			case 1:
				env->bgColorR = 160; env->bgColorG = 160; env->bgColorB =   0;
				break;
			case 2:
				env->bgColorR = 160; env->bgColorG =   0; env->bgColorB =   0;
				break;
			case 3:
				env->bgColorR =  80; env->bgColorG =  80; env->bgColorB =  80;
				break;
			}
		}
		switch (gameStat->GameRound / PLAYERS) {
		case 4:
			env->bgColorR = 120; env->bgColorG = 120; env->bgColorB = 160;
			break;
		case 5:
			env->bgColorR = 120; env->bgColorG = 160; env->bgColorB = 120;
			break;
		case 6:
			env->bgColorR = 160; env->bgColorG = 120; env->bgColorB = 120;
			break;
		}
		/* TODO: これ
		if ((getRoundLoop(GameStat)*roundLoopRate()+getRound(GameStat)) >= (getGameLength(GameStat)+1)) {
			setCenterTitle "延長戦"
		} else: if ((getRoundLoop(GameStat)*roundLoopRate()+getRound(GameStat)) == getGameLength(GameStat)) {
			setCenterTitle "オーラス"
		} else {
			setCenterTitle roundName(getRound(GameStat))
		}
		*/
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Transit(mihajong_graphic::sceneGameTable);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneBeginning);
	}
	void tileshuffle(GameTable* const gameStat) {
		shuffle(gameStat); unsigned tmpNumberOfTiles;
		if (chkGameType(gameStat, AllSanma))
			tmpNumberOfTiles = 108;
		else if (RuleData::chkRule("flower_tiles", "no"))
			tmpNumberOfTiles = 136;
		else if (RuleData::chkRule("flower_tiles", "8tiles"))
			tmpNumberOfTiles = 144;
		else
			tmpNumberOfTiles = 140;
		for (unsigned i = 0; i < tmpNumberOfTiles; i++) // サーバーの場合、牌山のデータを送信
			statsync(gameStat, gameStat->Deck[i].tile + (gameStat->Deck[i].red * TILE_NONFLOWER_MAX) + mihajong_socket::protocol::StartRound_Tile_Excess,
				[i](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool { // クライアントの場合、データを受信
					if ( ((ReceivedMsg - mihajong_socket::protocol::StartRound_Tile_Excess) > TILE_NONFLOWER_MAX) &&
						((ReceivedMsg - mihajong_socket::protocol::StartRound_Tile_Excess) < TILE_SUIT_FLOWERS) ) {
							gameStat->Deck[i].tile = (tileCode)((ReceivedMsg - mihajong_socket::protocol::StartRound_Tile_Excess) % TILE_NONFLOWER_MAX);
							gameStat->Deck[i].red  = (doraCol)((ReceivedMsg - mihajong_socket::protocol::StartRound_Tile_Excess) / TILE_NONFLOWER_MAX);
					} else {
						gameStat->Deck[i].tile = (tileCode)(ReceivedMsg - mihajong_socket::protocol::StartRound_Tile_Excess);
						gameStat->Deck[i].red = Normal;
					}
					return true;
				});
		/* TODO 画面更新 vanish */
	}
	void rolldice(GameTable* const gameStat) {
		// 賽を振る
		for (unsigned i = 0; i < 2; i++) {
			gameStat->Dice[i].Number = RndNum::dice();
			gameStat->Dice[i].Direction = RndNum::rnd(4);
		}
		mihajong_graphic::GameStatus::updateGameStat(gameStat); /* TODO: 画面更新 redrscreen: commonswitch GameStat, GameEnv */
		for (unsigned k = 0; k < 10; k++) { // 賽を振る
			for (unsigned i = 0; i < 2; i++) {
				gameStat->Dice[i].Number = RndNum::dice();
				gameStat->Dice[i].Direction = RndNum::rnd(4);
			}
			sound::Play(sound::IDs::sndSaikoro);
			mihajong_graphic::GameStatus::updateGameStat(gameStat); Sleep(80);
		}
		sound::Play(sound::IDs::sndSaikoro);
		/* サイコロの出目を送信 */
		for (unsigned i = 0; i < 2; i++)
			statsync(gameStat, gameStat->Dice[i].Number + mihajong_socket::protocol::StartRound_Dice_Excess,
				[i](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
					gameStat->Dice[i].Number = ReceivedMsg - mihajong_socket::protocol::StartRound_Dice_Excess;
					gameStat->Dice[i].Direction = RndNum::rnd(4);
					return true;
				});
		// --
		if (RuleData::chkRule("dora_twice", "yes") ||
			(RuleData::chkRule("dora_twice", "only_when_doublets") && (gameStat->Dice[0].Number == gameStat->Dice[1].Number)))
			gameStat->DeadTiles += 2; /* ドラドラ卓なら王牌の数を増やす */
		calcWareme(gameStat); // 割れ目
		// 通常機能の表示
		/* TODO vanish2@ */
	}
	void haipai(GameTable* const gameStat) { // 配牌
		for (int i = 0; i < (chkGameType(gameStat, AllSanma) ? 36 : 48); i++) { // ２幢ずつを３回
			unsigned handIndex = i % 4 + (i / (chkGameType(gameStat, AllSanma) ? 12 : 16)) * 4;
			PLAYER_ID player;
			if (chkGameType(gameStat, Sanma4))
				player = ((i % 12 / 4) + gameStat->GameRound) % 4;
			else if (chkGameType(gameStat, SanmaT))
				player = ((i % 12 / 4) + (gameStat->GameRound - (gameStat->GameRound / 4))) % 3;
			else
				player = ((i % 16 / 4) + gameStat->GameRound) % 4;
			gameStat->Player[player].Hand[handIndex].tile = gameStat->Deck[gameStat->TilePointer].tile;
			gameStat->Player[player].Hand[handIndex].red  = gameStat->Deck[gameStat->TilePointer].red;
			++gameStat->TilePointer;
			if ((i == (chkGameType(gameStat, AllSanma) ? 24 : 18)) && (gameStat->Honba > 0))
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneHonba);
			if (i % 4 == 3) {
				calcdoukasen(gameStat);
				sound::Play(sound::IDs::sndTsumo);
				mihajong_graphic::GameStatus::updateGameStat(gameStat); Sleep(250);
			}
		}
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
		for (int i = 0; i < (chkGameType(gameStat, AllSanma) ? 4 : 5); i++) { // １枚ずつを１回、親のチョンチョン
			unsigned handIndex = i / (chkGameType(gameStat, AllSanma) ? 3 : 4) + 12;
			PLAYER_ID player;
			if (chkGameType(gameStat, Sanma4))
				player = (i % 3 + gameStat->GameRound) % 4;
			else if (chkGameType(gameStat, SanmaT))
				player = (i + (gameStat->GameRound - (gameStat->GameRound / 4))) % 3;
			else
				player = (i + gameStat->GameRound) % 4;
			gameStat->Player[player].Hand[handIndex].tile = gameStat->Deck[gameStat->TilePointer].tile;
			gameStat->Player[player].Hand[handIndex].red  = gameStat->Deck[gameStat->TilePointer].red;
			++gameStat->TilePointer;
			calcdoukasen(gameStat);
			sound::Play(sound::IDs::sndTsumo);
			mihajong_graphic::GameStatus::updateGameStat(gameStat); Sleep(250);
		}

		initdora(gameStat); // ドラをめくる

		sound::Play(sound::IDs::sndMekuri);
		haifu::haifurechaipai(gameStat);
		/* TODO: ステータス表示 statmes "" */
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			lipai(gameStat, i);
		sound::Play(sound::IDs::sndBell);
	}
}
void tableinit(GameTable* const gameStat) {
	init_ai(gameStat);
	/* TODO: ifaceinit */ // 押しボタン類のステータス初期化
	inittable(gameStat);
	// 局の開始で同期する。1.7系列まではこのとき落ち戻りが可能(落ち戻り機能は1.8で廃止されました)
	statsync(gameStat, mihajong_socket::protocol::Server_StartRound_Signature,
		[](GameTable* const gameStat, std::uint8_t ReceivedMsg) -> bool {
			if (ReceivedMsg == mihajong_socket::protocol::Server_StartRound_Signature)
				return true;
			/* TODO: ここを移植する
			if ((ClientReceived == 1)&&(ReceivedMsg == 1023)) {
				chatappend "*** ホストとの接続が切れました。\n"
				chatappend "*** この局はツモ切り、次局からCPUが代走します。\n"
				repeat NUM_OF_PLAYERS
					if (cnt != getPlayer(GameStat)) {
						setDisconnectFlag GameStat, cnt, 1
					}
				loop
			}
			*/
			return false;
		});
	// 牌譜バッファの初期化
	haifu::haifuinit();
	/* 卓の情報を同期 */
	syncTableStat(gameStat);
	// BGMを鳴らす
	choosebgm(gameStat);
	// 画面の準備
	screen(gameStat);
	// 洗牌
	tileshuffle(gameStat);
	// 賽を振る
	rolldice(gameStat);
	// 配牌
	haipai(gameStat);
	return;
}
