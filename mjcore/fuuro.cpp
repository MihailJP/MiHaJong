#include "fuuro.h"

namespace {

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
		gameStat->KangFlag.chainFlag++; // �A���Ȃ̉񐔂��L�^
		if (gameStat->Player[fuuroPlayer].FirstDrawFlag)
			gameStat->KangFlag.topFlag = 1; /* ���Șa�̃t���O */
		if (RuleData::chkRule("minkan_penalty", "yes")) { /* �喾�Ȃ������1000�_�����ɂȂ郋�[�� */
			/* TODO: ���_�̃`�F�b�N */
			gameStat->Deposit++;
			gameStat->Player[fuuroPlayer].PlayerScore -= (LNum)1000;
		}
		break;
	case FuuroKakan: case FuuroAnkan:
		fuuroPlayer = gameStat->CurrentPlayer.Active;
		gameStat->KangFlag.kangFlag = 1; // ���J�Ԃ̃t���O�𗧂Ă�
		for (int i = 0; i < 4; i++) // �ԃh���o�O����̂���
			gameStat->Player[fuuroPlayer].Meld[gameStat->Player[fuuroPlayer].MeldPointer + 1].red[i] = Normal;
		haifu::haifurecankan(gameStat, DiscardTileIndex); // �ÞȂ����ɔv���ɋL�^
		gameStat->KangFlag.chainFlag++; // �A���Ȃ̉񐔂��L�^
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
		checkpao(gameStat);
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
					if (Mode == FuuroAnkan) gameStat->KangFlag.chankanFlag = chankanOfAnkan;
					else if (Mode == FuuroNorth) gameStat->KangFlag.chankanFlag = chankanOfNorth;
					else gameStat->KangFlag.chankanFlag = chankanRegular;
					*RoundEndType = ronhuproc(gameStat);
					if (Mode == FuuroNorth) gameStat->KangFlag.chankanFlag = chankanNone;
					if (*RoundEndType == TripleRon) return true;
					if (RonPlayers(gameStat) > 0) {
						info(_T("���Ȃ��錾����܂����B���[�v����o�܂��B"));
						return true;
					}
					gameStat->KangFlag.chankanFlag = chankanNone;
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
			gameStat->PaoFlag[pyMinkan].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyMinkan].agariPlayer = gameStat->CurrentPlayer.Passive;
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
			checkpao(gameStat);
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

} /* namespace */

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

/* ��𔻒肷�� */
void checkpao(GameTable* const gameStat) {
	// ��̔��� ��������
	debug(_T("��̏����𔻒肵�܂��B"));
	unsigned DragonPons = 0, WindPons = 0, NumOfKangs = 0;
	const PlayerTable* playerStat = &gameStat->Player[gameStat->CurrentPlayer.Passive];
	for (unsigned i = 1; i <= playerStat->MeldPointer; i++) {
		switch (playerStat->Meld[i].tile) {
		case WhiteDragon: case GreenDragon: case RedDragon:
			++DragonPons; break;
		case EastWind: case SouthWind: case WestWind: case NorthWind:
			++WindPons; break;
		}
		switch (playerStat->Meld[i].mstat) {
		case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight:
		case meldQuadAddedLeft:   case meldQuadAddedCenter:   case meldQuadAddedRight:
		case meldQuadConcealed:
			++NumOfKangs; break;
		}
	}
	CodeConv::tostringstream o;
	if (RuleData::chkRuleApplied("normal_pao")) {
		if ((DragonPons == 3) && (gameStat->PaoFlag[pyDaisangen].paoPlayer == -1)) {
			o << _T("�v���C���[ [") << (int)gameStat->CurrentPlayer.Active << _T("] �́A��O�����ɂȂ�܂��B");
			debug(o.str().c_str());
			gameStat->PaoFlag[pyDaisangen].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyDaisangen].agariPlayer = gameStat->CurrentPlayer.Passive;
		}
		if ((WindPons == 4) && (gameStat->PaoFlag[pyDaisixi].paoPlayer == -1)) {
			o << _T("�v���C���[ [") << (int)gameStat->CurrentPlayer.Active << _T("] �́A��l����ɂȂ�܂��B");
			debug(o.str().c_str());
			gameStat->PaoFlag[pyDaisixi].paoPlayer = gameStat->CurrentPlayer.Active;
			gameStat->PaoFlag[pyDaisixi].agariPlayer = gameStat->CurrentPlayer.Passive;
		}
	}
	if ((RuleData::chkRule("suukantsu", "yakuman_with_pao") || RuleData::chkRule("suukantsu", "double_yakuman_with_pao")) &&
		(NumOfKangs == 4) && (gameStat->PaoFlag[pySikang].paoPlayer == -1)) {
		o << _T("�v���C���[ [") << (int)gameStat->CurrentPlayer.Active << _T("] �́A�l�Ȏq���ɂȂ�܂��B");
		debug(o.str().c_str());
		gameStat->PaoFlag[pySikang].paoPlayer = gameStat->CurrentPlayer.Active;
		gameStat->PaoFlag[pySikang].agariPlayer = gameStat->CurrentPlayer.Passive;
	}
	// ��̔��� �����܂�
	return;
}

