#pragma once

namespace mihajong_graphic {

enum class SceneID { // シーン番号
	null, // なにもなし（後始末用）
	splash, // スプラッシュスクリーン的なシーン
	title, // タイトル画面
	config, // コンフィグ画面
	serverWaiting, // サーバー待機画面
	clientWaiting, // クライアント待機画面
	waitingError, // 接続失敗
	gameTable, // 卓の画面
	result, // 結果画面
};

enum class TableSubsceneID : unsigned int { // 卓画面のサブシーン番号
	none, // 通常
	beginning, // 局開始画面
	honba, // 「○本場」
	ryuukyoku, // 「流局」
	sifeng, // 「四風連打」
	tripleRon, // 「三家和」
	sikang, // 「四開槓」
	fourRiichi, // 「四人立直」
	chonbo, // 「錯和」
	alice, // 「アリス判定」
	call, // 発声表示画面
	callFade, // 発声表示画面(フェード)
	callCut, // 発声表示画面(カット)
	callChankanPre, // 発声表示画面(槍槓用特殊)
	callChankan, // 発声表示画面(槍槓用特殊)
	callVal, // 点数の増分表示
	callValNotenBappu, // 点数の増分表示（ノーテン罰符）
	callValAgariten, // 点数の増分表示（和了り点）
	callValTsumibou, // 点数の増分表示（積み棒）
	callValChip, // 点数の増分表示（チップ）
	callValKyoutaku, // 点数の増分表示（供託）
	callValChonboBappu, // 点数の増分表示（錯和罰符）
	callValNagashiMangan, // 点数の増分表示（流し満貫）
	callValDobon, // 点数の増分表示（飛び罰符）
	callValKitamakura, // 点数の増分表示（飛び罰符）
	callValYakuman, // 点数の増分表示（役満祝儀）
	chkTenpai, // 流局時の聴牌確認
	playerDahai, // プレイヤーの打牌選択用
	playerNaki, // プレイヤーの副露選択用
	playerChankan, // プレイヤーの副露選択用(槍槓)
	agari, // 和了役表示画面
	agariUradora, // 和了役表示画面(裏ドラあり)
};

enum class ServerWaitingSubsceneID : unsigned int { // サーバー待機画面のサブシーン番号
	none,         // 通常
	oneOfFour,    // 待機中の人数
	oneOfThree,   //      〃
	twoOfFour,    //      〃
	twoOfThree,   //      〃
	threeOfFour,  //      〃
	threeOfThree, //      〃
	fourOfFour,   //      〃
};

enum class ClientWaitingSubsceneID : unsigned int { // クライアント待機画面のサブシーン番号
	none, // 通常
	connecting, // 接続試行中
	waiting, // 待機中
};

union SubSceneID {
public:
	TableSubsceneID tableSubsceneID;
	ServerWaitingSubsceneID serverWaitingSubsceneID;
	ClientWaitingSubsceneID clientWaitingSubsceneID;
	SubSceneID() = default;
	SubSceneID(const TableSubsceneID& id) : tableSubsceneID(id) {}
	SubSceneID(const ServerWaitingSubsceneID& id) : serverWaitingSubsceneID(id) {}
	SubSceneID(const ClientWaitingSubsceneID& id) : clientWaitingSubsceneID(id) {}
};

}
