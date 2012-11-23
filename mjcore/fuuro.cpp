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
		sound::Play(sound::IDs::voxFlower);
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
		sound::Play(sound::IDs::voxFlower);
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
		sound::Play(sound::IDs::voxPon);
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
		sound::Play(sound::IDs::voxChi);
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
		sound::Play(sound::IDs::voxKan);
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
		sound::Play(sound::IDs::sndTsumo);
		if (tilesLeft(gameStat) < 10)
			sound::Play(sound::IDs::sndCountdown);
		if ((Mode == FuuroAnkan) || (Mode == FuuroKakan) || (Mode == FuuroDaiminkan)) {
			/* �ȃh�����߂��� */
			if (RuleData::chkRuleApplied("kandora")) {
				gameStat->DoraPointer -= 2;
				sound::Play(sound::IDs::sndMekuri);
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

#if 0
/* ��𔻒肷�� */
#module
#include "const.hsp"
#include "mjcore.hsp"
#deffunc checkpao var GameStat
	// ��̔��� ��������
	debug "��̏����𔻒肵�܂��B"
	DragonPons = 0: WindPons = 0: NumOfKangs = 0
	repeat MeldPointer(GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)), 1
		if (((getMeld(GameStat, MELD_TILECODE, cnt , getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE))\TILE_CODE_MAXIMUM) >= TILE_DRAGON_MIN)&&((getMeld(GameStat, MELD_TILECODE, cnt , getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE))\TILE_CODE_MAXIMUM) <= TILE_DRAGON_MAX)) {
			DragonPons++
		}
		if (((getMeld(GameStat, MELD_TILECODE, cnt , getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE))\TILE_CODE_MAXIMUM) >= TILE_WIND_MIN)&&((getMeld(GameStat, MELD_TILECODE, cnt , getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE))\TILE_CODE_MAXIMUM) <= TILE_WIND_MAX)) {
			WindPons++
		}
		if ((getMeld(GameStat, MELD_TILECODE, cnt , getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE))/MELD_TYPE_STEP) >= MELD_QUAD) {
			NumOfKangs++
		}
	loop
	if (chkRule("normal_pao", "yes")) {
		if ((DragonPons == 3)&&(getPao(GameStat, PAO_PLAYER_PAO, PAO_YAKU_DAISANGEN) == -1)) {
			debug strf("�v���C���[ [%d] �́A��O�����ɂȂ�܂��B", getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE))
			setPao GameStat, PAO_PLAYER_PAO, PAO_YAKU_DAISANGEN, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
			setPao GameStat, PAO_PLAYER_AGARI, PAO_YAKU_DAISANGEN, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)
		}
		if ((WindPons == 4)&&(getPao(GameStat, PAO_PLAYER_PAO, PAO_YAKU_DAISIXI) == -1)) {
			debug strf("�v���C���[ [%d] �́A��l����ɂȂ�܂��B", getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE))
			setPao GameStat, PAO_PLAYER_PAO, PAO_YAKU_DAISIXI, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
			setPao GameStat, PAO_PLAYER_AGARI, PAO_YAKU_DAISIXI, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)
		}
	}
	if (((chkRule("suukantsu", "yakuman_with_pao") != 0) || (chkRule("suukantsu", "double_yakuman_with_pao") != 0))&&(NumOfKangs == 4)&&(getPao(GameStat, PAO_PLAYER_PAO, PAO_YAKU_SIKANG) == -1)) {
		debug strf("�v���C���[ [%d] �́A�l�Ȏq���ɂȂ�܂��B", getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE))
		setPao GameStat, PAO_PLAYER_PAO, PAO_YAKU_SIKANG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
		setPao GameStat, PAO_PLAYER_AGARI, PAO_YAKU_SIKANG, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)
	}
	// ��̔��� �����܂�
	return
#global

