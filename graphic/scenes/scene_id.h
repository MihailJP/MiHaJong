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
	tblSubsceneCall, // 発声表示画面
};

}
