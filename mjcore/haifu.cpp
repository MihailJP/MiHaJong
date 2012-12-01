#include "haifu.h"

/* ���v�̖��O�f�[�^ */
const CodeConv::tstring haifu::tilecodelabel =
	_T("�H���O�l�ܘZ������H�@�A�B�C�D�E�F�G�H�H�P�Q�R�S�T�U�V�W�X�H���쐼�k��ᢒ��H�H�H���Q��ޗ����J��H�J�K�L�M�N�O�P�Q�R�H�T�U�V�W�X�Y�Z�[�\�H�d�r�v�m�o�e�b�H�H�H�b��������ȍM�h�p�H����͂ɂقւƂ���H�@�A�B�C�D�E�F�G�H�H���������������H�H�H�t�ďH�~�S�~���e�|�H");
const CodeConv::tstring haifu::HTtilecodelabel1 =
	_T(" qwertyuio zxcvbnm,. asdfghjkl 1234567   qwertyuio zxcvbnm,. asdfghjkl 1234567   qwertyuio zxcvbnm,. asdfghjkl 1234567   -^\\[5@;:]");
const CodeConv::tstring haifu::HTtilecodelabel2 =
	_T(" QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   -^\\[%@;:]");

InfoByPlayer<LargeNum> haifu::origPoint;
CodeConv::tostringstream haifu::haifuBuffer, haifu::HThaifuBuffer;
bool haifu::haifukanflag = false;

haifu::HaifuStreams haifu::haifuP, haifu::HThaifuP;

/* �v���L�^�p�̕⏕���[�`�� */
void haifu::tools::haifuskip(
	HaifuStreams* haifuP, HaifuStreams* HThaifuP,
	PLAYER_ID PassivePlayer, PLAYER_ID ActivePlayer
	) {
		static CodeConv::tostringstream* p[] = {
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumo,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumolabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehai,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehailabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumo,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumolabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehai,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehailabel,
		};
		static CodeConv::tostringstream* h[] = {
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumo,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumolabel,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehai,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehailabel,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumo,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumolabel,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehai,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehailabel,
		};
		if (playerRelative(ActivePlayer, PassivePlayer) == sOpposite) {
			// �Ζʂ���|�������ꍇ
			for (int i = 0; i < 4; i++) {
				*p[i] << _T("�@ "); *h[i] << _T("<td></td>");
			}
		}
		if (playerRelative(ActivePlayer, PassivePlayer) == sRight) {
			// ���Ƃ���|�������ꍇ
			for (int i = 0; i < 8; i++) {
				*p[i] << _T("�@ "); *h[i] << _T("<td></td>");
			}
		}
}

CodeConv::tstring haifu::tools::haifudoraClass(doraCol Akadora) {
	switch (Akadora) {
	case AkaDora:
		return _T(" class=\"akadora\"");
	case AoDora:
		return _T(" class=\"aodora\"");
	default:
		return _T("");
	}
}

void haifu::tools::recordDoraStream(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, tileCode tmpDora) {
	*p << tilecodelabel.substr((int)tmpDora * (sizeof(_T("��")) - 1), 2);
	*h << HTtilecodelabel1.substr((int)tmpDora, 1);
}

void haifu::tools::recordTile_Inline(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, TILE tlCode, bool rotate) {
	*p << (rotate ? _T("[") : _T("")) <<
		tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX) * 2, 2) <<
		(rotate ? _T("]") : _T(""));
	if (tlCode.red) *h << _T("<span") << haifudoraClass(tlCode.red) << _T(">");
	*h << (rotate ? HTtilecodelabel2 : HTtilecodelabel1).substr(
		(int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX, 1);
	if (tlCode.red) *h << _T("</span>");
}
void haifu::tools::recordTile_Inline(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, TILE tlCode, doraCol kakanCol) {
	*p << _T("[") <<
		tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX) * 2, 2) <<
		tilecodelabel.substr(((int)tlCode.tile + (int)kakanCol * TILE_NONFLOWER_MAX) * 2, 2) <<
		_T("]");
	*h << _T("<table class=\"kakan\"><tr><td>");
		if (kakanCol) *h << _T("<span") << haifudoraClass(kakanCol) << _T(">");
		*h << HTtilecodelabel2.substr((int)tlCode.tile + (int)kakanCol * TILE_NONFLOWER_MAX, 1);
		if (kakanCol) *h << _T("</span>");
	*h << _T("</span>");
		if (tlCode.red) *h << _T("<span") << haifudoraClass(tlCode.red) << _T(">");
		*h << HTtilecodelabel2.substr((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX, 1);
		if (tlCode.red) *h << _T("</span>");
	*h << _T("</tr></td></table>");
}
void haifu::tools::recordTile_Table(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, TILE tlCode) {
	*p << tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX) * 2, 2) << _T(" ");
	*h << _T("<td") << haifudoraClass(tlCode.red) << _T(">") <<
		HTtilecodelabel1.substr((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX, 1) <<
		_T("</td>");
}

