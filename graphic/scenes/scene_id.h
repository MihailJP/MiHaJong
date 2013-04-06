#pragma once

namespace mihajong_graphic {

enum sceneID { // シーン番号
	sceneSplash, // スプラッシュスクリーン的なシーン
	sceneTitle, // タイトル画面
	sceneConfig, // コンフィグ画面
	sceneSetting, // 環境設定画面
	sceneServerWaiting, // サーバー待機画面
	sceneClientWaiting, // クライアント待機画面
	sceneGameTable, // 卓の画面
	sceneResult, // 結果画面
};

enum TableSubsceneID : unsigned int { // 卓画面のサブシーン番号
	tblSubsceneNone, // 通常
	tblSubsceneBeginning, // 局開始画面
	tblSubsceneHonba, // 「○本場」
	tblSubsceneRyuukyoku, // 「流局」
	tblSubsceneSifeng, // 「四風連打」
	tblSubsceneTripleRon, // 「三家和」
	tblSubsceneSikang, // 「四開槓」
	tblSubsceneFourRiichi, // 「四人立直」
	tblSubsceneChonbo, // 「錯和」
	tblSubsceneAlice, // 「アリス判定」
	tblSubsceneCall, // 発声表示画面
	tblSubsceneCallFade, // 発声表示画面(フェード)
	tblSubsceneCallCut, // 発声表示画面(カット)
	tblSubsceneCallChankanPre, // 発声表示画面(槍槓用特殊)
	tblSubsceneCallChankan, // 発声表示画面(槍槓用特殊)
	tblSubsceneCallVal, // 点数の増分表示
	tblSubsceneCallValNotenBappu, // 点数の増分表示（ノーテン罰符）
	tblSubsceneCallValAgariten, // 点数の増分表示（和了り点）
	tblSubsceneCallValTsumibou, // 点数の増分表示（積み棒）
	tblSubsceneCallValChip, // 点数の増分表示（チップ）
	tblSubsceneCallValKyoutaku, // 点数の増分表示（供託）
	tblSubsceneCallValChonboBappu, // 点数の増分表示（錯和罰符）
	tblSubsceneCallValNagashiMangan, // 点数の増分表示（流し満貫）
	tblSubsceneCallValDobon, // 点数の増分表示（飛び罰符）
	tblSubsceneCallValKitamakura, // 点数の増分表示（飛び罰符）
	tblSubsceneCallValYakuman, // 点数の増分表示（役満祝儀）
	tblSubsceneChkTenpai, // 流局時の聴牌確認
	tblSubscenePlayerDahai, // プレイヤーの打牌選択用
	tblSubscenePlayerNaki, // プレイヤーの副露選択用
	tblSubscenePlayerChankan, // プレイヤーの副露選択用(槍槓)
	tblSubsceneAgari, // 和了役表示画面
	tblSubsceneAgariUradora, // 和了役表示画面(裏ドラあり)
};

enum ServerWaitingSubsceneID : unsigned int { // サーバー待機画面のサブシーン番号
	srvwSubsceneNone, // 通常
	srvwSubscene1of4, // 待機中の人数
	srvwSubscene1of3, //      〃
	srvwSubscene2of4, //      〃
	srvwSubscene2of3, //      〃
	srvwSubscene3of4, //      〃
	srvwSubscene3of3, //      〃
	srvwSubscene4of4, //      〃
};

}
