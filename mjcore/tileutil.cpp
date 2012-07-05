#include "tileutil.h"

/* ���v���� */
void lipai(GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// ���v����

	std::ostringstream o;
	o.str(""); o << "���v���s���܂��B�v���C���[ [" << (int)targetPlayer << "]"; debug(o.str());

	/* ��v�f�[�^���f�o�b�O���O�ɏo�́F�r�t�H�[ */
	o.str(""); o << "���v�O�̎�v [";
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++)
		o << std::setw(2) << std::setfill('0') << (int)gameStat->Player[targetPlayer].Hand[i].tile
		<< ((i < (NUM_OF_TILES_IN_HAND - 1)) ? " " : "");
	o << "]"; trace(o.str());

	/* �\�[�g�̏����Ƃ��Ĕv�̂Ȃ��g������ */
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == NoTile) {
			gameStat->Player[targetPlayer].Hand[i].tile = TilePad;
			gameStat->Player[targetPlayer].Hand[i].red = Normal;
		}
	}

	/* ��������\�[�g��� */
	TILE tmpTile;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		for (int j = i + 1; j < NUM_OF_TILES_IN_HAND; j++) {
			if (gameStat->Player[targetPlayer].Hand[i].tile >
				gameStat->Player[targetPlayer].Hand[j].tile) {
					/* TILE�\���̂�POD������Amemcpy��������Ă�������ˁH */
					memcpy(&tmpTile, &(gameStat->Player[targetPlayer].Hand[i]), sizeof(TILE));
					memcpy( &(gameStat->Player[targetPlayer].Hand[i]),
						&(gameStat->Player[targetPlayer].Hand[j]), sizeof(TILE));
					memcpy(&(gameStat->Player[targetPlayer].Hand[j]), &tmpTile, sizeof(TILE));
			}
			else if ((gameStat->Player[targetPlayer].Hand[i].tile ==
				gameStat->Player[targetPlayer].Hand[j].tile) &&
				(gameStat->Player[targetPlayer].Hand[i].red <
				gameStat->Player[targetPlayer].Hand[j].red)) {
					/* ������ނ̔v�ł��ԃh���������łȂ����ŕ��בւ��� */
					memcpy(&tmpTile, &(gameStat->Player[targetPlayer].Hand[i]), sizeof(TILE));
					memcpy( &(gameStat->Player[targetPlayer].Hand[i]),
						&(gameStat->Player[targetPlayer].Hand[j]), sizeof(TILE));
					memcpy(&(gameStat->Player[targetPlayer].Hand[j]), &tmpTile, sizeof(TILE));
			}
		}
	}

	/* �󗓂������Ƃ���͌��ɖ߂��Ă����܂��傤 */
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile == TilePad) {
			gameStat->Player[targetPlayer].Hand[i].tile = NoTile;
			gameStat->Player[targetPlayer].Hand[i].red = Normal;
		}
	}

	/* ��v�f�[�^���f�o�b�O���O�ɏo�́F�A�t�^�[ */
	o.str(""); o << "���v��̎�v [";
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++)
		o << std::setw(2) << std::setfill('0') << (int)gameStat->Player[targetPlayer].Hand[i].tile
		<< ((i < (NUM_OF_TILES_IN_HAND - 1)) ? " " : "");
	o << "]"; trace(o.str());

	/* ���v�����I */
	return;
}
__declspec(dllexport) void lipai(GameTable* const gameStat, int targetPlayer) {
	// ���v����(HSP�݊��p)
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	lipai(gameStat, (PLAYER_ID)targetPlayer);
}