/* �h�a�̂Ƃ��̏��� */
EndType ronhuproc(GameTable* const gameStat) {
	EndType RoundEndType = Continuing;
	info(_T("���A�N�V������₢���킹�܂��B"));
	/* �h�a����d�|�������邩�ǂ����₢���킹�� */
	for (int pl = 0; pl < PLAYERS; pl++) {
		gameStat->Player[pl].DeclarationFlag.Ron =
			gameStat->Player[pl].DeclarationFlag.Pon =
			gameStat->Player[pl].DeclarationFlag.Kan = false;
		gameStat->Player[pl].DeclarationFlag.Chi = chiiNone;
	}
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		if (gameStat->CurrentPlayer.Active != i) {
			gameStat->CurrentPlayer.Passive = i;
			if (gameStat->CurrentPlayer.Passive == gameStat->PlayerID) {
				/* TODO: �v���C���[���I�� playerfuuro GameStat, GameEnv */
			} else if (EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Passive].RemotePlayerFlag == 0) {
				/* COM���u�J���j���O�v���Ȃ��悤�ɏ��� */
				GameTable* sandbox = makesandBox(gameStat, gameStat->CurrentPlayer.Passive);
				/* ���� */
				aiscript::compfuuro(sandbox);
				for (PLAYER_ID j = 0; j < PLAYERS; j++) {
					if (!gameStat->Player[j].DeclarationFlag.Ron) gameStat->Player[j].DeclarationFlag.Ron = sandbox->Player[j].DeclarationFlag.Ron;
					if (!gameStat->Player[j].DeclarationFlag.Kan) gameStat->Player[j].DeclarationFlag.Kan = sandbox->Player[j].DeclarationFlag.Kan;
					if (!gameStat->Player[j].DeclarationFlag.Pon) gameStat->Player[j].DeclarationFlag.Pon = sandbox->Player[j].DeclarationFlag.Pon;
					if (gameStat->Player[j].DeclarationFlag.Chi == chiiNone) gameStat->Player[j].DeclarationFlag.Chi = sandbox->Player[j].DeclarationFlag.Chi;
				}
			}
		} else if (i == gameStat->PlayerID) {
			if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
				mihajong_socket::client::send(mihajong_socket::protocol::Naki_Ignore);
		}
	}
	/* �ʐM�ΐ펞�̏��� */
	if (EnvTable::Instantiate()->GameMode != EnvTable::Standalone)
		RemoteAction::remotenaki(gameStat);
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
	{
		CodeConv::tostringstream o;
		o << _T("���� [");
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			o << (i ? _T(" ") : _T("")) << (int)gameStat->Player[i].DeclarationFlag.Ron;
		o << _T("] �J�� [");
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			o << (i ? _T(" ") : _T("")) << (int)gameStat->Player[i].DeclarationFlag.Kan;
		o << _T("] �|�� [");
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			o << (i ? _T(" ") : _T("")) << (int)gameStat->Player[i].DeclarationFlag.Pon;
		o << _T("] �`�[ [");
		for (PLAYER_ID i = 0; i < PLAYERS; i++)
			o << (i ? _T(" ") : _T("")) << (int)gameStat->Player[i].DeclarationFlag.Chi;
		o << _T("]"); trace(o.str().c_str());
	}
	/* �a��������̎��͐錾�𖳌��ɂ��� */
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].AgariHouki) {
			{
				CodeConv::tostringstream o;
				o << _T("�v���C���[ [") << (int)i << _T("] �͘a��������ł��B�錾�t���O�����낵�܂��B");
				debug(o.str().c_str());
			}
			gameStat->Player[i].DeclarationFlag.Ron = gameStat->Player[i].DeclarationFlag.Pon =
				gameStat->Player[i].DeclarationFlag.Kan = false;
			gameStat->Player[i].DeclarationFlag.Chi = chiiNone;
		}
	}
	/* �������悤�Ƃ���l��\��(���n�l�ŏR����悤�Ȑl���܂�) */
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].DeclarationFlag.Ron) {
			{
				CodeConv::tostringstream o;
				o << _T("�v���C���[ [") << (int)i << _T("] �́A�h�a��錾���܂��B");
				debug(o.str().c_str());
			}
			// �D�挠�̂Ȃ��������\�������悤�ɂ���
			/* TODO: ���� setCall cnt, " ���� " */
			gameStat->Player[i].HandStat = handExposed;
		}
	}
	/* ���ۂɃ����ł���l��\�� */
	unsigned roncount = 0;
	for (int i = 0; i < (PLAYERS - 1); i++) {
		if (gameStat->Player[RelativePositionOf(gameStat->CurrentPlayer.Active, (seatRelative)(i + 1))].DeclarationFlag.Ron) {
			bool accept = false;
			switch (roncount) {
			case 0:
				accept = true; break;
			case 1:
				accept =
					RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw") ||
					RuleData::chkRule("multiple_mahjong", "dual_mahjong") ||
					RuleData::chkRule("multiple_mahjong", "trial_mahjong");
				break;
			case 2:
				accept = RuleData::chkRule("multiple_mahjong", "trial_mahjong");
				break;
			}
			if (accept) {
				CodeConv::tostringstream o;
				o << _T("�v���C���[ [") << (int)i << _T("] �́A�h�a�ł��܂��B");
				debug(o.str().c_str());
				/* TODO: ���� setCall RelativePositionOf(getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE), cnt+1), "����" */
			}
			++roncount;
		}
	}
	/* ���ۂɉh�a���s�Ȃ����� */
	for (int i = 0; i < (PLAYERS - 1); i++) {
		Sleep(1); PLAYER_ID pl = RelativePositionOf(gameStat->CurrentPlayer.Active, (seatRelative)(i + 1));
		if (gameStat->Player[pl].DeclarationFlag.Ron) {
			/* �E�H�b�`���[�h�̏ꍇ�͘a�������l�Ɏ��_�������� */
			if (EnvTable::Instantiate()->WatchModeFlag)
				gameStat->PlayerID = pl;
			/* �h�a�������Ƃ�ϐ��ɋL�^ */
			RoundEndType = Agari; gameStat->TsumoAgariFlag = false;
			gameStat->CurrentPlayer.Furikomi = gameStat->CurrentPlayer.Active;
			gameStat->CurrentPlayer.Agari = pl;
			/* ���A���̔���Ɏg���ϐ� */
			if (RuleData::chkRuleApplied("paarenchan")) {
				if (gameStat->LastAgariPlayer == gameStat->CurrentPlayer.Agari) {
					++gameStat->AgariChain;
				} else if (RuleData::chkRuleApplied("breaking_paarenchan") && (gameStat->AgariChain == 7)) {
					gameStat->AgariChain = -1;
					gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Agari;
				} else { /* FIXME: ���A������̎��Ƀ_�u�����E�g�������������̏��� */
					gameStat->AgariChain = 1;
					gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Agari;
				}
			}
			/* �a����v��ݒ� */
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].red = gameStat->CurrentDiscard.red;
			/* �����錾�v�ł̕��e�̏ꍇ�A�����𖳌��Ƃ������_�_��ԋp���� */
			if (gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag) {
				if (gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.DoubleFlag) {
					gameStat->DoubleRichiCounter = true;
					trace(_T("�_�u�������錾�v�ł̕��e�̂��߁A�����_��Ԋ҂��܂��B"));
				} else {
					gameStat->RichiCounter = true;
					trace(_T("�����錾�v�ł̕��e�̂��߁A�����_��Ԋ҂��܂��B"));
				}
				gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag =
					gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.DoubleFlag =
					gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.OpenFlag =
					gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.RichiFlag = false;
				--gameStat->Deposit;
				gameStat->Player[gameStat->CurrentPlayer.Active].PlayerScore += (LNum)1000;
			}
			/* ����U���̔��� */
			yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, pl);
			MachihaiInfo mInfo = chkFuriten(gameStat, pl);
			// ����𖞂����Ȃ����A�U���̂Ƃ�
			if (((yakuInfo.CoreHan < (gameStat->ShibariFlag ? 2 : 1)) && (yakuInfo.CoreSemiMangan == 0)) ||
				(mInfo.FuritenFlag) || (gameStat->Player[pl].DoujunFuriten) ||
				(RuleData::chkRuleApplied("riichi_shibari") && (!gameStat->Player[pl].RichiFlag.RichiFlag))) {
					trace(_T("����𖞂����Ȃ����U���ł��B���̏������`�����{�p�ɐ؂�ւ��܂��B"));
					RoundEndType = Chonbo; // �`�����{�ɂ���
			}
			// �������������Ƃ�\��
			/* TODO: ���� setCall getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), "����" */
			gameStat->Player[gameStat->CurrentPlayer.Agari].HandStat = handExposed;
			/* �a����v��ݒ� */
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
			gameStat->Player[gameStat->CurrentPlayer.Agari].Hand[NUM_OF_TILES_IN_HAND - 1].red = gameStat->CurrentDiscard.red;
			/* �h�a�̃T�E���h��炷 */
			if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
				sound::Play(sound::IDs::voxRonFurikomi);
			else
				sound::Play(sound::IDs::voxRon);
			/* ��ʍX�V���Ė߂� */
			/* TODO: ��ʍX�V redrscreen */
			break;
		}
	}
	/* �v���ɋL�^ */
	roncount = 0;
	if (RonPlayers(gameStat) > 0) {
		if (gameStat->KangFlag.chankanFlag != chankanNone) {
			if (gameStat->KangFlag.chankanFlag == chankanOfNorth)
				haifu::haifurecchanpei(gameStat);
			else
				haifu::haifurecchankan(gameStat);
		} else {
			haifu::haifurecfurikomi(gameStat);
		}
	} else if (!gameStat->KangFlag.kangFlag) { // �����k�Ŕv���������̂�}�~
		haifu::haifurecfurikomi(gameStat);
	}
	if (chkGameType(gameStat, AllSanma)) {
		// ��Ƙa�̔���
		if ((RonPlayers(gameStat) >= 2) && RuleData::chkRule("multiple_mahjong", "aborted"))
			return TripleRon;
	} else {
		// �O�Ƙa�̔���
		if ((RonPlayers(gameStat) >= 3) &&
			(RuleData::chkRule("multiple_mahjong", "single_mahjong_with_draw") ||
			RuleData::chkRule("multiple_mahjong", "dual_mahjong_with_draw")))
			return TripleRon;
	}
	return RoundEndType;
}

