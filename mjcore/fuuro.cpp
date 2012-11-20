#include "fuuro.h"

/* �v���ւ̋L�^�ȂǏ������� */
PLAYER_ID PrepareFuuro(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex, FuuroType Mode) {
	PLAYER_ID fuuroPlayer;
	switch (Mode) {
	case FuuroChii:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		for (int i = 0; i < 4; i++) // �ԃh���o�O����̂���
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecchi(gameStat); // �v���ɋL�^
		break;
	case FuuroPon:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		for (int i = 0; i < 4; i++) // �ԃh���o�O����̂���
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecpon(gameStat); // �v���ɋL�^
		break;
	case FuuroFlower:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // ���J�Ԃ̃t���O�𗧂Ă�
		haifu::haifurecflower(gameStat, DiscardTileIndex); // �v���ɋL�^
		break;
	case FuuroNorth:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // ���J�Ԃ̃t���O�𗧂Ă�
		haifu::haifurecnorth(gameStat, DiscardTileIndex); // �v���ɋL�^
		break;
	case FuuroDaiminkan:
		fuuroPlayer = gameStat->CurrentPlayer.Passive;
		gameStat->KangFlag.kangFlag = 1; // ���J�Ԃ̃t���O�𗧂Ă�
		for (int i = 0; i < 4; i++) // �ԃh���o�O����̂���
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecminkan(gameStat);
		gameStat->KangFlag.chankanFlag++; // �A���Ȃ̉񐔂��L�^
		if (gameStat->Player[fuuroPlayer].FirstDrawFlag)
			gameStat->KangFlag.topFlag = 1; /* ���Șa�̃t���O */
		if (RuleData::chkRule("minkan_penalty", "yes")) { /* �喾�Ȃ������1000�_�����ɂȂ郋�[�� */
			/* TODO: ���_�̃`�F�b�N */
			gameStat->Deposit++;
			gameStat->Player[fuuroPlayer].PlayerScore =
				gameStat->Player[fuuroPlayer].PlayerScore - LargeNum::fromInt(1000);
		}
		break;
	case FuuroKakan: case FuuroAnkan:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // ���J�Ԃ̃t���O�𗧂Ă�
		for (int i = 0; i < 4; i++) // �ԃh���o�O����̂���
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecankan(gameStat, DiscardTileIndex); // �ÞȂ����ɔv���ɋL�^
		gameStat->KangFlag.chankanFlag++; // �A���Ȃ̉񐔂��L�^
		if (gameStat->Player[fuuroPlayer].FirstDrawFlag)
			gameStat->KangFlag.topFlag = 1; /* ���Șa�̃t���O */
		break;
	default:
		fuuroPlayer = -1;
	}
	return fuuroPlayer;
}