void haifu::tools::recordBlank_Table(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h) {
	*p << _T("�@ ");
	*h << _T("<td></td>");
}

void haifu::tools::haifuwritetsumohai(
	HaifuStreams* haifuP, HaifuStreams* HThaifuP,
	PLAYER_ID ActivePlayer, TILE tlCode,
	CodeConv::tstring PText, CodeConv::tstring HTText
	) {
		recordTile_Table(
			&haifuP->streamDat[ActivePlayer].tsumo,
			&HThaifuP->streamDat[ActivePlayer].tsumo,
			tlCode);
		haifuP->streamDat[ActivePlayer].tsumolabel << PText;
		HThaifuP->streamDat[ActivePlayer].tsumolabel << HTText;
}

void haifu::tools::haifuskipall(HaifuStreams* haifuP, HaifuStreams* HThaifuP, PLAYER_ID PassivePlayer) {
	static CodeConv::tostringstream* p[] = {
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].tsumo,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].tsumolabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].sutehai,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].sutehailabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].tsumo,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].tsumolabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].sutehai,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].sutehailabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].tsumo,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].tsumolabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].sutehai,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].sutehailabel,
	};
	static CodeConv::tostringstream* h[] = {
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].tsumo,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].tsumolabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].sutehai,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].sutehailabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].tsumo,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].tsumolabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].sutehai,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].sutehailabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].tsumo,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].tsumolabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].sutehai,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].sutehailabel,
	};
	for (int i = 0; i < 12; i++) {
		*p[i] << _T("�@ "); *h[i] << _T("<td></td>");
	}
}

/* �ꔼ�����̔v���o�b�t�@�������� */
__declspec(dllexport) void haifu::haifubufinit() {
	CodeConv::tostringstream headerTxt; headerTxt.str(_T(""));
	headerTxt << _T("MiHaJong ");
	switch (GameStat.gameType) {
		case SanmaS: headerTxt << _T("���v�O��"); break;
		case Sanma4: headerTxt << _T("�l�l�O��"); break;
		case Sanma:  headerTxt << _T("�O�l�ł�"); break;
		case Yonma:  headerTxt << _T("�l�l�ł�"); break;
	}
	headerTxt << _T("�v���f�[�^ Ver. ") << MIHAJONG_VER;

	/* �v���[���e�L�X�g�v�� */
	haifuBuffer.str(_T(""));
	haifuBuffer << headerTxt.str() << std::endl << std::endl <<
		_T("------------------------------------------------------------------------------") <<
		std::endl << std::endl;

	/* HTML�v�� */
	HThaifuBuffer.str(_T(""));
	HThaifuBuffer <<  _T("<html>") << std::endl << _T("<head>") << std::endl <<
		_T("<title>") << headerTxt.str() << _T("</title>") << std::endl <<
#ifdef UNICODE
		_T("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">") <<
#else
		_T("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=Shift_JIS\">") <<
#endif
		std::endl <<
		_T("<link rel=\"stylesheet\" href=\"haifu.css\" type=\"text/css\">") << std::endl <<
		_T("</head>") << std::endl << _T("<body>") << std::endl <<
		_T("<h1>") << headerTxt.str() << _T("</h1>") << std::endl << _T("<hr>") << std::endl;
}

/* ��Ǖ��̔v���o�b�t�@�������� */
__declspec(dllexport) void haifu::haifuinit() {
	static HaifuStreams* const bufs[] = {&haifuP, &HThaifuP};
	for (int i = 0; i < 2; i++) {
		for (int p = 0; p < PLAYERS; p++) {
			bufs[i]->streamDat[p].haipai.str(_T(""));
			bufs[i]->streamDat[p].tsumo.str(_T(""));
			bufs[i]->streamDat[p].tsumolabel.str(_T(""));
			bufs[i]->streamDat[p].sutehai.str(_T(""));
			bufs[i]->streamDat[p].sutehailabel.str(_T(""));
			bufs[i]->streamDat[p].final.str(_T(""));
		}
		bufs[i]->dora.str(_T(""));
		bufs[i]->uraDora.str(_T(""));
		bufs[i]->aliceDora.str(_T(""));
		bufs[i]->aliceDoraMax.str(_T(""));
		bufs[i]->resultDesc.str(_T(""));
	}
}

/* �z�v��v���ɋL�^ */
__declspec(dllexport) void haifu::haifurechaipai(const GameTable* const gameStat) {
	for (int p = 0; p < PLAYERS; p++) {
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if (gameStat->Player[p].Hand[i].tile != NoTile) {
				tools::recordTile_Inline(
					&haifuP.streamDat[p].haipai,
					&HThaifuP.streamDat[p].haipai,
					gameStat->Player[p].Hand[i],
					false
				);
			}
		}
		origPoint[p] = gameStat->Player[p].PlayerScore;
	}
}

