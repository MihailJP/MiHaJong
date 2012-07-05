#include "haifu.h"

namespace haifu { // �v���L�^�p�̃R�[�h
	using ::RonPlayers;

	/* ���v�̖��O�f�[�^ */
	static const std::string tilecodelabel =
		"�H���O�l�ܘZ������H�@�A�B�C�D�E�F�G�H�H�P�Q�R�S�T�U�V�W�X�H���쐼�k��ᢒ��H�H�H���Q��ޗ����J��H�J�K�L�M�N�O�P�Q�R�H�T�U�V�W�X�Y�Z�[�\�H�d�r�v�m�o�e�b�H�H�H�b��������ȍM�h�p�H����͂ɂقւƂ���H�@�A�B�C�D�E�F�G�H�H���������������H�H�H�t�ďH�~�S�~���e�|�H";
	static const std::string HTtilecodelabel1 =
		" qwertyuio zxcvbnm,. asdfghjkl 1234567   qwertyuio zxcvbnm,. asdfghjkl 1234567   qwertyuio zxcvbnm,. asdfghjkl 1234567   -^\\[5@;:]";
	static const std::string HTtilecodelabel2 =
		" QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   -^\\[%@;:]";

	static InfoByPlayer<LargeNum> origPoint;
	static std::ostringstream haifuBuffer, HThaifuBuffer;
	static bool haifukanflag = false;

	class PlayerStream {
	public:
		std::ostringstream haipai, tsumo, tsumolabel, sutehai, sutehailabel, final;
	};
	class HaifuStreams {
	public:
		InfoByPlayer<PlayerStream> streamDat;
		std::ostringstream dora, uraDora, aliceDora, aliceDoraMax, resultDesc;
	};
	static HaifuStreams haifuP, HThaifuP;

	namespace tools {
		using haifu::tilecodelabel;
		using haifu::HTtilecodelabel1; using haifu::HTtilecodelabel2;
		using ::playerRelative; using ::RelativePositionOf;
		using haifu::haifukanflag;

		void haifuskip(
			HaifuStreams* haifuP, HaifuStreams* HThaifuP,
			PLAYER_ID PassivePlayer, PLAYER_ID ActivePlayer
			) {
				static std::ostringstream* p[] = {
					&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumo,
					&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumolabel,
					&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehai,
					&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehailabel,
					&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumo,
					&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumolabel,
					&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehai,
					&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehailabel,
				};
				static std::ostringstream* h[] = {
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
						*p[i] << "�@ "; *h[i] << "<td></td>";
					}
				}
				if (playerRelative(ActivePlayer, PassivePlayer) == sRight) {
					// ���Ƃ���|�������ꍇ
					for (int i = 0; i < 8; i++) {
						*p[i] << "�@ "; *h[i] << "<td></td>";
					}
				}
		}

		std::string haifudoraClass(doraCol Akadora) {
			switch (Akadora) {
			case AkaDora:
				return " class=\"akadora\"";
			case AoDora:
				return " class=\"aodora\"";
			default:
				return "";
			}
		}

		void recordDoraStream(std::ostringstream* const p, std::ostringstream* const h, tileCode tmpDora) {
			*p << tilecodelabel.substr((int)tmpDora * 2, 2);
			*h << HTtilecodelabel1.substr((int)tmpDora, 1);
		}

