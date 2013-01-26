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
	tblSubsceneChonbo, // 「錯和」
	tblSubsceneCall, // 発声表示画面
	tblSubsceneCallFade, // 発声表示画面(フェード)
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
};

}
