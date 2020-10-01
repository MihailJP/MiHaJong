#pragma once

/* 局終了コード */
enum class EndType : int { // 局の終了理由
	drawRinshan = -2,// 嶺上を引く処理に戻る
	continuing,      // 継続中
	ryuukyoku,       // 普通の流局
	agari,           // 誰かの和了
	chonbo,          // 錯和で終了
	kyuushuKyuuhai,  // 九種九牌
	suukaikan,       // 四開槓
	tripleRon,       // 三家和
	suufonRenda,     // 四風連打
	suuchaRiichi,    // 四人立直
	nagashiMangan,   // 流し満貫
	disconnect,      // 通信途絶(ネットワーク対戦時のみ)
	uukaikan,        // 五回目の槓で流局した場合(誰かが四槓子聴牌の場合のみ)
};