/* �ÞȁE���Ȃ����s����(���̑��̖��̏������s��) */
void MakeMeld(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex, FuuroType Mode, PLAYER_ID kangPlayer) {
	unsigned nakiCount = 0;
	switch (Mode) {
	case FuuroAnkan: /* �Þ� */
		++gameStat->Player[kangPlayer].MeldPointer;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadConcealed;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if (gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) {
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[nakiCount] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i].tile = NoTile;
				gameStat->Player[kangPlayer].Hand[i].red = Normal;
				++nakiCount;
			}
		}
		if (nakiCount != 4) {
			CodeConv::tostringstream o;
			o << _T("�ÞȂ��邽�߂̔v��") <<
				((nakiCount < 4) ? _T("����܂���") : _T("�������܂�")) <<
				_T("�B�v�R�[�h [") << (int)gameStat->CurrentDiscard.tile << _T("] �� [") <<
				nakiCount << _T("] ��") <<
				((nakiCount < 4) ? _T("��������܂���B") : _T("����܂��B"));
			error(o.str().c_str());
		}
		break;
	case FuuroKakan: /* ���� */
		for (unsigned i = 1; i <= gameStat->Player[kangPlayer].MeldPointer; i++) {
			if (gameStat->Player[kangPlayer].Meld[i].tile == gameStat->CurrentDiscard.tile) {
				bool ProcessFlag = false;
				switch (gameStat->Player[kangPlayer].Meld[i].mstat) {
				case meldTripletExposedLeft:
					gameStat->Player[kangPlayer].Meld[i].mstat = meldQuadAddedLeft;
					ProcessFlag = true; break;
				case meldTripletExposedCenter:
					gameStat->Player[kangPlayer].Meld[i].mstat = meldQuadAddedCenter;
					ProcessFlag = true; break;
				case meldTripletExposedRight:
					gameStat->Player[kangPlayer].Meld[i].mstat = meldQuadAddedRight;
					ProcessFlag = true; break;
				}
				if (ProcessFlag) {
					gameStat->Player[kangPlayer].Meld[i].red[3] = gameStat->CurrentDiscard.red;
					++nakiCount;
				}
			}
		}
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].red = Normal;
		if (nakiCount != 1) {
			CodeConv::tostringstream o;
			o << _T("���Ȃ��邽�߂̖����q��") <<
				(nakiCount ? _T("����������܂����B�ʎq�f�[�^�ɕs�������������Ă��܂�") : _T("������܂���")) <<
				_T("�B�v�R�[�h [") << (int)gameStat->CurrentDiscard.tile << _T("]");
			error(o.str().c_str());
		}
		break;
	case FuuroDaiminkan: /* �喾�� */
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if ((gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) && (nakiCount < 3)) {
				// �����v������v�������
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[nakiCount + 1] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i].tile = NoTile;
				gameStat->Player[kangPlayer].Hand[i].red = Normal;
				++nakiCount;
			}
		}
		if (nakiCount != 3) {
			CodeConv::tostringstream o;
			o << _T("���Ȃ��邽�߂̔v��") <<
				((nakiCount < 3) ? _T("����܂���") : _T("�������܂�")) <<
				_T("�B�v�R�[�h [") << (int)gameStat->CurrentDiscard.tile << _T("] �� [") <<
				nakiCount << _T("] ��") <<
				((nakiCount < 3) ? _T("��������܂���B") : _T("����܂��B"));
			error(o.str().c_str());
		}
		/* ��O���t���O���~�낷 */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* �Ȏq�Ƃ��ĎN�� */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sLeft)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedLeft;
		else if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sOpposite)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedCenter;
		else if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sRight)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldQuadExposedRight;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[0] = gameStat->CurrentDiscard.red;
		/* ���v���� */
		lipai(gameStat, kangPlayer);
		break;
	case FuuroNorth: /* �O�������h�� */
		/* TODO: ���ʉ���炷 dsplay@ VOX_FLOWER */
		++(gameStat->Player[kangPlayer].NorthFlag);
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].red = Normal;
		gameStat->TianHuFlag = false;
		/* TODO: ���� setCall kangPlayer, "�k" */
		lipai(gameStat, kangPlayer);
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			gameStat->Player[i].FirstDrawFlag = false;
		break;
	case FuuroFlower: /* �Ԕv */
		/* TODO: ���ʉ���炷 dsplay@ VOX_FLOWER */
		switch (gameStat->CurrentDiscard.tile) {
			case Spring:        gameStat->Player[kangPlayer].FlowerFlag.Spring = true; break;
			case Summer:        gameStat->Player[kangPlayer].FlowerFlag.Summer = true; break;
			case Autumn:        gameStat->Player[kangPlayer].FlowerFlag.Autumn = true; break;
			case Winter:        gameStat->Player[kangPlayer].FlowerFlag.Winter = true; break;
			case Plum:          gameStat->Player[kangPlayer].FlowerFlag.Plum   = true; break;
			case Orchid:        gameStat->Player[kangPlayer].FlowerFlag.Orchid = true; break;
			case Chrysanthemum: gameStat->Player[kangPlayer].FlowerFlag.Chrys  = true; break;
			case Bamboo:        gameStat->Player[kangPlayer].FlowerFlag.Bamboo = true; break;
		default:
			{
				CodeConv::tostringstream o;
				o << _T("�v�R�[�h [") << (int)gameStat->CurrentDiscard.tile << _T("] �͉Ԕv�ł͂���܂���B");
				warn(o.str().c_str());
			}
		}
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->Player[kangPlayer].Hand[DiscardTileIndex.id].red = Normal;
		gameStat->TianHuFlag = false;
		/* TODO: ���� setCall kangPlayer, "��" */
		lipai(gameStat, kangPlayer);
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			gameStat->Player[i].FirstDrawFlag = false;
		break;
	case FuuroPon: /* �|�� */
		/* �|�������Ύq������ */
		nakiCount = 0;
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if ((gameStat->Player[kangPlayer].Hand[i].tile == gameStat->CurrentDiscard.tile) && (nakiCount < 2)) {
				// �����v������v�������
				gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[nakiCount + 1] = gameStat->Player[kangPlayer].Hand[i].red;
				gameStat->Player[kangPlayer].Hand[i].tile = NoTile;
				gameStat->Player[kangPlayer].Hand[i].red = Normal;
				++nakiCount;
			}
		}
		if (nakiCount < 2) {
			CodeConv::tostringstream o;
			o << _T("�|�����邽�߂̔v������܂���B�v�R�[�h [") << (int)gameStat->CurrentDiscard.tile <<
				_T("] �� [") << nakiCount << _T("] ����������܂���B");
			error(o.str().c_str());
		}
		/* ��O���t���O���~�낷 */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* �����Ƃ��ĎN�� */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sLeft)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedLeft;
		else if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sOpposite)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedCenter;
		else if (playerRelative(kangPlayer, gameStat->CurrentPlayer.Active) == sRight)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldTripletExposedRight;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile = gameStat->CurrentDiscard.tile;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].red[0] = gameStat->CurrentDiscard.red;
		/* ���v���� */
		lipai(gameStat, kangPlayer);
		/* �|����錾���� */
		/* TODO: ���ʉ���炷 dsplay@ VOX_PON */
		/* TODO: ���� setCall getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE), "�|��"*/
		/* �򂢑ւ��̔���Ɏg���ϐ���ݒ� */
		gameStat->PreviousMeld.Discard = gameStat->CurrentDiscard.tile;
		gameStat->PreviousMeld.Stepped = NoTile;
		/* ��̔��� */
		/* TODO: ��̔��� checkpao GameStat */
		break;
	case FuuroChii: /* �`�[ */
		/* �`�[����v��ݒ� */
		gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
		gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].red = gameStat->CurrentDiscard.red;
		/* ����v����������q������ */
		{
			bool nakiCount[3] = {false};
			for (int i = NUM_OF_TILES_IN_HAND - 1; i < NUM_OF_TILES_IN_HAND * 2 - 1; i++) {
				for (unsigned j = 0; j < 3; j++) {
					if ((!nakiCount[j]) && (gameStat->Player[kangPlayer].Hand[i % NUM_OF_TILES_IN_HAND].tile ==
						(gameStat->CurrentDiscard.tile + j + 1 - gameStat->Player[kangPlayer].DeclarationFlag.Chi))) {
							gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer + 1].red[j] = gameStat->Player[kangPlayer].Hand[i % NUM_OF_TILES_IN_HAND].red;
							gameStat->Player[kangPlayer].Hand[i % NUM_OF_TILES_IN_HAND].tile = NoTile;
							gameStat->Player[kangPlayer].Hand[i % NUM_OF_TILES_IN_HAND].red = Normal;
							nakiCount[j] = true;
					}
				}
			}
			assert(gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile == NoTile);
		}
		/* ��O�t���O���~�낷 */
		gameStat->Player[kangPlayer].MenzenFlag = false;
		/* ���q���N�� */
		++gameStat->Player[kangPlayer].MeldPointer;
		if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == 1)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedLower;
		else if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == 2)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedMiddle;
		else if (gameStat->Player[kangPlayer].DeclarationFlag.Chi == 3)
			gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].mstat = meldSequenceExposedUpper;
		gameStat->Player[kangPlayer].Meld[gameStat->Player[kangPlayer].MeldPointer].tile =
			(tileCode)(gameStat->CurrentDiscard.tile + 1 - gameStat->Player[kangPlayer].DeclarationFlag.Chi);
		/* �������v */
		lipai(gameStat, kangPlayer);
		/* �`�[��錾 */
		/* TODO: ���ʉ���炷 dsplay@ VOX_CHI */
		/* TODO: ���� setCall getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE), "�`�[" */
		/* �򂢑ւ�����p�̕ϐ����w�� */
		gameStat->PreviousMeld.Discard = gameStat->CurrentDiscard.tile;
		if (RuleData::chkRule("kuikae", "agari_houki") || RuleData::chkRule("kuikae", "chombo")) {
			switch (gameStat->Player[kangPlayer].DeclarationFlag.Chi) {
				case 1: gameStat->PreviousMeld.Stepped = (tileCode)(gameStat->CurrentDiscard.tile + 3); break;
				case 2: gameStat->PreviousMeld.Stepped = NoTile; break;
				case 3: gameStat->PreviousMeld.Stepped = (tileCode)(gameStat->CurrentDiscard.tile - 3); break;
			}
		} else {
			gameStat->PreviousMeld.Stepped = NoTile;
		}
		break;
	default:
		{
			CodeConv::tostringstream o;
			o << _T("���[�h������������܂��� [") << Mode << _T("]");
			error(o.str().c_str());
		}
	}

	/* �ÞȁE���Ȃ����s���� */
	nakiCount = 0;
	if ((Mode == FuuroChii) || (Mode == FuuroPon) || (Mode == FuuroDaiminkan)) {
		/* �����̔v���͂Ŕ�\���ɂ��� */
		discardTile* tmpSutehaiVar = &gameStat->Player[gameStat->CurrentPlayer.Active].Discard[gameStat->Player[gameStat->CurrentPlayer.Active].DiscardPointer];
		if (tmpSutehaiVar->dstat == discardNormal)
			tmpSutehaiVar->dstat = discardTaken;
		else if (tmpSutehaiVar->dstat == discardRiichi)
			tmpSutehaiVar->dstat = discardRiichiTaken;
	}
	/* �J����錾�A�V�a��n�a�̃t���O�������Ă���΍~�낷 */
	if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
		gameStat->TianHuFlag = false;
		/* TODO: ���ʉ���炷 dsplay@ VOX_KAN */
		/* TODO: ���� setCall kangPlayer, "�J��" */
		lipai(gameStat, kangPlayer);
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			gameStat->Player[i].FirstDrawFlag = false;
	}
}