/* �h����v���ɋL�^ */
void haifu::haifurecdora(tileCode tmpDora) {
	tools::recordDoraStream(&haifuP.dora, &HThaifuP.dora, tmpDora);
}
__declspec(dllexport) void haifu::haifurecdora(int tmpDora) {
	haifurecdora((tileCode)tmpDora);
}

/* ���h����v���ɋL�^ */
void haifu::haifurecuradora(tileCode tmpDora) {
	tools::recordDoraStream(&haifuP.uraDora, &HThaifuP.uraDora, tmpDora);
}

/* �A���X�h����v���ɋL�^ */
void haifu::haifurecalicedora(tileCode tmpDora) {
	tools::recordDoraStream(&haifuP.aliceDora, &HThaifuP.aliceDora, tmpDora);
}

/* �A���X�h�����X�V */
__declspec(dllexport) void haifu::haifualicedoraupd() {
	if (haifuP.aliceDora.str().length() > haifuP.aliceDoraMax.str().length()) {
		haifuP.aliceDoraMax.str(haifuP.aliceDora.str());
		HThaifuP.aliceDoraMax.str(HThaifuP.aliceDora.str());
	}
}

/* �A���X�h�������� */
void haifu::haifuresetalicedora() {
	haifuP.aliceDora.str(_T(""));
	HThaifuP.aliceDora.str(_T(""));
}

/* �h���̃Z�p���[�^�[ */
__declspec(dllexport) void haifu::haifurecdorap() {
	haifuP.dora << _T(" ");
	haifuP.uraDora << _T(" ");
	haifuP.aliceDora << _T(" ");
	HThaifuP.dora << _T("</span> <span class=\"tile\">");
	HThaifuP.uraDora << _T("</span> <span class=\"tile\">");
	HThaifuP.aliceDora << _T("</span> <span class=\"tile\">");
}