/* ��Ɍ����Ă�� */
MJCORE Int8ByTile countseentiles(const GameTable* const gameStat) {
	// ��Ɍ����Ă���v�̐��𐔂���
	std::ostringstream o; // �G���[���b�Z�[�W�p

	Int8ByTile seenTiles; // �v�Z����v
	memset(&seenTiles, 0, sizeof(seenTiles)); // �܂��́A������

	// �̔v�Ō����Ă閇��
	for (int i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].DiscardPointer == 0) continue;
		for (int j = 1; j <= gameStat->Player[i].DiscardPointer; j++)
			seenTiles[gameStat->Player[i].Discard[j].tcode.tile]++;
	}

	// �N���̕��I�Ō����Ă閇��
	for (int i = 0; i < PLAYERS; i++) {
		if (gameStat->Player[i].MeldPointer == 0) continue;
		for (int j = 1; j <= gameStat->Player[i].MeldPointer; j++) {
			switch (gameStat->Player[i].Meld[j].mstat) {
			case meldSequenceExposedLower:
			case meldSequenceExposedMiddle:
			case meldSequenceExposedUpper:
				// �����q(�`�[)
				// �܂��A�������R�[�h�ɂȂ��Ă��邩���m�F����(�f�o�b�O��)
				assert(gameStat->Player[i].Meld[j].tile % TILE_SUIT_STEP <= 7);
				assert(gameStat->Player[i].Meld[j].tile % TILE_SUIT_STEP > 0);
				assert(gameStat->Player[i].Meld[j].tile < TILE_SUIT_HONORS);
				// �J�E���g�A�b�v
				for (int k = gameStat->Player[i].Meld[j].tile;
					k <= gameStat->Player[i].Meld[j].tile + 2; k++)
					seenTiles[gameStat->Player[i].Meld[j].tile]++;
				// �����܂�
				break;
			case meldTripletExposedLeft:
			case meldTripletExposedCenter:
			case meldTripletExposedRight:
				// �����q(�|��)
				seenTiles[gameStat->Player[i].Meld[j].tile] += 3;
				break;
			case meldQuadConcealed:
				// �Þ�
				if (getRule(RULE_ANKAN_CONCEAL) != 0) break; // �ÞȔ�J�����[����������J�E���g���Ȃ�
				/* FALLTHRU */
			case meldQuadExposedLeft:   case meldQuadAddedLeft:
			case meldQuadExposedCenter: case meldQuadAddedCenter:
			case meldQuadExposedRight:  case meldQuadAddedRight:
				// ����
				seenTiles[gameStat->Player[i].Meld[j].tile] += 4;
				break;
			default:
				RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "���I�f�[�^�ɈÏ��q�A�Í��q�A�܂��͕s���Ȏ�ނ̖ʎq�����o����܂���");
			}
		}
	}

	// �h���\���v�Ō����Ă閇��
	for (int i = 0; i < 6; i++) {
		if (chkGameType(gameStat, AllSanma)) {
			if (gameStat->DoraPointer <= (102 - gameStat->ExtraRinshan - i * 2))
				seenTiles[gameStat->Deck[102 - gameStat->ExtraRinshan - i * 2].tile]++;
		} else {
			if (gameStat->DoraPointer <= (130 - i * 2))
				seenTiles[gameStat->Deck[130 - i * 2].tile]++;
		}
	}

	return seenTiles;
}
__declspec(dllexport) void countseentiles(int* const seenTiles, const GameTable* const gameStat) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	Int8ByTile tileCnt = countseentiles(gameStat);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) seenTiles[i] = (int)tileCnt[i];
}

/* ��v�ɑ��݂���v����ޕʂɃJ�E���g���� */
MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PLAYER_ID playerID) {
	// ��v�ɑ��݂���v����ޕʂɃJ�E���g����i���ʎq�E�ÞȂ͏����j
	Int8ByTile count; memset(&count, 0, sizeof(count)); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = gameStat->Player[playerID].Hand[i].tile) != NoTile)
			count[tmpTC]++;
	}
	return count;
}
__declspec(dllexport) void countTilesInHand(int* const tileCount, const GameTable* const gameStat, int playerID) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	Int8ByTile tCount = countTilesInHand(gameStat, (PLAYER_ID)playerID);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) tileCount[i] = (int)tCount[i];
}