/* ���Ȃ̔��� */
bool CheckChankan(GameTable* const gameStat, EndType* RoundEndType, FuuroType Mode) {
	if ((Mode != FuuroDaiminkan) && (Mode != FuuroPon) && (Mode != FuuroChii)) {
		/* ���m���o�Ɍ���A�ÞȂ𝏞ȉ\ */
		/* �f���v�̈ÞȂ͝��Ȃ��悤���Ȃ���ł����A�ʓ|�Ȃ̂ŋ�ʂ�������
		   �ÞȔ�J���ݒ�̎��͉����ÞȂ�����������Ȃ��̂ŏ������X�L�b�v*/
		/* ���Ȃ̔��� �������� */
		/* ���Ȃ̍ۂ̒��ӎ����B���X�̃|���������̎̔v�̂��̂̏ꍇ
		   �U���ƂȂ�̂ŝ��Ȃ͂ł��܂��� */
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroNorth)) {
			if ((Mode == FuuroKakan) || (Mode == FuuroNorth) ||
				(RuleData::chkRule("ankan_conceal", "open") && RuleData::chkRule("ankan_chankan", "yes"))) {
					debug(_T("���Ȃ̔�������܂��B"));
					if (Mode == FuuroAnkan) gameStat->KangFlag.chankanFlag = 2;
					else if (Mode == FuuroNorth) gameStat->KangFlag.chankanFlag = 3;
					else gameStat->KangFlag.chankanFlag = 1;
					/* TODO: �o�a���蔻�� ronhuproc GameStat, GameEnv: RoundEndType = stat */
					if (Mode == FuuroNorth) gameStat->KangFlag.chankanFlag = 0;
					if (*RoundEndType == TripleRon) return true;
					if (RonPlayers(gameStat) > 0) {
						info(_T("���Ȃ��錾����܂����B���[�v����o�܂��B"));
						return true;
					}
					gameStat->KangFlag.chankanFlag = 0;
			}
		}
		/* ���Ȃ̔��� �����܂� */
		debug(_T("���I�ɐ������܂����B"));
		switch (Mode) {
			case FuuroAnkan: case FuuroKakan:
				haifu::haifureckanproc(gameStat); // ���Ȃ������ꍇ�A�v���ɋL�^
				break;
			case FuuroNorth:
				haifu::haifurecnorthproc(gameStat);
				break;
		}
	}
	if (Mode == FuuroDaiminkan) {
		/* �o�O�h�~�̈� */
		for (PLAYER_ID i = 0; i < PLAYERS; i++) {
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND].tile = NoTile;
			gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND].red = Normal;
		}
		/* �喾�Ȃ̕��p */
		if ((!RuleData::chkRule("minkan_pao", "no")) && (!RuleData::chkRule("minkan_pao", "no_but_2han"))) {
			/* TODO: ��̐ݒ�
				setPao GameStat, PAO_PLAYER_PAO, PAO_YAKU_MINKAN, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
				setPao GameStat, PAO_PLAYER_AGARI, PAO_YAKU_MINKAN, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)
			*/
		}
	}
	return false;
}