/* �̑ł�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecmota(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecmota(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecmota(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	// �c�����Ă����v���L�^
	if (gameStat->TianHuFlag) {
		// �e�̂P����
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo);
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
	} else if (gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].tile == NoTile) {
		// �������� (�������Ȃ�)
	} else if ((DiscardTileIndex.id) == (NUM_OF_TILES_IN_HAND - 1)) {
		// �c���؂�
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo << _T("�� ");
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo << _T("<td class=\"fallthru\">��</td>");
	} else {
		tools::haifuwritetsumohai(
			&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active,
			gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
			_T("�@ "), _T("<td></td>"));
	}
	// �̂Ă��v���L�^
	tools::recordTile_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
		gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
}

/* ���e�������ۂ���v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecfurikomi(const GameTable* const gameStat) {
	if (gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag) {
		// �����錾�v�̏ꍇ
		if (RonPlayers(gameStat) > 0) {
			// �����錾�v�ł̐U�荞��
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("�X ");
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("<td>���[�`<br>�E�`</td>");
		} else {
			// �������ʂ����ꍇ
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("ذ�");
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("<td>���[�`</td>");
		}
	} else {
		// ����ȊO�̏ꍇ
		if (RonPlayers(gameStat) > 0) {
			// �ʏ�̐U�荞��
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("�� ");
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("<td>�E�`</td>");
		} else {
			// �����Ȃ������ꍇ
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("�@ ");
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("<td></td>");
		}
	}
}

/* �`�[�������Ƃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecchi(const GameTable* const gameStat) {
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T("�� "), _T("<td>�`�[</td>"));
}
/* �|���������Ƃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecpon(const GameTable* const gameStat) {
	tools::haifuskip(
		&haifuP, &HThaifuP,
		gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T("���"), _T("<td>�|��</td>"));
}
/* ���̘a�������Ƃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurectsumo(const GameTable* const gameStat) {
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active,
		gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
		_T("�� "), _T("<td>�c��</td>"));
}
/* �喾�Ȃ������Ƃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecminkan(const GameTable* const gameStat) {
	tools::haifuskip(
		&haifuP, &HThaifuP,
		gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T("�� "), _T("<td>�J��</td>"));
	tools::recordBlank_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehai,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehai);
	tools::recordBlank_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehailabel,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehailabel);
	// �v���̋L�q��́A���v�̎擾�͎����Ƃ��Ĉ���
	tools::haifuskipall(&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive);
}

/* �J������������ */
void haifu::tools::kan_sub::recordKanOrFlower(
	const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex,
	HaifuStreams* haifuP, HaifuStreams* HThaifuP
	) {
		if ((gameStat->TianHuFlag)||((DiscardTileIndex.id) != (NUM_OF_TILES_IN_HAND - 1))) {
			// �e�̂P���ڂ̏ꍇ���A�c�����Ă����v�ȊO���J�������ꍇ
			if (gameStat->TianHuFlag) {
				recordBlank_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo);
				recordBlank_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
				recordTile_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
				haifukanflag = true;
			} else if (gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].tile ==
				gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].tile) {
					// �c�����Ă����v�Ɠ���������
					recordTile_Table(
						&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
						&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
						gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
					haifukanflag = false;
			} else {
				haifuwritetsumohai(
					haifuP, HThaifuP, gameStat->CurrentPlayer.Active,
					gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
					_T("�@ "), _T("<td></td>"));
				recordTile_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
				haifukanflag = true;
			}
		} else {
			recordTile_Table(
				&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
				&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
				gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
			haifukanflag = false;
		}
}
/* ���g�k�h��v���ɋL�^ */
void inline haifu::tools::kan_sub::recordChanKan(const GameTable* const gameStat, CodeConv::tstring pTxt, CodeConv::tstring hTxt) {
	if (haifukanflag) {
		// �e�̂P���ڂ̏ꍇ���A�c�����Ă����v�ȊO���J�������ꍇ
		haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << hTxt;
	} else {
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << hTxt;
	}
}
/* �����k�����������ꍇ�̔v������ */
void inline haifu::tools::kan_sub::recordKan(const GameTable* const gameStat, CodeConv::tstring pTxt, CodeConv::tstring hTxt) {
	if (haifukanflag == 1) {
		// �e�̂P���ڂ̏ꍇ���A�c�����Ă����v�ȊO���J�������ꍇ
		haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << hTxt;
	} else {
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << hTxt;
		recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai);
		recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel);
	}
		
	// �v���̋L�q��́A���v�̎擾�͎����Ƃ��Ĉ���
	haifuskipall(&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active);
}

/* �����k��v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecnorth(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecnorth(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecnorth(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
}
/* ���g�k�h��v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecchanpei(const GameTable* const gameStat) {
	tools::kan_sub::recordChanKan(gameStat, _T("��X"), _T("<td>��<br>�E�`</td>"));
}
/* �����k�����������ꍇ�̔v������ */
__declspec(dllexport) void haifu::haifurecnorthproc(const GameTable* const gameStat) {
	tools::kan_sub::recordKan(gameStat, _T("�� "), _T("<td>��</td>"));
}

/* �ÞȂȂ������Ȃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecankan(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecankan(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecankan(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
}
/* ���Ȃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecchankan(const GameTable* const gameStat) {
	tools::kan_sub::recordChanKan(gameStat, _T("��X"), _T("<td>�J��<br>�E�`</td>"));
}
/* �J�������������ꍇ�̔v������ */
__declspec(dllexport) void haifu::haifureckanproc(const GameTable* const gameStat) {
	tools::kan_sub::recordKan(gameStat, _T("�� "), _T("<td>�J��</td>"));
}

