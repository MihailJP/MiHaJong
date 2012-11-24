#include "mouda.h"

DiscardTileNum getdahai(GameTable* const gameStat) {
	DiscardTileNum DiscardTileIndex;
	/* COMが「カンニング」しないように処理 */
	GameTable* sandbox = makesandBox(gameStat, gameStat->CurrentPlayer.Active);
	/* 打牌を取得する */
	if (gameStat->CurrentPlayer.Active == gameStat->PlayerID) {
		if (gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki) {
			debug(_T("プレイヤーのツモ番ですが残念ながらアガリ放棄です。"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NUM_OF_TILES_IN_HAND - 1;
		} else if (EnvTable::Instantiate()->WatchModeFlag) {
			DiscardTileIndex = aiscript::compdahai(sandbox);
		} else {
			debug(_T("プレイヤーのツモ番です。"));
			/* TODO: プレイヤー打牌選択 playerdahai GameStat, GameEnv: DiscardTileIndex = stat */
		}
	} else if (
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag == -1) ||
		gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki) {
			debug(_T("アガリ放棄か回線切断したプレイヤーのツモ番です。"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NUM_OF_TILES_IN_HAND - 1;
	} else if ((EnvTable::Instantiate()->GameMode == EnvTable::Client) ||
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag > 0)) {
			/* ネット対戦時の処理 */
			debug(_T("リモートプレイヤーのツモ番です。"));
			DiscardTileIndex = RemoteAction::remotedahai(gameStat);
			/* TODO: 切断時の処理
			if (getGameMode(GameEnv) == GAMEMODE_CLIENT) {
				if (ReceivedMsg == 1023) {
					chatappend "*** ホストとの接続が切れました。\n"
					chatappend "*** この局はツモ切り、次局からCPUが代走します。\n"
					repeat NUM_OF_PLAYERS
						if (cnt != getPlayer(GameStat)) {
							setDisconnectFlag GameStat, cnt, 1
						}
					loop
				}
			}
			*/
	} else {
		debug(_T("AIのツモ番です。"));
		DiscardTileIndex = aiscript::compdahai(sandbox);
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
			assert((DiscardTileIndex.type == DiscardTileNum::Kyuushu) ||
				(DiscardTileIndex.type == DiscardTileNum::Agari) ||
				((DiscardTileIndex.id >= 0) && (DiscardTileIndex.id < NUM_OF_TILES_IN_HAND)));
			switch (DiscardTileIndex.type) {
			case DiscardTileNum::Kyuushu:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Kyuushu);
				break;
			case DiscardTileNum::Agari:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Tsumo);
				break;
			case DiscardTileNum::Normal:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Normal_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Ankan:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Ankan_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Kakan:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Kakan_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Riichi:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Riichi_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::OpenRiichi:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_ORiichi_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Flower:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Flower_Offset + DiscardTileIndex.id);
				break;
			default:
				error(_T("CPUの打牌番号がなんかおかしい"));
				break;
			}
		}
	}
	return DiscardTileIndex;
}
