#ifndef ENDTYPE_H
#define ENDTYPE_H

/* 局終了コード */
enum EndType : int { // 局の終了理由
	Continuing = -1, // 継続中
	Ryuukyoku,       // 普通の流局
	Agari,           // 誰かの和了
	Chonbo,          // 錯和で終了
	KyuushuKyuuhai,  // 九種九牌
	Suukaikan,       // 四開槓
	TripleRon,       // 三家和
	SuufonRenda,     // 四風連打
	SuuchaRiichi,    // 四人立直
	NagashiMangan,   // 流し満貫
	Disconnect,      // 通信途絶(ネットワーク対戦時のみ)
	Uukaikan,        // 五回目の槓で流局した場合(誰かが四槓子聴牌の場合のみ)
};

#endif
