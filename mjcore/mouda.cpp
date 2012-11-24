#include "mouda.h"

DiscardTileNum getdahai(GameTable* const gameStat) {
	DiscardTileNum DiscardTileIndex;
	/* COM���u�J���j���O�v���Ȃ��悤�ɏ��� */
	GameTable* sandbox = makesandBox(gameStat, gameStat->CurrentPlayer.Active);
	/* �Ŕv���擾���� */
	if (gameStat->CurrentPlayer.Active == gameStat->PlayerID) {
		if (gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki) {
			debug(_T("�v���C���[�̃c���Ԃł����c�O�Ȃ���A�K�������ł��B"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NUM_OF_TILES_IN_HAND - 1;
		} else if (EnvTable::Instantiate()->WatchModeFlag) {
			DiscardTileIndex = aiscript::compdahai(sandbox);
		} else {
			debug(_T("�v���C���[�̃c���Ԃł��B"));
			/* TODO: �v���C���[�Ŕv�I�� playerdahai GameStat, GameEnv: DiscardTileIndex = stat */
		}
	} else if (
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag == -1) ||
		gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki) {
			debug(_T("�A�K������������ؒf�����v���C���[�̃c���Ԃł��B"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NUM_OF_TILES_IN_HAND - 1;
	} else if ((EnvTable::Instantiate()->GameMode == EnvTable::Client) ||
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag > 0)) {
			/* �l�b�g�ΐ펞�̏��� */
			debug(_T("�����[�g�v���C���[�̃c���Ԃł��B"));
			DiscardTileIndex = RemoteAction::remotedahai(gameStat);
			/* TODO: �ؒf���̏���
			if (getGameMode(GameEnv) == GAMEMODE_CLIENT) {
				if (ReceivedMsg == 1023) {
					chatappend "*** �z�X�g�Ƃ̐ڑ����؂�܂����B\n"
					chatappend "*** ���̋ǂ̓c���؂�A���ǂ���CPU���㑖���܂��B\n"
					repeat NUM_OF_PLAYERS
						if (cnt != getPlayer(GameStat)) {
							setDisconnectFlag GameStat, cnt, 1
						}
					loop
				}
			}
			*/
	} else {
		debug(_T("AI�̃c���Ԃł��B"));
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
				error(_T("CPU�̑Ŕv�ԍ����Ȃ񂩂�������"));
				break;
			}
		}
	}
	return DiscardTileIndex;
}
