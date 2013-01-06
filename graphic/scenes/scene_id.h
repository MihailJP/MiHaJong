#pragma once

namespace mihajong_graphic {

enum sceneID { // シーン番号
	sceneSplash, // スプラッシュスクリーン的なシーン
	sceneTitle, // タイトル画面
	sceneConfig, // コンフィグ画面
	sceneGameTable, // 卓の画面
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
	tblSubsceneCall, // 発声表示画面
	tblSubsceneCallFade, // 発声表示画面(フェード)
	tblSubsceneChkTenpai, // 流局時の聴牌確認
};

}
