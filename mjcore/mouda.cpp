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
			DiscardTileIndex.type = DiscardTileNum::Normal; // �e�X�g�_�u��
			DiscardTileIndex.id = NUM_OF_TILES_IN_HAND - 1; // (�c���؂�)
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

namespace { /* �������������p */
	EndType procDahaiSubAgari(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) { /* ���̘a�̏��� */
		EndType RoundEndType = Continuing;
		haifu::haifurectsumo(gameStat); // �v���ɋL�^
		/* ���A������p�̕ϐ� */
		if (RuleData::chkRuleApplied("paarenchan")) {
			if (gameStat->LastAgariPlayer == gameStat->CurrentPlayer.Active) {
				++gameStat->AgariChain;
			} else if (RuleData::chkRuleApplied("breaking_paarenchan") && (gameStat->AgariChain == 7)) {
				gameStat->AgariChain = -1; gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Active;
			} else {
				gameStat->AgariChain = 1; gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Active;
			}
		}
		/* ���̘a�������Ƃ�ϐ��ɐݒ� */
		gameStat->TsumoAgariFlag = true;
		yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Active);
		if (((yakuInfo.CoreHan < (gameStat->ShibariFlag ? 2 : 1)) && (yakuInfo.CoreSemiMangan == 0)) ||
			(RuleData::chkRuleApplied("riichi_shibari") && (!gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.RichiFlag)) ||
			((gameStat->PaoFlag[pyMinkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong")))
			RoundEndType = Chonbo; /* ����𖞂����Ă��Ȃ��ꍇ(���������Ȃ�)�c���a�Ƃ��ċǂ��I������ */
		else
			RoundEndType = Agari; /* ����𖞂����Ȃ�a����Ƃ��Đ��� */
		gameStat->TsumoAgariFlag = true;
		gameStat->CurrentPlayer.Agari = gameStat->CurrentPlayer.Active;
		/* TODO: ����
		setCall getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), "�c��"
		if (getHeavenHandFlag(GameStat) == 1) {setCall getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), "����"} //�V�a�̎��̓����ƌ������킵
		*/
		gameStat->Player[gameStat->CurrentPlayer.Agari].HandStat = handExposed;
		if (gameStat->TianHuFlag) sound::Play(sound::IDs::voxRon);
		else sound::Play(sound::IDs::voxTsumo);
		/* TODO: ��ʍX�V redrscreen */
		return RoundEndType;
	}
	EndType procDahaiSubKyuushu(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* ����v���錾���ꂽ�ꍇ */
		DiscardTileIndex.type = DiscardTileNum::Normal;
		DiscardTileIndex.id = NUM_OF_TILES_IN_HAND; // ��헬�����ł��Ȃ����̓c���؂�Ƃ݂Ȃ�
		if (RuleData::chkRuleApplied("nine_terminals") &&
			chkdaopaiability(gameStat, gameStat->CurrentPlayer.Active) &&
			gameStat->Player[gameStat->CurrentPlayer.Active].FirstDrawFlag) {
				/* TODO: ���� setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "����v" */
				gameStat->Player[gameStat->CurrentPlayer.Active].HandStat = handExposed;
				sound::Play(sound::IDs::voxKyuushu);
				/* TODO: ��ʍX�V redrscreen */
				return KyuushuKyuuhai;
		} else {
			warn(_T("����v�͂ł��܂���B�c���؂�Ƃ݂Ȃ��܂��B"));
			return Continuing;
		}
	}
	EndType procDahaiSubFlower(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* �Ԕv�𔲂����ꍇ�̏��� */
		if ((DiscardTileIndex.type == DiscardTileNum::Ankan) &&
			(gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].tile > TILE_SUIT_FLOWERS)) {
				DiscardTileIndex.type = DiscardTileNum::Flower;
				info(_T("�Ԕv�̏����Ɉڂ�܂��B�Ŕv�R�[�h��␳���܂����B"));
		}
		if (DiscardTileIndex.type == DiscardTileNum::Flower) {
			EndType RoundEndType;
			if (chkGameType(gameStat, SanmaX)) {
				/* �K���O�����[���Ŗk���v�𔲂����Ƃ��̏��� */
				/* ���̃Q�[���ł͂ǂ�Ȏ�ł�(���m���l��łȂ��Ă�����)
					�����k�������ł��郋�[�� */
				if (fuuroproc(gameStat, &RoundEndType, DiscardTileIndex, FuuroNorth))
					return RoundEndType;
				else return DrawRinshan;
			} else {
				/* �Ԕv�𔲂����Ƃ��̏��� */
				/* ���̃Q�[���͎����ꂪ�Ȃ��̂ŉԔv�Ń�������邱�Ƃ͖��� */
				if (fuuroproc(gameStat, &RoundEndType, DiscardTileIndex, FuuroFlower))
					return RoundEndType;
				else return DrawRinshan;
			}
		}
		return Continuing;
	}
	EndType procDahaiSubKan(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* �ÞȁE���Ȃ���Ƃ��̏��� */
		const unsigned kanLim = (RuleData::chkRuleApplied("fifth_kong") ? 5 : 4);
		if ((gameStat->TilePointer < (gameStat->RinshanPointer - (gameStat->DeadTiles - 1))) && // �n�C�e�C�łȂ�
			(gameStat->KangNum < kanLim)) { // ���v���̐������ł���
				if ((DiscardTileIndex.type == DiscardTileNum::Ankan) ||
					(DiscardTileIndex.type == DiscardTileNum::Kakan)) {
						if (RuleData::chkRule("minkan_pao", "yes") || RuleData::chkRule("minkan_pao", "yes_2han")) {
							gameStat->PaoFlag[pyMinkan].paoPlayer =
								gameStat->PaoFlag[pyMinkan].agariPlayer = -1;
						}
						/* �Ȃ�����Ɨ��v�̕����̂�������̂Ŏ��̑Ŕv�� */
						EndType roundEndType;
						if (fuuroproc(gameStat, &roundEndType, DiscardTileIndex,
							(DiscardTileIndex.type == DiscardTileNum::Ankan) ?
							FuuroAnkan : FuuroKakan))
							return roundEndType;
						else return DrawRinshan;
				}
		}
		return Continuing;
	}
	void procDahaiSubRiichi(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* ����������Ƃ��̏��� */
		if (tilesLeft(gameStat) < ACTUAL_PLAYERS) {
			// �c��S�������̎��̓��[�`����
			DiscardTileIndex.type = DiscardTileNum::Normal;
			warn(_T("�R�v�̎c���v���𖞂����Ă��܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B"));
		} else if (!isRichiReqSatisfied(gameStat, gameStat->CurrentPlayer.Active)) {
			// �_�_�����𖞂����Ă��Ȃ����̓��[�`����
			DiscardTileIndex.type = DiscardTileNum::Normal;
			warn(_T("�����_�̗v���𖞂����Ă��܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B"));
		}
		if ((!RuleData::chkRuleApplied("open_riichi")) && (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)) {
			// �I�[�v�����������̎�
			DiscardTileIndex.type = DiscardTileNum::Riichi;
			warn(_T("�I�[�v�������͂ł��܂���B�ʏ�̗����Ƃ݂Ȃ��܂��B"));
		}
		/* ������錾���� */
		if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi) {
			/* TODO: ���� setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "���[�`" */
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag))
				sound::Play(sound::IDs::musOpenrichi);
			/* TODO: ��ʍX�V redrscreen */
			/* TODO: 1�b�҂� await 1000 */
			gameStat->Player[gameStat->CurrentPlayer.Active].HandStat = handOpenRiichi;
			gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.OpenFlag = true;
			/* TODO: �ꕔ�{�^���̖����� vanish2@ */
		}
		if (DiscardTileIndex.type == DiscardTileNum::Riichi) {
			/* TODO: ���� setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "���[�`" */
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag)) {
					if (!(gameStat->Player[0].RichiFlag.RichiFlag || gameStat->Player[1].RichiFlag.RichiFlag ||
						gameStat->Player[2].RichiFlag.RichiFlag || gameStat->Player[3].RichiFlag.RichiFlag)) {
							if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
								sound::Play(sound::IDs::musRichi2);
							else
								sound::Play(sound::IDs::musRichi1);
					} else if (!EnvTable::Instantiate()->WatchModeFlag) {
						if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
							sound::Play(sound::IDs::musRichi3);
					}
			}
			/* TODO: ��ʍX�V redrscreen */
			/* TODO: 1�b�҂� await 1000 */
			/* TODO: �ꕔ�{�^���̖����� vanish2@ */
		}
	}
	void procDahaiSubPost(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) { /* ���㏈�� */
		/* �Ŕv���L�^���� */
		discardTile* const newDiscard = &(gameStat->Player[gameStat->CurrentPlayer.Active].Discard[++gameStat->Player[gameStat->CurrentPlayer.Active].DiscardPointer]);
		newDiscard->tcode.tile = gameStat->CurrentDiscard.tile = gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].tile;
		newDiscard->tcode.red  = gameStat->CurrentDiscard.red  = gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].red;
		if (DiscardTileIndex.type == DiscardTileNum::Riichi) /* �����錾�v�̏ꍇ */
			newDiscard->dstat = discardRiichi;
		else if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi) /* �I�[�v�������錾�v�̏ꍇ */
			newDiscard->dstat = discardRiichi;
		else /* ����ȊO�̏ꍇ */
			newDiscard->dstat = discardNormal;
		newDiscard->isDiscardThrough = (DiscardTileIndex.id == NUM_OF_TILES_IN_HAND - 1) && (!gameStat->TianHuFlag);
		gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].red  = Normal;
		/* �ꔭ�̃t���O���~�낷 */
		gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag = false;
		/* �����I�ɗ��v���s�Ȃ� */
		lipai(gameStat, gameStat->CurrentPlayer.Active);
		// ���̂Ƃ��v���̂ĂĂ���͂� �\���o�O�h�~�̂���
		for (PLAYER_ID i = 0; i < PLAYERS; i++) {
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].red  = Normal;
		}
		/* ��������������̏ꍇ�A��_���������ꔭ�̃t���O�𗧂Ă� */
		if ((DiscardTileIndex.type == DiscardTileNum::Riichi) || (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)) {
			/* TODO: ��������������� setCall getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), "" */
			gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.RichiFlag =
				gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag = true;
			gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.DoubleFlag =
				gameStat->Player[gameStat->CurrentPlayer.Active].FirstDrawFlag;
			gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.OpenFlag =
				(DiscardTileIndex.type == DiscardTileNum::OpenRiichi);
			++gameStat->Deposit;
			gameStat->Player[gameStat->CurrentPlayer.Active].PlayerScore += LargeNum::fromInt(-1000);
			if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)
				chkOpenMachi(gameStat, gameStat->CurrentPlayer.Active);
		}
		/* �V�a��n�a�̃t���O���~�낷 */
		gameStat->Player[gameStat->CurrentPlayer.Active].FirstDrawFlag =
			gameStat->TianHuFlag = false;
		/* �Ŕv����Ƃ��̉���炷 */
		/* �h�����̂Ă鎞�͋��ł̉��ɂ��� */
		if (gameStat->CurrentDiscard.tile > TILE_SUIT_FLOWERS)
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == AkaDora) || (gameStat->DoraFlag.Omote[gameStat->CurrentDiscard.tile] > 0))
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == AoDora) && (!RuleData::chkRule("blue_tiles", "-1han")))
			sound::Play(sound::IDs::sndDahai2);
		else
			sound::Play(sound::IDs::sndDahai1);
		/* ���̂Ƃ��v���̂ĂĂ���͂��Ȃ̂ŁA�o�O�h�~�̂��߂̏��� */
		for (PLAYER_ID i = 0; i < PLAYERS; i++) {
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].red  = Normal;
		}
		/* �ĕ`�� */
		/* TODO: �ĕ`�� redrscreen */
	}
}