/* �Ԕv��v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecflower(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecflower(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecflower(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
	tools::kan_sub::recordKan(gameStat, _T("�� "), _T("<td>��</td>"));
}

/* hfwriter */
void haifu::tools::hfwriter::hfWriteHead(const GameTable* const gameStat,
	int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
	CodeConv::tstring ResultDesc, EndType RoundEndType) {
		haifuBuffer << ::roundName(OrigTurn, gameStat);
		if (OrigHonba > 0) haifuBuffer << _T(" ") << OrigHonba << _T("�{��");
		haifuBuffer << _T(" �h���F") << haifuP.dora.str();
		if ((RoundEndType == Agari)&&(tmpUraFlag == 1)&&(RuleData::chkRuleApplied("uradora")))
			haifuBuffer << _T("���h���F") << haifuP.uraDora.str();
		if ((RoundEndType == Agari)&&(tmpAliceFlag == 1)&&(RuleData::chkRuleApplied("alice")))
			haifuBuffer << _T("�A���X�F") << haifuP.aliceDoraMax.str();
		haifuBuffer << std::endl << std::endl <<
			_T("���ʁF") << ResultDesc << std::endl << std::endl;

		HThaifuBuffer << _T("<h2>") << ::roundName(OrigTurn, gameStat);
		if (OrigHonba > 0) HThaifuBuffer << _T(" ") << OrigHonba <<_T("�{��");
		HThaifuBuffer << _T(" �h���F<span class=\"tile\">") <<
			HThaifuP.dora.str() << _T("</span>");
		if ((RoundEndType == Agari)&&(tmpUraFlag == 1)&&(RuleData::chkRuleApplied("uradora")))
			HThaifuBuffer << _T("���h���F<span class=\"tile\">") <<
			haifuP.uraDora.str() << _T("</span>");
		if ((RoundEndType == Agari)&&(tmpAliceFlag == 1)&&(RuleData::chkRuleApplied("alice")))
			HThaifuBuffer << _T("�A���X�F<span class=\"tile\">") <<
			haifuP.aliceDoraMax.str() << _T("</span>");
		HThaifuBuffer << _T("</h2>") << std::endl <<
			_T("<p>���ʁF") << ResultDesc << _T("</p>") << std::endl <<
			_T("<table>") << std::endl << _T("<tr>");
		for (unsigned int i = 0u; i < cols; i++)
			HThaifuBuffer << _T("<td width=") << (100.0/((double)cols)) << _T("%></td>");
		HThaifuBuffer << _T("</tr>") << std::endl;
}

// �ŏI�v�p
void haifu::tools::hfwriter::finalformWriter::hfFinalForm(const GameTable* const gameStat, PLAYER_ID player, EndType RoundEndType) {
	// �ŏI�v�p(����v�̂�)
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[player].Hand[i].tile != NoTile) {
			if (i == NUM_OF_TILES_IN_HAND - 1) {
				if ((RoundEndType == Ryuukyoku)||(RoundEndType == Agari)||(RoundEndType == Chonbo)) {
					if (gameStat->TsumoAgariFlag) {
						haifuP.streamDat[player].final << _T(" �c��");
						HThaifuP.streamDat[player].final << _T("</span> �c��<span class=\"tile\">");
					} else {
						haifuP.streamDat[player].final << _T(" ����");
						HThaifuP.streamDat[player].final << _T("</span> ����<span class=\"tile\">");
					}
				}
				else haifuP.streamDat[player].final << _T(" ");
			}
			recordTile_Inline(
				&haifuP.streamDat[player].final, &HThaifuP.streamDat[player].final,
				gameStat->Player[player].Hand[i], false);
		}
	}
}
void haifu::tools::hfwriter::finalformWriter::hfFlower(const GameTable* const gameStat, PLAYER_ID player) {
	if (gameStat->Player[player].FlowerFlag.Spring ||
		gameStat->Player[player].FlowerFlag.Summer ||
		gameStat->Player[player].FlowerFlag.Autumn ||
		gameStat->Player[player].FlowerFlag.Winter) {
			haifuP.streamDat[player].final << _T(" ");
			HThaifuP.streamDat[player].final << _T("</span> <span class=\"tile\">");
			if (gameStat->Player[player].FlowerFlag.Spring) {
				haifuP.streamDat[player].final << _T("�t");
				HThaifuP.streamDat[player].final << _T("@");
			}
			if (gameStat->Player[player].FlowerFlag.Summer) {
				haifuP.streamDat[player].final << _T("��");
				HThaifuP.streamDat[player].final << _T(";");
			}
			if (gameStat->Player[player].FlowerFlag.Autumn) {
				haifuP.streamDat[player].final << _T("�H");
				HThaifuP.streamDat[player].final << _T(":");
			}
			if (gameStat->Player[player].FlowerFlag.Winter) {
				haifuP.streamDat[player].final << _T("�~");
				HThaifuP.streamDat[player].final << _T("]");
			}
	}
	if (gameStat->Player[player].FlowerFlag.Plum ||
		gameStat->Player[player].FlowerFlag.Orchid ||
		gameStat->Player[player].FlowerFlag.Chrys ||
		gameStat->Player[player].FlowerFlag.Bamboo) {
			haifuP.streamDat[player].final << _T(" ");
			HThaifuP.streamDat[player].final << _T("</span> <span class=\"tile\">");
			if (gameStat->Player[player].FlowerFlag.Plum) {
				haifuP.streamDat[player].final << _T("�~");
				HThaifuP.streamDat[player].final << _T("-");
			}
			if (gameStat->Player[player].FlowerFlag.Orchid) {
				haifuP.streamDat[player].final << _T("��");
				HThaifuP.streamDat[player].final << _T("^");
			}
			if (gameStat->Player[player].FlowerFlag.Chrys) {
				haifuP.streamDat[player].final << _T("�e");
				HThaifuP.streamDat[player].final << _T("[");
			}
			if (gameStat->Player[player].FlowerFlag.Bamboo) {
				haifuP.streamDat[player].final << _T("�|");
				HThaifuP.streamDat[player].final << _T("\\");
			}
	}
}