/* �h�a�̂Ƃ��̏��� */
#module
#include "const.hsp"
#include "mjcore.hsp"
#include "socket.hsp"
#deffunc ronhuproc var GameStat, var GameEnv
	RoundEndType = -1
	info "���A�N�V������₢���킹�܂��B"
	/* �h�a����d�|�������邩�ǂ����₢���킹�� */
	resetDeclarationFlag GameStat
	repeat NUM_OF_PLAYERS
		if (getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE) != cnt) {
			setCurrentPlayer GameStat, CURRENTPLAYER_PASSIVE, cnt
			if (getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE) == getPlayer(GameStat)) {
				playerfuuro GameStat, GameEnv
			} else {
				if (IsRemotePlayer(GameEnv, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)) == 0) {
					/* COM���u�J���j���O�v���Ȃ��悤�ɏ��� */
					makesandbox Sandbox, GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)
					/* ���� */
					compfuuro_begin Sandbox
					repeat
						if (compfuuro_check()) {break}
						await 0
					loop
					compfuuro_end
					repeat NUM_OF_PLAYERS
						if (getDeclarationFlag(GameStat, DECLARATIONFLAG_RON, cnt) == 0) {setDeclarationFlag GameStat, DECLARATIONFLAG_RON, cnt, getDeclarationFlag(Sandbox, DECLARATIONFLAG_RON, cnt)}
						if (getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, cnt) == 0) {setDeclarationFlag GameStat, DECLARATIONFLAG_MINKAN, cnt, getDeclarationFlag(Sandbox, DECLARATIONFLAG_MINKAN, cnt)}
						if (getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, cnt) == 0) {setDeclarationFlag GameStat, DECLARATIONFLAG_PON, cnt, getDeclarationFlag(Sandbox, DECLARATIONFLAG_PON, cnt)}
						if (getDeclarationFlag(GameStat, DECLARATIONFLAG_CHI, cnt) == 0) {setDeclarationFlag GameStat, DECLARATIONFLAG_CHI, cnt, getDeclarationFlag(Sandbox, DECLARATIONFLAG_CHI, cnt)}
					loop
				}
			}
		} else {
			if (cnt == getPlayer(GameStat)) {
				if (getGameMode(GameEnv) == GAMEMODE_CLIENT) {clientsend 0xa0}
			}
		}
	loop
	/* �ʐM�ΐ펞�̏��� */
	if (getGameMode(GameEnv) != GAMEMODE_STANDALONE) {
		remotenaki GameStat, GameEnv
	}
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
	trace strf("���� [%d %d %d %d] �J�� [%d %d %d %d] �|�� [%d %d %d %d] �`�[ [%d %d %d %d]", getDeclarationFlag(GameStat, DECLARATIONFLAG_RON, 0),getDeclarationFlag(GameStat, DECLARATIONFLAG_RON, 1),getDeclarationFlag(GameStat, DECLARATIONFLAG_RON, 2),getDeclarationFlag(GameStat, DECLARATIONFLAG_RON, 3),getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 0),getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 1),getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 2),getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 3),getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 0),getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 1),getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 2),getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 3),getDeclarationFlag(GameStat, DECLARATIONFLAG_CHI, 0),getDeclarationFlag(GameStat, DECLARATIONFLAG_CHI, 1),getDeclarationFlag(GameStat, DECLARATIONFLAG_CHI, 2),getDeclarationFlag(GameStat, DECLARATIONFLAG_CHI, 3))
	/* �a��������̎��͐錾�𖳌��ɂ��� */
	repeat NUM_OF_PLAYERS
		if (getAgariHouki(GameStat, cnt) == 1) {
			debug strf("�v���C���[ [%d] �͘a��������ł��B�錾�t���O�����낵�܂��B", cnt)
			setDeclarationFlag GameStat, DECLARATIONFLAG_RON, cnt, 0: setDeclarationFlag GameStat, DECLARATIONFLAG_PON, cnt, 0
			setDeclarationFlag GameStat, DECLARATIONFLAG_CHI, cnt, 0: setDeclarationFlag GameStat, DECLARATIONFLAG_MINKAN, cnt, 0
		}
	loop
	/* �������悤�Ƃ���l��\��(���n�l�ŏR����悤�Ȑl���܂�) */
	repeat NUM_OF_PLAYERS
		if (getDeclarationFlag(GameStat, DECLARATIONFLAG_RON, cnt) == 1) {
			debug strf("�v���C���[ [%d] �́A�h�a��錾���܂��B", cnt)
			// �D�挠�̂Ȃ��������\�������悤�ɂ���
			setCall cnt, " ���� "
			setHandStat GameStat, cnt, 1
		}
	loop
	/* ���ۂɃ����ł���l��\�� */
	roncount = 0: repeat NUM_OF_PLAYERS-1
		if (getDeclarationFlag(GameStat, DECLARATIONFLAG_RON, RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1)) == 1) {
			if (roncount == 0) {
				debug strf("�v���C���[ [%d] �́A�h�a�ł��܂��B", RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1))
				setCall RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1), "����"
			}
			if ((roncount == 1)&&((chkRule("multiple_mahjong", "dual_mahjong_with_draw") != 0) || (chkRule("multiple_mahjong", "dual_mahjong") != 0) || (chkRule("multiple_mahjong", "trial_mahjong") != 0))) {
				debug strf("�v���C���[ [%d] �́A�h�a�ł��܂��B", RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1))
				setCall RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1), "����"
			}
			if ((roncount == 2)&&(chkRule("multiple_mahjong", "trial_mahjong") != 0)) {
				debug strf("�v���C���[ [%d] �́A�h�a�ł��܂��B", RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1))
				setCall RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1), "����"
			}
			roncount++
		}
	loop
	/* ���ۂɉh�a���s�Ȃ����� */
	repeat NUM_OF_PLAYERS-1
		await 0
		if (getDeclarationFlag(GameStat, DECLARATIONFLAG_RON, RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1)) == 1) {
			/* �E�H�b�`���[�h�̏ꍇ�͘a�������l�Ɏ��_�������� */
			if (GetWatchModeFlag(GameEnv) == 1) {
				setPlayer GameStat, RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1)
			}
			/* �h�a�������Ƃ�ϐ��ɋL�^ */
			RoundEndType = ENDKYOKU_AGARI: setTsumoAgariFlag GameStat, 0
			setCurrentPlayer GameStat, CURRENTPLAYER_FURIKOMI, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
			setCurrentPlayer GameStat, CURRENTPLAYER_AGARI, RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1)
			/* ���A���̔���Ɏg���ϐ� */
			if (chkRule("paarenchan", "no") == 0) {
				if (getLastAgariPlayer(GameStat) == getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI)) {
					incAgariChain GameStat
				} else { /* FIXME: ���A������̎��Ƀ_�u�����E�g�������������̏��� */
					if ((chkRule("breaking_paarenchan", "no") == 0)&&(getAgariChain(GameStat) == 7)) {setAgariChain GameStat, -1: setLastAgariPlayer GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI)}
					else {setAgariChain GameStat, 1: setLastAgariPlayer GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI)}
				}
			}
			/* �a����v��ݒ� */
			setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getCurrentDiscard(GameStat, CURRENTDISCARD_TILECODE)
			setHand GameStat, HAND_REDTILE, TSUMOHAI_INDEX, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getCurrentDiscard(GameStat, CURRENTDISCARD_REDTILE)
			if (getRichiFlag(GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)) == RIICHI_IPPATSU) {setRichiFlag GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), RIICHI_NO: addDeposit GameStat, -1: addScore GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), 0, 10: setRichiCounterFlag GameStat, 1}
			if (getRichiFlag(GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)) == RIICHI_DOUBLE_IPPATSU) {setRichiFlag GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), RIICHI_NO: addDeposit GameStat, -1: addScore GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), 0, 10: setRichiCounterFlag GameStat, 2}
			/* ����U���̔��� */
			countyaku GameStat, yakuInfo, RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1)
			chkfuriten FuritenFlag, MachihaiFlag, MachihaiCount, MachihaiTotal, MachiMen, GameStat, RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1)
			// ����𖞂����Ȃ����A�U���̂Ƃ�
			if ((getYakuInfo(yakuInfo, YAKUINF_HAN_BASE) <= getShibari(GameStat))||(FuritenFlag == 1)||(getDoujunFuriten(GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI)) == 1)||((chkRule("riichi_shibari", "yes") != 0)&&(getRichiFlag(GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI)) == RIICHI_NO))) {
				trace "����𖞂����Ȃ����U���ł��B���̏������`�����{�p�ɐ؂�ւ��܂��B"
				RoundEndType = ENDKYOKU_CHONBO // �`�����{�ɂ���
			}
			// �������������Ƃ�\��
			//setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getCurrentDiscard(GameStat, CURRENTDISCARD_TILECODE)
			//setHand GameStat, HAND_REDTILE, TSUMOHAI_INDEX, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getCurrentDiscard(GameStat, CURRENTDISCARD_REDTILE)
			setCall getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), "����"
			setHandStat GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), 1
			/* �����錾�v�ł̕��e�̏ꍇ�A�����𖳌��Ƃ������_�_��ԋp���� */
			if (getRichiFlag(GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)) == RIICHI_IPPATSU) {
				trace "�����錾�v�ł̕��e�̂��߁A�����_��Ԋ҂��܂��B"
				setRichiFlag GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), RIICHI_NO: addDeposit GameStat, -1: addScore GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), 0, 10: setRichiCounterFlag GameStat, 1
			}
			if (getRichiFlag(GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)) == RIICHI_DOUBLE_IPPATSU) {
				trace "�_�u�������錾�v�ł̕��e�̂��߁A�����_��Ԋ҂��܂��B"
				setRichiFlag GameStat, RICHI_FLAG, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), RIICHI_NO: addDeposit GameStat, -1: addScore GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), 0, 10: setRichiCounterFlag GameStat, 2
			}
			/* �a����v��ݒ� */
			setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getCurrentDiscard(GameStat, CURRENTDISCARD_TILECODE)
			setHand GameStat, HAND_REDTILE, TSUMOHAI_INDEX, getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getCurrentDiscard(GameStat, CURRENTDISCARD_REDTILE)
			/* �h�a�̃T�E���h��炷 */
			if (getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE) == getPlayer(GameStat)) {
				dsplay@ VOX_RON_FURIKOMI
			} else {
				dsplay@ VOX_RON
			}
			/* ��ʍX�V���Ė߂� */
			redrscreen
			break
		}
	loop
	/* �v���ɋL�^ */
	roncount = 0
	if (RonPlayers(GameStat) > 0) {
		if (getKangFlag(GameStat, KANG_CHANKAN) > 0) {
#ifdef SANMAX
			if (getKangFlag(GameStat, KANG_CHANKAN) == 3) {
				haifurecchanpei GameStat
			} else {
#endif
				haifurecchankan GameStat
#ifdef SANMAX
			}
#endif
		} else {
			haifurecfurikomi GameStat
		}
	} else {
		if (getKangFlag(GameStat, KANG_FLAG) == 0) { // �����k�Ŕv���������̂�}�~
			haifurecfurikomi GameStat
		}
	}