EndType procdahai(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) {
	EndType RoundEndType = Continuing;
	{
		CodeConv::tostringstream o;
		o << _T("�v���C���[ [") << (int)gameStat->CurrentPlayer.Active <<
			_T("] �Ŕv�^�C�v [") << (int)DiscardTileIndex.type <<
			_T("] ��v�ԍ� [") << (int)DiscardTileIndex.id << _T("]");
		info(o.str().c_str());
	}
	/* �������Ă��Ȃ������U���Ȃ炻�̊����̂��ߐU������������ */
	if (!gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.RichiFlag)
		gameStat->Player[gameStat->CurrentPlayer.Active].DoujunFuriten = false;
	/* ���̘a�̏��� */
	if (DiscardTileIndex.type == DiscardTileNum::Agari) {
		RoundEndType = procDahaiSubAgari(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* ����v���錾���ꂽ�ꍇ */
	if ((!chkGameType(gameStat, SanmaS)) && (DiscardTileIndex.type == DiscardTileNum::Kyuushu)) {
		RoundEndType = procDahaiSubKyuushu(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* �Ŕv��v���ɋL�^���� */
	if ((DiscardTileIndex.type == DiscardTileNum::Normal) ||
		(DiscardTileIndex.type == DiscardTileNum::Riichi) ||
		(DiscardTileIndex.type == DiscardTileNum::OpenRiichi))
		haifu::haifurecmota(gameStat, DiscardTileIndex);
	/* �Ԕv�𔲂����ꍇ�̏��� */
	if (!chkGameType(gameStat, SanmaS)) {
		RoundEndType = procDahaiSubFlower(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* �ÞȁE���Ȃ���Ƃ��̏��� */
	{
		RoundEndType = procDahaiSubKan(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	gameStat->KangFlag.kangFlag = false; // ���J�Ԃ̃t���O���~�낷
	gameStat->PaoFlag[pyMinkan].paoPlayer = gameStat->PaoFlag[pyMinkan].agariPlayer = -1;
	/* ����������Ƃ��̏��� */
	if ((DiscardTileIndex.type == DiscardTileNum::Riichi) ||
		(DiscardTileIndex.type == DiscardTileNum::OpenRiichi))
		procDahaiSubRiichi(gameStat, DiscardTileIndex);
	/* ���㏈�� */
	procDahaiSubPost(gameStat, DiscardTileIndex);
	return Continuing;
}

void tsumoproc(GameTable* const gameStat) {
	/* ���̃v���C���[���v�����̂� */
	++gameStat->TilePointer; gameStat->TianHuFlag = false;
	if (chkGameType(gameStat, SanmaT)) {
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % ACTUAL_PLAYERS;
	} else {
		if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, gameStat->CurrentPlayer.Active, gameStat->GameRound) == sWest)) /* �l�l�O���̏ꍇ�͖k�Ƃ��X�L�b�v */
			gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % PLAYERS;
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % PLAYERS;
	}
	/* ���Ƃ̏��Ԃ�����Ă����玟�̏��ڂƂȂ� */
	if (playerwind(gameStat, gameStat->CurrentPlayer.Active, gameStat->GameRound) == sEast)
		++gameStat->TurnRound;
	gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].tile =
		gameStat->Deck[gameStat->TilePointer].tile;
	gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].red =
		gameStat->Deck[gameStat->TilePointer].red;
	gameStat->PreviousMeld.Discard = gameStat->PreviousMeld.Stepped = NoTile;
	sound::Play(sound::IDs::sndTsumo);
	if (tilesLeft(gameStat) < 10)
		sound::Play(sound::IDs::sndCountdown);
	return;
}