/* ���̏��� */
bool ProcRinshan(GameTable* const gameStat, EndType* RoundEndType, FuuroType Mode, PLAYER_ID kangPlayer) {
	if ((Mode != FuuroPon) && (Mode != FuuroChii)) {
		/* �l�Ȏq�̒��v�҂����āA�T��ڂ̞Ȃ�����Ɨ��ǂƂ��郋�[���̏ꍇ
		   ���̏����𔻒肷��*/
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
			if (gameStat->KangNum >= 4) {
				debug(_T("5�ڂ̞ȂȂ̂Œ����ɗ��ǂƂ��A���[�v����o�܂��B"));
				*RoundEndType = Uukaikan; return true;
			}
		}
		if (gameStat->TilePointer >= (gameStat->RinshanPointer - (gameStat->DeadTiles - 1))) {
			*RoundEndType = Ryuukyoku; return true; /* �r�v�Ȃ�I�� */
		}
		/* ���v�����̂� */
		gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->Deck[gameStat->RinshanPointer].tile;
		gameStat->Player[kangPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].red = gameStat->Deck[gameStat->RinshanPointer].red;
		--gameStat->RinshanPointer;
		/* TODO: ���ʉ���炷
		dsplay@ SND_TSUMO
		if (tilesLeft(GameStat) < 10) {
			dsplay@ SND_COUNTDOWN
		}
		*/
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
			/* �ȃh�����߂��� */
			if (RuleData::chkRuleApplied("kandora")) {
				gameStat->DoraPointer -= 2;
				/* TODO: ���ʉ���炷 dsplay@ SND_MEKURI */
				setdora(gameStat, 0); // �\�h����ݒ肷��
				if (RuleData::chkRuleApplied("kandora") && RuleData::chkRuleApplied("uradora")) {
					setdora(gameStat, 1); // ���h����ݒ肷��
				}
				haifu::haifurecdorap();
			}
			/* ��̔��� */
			/* TODO: ��̔��� checkpao GameStat */
		}
		/* �Ȏq���J�E���g����(�l�J�Ȃ̔���̂���) */
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
			++gameStat->Player[kangPlayer].NumberOfQuads;
			++gameStat->KangNum;
			CodeConv::tostringstream o;
			o << _T("�v���C���[ [") << (int)kangPlayer << _T("] �� [") <<
				(int)gameStat->Player[kangPlayer].NumberOfQuads << _T("] ��J�ȁA�S���̍��v�� [") <<
				(int)gameStat->KangNum << _T("] ��J�Ȃ��Ă��܂��B");
			debug(o.str().c_str());
		}
	}
	return false;
}

