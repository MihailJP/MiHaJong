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
}
}

#endif
