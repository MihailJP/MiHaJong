#ifndef PROTOCOL_H
#define PROTOCOL_H

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
	const uint8_t StartString_Signature = 0xb8; // 文字列開始のシグネチャ
	
	// 開始のシグネチャ…… B4 NAME1 NAME2 NAME3 NAME4 RULES
	//                      NAME1 to NAME4 = プレイヤー名
	//                      RULES = ルール設定(80文字ずつに区切る)
	const uint8_t Server_StartGame_Signature = 0xb4; // サーバーからプレイ開始のシグネチャ

	// 捨牌コード送受信用
	const uint8_t Dahai_Type_Normal_Offset = 0x60; // 普通の捨牌 (0x60 .. 0x6d)
	const uint8_t Dahai_Type_Ankan_Offset = 0x70; // 暗槓
	const uint8_t Dahai_Type_Kakan_Offset = 0x80; // 加槓
	const uint8_t Dahai_Type_Riichi_Offset = 0x90; // 立直
	const uint8_t Dahai_Type_ORiichi_Offset = 0xc0; // オープン立直
	const uint8_t Dahai_Type_Flower_Offset = 0xd0; // 花牌
	const uint8_t Dahai_Kyuushu = 0x6e; // 九種九牌
	const uint8_t Dahai_Tsumo = 0x6f; // 和了
	const uint8_t Dahai_Remote_Disconnect = 0x9f; // ツモ順のプレイヤーの接続が切れた場合の符牒

	// 鳴きコード送受信用
	const uint8_t Naki_Ignore = 0xa0; // 無視
	const uint8_t Naki_Ron = 0xa1; // ロン
	const uint8_t Naki_Pon = 0xa2; // ポン
	const uint8_t Naki_Kan = 0xa3; // 明槓
	const uint8_t Naki_Chii_Lower = 0xa4; // 左チー
	const uint8_t Naki_Chii_Middle = 0xa5; // 中チー
	const uint8_t Naki_Chii_Upper = 0xa6; // 右チー
	const uint8_t Naki_Remote_Disconnect = 0xa7; // 鳴きの問い合わせ中にプレイヤーの接続が切れた場合の符牒
}
}

#endif