/* ���I�̏��� */
bool fuuroproc(GameTable* const gameStat, EndType* RoundEndType, const DiscardTileNum& DiscardTileIndex, FuuroType Mode) {
	{
		CodeConv::tostringstream o;
		o << _T("���I�̏������J�n���܂��B���[�h[") << (int)Mode << _T("]");
		info(o.str().c_str());
	}
	PLAYER_ID fuuroPlayer = PrepareFuuro(gameStat, DiscardTileIndex, Mode);

	if ((Mode != FuuroPon)&&(Mode != FuuroChii)&&(Mode != FuuroDaiminkan)) { /* �J������v��ݒ� */
		gameStat->CurrentDiscard.tile = gameStat->Player[fuuroPlayer].Hand[DiscardTileIndex.id].tile;
		gameStat->CurrentDiscard.red = gameStat->Player[fuuroPlayer].Hand[DiscardTileIndex.id].red;
	}

	MakeMeld(gameStat, DiscardTileIndex, Mode, fuuroPlayer);
	/* TODO: ��ʂ̍č\�z redrscreen */
	if (CheckChankan(gameStat, RoundEndType, Mode)) return true;
	/* TODO: ��ʂ̍č\�z redrscreen */
	/* TODO: 1�b�҂� await 1000 */
	if (ProcRinshan(gameStat, RoundEndType, Mode, fuuroPlayer)) return true;
	/* ���㏈�� */
	/* TODO: ��������������� repeat NUM_OF_PLAYERS: setCall cnt, "": loop */
	/* �ĕ`�� */
	/* TODO: ��ʂ̍č\�z redrscreen */
	/* �������������̂ŁA�ꔭ�̃t���O���~�낷 */
	for (PLAYER_ID i = 0; i < PLAYERS; i++)
		gameStat->Player[i].RichiFlag.IppatsuFlag =
		gameStat->Player[i].FirstDrawFlag = false;
	return false;
}