		void recordTile_Inline(std::ostringstream* const p, std::ostringstream* const h, TILE tlCode) {
			*p << tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX) * 2, 2);
			if (tlCode.red) *h << "<span" << haifudoraClass(tlCode.red) << ">";
			*h << HTtilecodelabel1.substr((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX, 1);
			if (tlCode.red) *h << "</span>";
		}
		void recordTile_Table(std::ostringstream* const p, std::ostringstream* const h, TILE tlCode) {
			*p << tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX) * 2, 2) << " ";
			*h << "<td" << haifudoraClass(tlCode.red) << ">" <<
				HTtilecodelabel1.substr((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX, 1) <<
				"</td>";
		}

		void recordBlank_Table(std::ostringstream* const p, std::ostringstream* const h) {
			*p << "�@ ";
			*h << "<td></td>";
		}

		void haifuwritetsumohai(
			HaifuStreams* haifuP, HaifuStreams* HThaifuP,
			PLAYER_ID ActivePlayer, TILE tlCode,
			std::string PText, std::string HTText
			) {
				recordTile_Table(
					&haifuP->streamDat[ActivePlayer].tsumo,
					&HThaifuP->streamDat[ActivePlayer].tsumo,
					tlCode);
				haifuP->streamDat[ActivePlayer].tsumolabel << PText;
				HThaifuP->streamDat[ActivePlayer].tsumolabel << HTText;
		}

		void haifuskipall(HaifuStreams* haifuP, HaifuStreams* HThaifuP, PLAYER_ID PassivePlayer) {
			static std::ostringstream* p[] = {
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
			static std::ostringstream* h[] = {
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
				*p[i] << "�@ "; *h[i] << "<td></td>";
			}
		}

	}

	/* �ꔼ�����̔v���o�b�t�@�������� */
	__declspec(dllexport) void haifubufinit() {
		std::ostringstream headerTxt; headerTxt.str("");
		headerTxt << "MiHaJong ";
		switch (GameStat.gameType) {
			case SanmaS: headerTxt << "���v�O��"; break;
			case Sanma4: headerTxt << "�l�l�O��"; break;
			case Sanma:  headerTxt << "�O�l�ł�"; break;
			case Yonma:  headerTxt << "�l�l�ł�"; break;
		}
		headerTxt << "�v���f�[�^ Ver. " << "???";

		/* �v���[���e�L�X�g�v�� */
		haifuBuffer.str("");
		haifuBuffer << headerTxt.str() << std::endl << std::endl <<
			"------------------------------------------------------------------------------" <<
			std::endl << std::endl;

		/* HTML�v�� */
		HThaifuBuffer.str("");
		HThaifuBuffer <<  "<html>" << std::endl << "<head>" << std::endl <<
			"<title>" << headerTxt.str() << "</title>" << std::endl <<
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=Shift_JIS\">" << std::endl <<
			"<link rel=\"stylesheet\" href=\"haifu.css\" type=\"text/css\">" << std::endl <<
			"</head>" << std::endl << "<body>" << std::endl <<
			"<h1>" << headerTxt.str() << "</h1>" << std::endl << "<hr>" << std::endl;
	}

	/* ��Ǖ��̔v���o�b�t�@�������� */
	__declspec(dllexport) void haifuinit() {
		static HaifuStreams* const bufs[] = {&haifuP, &HThaifuP};
		for (int i = 0; i < 2; i++) {
			for (int p = 0; p < PLAYERS; p++) {
				bufs[i]->streamDat[p].haipai.str("");
				bufs[i]->streamDat[p].tsumo.str("");
				bufs[i]->streamDat[p].tsumolabel.str("");
				bufs[i]->streamDat[p].sutehai.str("");
				bufs[i]->streamDat[p].sutehailabel.str("");
				bufs[i]->streamDat[p].final.str("");
			}
			bufs[i]->dora.str("");
			bufs[i]->uraDora.str("");
			bufs[i]->aliceDora.str("");
			bufs[i]->aliceDoraMax.str("");
			bufs[i]->resultDesc.str("");
		}
	}

	/* �z�v��v���ɋL�^ */
	__declspec(dllexport) void haifurechaipai(const GameTable* const gameStat) {
		for (int p = 0; p < PLAYERS; p++) {
			for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
				if (gameStat->Player[p].Hand[i].tile != NoTile) {
					tools::recordTile_Inline(
						&haifuP.streamDat[p].haipai,
						&HThaifuP.streamDat[p].haipai,
						gameStat->Player[p].Hand[i]
					);
				}
			}
			origPoint[p] = gameStat->Player[p].PlayerScore;
		}
	}

	/* �h����v���ɋL�^ */
	void haifurecdora(tileCode tmpDora) {
		tools::recordDoraStream(&haifuP.dora, &HThaifuP.dora, tmpDora);
	}
	__declspec(dllexport) void haifurecdora(int tmpDora) {
		haifurecdora((tileCode)tmpDora);
	}

	/* ���h����v���ɋL�^ */
	void haifurecuradora(tileCode tmpDora) {
		tools::recordDoraStream(&haifuP.uraDora, &HThaifuP.uraDora, tmpDora);
	}
	__declspec(dllexport) void haifurecuradora(int tmpDora) {
		haifurecdora((tileCode)tmpDora);
	}

	/* �A���X�h����v���ɋL�^ */
	void haifurecalicedora(tileCode tmpDora) {
		tools::recordDoraStream(&haifuP.aliceDora, &HThaifuP.aliceDora, tmpDora);
	}
	__declspec(dllexport) void haifurecalicedora(int tmpDora) {
		haifurecalicedora((tileCode)tmpDora);
	}

	/* �A���X�h�����X�V */
	__declspec(dllexport) void haifualicedoraupd() {
		if (haifuP.aliceDora.str().length() > haifuP.aliceDoraMax.str().length()) {
			haifuP.aliceDoraMax.str(haifuP.aliceDora.str());
			HThaifuP.aliceDoraMax.str(HThaifuP.aliceDora.str());
		}
	}

	/* �A���X�h�������� */
	__declspec(dllexport) void haifuresetalicedora() {
		haifuP.aliceDora.str("");
		HThaifuP.aliceDora.str("");
	}

	/* �h���̃Z�p���[�^�[ */
	__declspec(dllexport) void haifurecdorap() {
		haifuP.dora << " ";
		haifuP.uraDora << " ";
		haifuP.aliceDora << " ";
		HThaifuP.dora << "</span> <span class=\"tile\">";
		HThaifuP.uraDora << "</span> <span class=\"tile\">";
		HThaifuP.aliceDora << "</span> <span class=\"tile\">";
	}

	/* �̑ł�v���ɋL�^ */
	__declspec(dllexport) void haifurecmota(const GameTable* const gameStat, int DiscardTileIndex) {
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
		} else if ((DiscardTileIndex % 20) == (NUM_OF_TILES_IN_HAND - 1)) {
			// �c���؂�
			tools::recordBlank_Table(
				&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
				&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
			haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo << "�� ";
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo << "<td class=\"fallthru\">��</td>";
		} else {
			tools::haifuwritetsumohai(
				&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active,
				gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
				"�@ ", "");
		}
	}

	/* ���e�������ۂ���v���ɋL�^ */
	__declspec(dllexport) void haifurecfurikomi(const GameTable* const gameStat) {
		if (gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag) {
			// �����錾�v�̏ꍇ
			if (RonPlayers(gameStat) > 0) {
				// �����錾�v�ł̐U�荞��
				haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "�X ";
				HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "<td>���[�`<br>�E�`</td>";
			} else {
				// �������ʂ����ꍇ
				haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "ذ�";
				HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "<td>���[�`</td>";
			}
		} else {
			// ����ȊO�̏ꍇ
			if (RonPlayers(gameStat) > 0) {
				// �ʏ�̐U�荞��
				haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "�� ";
				HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "<td>�E�`</td>";
			} else {
				// �����Ȃ������ꍇ
				haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "�@ ";
				HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "<td></td>";
			}
		}
	}

	/* �`�[�������Ƃ�v���ɋL�^ */
	__declspec(dllexport) void haifurecchi(const GameTable* const gameStat) {
		tools::haifuwritetsumohai(
			&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
			gameStat->CurrentDiscard, "�� ", "�`�[");
	}
	/* �|���������Ƃ�v���ɋL�^ */
	__declspec(dllexport) void haifurecpon(const GameTable* const gameStat) {
		tools::haifuskip(
			&haifuP, &HThaifuP,
			gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
		tools::haifuwritetsumohai(
			&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
			gameStat->CurrentDiscard, "���", "�|��");
	}
	/* ���̘a�������Ƃ�v���ɋL�^ */
	__declspec(dllexport) void haifurectsumo(const GameTable* const gameStat) {
		tools::haifuwritetsumohai(
			&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active,
			gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
			"�� ", "�c��");
	}
	/* �喾�Ȃ������Ƃ�v���ɋL�^ */
	__declspec(dllexport) void haifurecminkan(const GameTable* const gameStat) {
		tools::haifuskip(
			&haifuP, &HThaifuP,
			gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
		tools::haifuwritetsumohai(
			&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
			gameStat->CurrentDiscard, "�� ", "�J��");
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehai,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehai);
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehailabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehailabel);
		// �v���̋L�q��́A���v�̎擾�͎����Ƃ��Ĉ���
		tools::haifuskipall(&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive);
	}

	namespace tools {
		namespace kan_sub {
			void recordKanOrFlower(
				const GameTable* const gameStat, int DiscardTileIndex,
				HaifuStreams* haifuP, HaifuStreams* HThaifuP
				) {
					if ((gameStat->TianHuFlag)||((DiscardTileIndex % 20) != (NUM_OF_TILES_IN_HAND - 1))) {
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
								gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex % 20]);
							haifukanflag = true;
						} else if (gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].tile ==
							gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex % 20].tile) {
								// �c�����Ă����v�Ɠ���������
								recordTile_Table(
									&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
									&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
									gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex % 20]);
								haifukanflag = false;
						} else {
							haifuwritetsumohai(
								haifuP, HThaifuP, gameStat->CurrentPlayer.Active,
								gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
								"�@ ", "");
							recordTile_Table(
								&haifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
								&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
								gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex % 20]);
							haifukanflag = true;
						}
					} else {
						recordTile_Table(
							&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
							&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
							gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex % 20]);
						haifukanflag = false;
					}
			}
			/* ���g�k�h��v���ɋL�^ */
			void inline recordChanKan(const GameTable* const gameStat, std::string pTxt, std::string hTxt) {
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
			void inline recordKan(const GameTable* const gameStat, std::string pTxt, std::string hTxt) {
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
		}
	}

	/* �����k��v���ɋL�^ */
	__declspec(dllexport) void haifurecnorth(const GameTable* const gameStat, int DiscardTileIndex) {
		tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
	}
	/* ���g�k�h��v���ɋL�^ */
	__declspec(dllexport) void haifurecchanpei(const GameTable* const gameStat) {
		tools::kan_sub::recordChanKan(gameStat, "��X", "<td>��<br>�E�`</td>");
	}
	/* �����k�����������ꍇ�̔v������ */
	__declspec(dllexport) void haifurecnorthproc(const GameTable* const gameStat) {
		tools::kan_sub::recordKan(gameStat, "�� ", "<td>��</td>");
	}

	/* �ÞȂȂ������Ȃ�v���ɋL�^ */
	__declspec(dllexport) void haifurecankan(const GameTable* const gameStat, int DiscardTileIndex) {
		tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
	}
	/* ���Ȃ�v���ɋL�^ */
	__declspec(dllexport) void haifurecchankan(const GameTable* const gameStat) {
		tools::kan_sub::recordChanKan(gameStat, "��X", "<td>�J��<br>�E�`</td>");
	}
	/* �J�������������ꍇ�̔v������ */
	__declspec(dllexport) void haifureckanproc(const GameTable* const gameStat) {
		tools::kan_sub::recordKan(gameStat, "�� ", "<td>�J��</td>");
	}

	/* �Ԕv��v���ɋL�^ */
	__declspec(dllexport) void haifurecflower(const GameTable* const gameStat, int DiscardTileIndex) {
		tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
		tools::kan_sub::recordKan(gameStat, "�� ", "<td>��</td>");
	}