void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfChii(PLAYER_ID player, meldCode meld) {
	// �`�[
	haifuP.streamDat[player].final << _T(" ���`�[");
	HThaifuP.streamDat[player].final << _T("</span> �`�[<span class=\"tile\">");
	TILE meldTile[3];
	switch (meld.mstat) {
	case meldSequenceExposedLower:
		meldTile[0].tile = meld.tile; meldTile[0].red = meld.red[0];
		meldTile[1].tile = tileCode(meld.tile + 1); meldTile[1].red = meld.red[1];
		meldTile[2].tile = tileCode(meld.tile + 2); meldTile[2].red = meld.red[2];
		break;
	case meldSequenceExposedMiddle:
		meldTile[1].tile = meld.tile; meldTile[1].red = meld.red[0];
		meldTile[0].tile = tileCode(meld.tile + 1); meldTile[0].red = meld.red[1];
		meldTile[2].tile = tileCode(meld.tile + 2); meldTile[2].red = meld.red[2];
	case meldSequenceExposedUpper:
		meldTile[1].tile = meld.tile; meldTile[1].red = meld.red[0];
		meldTile[2].tile = tileCode(meld.tile + 1); meldTile[2].red = meld.red[1];
		meldTile[0].tile = tileCode(meld.tile + 2); meldTile[0].red = meld.red[2];
	}
	for (int i = 0; i < 3; i++) {
		recordTile_Inline(
			&haifuP.streamDat[player].final,
			&HThaifuP.streamDat[player].final,
			meldTile[i], i == 0);
	}
}
inline void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfPon1(PLAYER_ID player, meldCode meld) {
	TILE meldTile = {meld.tile, meld.red[0]};
	if ((meld.mstat == meldQuadAddedLeft) ||
		(meld.mstat == meldQuadAddedCenter) ||
		(meld.mstat == meldQuadAddedRight))
			recordTile_Inline(
				&haifuP.streamDat[player].final,
				&HThaifuP.streamDat[player].final,
				meldTile, meld.red[3]);
	else recordTile_Inline(
		&haifuP.streamDat[player].final,
		&HThaifuP.streamDat[player].final,
		meldTile, true);
}
void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfPon(PLAYER_ID player, meldCode meld) {
	int tiles, interrupt;
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldQuadExposedLeft: case meldQuadAddedLeft:
		haifuP.streamDat[player].final << _T(" ��"); interrupt = 1; break;
	case meldTripletExposedCenter: case meldQuadExposedCenter: case meldQuadAddedCenter:
		haifuP.streamDat[player].final << _T(" ��"); interrupt = 2; break;
	case meldTripletExposedRight: case meldQuadExposedRight: case meldQuadAddedRight:
		haifuP.streamDat[player].final << _T(" ��"); interrupt = 8; break;
	case meldQuadConcealed:
		haifuP.streamDat[player].final << _T(" ��"); interrupt = 7; break;
	}
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldTripletExposedCenter:
	case meldTripletExposedRight:
		tiles = 3;
		haifuP.streamDat[player].final << _T("�|��");
		HThaifuP.streamDat[player].final << _T("</span> �|��<span class=\"tile\">");
		break;
	case meldQuadExposedLeft: case meldQuadExposedCenter:
	case meldQuadExposedRight: case meldQuadConcealed:
		tiles = 4;
		haifuP.streamDat[player].final << _T("�J��");
		HThaifuP.streamDat[player].final << _T("</span> �J��<span class=\"tile\">");
		break;
	case meldQuadAddedLeft: case meldQuadAddedCenter:
	case meldQuadAddedRight:
		tiles = 3;
		haifuP.streamDat[player].final << _T("���J��");
		HThaifuP.streamDat[player].final << _T("</span> �J��<span class=\"tile\">");
		break;
	}
	for (int i = (meld.mstat == meldQuadConcealed ? 0 : 1); i < tiles; i++) {
		if (i == interrupt) hfPon1(player, meld);
		TILE meldTile = {meld.tile, meld.red[i]};
		recordTile_Inline(
			&haifuP.streamDat[player].final,
			&HThaifuP.streamDat[player].final,
			meldTile, false);
	}
	if (interrupt == 8) hfPon1(player, meld);
}