bool executeFuuro(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	/* �̔v���|���A�܂��͑喾�Ȃ���ꍇ�̏��� */
	/* �����v�͂S�������Ȃ��̂ŁA�|���Ɩ��Ȃ͓����ɋN���邱�Ƃ��Ȃ� */
	unsigned declCount = 0; EndType roundEndType = Continuing;
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].DeclarationFlag.Pon) ++declCount;
		if (gameStat->Player[i].DeclarationFlag.Kan) ++declCount;
	}
	if (declCount > 1)
		error(_T("���������̃|���E�Ȃ��錾����Ă��܂��B"));
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		Sleep(1);
		/* �̔v���|������ꍇ */
		if (gameStat->Player[i].DeclarationFlag.Pon) {
			gameStat->CurrentPlayer.Passive = i; // �����v���C���[��ݒ�
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroPon);
			break;
		}
		/* ���Ȃ̏ꍇ */
		const unsigned kanLim = (RuleData::chkRuleApplied("fifth_kong") ? 5 : 4);
		if (gameStat->Player[i].DeclarationFlag.Kan && (gameStat->KangNum < kanLim)) {
			gameStat->CurrentPlayer.Passive = i; // �����v���C���[��ݒ�
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroDaiminkan);
			break;
		}
	}
	/* �h�ƃ|���������ɋN�������ꍇ�A�|����D�悷�� */
	if (declCount > 0) {
		/* �|����Ȃ̎��̓c�������΂����Ƃ݂Ȃ��Đ����A�k�Ɓ����Ƃ��܂������ꍇ�͎��̏��ڂƂ��Ĉ��� */
		if (playerwind(gameStat, gameStat->CurrentPlayer.Passive, gameStat->GameRound) < playerwind(gameStat, gameStat->CurrentPlayer.Active, gameStat->GameRound))
			++gameStat->TurnRound;
		gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive;
		return true;
	} else if (!chkGameType(gameStat, AllSanma)) {
		/* �h�̏��� */
		/* �O�l�ł��ł͋h�Ȃ� */
		if (gameStat->Player[RelativePositionOf(gameStat->CurrentPlayer.Passive, sRight)].DeclarationFlag.Chi > 0) {
			/* �|����Ȃ̎��̓c�������΂����Ƃ݂Ȃ��Đ����A�k�Ɓ����Ƃ��܂������ꍇ�͎��̏��ڂƂ��Ĉ��� */
			gameStat->CurrentPlayer.Passive = RelativePositionOf(gameStat->CurrentPlayer.Passive, sRight); // �h���ł���̂͏�Ƃ̎̔v�̂�
			if (playerwind(gameStat, gameStat->CurrentPlayer.Passive, gameStat->GameRound) < playerwind(gameStat, gameStat->CurrentPlayer.Active, gameStat->GameRound))
				++gameStat->TurnRound;
			fuuroproc(gameStat, &roundEndType, DiscardTileIndex, FuuroChii);
			gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive;
			return true;
		}
	}
	/* �o�O�h�~�̃A�� */
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND].tile = NoTile;
		gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND].red = Normal;
	}
	return false;
}