#if 0

/* �z�v���o�b�t�@�ɏo�� */
#deffunc haifuwritebuffer var GameStat, var GameEnv, int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag, str ResultDesc, int RoundEndType
	notesel haifuBuffer
	tmpstr = "���쐼�k"
	tmptxt = ""+roundName(OrigTurn)
	if (OrigHonba > 0) {tmptxt += " "+OrigHonba+"�{��"}
	tmptxt += " �h���F"+haifuDora
	if ((RoundEndType == ENDKYOKU_AGARI)&&(tmpUraFlag == 1)&&(getRule(RULE_URADORA) != 1)) {tmptxt += "���h���F"+haifuUraDora}
	if ((RoundEndType == ENDKYOKU_AGARI)&&(tmpAliceFlag == 1)&&(getRule(RULE_ALICE) != 0)) {tmptxt += "�A���X�F"+haifuAliceDoraMax}
	noteadd tmptxt
	noteadd ""
	noteadd "���ʁF"+ResultDesc
	noteadd ""
	notesel HThaifuBuffer
	tmptxt = ""+roundName(OrigTurn)
	if (OrigHonba > 0) {tmptxt += " "+OrigHonba+"�{��"}
	tmptxt += " �h���F<span class=\"tile\">"+HThaifuDora+"</span>"
	if ((RoundEndType == ENDKYOKU_AGARI)&&(tmpUraFlag == 1)&&(getRule(RULE_URADORA) != 1)) {tmptxt += "���h���F<span class=\"tile\">"+HThaifuUraDora+"</span>"}
	if ((RoundEndType == ENDKYOKU_AGARI)&&(tmpAliceFlag == 1)&&(getRule(RULE_ALICE) != 0)) {tmptxt += "�A���X�F<span class=\"tile\">"+HThaifuAliceDoraMax+"</span>"}
	noteadd "<h2>"+tmptxt+"</h2>"
	noteadd "<p>���ʁF"+ResultDesc+"</p>"
	noteadd "<table>"
	tmptxt = ""
	repeat COLUMNS
		tmptxt += "<td width="+(100.0/double(COLUMNS))+"%></td>"
	loop
	noteadd "<tr>"+tmptxt+"</tr>"
	noteunsel
	repeat NUM_OF_ACTUAL_PLAYERS
		tmpcnt = RelativePositionOf(cnt, OrigTurn\NUM_OF_PLAYERS)