/* ��v�ɑ��݂���ԃh������ޕʂɃJ�E���g���� */
MJCORE Int8ByTile countRedTilesInHand(const GameTable* const gameStat, PLAYER_ID playerID, int doraCol) {
	assert((doraCol == 1)||(doraCol == 2));
	// ��v�ɑ��݂���v����ޕʂɃJ�E���g����i���ʎq�E�ÞȂ͏����j
	Int8ByTile count; memset(&count, 0, sizeof(count)); tileCode tmpTC;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if ((tmpTC = tileCode(gameStat->Player[playerID].Hand[i].tile)) != NoTile)
			if (tileCode(gameStat->Player[playerID].Hand[i].red) == doraCol)
				count[tmpTC]++;
	}
	// �ԃh���̂ݐ�����Ƃ��͕֋X�̂��߁A���ʎq��������
	for (int i = 1; i < gameStat->Player[playerID].MeldPointer; i++) {
		switch (gameStat->Player[playerID].Meld[i].mstat) {
		case meldSequenceExposedLower: case meldSequenceExposedMiddle:
		case meldSequenceExposedUpper:
			// ���q�̎�
			if (gameStat->Player[playerID].Meld[i].red[0] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			if (gameStat->Player[playerID].Meld[i].red[1] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile + 1]++;
			if (gameStat->Player[playerID].Meld[i].red[2] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile + 2]++;
			break;
		case meldQuadExposedLeft: case meldQuadAddedLeft:
		case meldQuadExposedCenter: case meldQuadAddedCenter:
		case meldQuadExposedRight: case meldQuadAddedRight:
		case meldQuadConcealed: // �ÞȂ������Ă����܂��傤�c�c
			// �Ȏq�̎�
			if (gameStat->Player[playerID].Meld[i].red[3] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			/* FALLTHRU */
		case meldTripletExposedLeft: case meldTripletExposedCenter:
		case meldTripletExposedRight:
			// ���q�̎�(�Ȏq���܂�)
			if (gameStat->Player[playerID].Meld[i].red[0] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			if (gameStat->Player[playerID].Meld[i].red[1] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			if (gameStat->Player[playerID].Meld[i].red[2] == doraCol)
				count[gameStat->Player[playerID].Meld[i].tile]++;
			break;
		default:
			// �ُ�f�[�^
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, "���I�f�[�^�ɈÏ��q�A�Í��q�A�܂��͕s���Ȏ�ނ̖ʎq�����o����܂���");
		}
	}
	return count;
}
__declspec(dllexport) void countRedTilesInHand(int* const tileCount, const GameTable* const gameStat, int playerID) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	Int8ByTile tCount = countRedTilesInHand(gameStat, (PLAYER_ID)playerID, 1);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) tileCount[i] = (int)tCount[i];
}


/* ��v�̂�������P���ɒ��ڂ��� */
/* ���Ƃŏ��� */

