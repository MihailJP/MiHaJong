#pragma once

#include <cstdint>

namespace mihajong_socket {
namespace protocol {
	// Port 50000 にクライアント接続時、サーバーはクライアント番号(1 byte)を返して切断する。
	// Port (50000 + Client№ - 1) に再接続し、クライアントは自分の名前を送信する。
	// その後、クライアントは開始合図を待機する。
	// ※このポート番号は四人打ちバージョンの場合

	// 文字列…… B8 LL SS SS SS ...
	//            LL = 文字列の長さ(bytes)
	//            SS = 文字列の中身(LL bytes, up to 255)
	constexpr uint8_t StartString_Signature = 0xb8; // 文字列開始のシグネチャ
	
	// 開始のシグネチャ…… B4 NAME1 NAME2 NAME3 NAME4 RULES
	//                      NAME1 to NAME4 = プレイヤー名
	//                      RULES = ルール設定(80文字ずつに区切る)
	constexpr uint8_t Server_StartGame_Signature = 0xb4; // サーバーからプレイ開始のシグネチャ
	
	// 開始のシグネチャ…… B6 RN HB RB PR SCORE1 SCORE2 SCORE3 SCORE4 YK CH CH CH CH SU \
	//                      DECK DI DI
	//                      RN = 局番号(東1局=0、東2局=1、以下同様にして南4局=7)
	//                      HB = 積み棒の数
	//                      RB = リーチ棒の数
	//                      PR = 八連荘判定用((最終和了プレイヤー番号(なければ-1) + 1) << 4 + 連続和了数)
	//                      SCORE1 to SCORE4 = 持ち点
	//                      YK = 焼き鳥フラグ(bit4〜bit7 = 0, bit3 = Player0, ..., bit0 = Player3)
	//                      CH = チップ(excess-128, Player0 to Player3)
	//                      YK = 四馬路解禁フラグ(bit4〜bit7 = 0, bit3 = Player0, ..., bit0 = Player3)
	//                      DECK = 洗牌済みの山牌
	//                      DI = サイコロの目(0xa9〜0xae)
	constexpr uint8_t Server_StartRound_Signature = 0xb6; // サーバーから局開始のシグネチャ
	constexpr uint8_t StartRound_Dice_Excess = 0xa8; // サイコロの出目 0xa9〜0xae
	constexpr uint8_t StartRound_Tile_Excess = 0x10; // 牌番号のゲタ

	// 理牌コード送受信用
	constexpr uint8_t Lipai_To = 0x40; // 理牌先の位置指定
	constexpr uint8_t Lipai_From = 0x50; // 理牌元の牌
	constexpr uint8_t Lipai_Reset = 0x5f; // 理牌リセット

	// 捨牌コード送受信用
	constexpr uint8_t Dahai_Type_Normal_Offset = 0x60; // 普通の捨牌 (0x60 .. 0x6d)
	constexpr uint8_t Dahai_Type_Ankan_Offset = 0x70; // 暗槓
	constexpr uint8_t Dahai_Type_Kakan_Offset = 0x80; // 加槓
	constexpr uint8_t Dahai_Type_Riichi_Offset = 0x90; // 立直
	constexpr uint8_t Dahai_Type_ORiichi_Offset = 0xc0; // オープン立直
	constexpr uint8_t Dahai_Type_Flower_Offset = 0xd0; // 花牌
	constexpr uint8_t Dahai_Kyuushu = 0x6e; // 九種九牌
	constexpr uint8_t Dahai_Tsumo = 0x6f; // 和了
	constexpr uint8_t Dahai_Remote_Disconnect = 0x9f; // ツモ順のプレイヤーの接続が切れた場合の符牒

	// 鳴きコード送受信用
	constexpr uint8_t Naki_Ignore = 0xa0; // 無視
	constexpr uint8_t Naki_Ron = 0xa1; // ロン
	constexpr uint8_t Naki_Pon = 0xa2; // ポン
	constexpr uint8_t Naki_Kan = 0xa3; // 明槓
	constexpr uint8_t Naki_Chii_Lower = 0xa4; // 左チー
	constexpr uint8_t Naki_Chii_Middle = 0xa5; // 中チー
	constexpr uint8_t Naki_Chii_Upper = 0xa6; // 右チー
	constexpr uint8_t Naki_Remote_Disconnect = 0xa7; // 鳴きの問い合わせ中にプレイヤーの接続が切れた場合の符牒
}
}