#ifdef ALLSANMA
	// ��Ƙa�̔���
	if ((RonPlayers(GameStat) >= 2)&&(chkRule("multiple_mahjong", "aborted") != 0)) {
		RoundEndType = ENDKYOKU_TRIPLERON: return RoundEndType
	}
#else
	// �O�Ƙa�̔���
	if ((RonPlayers(GameStat) >= 3)&&((chkRule("multiple_mahjong", "single_mahjong_with_draw") != 0)||(chkRule("multiple_mahjong", "dual_mahjong_with_draw") != 0))) {
		RoundEndType = ENDKYOKU_TRIPLERON: return RoundEndType
	}
#endif
return RoundEndType
#global

#module
#include "const.hsp"
#include "mjcore.hsp"
#deffunc executeFuuro var GameStat, int DiscardTileIndex
	/* �̔v���|���A�܂��͑喾�Ȃ���ꍇ�̏��� */
	/* �����v�͂S�������Ȃ��̂ŁA�|���Ɩ��Ȃ͓����ɋN���邱�Ƃ��Ȃ� */
	if (getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 0)+getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 1)+getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 2)+getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 3)+getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 0)+getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 1)+getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 2)+getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 3) > 1) {
		error "���������̃|���E�Ȃ��錾����Ă��܂��B"
	}
	repeat NUM_OF_PLAYERS: await 0
		/* �̔v���|������ꍇ */
		if (getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, cnt) == 1) {
			setCurrentPlayer GameStat, CURRENTPLAYER_PASSIVE, cnt // �����v���C���[��ݒ�
			fuuroproc GameStat, GameEnv, RoundEndType, DiscardTileIndex, FUURO_PON
			break
		}
		/* ���Ȃ̏ꍇ */
		if (chkRule("fifth_kong", "no")) {kanLim = 4} else {kanLim = 5}
		if ((getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, cnt) == 1)&&(getTotalKang(GameStat) < kanLim)) {
			setCurrentPlayer GameStat, CURRENTPLAYER_PASSIVE, cnt // �����v���C���[��ݒ�
			fuuroproc GameStat, GameEnv, RoundEndType, DiscardTileIndex, FUURO_DAIMINKAN
			break
		}
	loop
	/* �h�ƃ|���������ɋN�������ꍇ�A�|����D�悷�� */
	if ((getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 0)+getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 1)+getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 2)+getDeclarationFlag(GameStat, DECLARATIONFLAG_PON, 3)+getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 0)+getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 1)+getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 2)+getDeclarationFlag(GameStat, DECLARATIONFLAG_MINKAN, 3)) > 0) {
		/* �|����Ȃ̎��̓c�������΂����Ƃ݂Ȃ��Đ����A�k�Ɓ����Ƃ��܂������ꍇ�͎��̏��ڂƂ��Ĉ��� */
		if (playerWind(getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE), getRound(GameStat)) < playerWind(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), getRound(GameStat))) {
			incTurn GameStat
		}
		setCurrentPlayer GameStat, CURRENTPLAYER_ACTIVE, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)
		return 1
#ifndef ALLSANMA
	} else {
		/* �h�̏��� */
		/* �O�l�ł��ł͋h�Ȃ� */
		if (getDeclarationFlag(GameStat, DECLARATIONFLAG_CHI, ShimochaOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE))) > 0) {
			/* �|����Ȃ̎��̓c�������΂����Ƃ݂Ȃ��Đ����A�k�Ɓ����Ƃ��܂������ꍇ�͎��̏��ڂƂ��Ĉ��� */
			if (playerWind(getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE), getRound(GameStat)) < playerWind(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), getRound(GameStat))) {
				incTurn GameStat
			}
			setCurrentPlayer GameStat, CURRENTPLAYER_PASSIVE, ShimochaOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)) // �h���ł���̂͏�Ƃ̎̔v�̂�
			fuuroproc GameStat, GameEnv, RoundEndType, DiscardTileIndex, FUURO_CHII
			setCurrentPlayer GameStat, CURRENTPLAYER_ACTIVE, getCurrentPlayer(GameStat, CURRENTPLAYER_PASSIVE)
			return 1
		}
#endif
	}
	/* �o�O�h�~�̃A�� */
	repeat NUM_OF_PLAYERS: setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, cnt, 0: loop
	return 0
#global
#endif