#ifdef SANMAT
		if (((OrigTurn\NUM_OF_PLAYERS)+cnt) >= NUM_OF_ACTUAL_PLAYERS) {tmpcnt = (tmpcnt+1)\NUM_OF_PLAYERS}
#endif
		// �ŏI�v�p
		repeat NUM_OF_TILES_IN_HAND
			if (getHand(GameStat, HAND_TILECODE, cnt, tmpcnt) > 0) {
				if (cnt == TSUMOHAI_INDEX) {
					if ((RoundEndType == ENDKYOKU_RYUUKYOKU)||(RoundEndType == ENDKYOKU_AGARI)||(RoundEndType == ENDKYOKU_CHONBO)) {
						if (getTsumoAgariFlag(GameStat) == 1) {haifuP(tmpcnt*6+5) += " �c��": HThaifuP(tmpcnt*6+5) += "</span> �c��<span class=\"tile\">"}
						else {haifuP(tmpcnt*6+5) += " ����": HThaifuP(tmpcnt*6+5) += "</span> ����<span class=\"tile\">"}
					} else {
						haifuP(tmpcnt*6+5) += " "
					}
				}
				haifuP(tmpcnt*6+5) += strmid(tilecodelabel, (getHand(GameStat, HAND_TILECODE, cnt, tmpcnt)+(getHand(GameStat, HAND_REDTILE, cnt, tmpcnt)*TILE_NONFLOWER_MAX))*2, 2)
				if (getHand(GameStat, HAND_REDTILE, cnt, tmpcnt)) {HThaifuP(tmpcnt*6+5) += "<span"+haifudoraClass(getHand(GameStat, HAND_REDTILE, cnt, tmpcnt))+">"}
				HThaifuP(tmpcnt*6+5) += strmid(HTtilecodelabel1, (getHand(GameStat, HAND_TILECODE, cnt, tmpcnt)+(getHand(GameStat, HAND_REDTILE, cnt, tmpcnt)*TILE_NONFLOWER_MAX)), 1)
				if (getHand(GameStat, HAND_REDTILE, cnt, tmpcnt)) {HThaifuP(tmpcnt*6+5) += "</span>"}
			}
		loop
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_ALL_SEASONS) {haifuP(tmpcnt*6+5) += " ": HThaifuP(tmpcnt*6+5) += "</span> <span class=\"tile\">"}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_SPRING) {haifuP(tmpcnt*6+5) += "�t": HThaifuP(tmpcnt*6+5) += "@"}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_SUMMER) {haifuP(tmpcnt*6+5) += "��": HThaifuP(tmpcnt*6+5) += ";"}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_AUTUMN) {haifuP(tmpcnt*6+5) += "�H": HThaifuP(tmpcnt*6+5) += ":"}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_WINTER) {haifuP(tmpcnt*6+5) += "�~": HThaifuP(tmpcnt*6+5) += "]"}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_ALL_FLOWERS) {haifuP(tmpcnt*6+5) += " ": HThaifuP(tmpcnt*6+5) += "</span> <span class=\"tile\">"}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_PLUM) {haifuP(tmpcnt*6+5) += "�~": HThaifuP(tmpcnt*6+5) += "-"}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_ORCHID) {haifuP(tmpcnt*6+5) += "��": HThaifuP(tmpcnt*6+5) += "^"}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_CHRYSANTHEMUM) {haifuP(tmpcnt*6+5) += "�e": HThaifuP(tmpcnt*6+5) += "["}
		if (getFlowerFlag(GameStat, tmpcnt)&FLOWER_BAMBOO) {haifuP(tmpcnt*6+5) += "�|": HThaifuP(tmpcnt*6+5) += "\\"}
		repeat MeldPointer(GameStat, tmpcnt), 1
			tmpAkaDoraFlag1 = (getMeld(GameStat, MELD_REDTILE, cnt, tmpcnt)&0x01)/0x01
			tmpAkaDoraFlag2 = (getMeld(GameStat, MELD_REDTILE, cnt, tmpcnt)&0x02)/0x02
			tmpAkaDoraFlag3 = (getMeld(GameStat, MELD_REDTILE, cnt, tmpcnt)&0x04)/0x04
			tmpAkaDoraFlag4 = (getMeld(GameStat, MELD_REDTILE, cnt, tmpcnt)&0x08)/0x08
			tmpAkaDoraFlag1 += (getMeld(GameStat, MELD_REDTILE, cnt, tmpcnt)&0x10)/0x10*2
			tmpAkaDoraFlag2 += (getMeld(GameStat, MELD_REDTILE, cnt, tmpcnt)&0x20)/0x20*2
			tmpAkaDoraFlag3 += (getMeld(GameStat, MELD_REDTILE, cnt, tmpcnt)&0x40)/0x40*2
			tmpAkaDoraFlag4 += (getMeld(GameStat, MELD_REDTILE, cnt, tmpcnt)&0x80)/0x80*2
			#define HT_NakiMianzi(%1,%2,%3)  if (%2) {HThaifuP(tmpcnt*6+5) += "<span"+haifudoraClass(%2)+">"}: HThaifuP(tmpcnt*6+5) += strmid((%3), (%1)+((%2)*TILE_NONFLOWER_MAX), 1): if (%2) {HThaifuP(tmpcnt*6+5) += "</span>"}
			switch (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)/MELD_TYPE_STEP)
				/* �����q */
				case MELD_SEQUENCE_EXPOSED_LOWER:
					haifuP(tmpcnt*6+5) += " ���`�["
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+1)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+2)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �`�[<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+1), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+2), tmpAkaDoraFlag3, HTtilecodelabel1
					swbreak
				case MELD_SEQUENCE_EXPOSED_MIDDLE:
					haifuP(tmpcnt*6+5) += " ���`�["
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+1)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+2)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �`�[<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+1), tmpAkaDoraFlag2, HTtilecodelabel2
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+2), tmpAkaDoraFlag3, HTtilecodelabel1
					swbreak
				case MELD_SEQUENCE_EXPOSED_UPPER:
					haifuP(tmpcnt*6+5) += " ���`�["
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+2)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+1)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �`�[<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+2), tmpAkaDoraFlag3, HTtilecodelabel2
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM+1), tmpAkaDoraFlag2, HTtilecodelabel1
					swbreak
				/* �����q */
				case MELD_TRIPLET_EXPOSED_LEFT:
					haifuP(tmpcnt*6+5) += " ���|��"
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �|��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					swbreak
				case MELD_TRIPLET_EXPOSED_CENTER:
					haifuP(tmpcnt*6+5) += " �ȃ|��"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �|��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					swbreak
				case MELD_TRIPLET_EXPOSED_RIGHT:
					haifuP(tmpcnt*6+5) += " ���|��"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					HThaifuP(tmpcnt*6+5) += "</span> �|��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					swbreak
				/* �ÞȎq */
				case MELD_QUAD_CONCEALED:
					haifuP(tmpcnt*6+5) += " ���J��"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag4*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �J��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag4, HTtilecodelabel1
					swbreak
				/* �喾�� */
				case MELD_QUAD_EXPOSED_LEFT:
					haifuP(tmpcnt*6+5) += " ���J��"
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag4*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �J��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag4, HTtilecodelabel1
					swbreak
				case MELD_QUAD_EXPOSED_CENTER:
					haifuP(tmpcnt*6+5) += " �ȃJ��"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag4*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �J��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag4, HTtilecodelabel1
					swbreak
				case MELD_QUAD_EXPOSED_RIGHT:
					haifuP(tmpcnt*6+5) += " ���J��"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag4*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					HThaifuP(tmpcnt*6+5) += "</span> �J��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag4, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					swbreak
				/* ���� */
				case MELD_QUAD_ADDED_LEFT:
					haifuP(tmpcnt*6+5) += " �����J��"
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag4*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �J��<span class=\"tile\">"
					HThaifuP(tmpcnt*6+5) += "<table class=\"kakan\"><tr><td>"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag4, HTtilecodelabel2
					HThaifuP(tmpcnt*6+5) += "<br>"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					HThaifuP(tmpcnt*6+5) += "</tr></td></table>"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					swbreak
				case MELD_QUAD_ADDED_CENTER:
					haifuP(tmpcnt*6+5) += " �ȁ��J��"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag4*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					HThaifuP(tmpcnt*6+5) += "</span> �J��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HThaifuP(tmpcnt*6+5) += "<table class=\"kakan\"><tr><td>"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag4, HTtilecodelabel2
					HThaifuP(tmpcnt*6+5) += "<br>"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					HThaifuP(tmpcnt*6+5) += "</tr></td></table>"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					swbreak
				case MELD_QUAD_ADDED_RIGHT:
					haifuP(tmpcnt*6+5) += " �����J��"
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag2*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag3*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += "["+strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag4*TILE_NONFLOWER_MAX))*2, 2)
					haifuP(tmpcnt*6+5) += strmid(tilecodelabel, ((getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM)+(tmpAkaDoraFlag1*TILE_NONFLOWER_MAX))*2, 2)+"]"
					HThaifuP(tmpcnt*6+5) += "</span> �J��<span class=\"tile\">"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag2, HTtilecodelabel1
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag3, HTtilecodelabel1
					HThaifuP(tmpcnt*6+5) += "<table class=\"kakan\"><tr><td>"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag4, HTtilecodelabel2
					HThaifuP(tmpcnt*6+5) += "<br>"
					HT_NakiMianzi (getMeld(GameStat, MELD_TILECODE, cnt, tmpcnt)\TILE_CODE_MAXIMUM), tmpAkaDoraFlag1, HTtilecodelabel2
					HThaifuP(tmpcnt*6+5) += "</tr></td></table>"
					swbreak
			swend
		loop
		// �_�_��
		haifutmpscore = ""
			tmpscoretxt = bignumtotext(origPoint, tmpcnt, 100, "", "��")
			haifutmpscore += tmpscoretxt
			
		if (origPoint(tmpcnt) != getScore(GameStat,tmpcnt)) {
			haifutmpscore += " �� "
			exportScore GameStat, scoreArray
			tmpscoretxt = bignumtotext(scoreArray, tmpcnt, 100, "", "��")
			if (tmpscoretxt == "") {tmpscoretxt += "0"}
			haifutmpscore += tmpscoretxt

			dim diffPoint, NUM_OF_DIGIT_GROUPS
			repeat NUM_OF_DIGIT_GROUPS
				diffPoint(cnt) = getScore(GameStat,tmpcnt, cnt)-origPoint(tmpcnt, cnt)
			loop
			diffflagfix diffPoint

			tmpscoretxt = bignumtotext(diffPoint, 0, 100, "+", "-")

			haifutmpscore += " ("+(tmpscoretxt)+")"
		}
		if (getRule(RULE_CHIP) != 0) {
			if (getChip(GameStat, tmpcnt) >= 0) {haifutmpscore += " �`�b�v: +"+getChip(GameStat, tmpcnt)}
			else {haifutmpscore += " �`�b�v: "+getChip(GameStat, tmpcnt)}
		}
		chatappend "*** "+getPlayerName(GameEnv, tmpcnt)+"("+windName(cnt)+") "+haifutmpscore+"\n"

		noteadd windName(cnt)+" "+getPlayerName(GameEnv, tmpcnt)+" "+haifutmpscore
		notesel HThaifuBuffer
		noteadd "<tr><td colspan="+COLUMNS+" class=\"player\">"+windName(cnt)+" "+getPlayerName(GameEnv, tmpcnt)+" "+haifutmpscore+"</td></tr>"
		noteunsel