/* �U���łȂ����̃`�F�b�N�E�҂��v�𐔂��鏈�����܂� */
MJCORE MachihaiInfo chkFuriten(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	trace("�U�����ǂ����E�҂��v�𒲂ׂ܂��B");
	Int8ByTile seenTiles = countseentiles(gameStat); // �����Ă�v�̐��𐔂���
	Int8ByTile countTiles = countTilesInHand(gameStat, targetPlayer); // ��̓��̔v�̐��𐔂���
	MachihaiInfo machihaiInfo; memset(&machihaiInfo, 0, sizeof(machihaiInfo)); // ������
	GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(GameTable)); // �e���|��������I�u�W�F�N�g(��������͉���@�̐��E�c�c)

	for (tileCode i = CharacterOne; i < Flower; i = (tileCode)((int)i + 1)) {
		if ((int)i % TILE_SUIT_STEP == 0) continue; // �Ȃ��v��������߂�
		tmpGameStat.Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = i;
		if (calcShanten(&tmpGameStat, targetPlayer, shantenAll) == -1) { // �҂��ɂȂ��Ă�����
			machihaiInfo.MachiMen++; machihaiInfo.Machihai[i].MachihaiFlag = true; // �t���O���Z�b�g���܂��傤
			machihaiInfo.MachihaiTotal += // �J�E���g�����Z���܂��傤
				(machihaiInfo.Machihai[i].MachihaiCount =
				(4 - seenTiles[i] - countTiles[i]));
			// �t���e�����ǂ����𔻒f����
			for (int j = 1; j <= tmpGameStat.Player[targetPlayer].DiscardPointer; j++) {
				if (tmpGameStat.Player[targetPlayer].Discard[j].tcode.tile == i) // ������ނ̎̂Ĕv������������
					machihaiInfo.FuritenFlag = true; // �t���e���Ɣ��f���܂�
			}
			if (getRule(RULE_KAKAN_FURITEN) == 1) { // ���Ȃ̔v���t���e���Ƃ݂Ȃ����[���Ȃ�
				for (int j = 1; j <= tmpGameStat.Player[targetPlayer].MeldPointer; j++) {
					switch (tmpGameStat.Player[targetPlayer].Meld[j].mstat) {
					case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight: // ���ȂŁA�Ȃ�����
						if (tmpGameStat.Player[targetPlayer].Meld[j].tile == i) // ������ނ̎̂Ĕv������������
							machihaiInfo.FuritenFlag = true; // �t���e���Ɣ��f���܂�
						break;
					}
				}
			}
		}
	}

	// �I�[�v�������҂̎�v��������
	for (int p = 0; p < PLAYERS; p++) {
		if ((p != targetPlayer) && (gameStat->Player[p].RichiFlag.OpenFlag)) { // �����ȊO�̃I�[�v�����[�`�҂̎��A
			for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) { // ���ꂼ��̎�v�ɂ��āA
				if (gameStat->Player[p].Hand[i].tile != NoTile) { // �u�����N�ł͂Ȃ��āA
					if (machihaiInfo.Machihai[gameStat->Player[p].Hand[i].tile].MachihaiCount > 0) { // 1���ȏ゠��͂��̑҂��v�ƔF������Ă��鎞
						machihaiInfo.Machihai[gameStat->Player[p].Hand[i].tile].MachihaiCount--;
						machihaiInfo.MachihaiTotal--; // �҂��v�̐������炷
					}
				}
			}
		}
	}

	/* �v�Z�����A�l��Ԃ��܂��傤 */
	return machihaiInfo;
}
__declspec(dllexport) void chkFuriten(
	int* const furitenFlag, int* const machihaiFlag, int* const machihaiCount,
	int* const machihaiTotal, int* const machiMen, const GameTable* const gameStat,
	int targetPlayer) {
		assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
		MachihaiInfo machihaiInfo = chkFuriten(gameStat, (PLAYER_ID)targetPlayer);
		*furitenFlag = machihaiInfo.FuritenFlag ? 1 : 0;
		for (int i = 0; i < TILE_NONFLOWER_MAX; i++) {
			machihaiFlag[i] = machihaiInfo.Machihai[i].MachihaiFlag ? 1 : 0;
			machihaiCount[i] = machihaiInfo.Machihai[i].MachihaiFlag ?
				(int)machihaiInfo.Machihai[i].MachihaiCount : (-1);
		}
		*machihaiTotal = (int)machihaiInfo.MachihaiTotal;
		*machiMen = (int)machihaiInfo.MachiMen;
}

/* �I�[�v�������̑҂��v */
void chkOpenMachi(GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// �I�[�v�����[�`�̑҂��v�����X�V����
	assert(gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile == NoTile);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) {
		/* �ςȔv�Ōv�Z���Ȃ��悤�ɂ��܂��傤 */
		if (i % TILE_SUIT_STEP == 0) continue;
		if (i % TILE_SUIT_STEP > RedDragon) continue;
		/* �܂��́A����v���c�������Ɖ��肵�܂� */
		gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = (tileCode)i;
		/* �������ꂪ�a���ɂȂ��Ă�����A�t���O���Z�b�g���܂��傤 */
		if (calcShanten(gameStat, targetPlayer, shantenAll) == -1)
			gameStat->OpenRichiWait[i] = true;
		/* ��������ׂĂ̔v�ɂ��Ď��s���܂��傤 */
	}
	/* ���Ƃɖ߂��̂�Y��Ȃ��悤�ɂ��܂��傤�B�����Ȃ��Α��v�ɂȂ��Ă��܂��܂� */
	gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;

	/* ����ŏ������I���܂����B�߂�܂��傤 */
	return;
}
__declspec(dllexport) void chkOpenMachi(GameTable* const gameStat, int targetPlayer) {
	// �I�[�v�����[�`�̑҂��v�����X�V����(HSP�݊��p)
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	chkOpenMachi(gameStat, (PLAYER_ID)targetPlayer);
}