void haifu::tools::hfwriter::finalformWriter::hfExposedMeld(const GameTable* const gameStat, PLAYER_ID player) {
	for (int i = 1; i <= gameStat->Player[player].MeldPointer; i++) {
		switch (gameStat->Player[player].Meld[i].mstat) {
		case meldSequenceExposedLower: case meldSequenceExposedMiddle:
		case meldSequenceExposedUpper:
			MeldWriter::hfChii(player, gameStat->Player[player].Meld[i]);
			break;
		case meldTripletExposedLeft: case meldQuadExposedLeft: case meldQuadAddedLeft:
		case meldTripletExposedCenter: case meldQuadExposedCenter: case meldQuadAddedCenter:
		case meldTripletExposedRight: case meldQuadExposedRight: case meldQuadAddedRight:
		case meldQuadConcealed:
			MeldWriter::hfPon(player, gameStat->Player[player].Meld[i]);
			break;
		}
	}
}

void haifu::tools::hfwriter::hfScoreWriteOut(const GameTable* const gameStat, PLAYER_ID player, seatAbsolute wind) {
	// �_���̕ϓ�
	CodeConv::tostringstream o;
	o << _T(" ") << origPoint[player].bignumtotext(_T(""), _T("��"));
	if (origPoint[player] != gameStat->Player[player].PlayerScore) // �_������v���Ȃ��Ȃ�
		o << _T(" �� ") <<
			gameStat->Player[player].PlayerScore.bignumtotext(_T(""), _T("��")) << _T(" (") <<
			((LNum)gameStat->Player[player].PlayerScore -
			origPoint[player]).to_str(_T("+"), _T("-")) <<
			_T(")");
	if (RuleData::chkRuleApplied("chip")) // �`�b�v����̎�
		o << _T(" �`�b�v: ") <<
			((gameStat->Player[player].playerChip >= 0) ? _T("+") : _T("")) <<
			(int)gameStat->Player[player].playerChip;

	{
		CodeConv::tostringstream p;
		p << _T("*** ") << EnvTable::Instantiate()->PlayerDat[player].PlayerName << _T("(") <<
			windName(wind) << _T(") ") << o.str();
		chat::chatobj->sysmsg(p.str());
	}
					
	// �o��
	haifuBuffer << windName(wind) << _T(" ") <<
		EnvTable::Instantiate()->PlayerDat[player].PlayerName <<
		o.str() << std::endl;
	HThaifuBuffer << _T("<tr><td colspan=") << cols << _T(" class=\"player\">") <<
		windName(wind) << _T(" ") <<
		EnvTable::Instantiate()->PlayerDat[player].PlayerName <<
		o.str() << _T("</td></tr>") << std::endl;
}

void haifu::tools::hfwriter::hfWriteOut(const GameTable* const gameStat, PLAYER_ID player) {
	haifuBuffer << _T("�z�v�F") << haifuP.streamDat[player].haipai.str() <<
		std::endl << std::endl;
	HThaifuBuffer << _T("<tr><td class=\"label\">�z�v</td><td colspan=") << (cols - 1) <<
		_T("><span class=\"tile\">") << HThaifuP.streamDat[player].haipai.str() <<
		_T("</span></td></tr>") << std::endl << std::endl;
	if (!gameStat->TianHuFlag) { // �V�a(�܂��͐e�̏\�O�s�������v)�̏ꍇ�͏ȗ�
		haifuBuffer <<
			_T("���́F") << haifuP.streamDat[player].tsumo.str() << std::endl <<
			_T("�@�@�@") << haifuP.streamDat[player].tsumolabel.str() << std::endl <<
			_T("�Ŕv�F") << haifuP.streamDat[player].sutehai.str() << std::endl <<
			_T("�@�@�@") << haifuP.streamDat[player].sutehailabel.str() << std::endl <<
			_T("�v�p�F") << haifuP.streamDat[player].final.str() << std::endl <<
			std::endl;
		HThaifuBuffer <<
			_T("<tr class=\"tile\"><td class=\"label\" rowspan=2>����</td>") <<
			HThaifuP.streamDat[player].tsumo.str() << _T("</tr>") << std::endl <<
			_T("<tr class=\"notice\">") <<
			HThaifuP.streamDat[player].tsumolabel.str() << _T("</tr>") << std::endl <<
			_T("<tr class=\"tile\"><td class=\"label\" rowspan=2>�Ŕv</td>") <<
			HThaifuP.streamDat[player].sutehai.str() << _T("</tr>") << std::endl <<
			_T("<tr class=\"notice\">") <<
			HThaifuP.streamDat[player].sutehailabel.str() << _T("</tr>") << std::endl <<
			_T("<tr><td class=\"label\">�v�p</td><td colspan=") << (cols - 1) <<
			_T(" class=\"hand\"><span class=\"tile\">") <<
			HThaifuP.streamDat[player].final.str() << _T("</span></td></tr>") << std::endl <<
			std::endl;
	}
}