#ifdef SANMA4
		if (cnt < 3) {
#endif
			noteadd "�z�v�F"+haifuP(tmpcnt*6)
			notesel HThaifuBuffer
			noteadd "<tr><td class=\"label\">�z�v</td><td colspan="+(COLUMNS-1)+"><span class=\"tile\">"+HThaifuP(tmpcnt*6)+"</span></td></tr>"
			noteunsel
#ifdef SANMA4
		}
#endif
		noteadd ""
#ifdef SANMA4
		if (cnt < 3) {
#endif
			if (getHeavenHandFlag(GameStat) == 0) {
				// �V�a(�܂��͐e�̏\�O�s�������v)�̏ꍇ�͏ȗ�
				noteadd "���́F"+haifuP(tmpcnt*6+1)
				noteadd "�@�@�@"+haifuP(tmpcnt*6+2)
				noteadd "�Ŕv�F"+haifuP(tmpcnt*6+3)
				noteadd "�@�@�@"+haifuP(tmpcnt*6+4)
				noteadd "�v�p�F"+haifuP(tmpcnt*6+5)
				noteadd ""
				notesel HThaifuBuffer
				noteadd "<tr class=\"tile\"><td class=\"label\" rowspan=2>����</td>"+HThaifuP(tmpcnt*6+1)+"</tr>"
				noteadd "<tr class=\"notice\">"+HThaifuP(tmpcnt*6+2)+"</tr>"
				noteadd "<tr class=\"tile\"><td class=\"label\" rowspan=2>�Ŕv</td>"+HThaifuP(tmpcnt*6+3)+"</tr>"
				noteadd "<tr class=\"notice\">"+HThaifuP(tmpcnt*6+4)+"</tr>"
				noteadd "<tr><td class=\"label\">�v�p</td><td colspan="+(COLUMNS-1)+" class=\"hand\"><span class=\"tile\">"+HThaifuP(tmpcnt*6+5)+"</span></td></tr>"
				noteunsel
			}
#ifdef SANMA4
		}