/* ����v�������\���ǂ����̃`�F�b�N */
/* ���Ƃ� */

/* �h����ݒ肷�� */
/* ���Ƃŏ����B�v���ւ̋L�^���߂Ƃ����邵�c�c */

/* ������̈ÞȂ̉� */
namespace chkAnkanAbilityTools { // chkAnkanAbility�֐��p�̏���
	bool CheckForNumOfTiles(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
		/* �c�������v���S�������ĂȂ��Ȃ瓖�R�ɞȂ͕s�\ */
		/* ������ł��著��Ȃ͂ł��Ȃ��̂Ńc�������v�������ׂ�΂悢 */
		std::ostringstream o;
		Int8ByTile tlCount = countTilesInHand(gameStat, targetPlayer);
		if (tlCount[gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile] < 4) {
			o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] �́A4�������Ă��܂���B"; debug(o.str());
			return true;
		} else {
			o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] �́A��v�ɍ��킹��4������܂��B"; debug(o.str());
			return false;
		}
	}
	int CheckForTileClass(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
		/* ���v�ɏ��q�͂Ȃ��̂ł���ȍ~�̃`�F�b�N�������Ƃ��Ȃ��\�Ƃ킩�� */
		std::ostringstream o;
		switch (gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
		case EastWind: case SouthWind: case WestWind: case NorthWind:
		case WhiteDragon: case GreenDragon: case RedDragon:
			o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] �͎��v�ł��B"; debug(o.str());
			return 1;
		case Spring: case Summer: case Autumn: case Winter:
		case Plum: case Orchid: case Chrysanthemum: case Bamboo:
			/* �Ԕv��ȁH�@����k�� */
			o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] �͉Ԕv�ł��B"; debug(o.str());
			return 2;
		default:
			o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] �͐��v�ł��B"; debug(o.str());
			return 0;
		}
	}
	bool CheckForAtama(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
		/* �c�������v�������ɂȂ肤�邩���ׂ� */
		std::ostringstream o;
		GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(tmpGameStat));

		// 2������
		int tmpTakenCount = 0;
		for (int i = 0; i < (NUM_OF_TILES_IN_HAND-1); i++) {
			if (tmpGameStat.Player[targetPlayer].Hand[i].tile ==
				tmpGameStat.Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
					tmpGameStat.Player[targetPlayer].Hand[i].tile = NoTile;
					tmpTakenCount++;
			}
			if (tmpTakenCount == 2) break;
		}
		tmpGameStat.Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;
		/*	
			��F
			�ʎq���Q�A�Ύq�Ⓝ�q���P�A���������@�ƒ�`����B
			�ʎq�I�[�o�[�͏��O���邱�ƁB���v���W�Ȃ璮�v�A�X�Ȃ�a��
			�P�P�P�R�S�S�S������������ �����̏ꍇ
			+�@++�P++-�Q-++-�Q-++-�Q-+ �c �W(���v)
			�����P�R�S�S�S������������ ���P�𐝓��Ɏ���
				+�P++-�Q-++-�Q-++-�Q-+ �c �V(�����)
			�P�P�P�R�S�S�S������������ ��������낤�Ƃ����
			+�@++�P++-�Q-++-�Q-+�~     �c �U(�����)
		*/
		SHANTEN shanten = calcShanten(&tmpGameStat, targetPlayer, shantenAll);
		if (shanten == 1) {
			o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] �́A�A�^�}���ł��B"; debug(o.str());
			return true;
		} else {
			o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
				(int)gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
				"] �́A�A�^�}���ł͂���܂���B"; debug(o.str());
			return false;
		}
	}
	bool CheckIfMachihaiMatches(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
		/* �҂��v����v���邩�ǂ������ׂ� */
		std::ostringstream o;

		/* �ÞȂ���O�̑҂��v�𒲂ׂ� */
		MachihaiInfo machiInfo_Before = chkFuriten(gameStat, targetPlayer);
		/* �ÞȂ������Ƃ̑҂��v�𒲂ׂ� */
		int tmpTakenCount = 0; // �v�̐��̊m�F�p
		GameTable tmpGameStat; memcpy(&tmpGameStat, gameStat, sizeof(GameTable));
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if (tmpGameStat.Player[targetPlayer].Hand[i].tile ==
				tmpGameStat.Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
					tmpGameStat.Player[targetPlayer].Hand[i].tile = NoTile;
					tmpTakenCount++;
			}
		}
		assert(tmpTakenCount == 4); // �f�o�b�O�p�F���傤��4�����������m�F
		tmpGameStat.Player[targetPlayer].Meld[++ tmpGameStat.Player[targetPlayer].MeldPointer].tile =
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile; /* �c�������v�� */
		tmpGameStat.Player[targetPlayer].Meld[tmpGameStat.Player[targetPlayer].MeldPointer].mstat =
			meldQuadConcealed; /* �ÞȂ����Ƃ݂Ȃ� */
		MachihaiInfo machiInfo_After = chkFuriten(&tmpGameStat, targetPlayer);

		/* �҂��v�͈�v���邩�H */
		bool machihaiDiffers = false;
		for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
			if (machiInfo_Before.Machihai[i].MachihaiFlag != machiInfo_After.Machihai[i].MachihaiFlag)
				machihaiDiffers = true;
		if (machihaiDiffers) { // �҂��v����v���Ȃ��Ȃ�A�Ȃł��Ȃ�
			debug("�Ȃ̑O��ő҂��v����v���܂���B");
			return false;
		} else { // �����𖞂����Þ�
			debug("�Ȃ̑O��ő҂��v����v���܂����B");
			return true;
		}
	}
}