void haifu::tools::hfwriter::hfWriteFinalForms(const GameTable* const gameStat, int OrigTurn, EndType RoundEndType) {
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		PLAYER_ID k = RelativePositionOf(i, OrigTurn % PLAYERS);
		if (chkGameType(gameStat, SanmaT))
			if (((OrigTurn % PLAYERS) + i) >= ACTUAL_PLAYERS)
				k = (k + 1) % PLAYERS;
		// ���I�ʎq���o�͂���
		finalformWriter::hfFinalForm(gameStat, k, RoundEndType);
		finalformWriter::hfFlower(gameStat, k);
		finalformWriter::hfExposedMeld(gameStat, k);
		// �_�_�󋵂������o��
		hfScoreWriteOut(gameStat, k, (seatAbsolute)i);
		// �F�X�����o��
		if ((!chkGameType(gameStat, Sanma4))||(i < 3))
			hfWriteOut(gameStat, k);
		else haifuBuffer << std::endl;
	}
}

void haifu::tools::hfwriter::hfWriteBottom() {
	haifuBuffer <<
		_T("------------------------------------------------------------------------------") <<
		std::endl << std::endl;
	HThaifuBuffer << _T("</table>") << std::endl << _T("<hr>") << std::endl;
}

/* �z�v���o�b�t�@�ɏo�� */
__declspec(dllexport) void haifu::haifuwritebuffer(
	const GameTable* const gameStat, void *,
	int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
	LPCTSTR ResultDesc, EndType RoundEndType
	) { /* �z�v���o�b�t�@�ɏo�� */
		tools::hfwriter::hfWriteHead(
			gameStat, OrigTurn, OrigHonba, tmpUraFlag, tmpAliceFlag,
			ResultDesc, RoundEndType);
		tools::hfwriter::hfWriteFinalForms(
			gameStat, OrigTurn, RoundEndType);
		tools::hfwriter::hfWriteBottom();
}

/* �v����ۑ� */
void haifu::haifusave(const GameTable* const gameStat) {
	std::string configPath = confpath::confPath();
	HThaifuBuffer << _T("</body>") << std::endl << _T("</html>") << std::endl; // Finalize HTML
	std::ostringstream filename1, filename2;
	filename1 << configPath << "haifu\\";
	switch (gameStat->gameType) {
		case Yonma: filename1 << "mihajong"; break;
		case Sanma: filename1 << "mihasanm"; break;
		case Sanma4: filename1 << "mihaysnm"; break;
		case SanmaS: filename1 << "mihassnm"; break;
	}
	filename1 << "_" << MIHAJONG_MAJOR_VER << "_" <<
		MIHAJONG_MINOR_VER << "_" << MIHAJONG_PATCH_VER;

	SYSTEMTIME ltime; GetLocalTime(&ltime);
	filename2 << std::setw(4) << std::setfill('0') << ltime.wYear;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wMonth;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wDay << "_";
	filename2 << std::setw(2) << std::setfill('0') << ltime.wHour;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wMinute;

	std::ofstream fileout;
	fileout.open((filename1.str() + std::string("_haifu_") +
		filename2.str() + std::string(".txt")).c_str());
	fileout << CodeConv::EncodeStr(haifuBuffer.str()); fileout.close();
	fileout.open((filename1.str() + std::string("_chat_") +
		filename2.str() + std::string(".txt")).c_str());
	fileout << CodeConv::EncodeStr(chat::chatobj->getlog()); fileout.close();
	fileout.open((filename1.str() + std::string("_haifu_") +
		filename2.str() + std::string(".htm")).c_str());
	fileout << CodeConv::EncodeStr(HThaifuBuffer.str()); fileout.close();
}
__declspec(dllexport) void haifu::haifusave() {
	haifusave(&GameStat);
}
