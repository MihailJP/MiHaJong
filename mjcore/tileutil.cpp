#include "tileutil.h"

/* �l�l�O���p */
PLAYER_ID* tobePlayed(const GameTable* const gameStat) {
	static PLAYER_ID tp[] = {
		(gameStat->GameRound % PLAYERS),
		((gameStat->GameRound + 1) % PLAYERS),
		((gameStat->GameRound + 2) % PLAYERS)
	};
	if (chkGameType(gameStat, Sanma4)) return tp;
	else return NULL;
}
PLAYER_ID tobePlayed(const GameTable* const gameStat, int id) {
	assert((id >= 0)&&(id < 13));
	PLAYER_ID *tp = tobePlayed(gameStat);
	return tp[id];
}

/* ��㎚�v�̃R�[�h�ꗗ */
tileCode* Honor_Major_Tiles() {
	static const tileCode YaojiuPai[] = {
		CharacterOne, CharacterNine, CircleOne, CircleNine, BambooOne, BambooNine,
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	return (tileCode*)&YaojiuPai;
}
tileCode Honor_Major_Tiles(int code) {
	assert((code >= 0)&&(code < 13));
	static const tileCode* YaojiuPai = Honor_Major_Tiles();
	return YaojiuPai[code];
}
/* ��㎚�v���ǂ������ׂ� */
bool isYaojiu(tileCode code) {
	bool ans = false;
	for (int i = 0; i < 13; i++)
		if (code == Honor_Major_Tiles(i)) ans = true;
	return ans;
}

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
				if (RuleData::getRule("ankan_conceal") != 0) break; // �ÞȔ�J�����[����������J�E���g���Ȃ�
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
MJCORE TileStatus gettilestatus(
	const GameTable* const gameStat, PLAYER_ID targetPlayer, int targetTile, bool CheckMode)
{
	const tileCode* const theTile = &gameStat->Player[targetPlayer].Hand[targetTile].tile;

	TileStatus tlStat; memset(&tlStat, 0, sizeof(tlStat)); // ������
	if (*theTile == NoTile) return tlStat; // �Ȃ�������߂�

	tlStat.isExistent = true; // �����ɂ��ǂ蒅������A����͑��݂���v�ł�
	auto tlCount = countTilesInHand(gameStat, targetPlayer); // �v�𐔂���A�b�͂��ꂩ�炾

	if (tlCount[*theTile] == 4) tlStat.canFormQuad = true; // �ÞȂ��\�Ȕv
	if (tlCount[*theTile] >= 3) tlStat.formsTriplet = true; // �Í����`�����Ă���ꍇ
	
	if (*theTile < TILE_SUIT_HONORS) { // ���q���`�����Ă���ꍇ
		if ((tlCount[*theTile] >= 1)&&(tlCount[int(*theTile) + 1] >= 1)&&(tlCount[int(*theTile) + 2] >= 1))
			tlStat.formsSequence = true;
		else if ((tlCount[*theTile] >= 1)&&(tlCount[int(*theTile) + 1] >= 1)&&(tlCount[int(*theTile) - 1] >= 1))
			tlStat.formsSequence = true;
		else if ((*theTile > 1)&&(tlCount[*theTile] >= 1)&&(tlCount[int(*theTile) - 2] >= 1)&&(tlCount[int(*theTile) - 1] >= 1))
			tlStat.formsSequence = true;
	}

	if ((tlCount[*theTile] >= 2)&&(!tlStat.formsTriplet)) tlStat.formsPair = true; // �Ύq���`�����Ă���ꍇ

	if (*theTile < TILE_SUIT_HONORS) { // �Ӓ����`�����Ă���ꍇ
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[int(*theTile) + 1] >= 1)&&(*theTile % TILE_SUIT_STEP == 1))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[int(*theTile) + 1] >= 1)&&(*theTile % TILE_SUIT_STEP == 8))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[int(*theTile) - 1] >= 1)&&(*theTile % TILE_SUIT_STEP == 2))
				tlStat.seqSingleSideWait = true;
			else if ((tlCount[int(*theTile) - 1] >= 1)&&(*theTile % TILE_SUIT_STEP == 9))
				tlStat.seqSingleSideWait = true;
		}
	}
	
	if (*theTile < TILE_SUIT_HONORS) { // ���ʓ��q���`�����Ă���ꍇ
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[int(*theTile) + 1] >= 1)&&(*theTile % TILE_SUIT_STEP != 1)&&
				(*theTile % TILE_SUIT_STEP != 8)) tlStat.seqDoubleSideWait = true;
			else if ((tlCount[int(*theTile) - 1] >= 1)&&(*theTile % TILE_SUIT_STEP != 2)&&
				(*theTile % TILE_SUIT_STEP != 9)) tlStat.seqDoubleSideWait = true;
		}
	}
	
	if (*theTile < TILE_SUIT_HONORS) { // �ƒ����`�����Ă���ꍇ
		if ((tlCount[*theTile] >= 1)&&((!tlStat.formsSequence)||(CheckMode))) {
			if ((tlCount[int(*theTile) + 2] >= 1)&&(*theTile % TILE_SUIT_STEP != 9))
				tlStat.seqMidWait = true;
			else if ((*theTile > 1)&&(tlCount[int(*theTile) - 2] >= 1)&&(*theTile % TILE_SUIT_STEP != 1))
				tlStat.seqMidWait = true;
		}
	}

	return tlStat;
}
__declspec(dllexport) int gettilestatus(
	const GameTable* const gameStat, int targetPlayer, int targetTile, int CheckMode)
{
	auto ans = gettilestatus(gameStat, (PLAYER_ID)targetPlayer, targetTile, (bool)CheckMode);
	return
		(ans.isExistent ?
		(ans.canFormQuad ? 0x40 : 0) +
		(ans.seqMidWait ? 0x20 : 0) +
		(ans.seqDoubleSideWait ? 0x10 : 0) +
		(ans.seqSingleSideWait ? 0x08 : 0) +
		(ans.formsPair ? 0x04 : 0) +
		(ans.formsSequence ? 0x02 : 0) +
		(ans.formsTriplet ? 0x01 : 0)
		: -999);
}

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
		if (ShantenAnalyzer::calcShanten(&tmpGameStat, targetPlayer, ShantenAnalyzer::shantenAll) == -1) { // �҂��ɂȂ��Ă�����
			machihaiInfo.MachiMen++; machihaiInfo.Machihai[i].MachihaiFlag = true; // �t���O���Z�b�g���܂��傤
			machihaiInfo.MachihaiTotal += // �J�E���g�����Z���܂��傤
				(machihaiInfo.Machihai[i].MachihaiCount =
				(4 - seenTiles[i] - countTiles[i]));
			// �t���e�����ǂ����𔻒f����
			for (int j = 1; j <= tmpGameStat.Player[targetPlayer].DiscardPointer; j++) {
				if (tmpGameStat.Player[targetPlayer].Discard[j].tcode.tile == i) // ������ނ̎̂Ĕv������������
					machihaiInfo.FuritenFlag = true; // �t���e���Ɣ��f���܂�
			}
			if (RuleData::getRule("kakan_furiten") == 1) { // ���Ȃ̔v���t���e���Ƃ݂Ȃ����[���Ȃ�
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
		if (ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenAll) == -1)
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
MJCORE bool chkdaopaiability(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	Int8ByTile TileCount = countTilesInHand(gameStat, targetPlayer);
	int YaojiuCount = 0; bool AtamaFlag = false;
	for (int i = 0; i < 13; i++) // ���I��v�P��ނɂ��A�P���J�E���g����B
		if (TileCount[Honor_Major_Tiles(i)] >= 1) YaojiuCount++;
	return (YaojiuCount >= 9);
}
__declspec(dllexport) int chkdaopaiability(const GameTable* const gameStat, int targetPlayer) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	return chkdaopaiability(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

/* �h����ݒ肷�� */
namespace setdora_tools {
	tileCode getNextOf(const GameTable* const gameStat, tileCode tc) { // �l�N�X�g�v
		tileCode ans = (tileCode)((int)tc + 1);
		if ((chkGameType(gameStat, SanmaX))&&(ans == CharacterTwo)) ans = CharacterNine;
		else if (ans == (tileCode)10) ans = CharacterOne;
		else if (ans == (tileCode)20) ans = CircleOne;
		else if (ans == (tileCode)30) ans = BambooOne;
		else if (ans == (tileCode)35) ans = EastWind;
		else if (ans == (tileCode)38) ans = WhiteDragon;
		return ans;
	}

	tileCode getPrevOf(const GameTable* const gameStat, tileCode tc) { // �O�̔v
		tileCode ans = (tileCode)((int)tc - 1);
		if ((chkGameType(gameStat, SanmaX))&&(ans == CharacterEight)) ans = CharacterOne;
		else if (ans == (tileCode)0) ans = CharacterNine;
		else if (ans == (tileCode)10) ans = CircleNine;
		else if (ans == (tileCode)20) ans = BambooNine;
		else if (ans == (tileCode)30) ans = NorthWind;
		else if (ans == (tileCode)34) ans = RedDragon;
		return ans;
	}

	void addDora(GameTable* const gameStat, tileCode tc, int Mode) {
		for (int i = ((RuleData::getRule("nagatacho") != 0) ? tc % 10 : tc);
			i <= (((RuleData::getRule("nagatacho") != 0) && (tc < TILE_SUIT_HONORS)) ? TILE_SUIT_HONORS : tc);
			i += 10) {
				std::ostringstream o;
				if (Mode) gameStat->DoraFlag.Ura[i]++;	// �h����ݒ肷��
				else gameStat->DoraFlag.Omote[i]++;	// �h����ݒ肷��
				o << "�v�R�[�h [" << (int)tc << "] ���h���ɂ��܂����B";
				debug(o.str().c_str());
				if (Mode) haifu::haifurecuradora((tileCode)i);
				else haifu::haifurecdora((tileCode)i);
		}
	}
}

__declspec(dllexport) void setdora(GameTable* const gameStat, int Mode) {
	std::ostringstream o;
	o << "�h���̒ǉ� �|�C���^ [" << gameStat->DoraPointer <<
		"] �v�R�[�h [" << (int)gameStat->Deck[gameStat->DoraPointer + Mode].tile <<
		"] ���[�h [" << Mode << "]";
	debug(o.str().c_str());
	if (gameStat->Deck[gameStat->DoraPointer + Mode].tile > TILE_SUIT_FLOWERS) {
		// �Ԕv���h���\���v�ɂȂ����Ƃ�
		setdora_tools::addDora(gameStat, Flower, Mode);
	} else {
		if (RuleData::getRule("dora_indicator") == 2) {
			// �O�̔v���h���i���C���t���p�j
			if ((gameStat->Deck[gameStat->DoraPointer + Mode].tile >= 10) ||
				(!chkGameType(gameStat, SanmaX)))
					setdora_tools::addDora(gameStat,
						setdora_tools::getPrevOf(gameStat, gameStat->Deck[gameStat->DoraPointer + Mode].tile),
						Mode);
		}
		if ((RuleData::getRule("dora_indicator") == 1)||(RuleData::getRule("dora_indicator") == 2)) {
			// �����h��
			setdora_tools::addDora(gameStat,
				gameStat->Deck[gameStat->DoraPointer + Mode].tile,
				Mode);
		}
		if (RuleData::getRule("dora_indicator") != 1) {
			// �l�N�X�g�h��
			setdora_tools::addDora(gameStat,
				setdora_tools::getNextOf(gameStat, gameStat->Deck[gameStat->DoraPointer + Mode].tile),
				Mode);
		}
	}
}

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
		SHANTEN shanten = ShantenAnalyzer::calcShanten(&tmpGameStat, targetPlayer, ShantenAnalyzer::shantenAll);
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

/* ���ΐ��̈ʒu�𒲂ׂ� */
__declspec(dllexport) void calcdoukasen(GameTable* const gameStat) {
	/* ���ΐ��̈ʒu���v�Z���� */
	if (RuleData::getRule("doukasen") != 0) {
		if (chkGameType(gameStat, Sanma4)) {
			PLAYER_ID* tmpDoukasen = new PLAYER_ID(
				((30 - ((diceSum(gameStat) - 1) * 36 * 2 + 
				diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 36) + 30) % 3);
			gameStat->DoukasenPlayer = tobePlayed(gameStat, *tmpDoukasen);
			delete tmpDoukasen;
		} else if (chkGameType(gameStat, SanmaT)) {
			gameStat->DoukasenPlayer =
				((30 - ((diceSum(gameStat) - 1 +
				(gameStat->GameRound - (gameStat->GameRound / 4))) * 36 * 2 +
				diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 36) + 30) % 3;
		} else {
			int* tmp;
			switch (RuleData::getRule("flower_tiles")) {
			case 3:
				gameStat->DoukasenPlayer =
					((40 - ((diceSum(gameStat) - 1 +
					(gameStat->GameRound % PLAYERS)) * 36 * 3 + 
					diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 36) + 40)
					% PLAYERS;
				break;
			case 0:
				gameStat->DoukasenPlayer =
					((40 - ((diceSum(gameStat) - 1 +
					(gameStat->GameRound % PLAYERS)) * 34 * 3 +
					diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 34) + 40)
					% PLAYERS;
				break;
			default:
				gameStat->DoukasenPlayer =
					((40 - ((diceSum(gameStat) - 1 +
					(gameStat->GameRound % PLAYERS)) * 70 * 3 / 2 +
					diceSum(gameStat) * 2 + gameStat->TilePointer - 1) / 35) + 40)
					% PLAYERS;
				tmp = new int(
					((diceSum(gameStat) - 1 + (gameStat->GameRound % PLAYERS)) * 70 * 3 / 2 +
					diceSum(gameStat) * 2 + gameStat->TilePointer - 1) % 70
					);
				if (*tmp == (((diceSum(gameStat) + gameStat->GameRound) % 2) * 35))
					gameStat->DoukasenPlayer = (40 + gameStat->DoukasenPlayer + 1) % PLAYERS;
				delete tmp;
			}
		}
	}
}

/* ���v���ǂ������ׂ� */
bool isTenpai(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	auto Shanten = ShantenAnalyzer::calcShanten(gameStat, targetPlayer, ShantenAnalyzer::shantenAll);
	if (gameStat->Player[targetPlayer].AgariHouki) Shanten = 1; // �A�K�������Ȃ狭���s��
	if (EnvTable::Instantiate()->PlayerDat[targetPlayer].RemotePlayerFlag == -1)
		return false;
	return (Shanten <= 0);
}
__declspec(dllexport) int isTenpai(const GameTable* const gameStat, void *, int targetPlayer) {
	return isTenpai(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}

/* �������т��ǂ������ׂ� */
bool isNagashiMangan(const GameTable* const gameStat, PLAYER_ID targetPlayer) {
	if (gameStat->Player[targetPlayer].AgariHouki) return false; // �a����������̏������������т�F�߂Ȃ�
	if (EnvTable::Instantiate()->PlayerDat[targetPlayer].RemotePlayerFlag == -1)
		return false;
	int YaojiuSutehai = 0;
	for (int i = 1; i <= gameStat->Player[targetPlayer].DiscardPointer; i++) {
		// ���ꂽ�v�������疳��
		if (gameStat->Player[targetPlayer].Discard[i].dstat == discardTaken) continue;
		if (gameStat->Player[targetPlayer].Discard[i].dstat == discardRiichiTaken) continue;
		// ����Ă��Ȃ��v
		if (isYaojiu(gameStat->Player[targetPlayer].Discard[i].tcode.tile)) YaojiuSutehai++;
	}
	// �S���Y������v��������true
	return (YaojiuSutehai == gameStat->Player[targetPlayer].DiscardPointer);
}
__declspec(dllexport) int isNagashiMangan(const GameTable* const gameStat, void *, int targetPlayer) {
	return isNagashiMangan(gameStat, (PLAYER_ID)targetPlayer) ? 1 : 0;
}