#endif
	loop
	noteadd "------------------------------------------------------------------------------"
	noteadd ""
	noteunsel
	notesel HThaifuBuffer
	noteadd "</table>"
	noteadd "<hr>"
	noteunsel
return

/* �z�v��ۑ� */
#deffunc haifusave
	configPath = confPath()
	if (configPath != "") {configPath += "\\"}
	notesel HThaifuBuffer
	noteadd "</body>"
	noteadd "</html>"
	noteunsel
	notesel haifuBuffer
#ifdef SANMAS
	tmpfilename1 = configPath+"haifu\\mihassnm_"+VERSION_MAJ+"_"+VERSION_MED+"_"+VERSION_MIN+""+VERSION_MIC
#else
	#ifdef SANMA4
		tmpfilename1 = configPath+"haifu\\mihaysnm_"+VERSION_MAJ+"_"+VERSION_MED+"_"+VERSION_MIN+""+VERSION_MIC
	#else
		#ifdef SANMA
			tmpfilename1 = configPath+"haifu\\mihasanm_"+VERSION_MAJ+"_"+VERSION_MED+"_"+VERSION_MIN+""+VERSION_MIC
		#else
			tmpfilename1 = configPath+"haifu\\mihajong_"+VERSION_MAJ+"_"+VERSION_MED+"_"+VERSION_MIN+""+VERSION_MIC
		#endif
	#endif
#endif
	tmpfilename2 = strf("%04d%02d%02d_%02d%02d", gettime(0), gettime(1), gettime(3), gettime(4), gettime(5))
	notesave tmpfilename1+"_haifu_"+tmpfilename2+".txt"
	noteunsel
	notesel ChatLog
	notesave tmpfilename1+"_chatlog_"+tmpfilename2+".txt"
	noteunsel
	notesel HThaifuBuffer
	notesave tmpfilename1+"_haifu_"+tmpfilename2+".htm"
	noteunsel
return

#global
#endif
}