MJCORE bool chkAnkanAbility(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	// ���[�`��̈ÞȂ��ł��邩�ǂ����𔻒肷��

	/*
	 * ���[�`��̈ÞȂ������������F
	 * 1) �����(�Ⴆ��555567�Ǝ����Ă��鎞8�����̂���5��Ȃ��邱��)�����Ă͂����Ȃ�
	 * 2) �ʎq�\�����ς���Ă͂����Ȃ�
	 * ����(2)���������₷���悤�ɏ���������Ǝ��̂悤�ɂȂ�F
	 * 2a) �������ƂȂ�v(��F22234�Ƃ����`�ɂ�����2)���ÞȂ��Ă͂����Ȃ�
	 * 2b) �҂����ς���Ă͂����Ȃ�
	 */

	std::ostringstream o;
	o.str(""); o << "�ÞȂ̉ۂ𔻒肵�܂��B�v���C���[ [" << (int)targetPlayer << "]"; debug(o.str());

	/* �c�������v���S�������ĂȂ��Ȃ瓖�R�ɞȂ͕s�\ */
	/* ������ł��著��Ȃ͂ł��Ȃ��̂Ńc�������v�������ׂ�΂悢 */
	if (chkAnkanAbilityTools::CheckForNumOfTiles(gameStat, targetPlayer)) return false;

	/* ���v�ɏ��q�͂Ȃ��̂ł���ȍ~�̃`�F�b�N�������Ƃ��Ȃ��\�Ƃ킩�� */
	switch (chkAnkanAbilityTools::CheckForTileClass(gameStat, targetPlayer)) {
		case 1: return true; // ���v
		case 2: return false; // �Ԕv
	}

	/* �c�������v�������ɂȂ肤��Ȃ�߂� */
	if (chkAnkanAbilityTools::CheckForAtama(gameStat, targetPlayer)) return false;

	/* �҂��v����v���邩�ǂ������ׂ� */
	return (chkAnkanAbilityTools::CheckIfMachihaiMatches(gameStat, targetPlayer));
}
__declspec(dllexport) int chkAnkanAbility(const GameTable* const gameStat, int targetPlayer) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	return chkAnkanAbility(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}
