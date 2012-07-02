#include "tileutil.h"

/* ���v���� */
void lipai(GameTable* gameStat, PLAYER_ID targetPlayer) {
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
			gameStat->Player[targetPlayer].Hand[i].tile = (tileCode)999;
			gameStat->Player[targetPlayer].Hand[i].red = 0;
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
		if (gameStat->Player[targetPlayer].Hand[i].tile == (tileCode)999) {
			gameStat->Player[targetPlayer].Hand[i].tile = NoTile;
			gameStat->Player[targetPlayer].Hand[i].red = 0;
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
__declspec(dllexport) void lipai(GameTable* gameStat, int targetPlayer) {
	// ���v����(HSP�݊��p)
	lipai(gameStat, (PLAYER_ID)targetPlayer);
}

/* ��Ɍ����Ă�� */
/* ���Ƃŏ��� */
MJCORE void countseentiles(Int8ByTile* seenTiles, GameTable* gameStat) {
	// ��Ɍ����Ă���v�̐��𐔂���

	
}
#if 0
/* ��Ɍ����Ă�� */
#module
#include "const.hsp"
#include "mjcore.hsp"
#deffunc countseentiles array SeenTiles, var GameStat
/*
		countseentiles p1, p2
		��Ɍ����Ă���v�̐��𐔂���

		p1 : ���ʂ��i�[����z��
		p2 : ��̏󋵂��i�[�����\����

		���ꂼ��̔v����Ɍ����Ă��鐔�𐔂��܂��B
*/
	dim SeenTiles, TILE_CODE_MAXIMUM // �v�Z����v
	// �̔v�Ō����Ă閇��
	repeat NUM_OF_PLAYERS: tmp = cnt
		if (DiscardPointer(GameStat, tmp) == 0) {continue}
		repeat DiscardPointer(GameStat, tmp), 1
			SeenTiles(getDiscard(GameStat, DISCARD_TILECODE, cnt, tmp))++
		loop
	loop
	// �N���̕��I�Ō����Ă閇��
	repeat NUM_OF_PLAYERS: tmp = cnt
		if (MeldPointer(GameStat, tmp) == 0) {continue}
		repeat MeldPointer(GameStat, tmp), 1
			if (getMeld(GameStat, MELD_TILECODE, cnt, tmp) < MELD_TRIPLET*MELD_TYPE_STEP) {
				// �����q
				SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+0)++
				SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+1)++
				SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+2)++
			} else: if (getMeld(GameStat, MELD_TILECODE, cnt, tmp) < MELD_QUAD*MELD_TYPE_STEP) {
				// �����q
				SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+0) += 3
			} else {
				if ((getMeld(GameStat, MELD_TILECODE, cnt, tmp)/MELD_TYPE_STEP != MELD_QUAD_CONCEALED)||(getRule(RULE_ANKAN_CONCEAL) == 0)) {
					// �Ȏq (�ÞȔ�J���ݒ�Ō����ĂȂ��ÞȂ܂ŃJ�E���g���Ȃ��悤�ɏ��������׍H���Ă���)
					SeenTiles((getMeld(GameStat, MELD_TILECODE, cnt, tmp)\TILE_CODE_MAXIMUM)+0) += 4
				}
			}
		loop
	loop
	// �h���\���v�Ō����Ă閇��
	repeat 6
#ifdef ALLSANMA
		if (getDoraPointer(GameStat) <= (102-getRinshanExtension(GameStat)-cnt*2)) {SeenTiles(getWall(GameStat, WALL_TILECODE, 102-getRinshanExtension(GameStat)-cnt*2))++}
#else
		if (getDoraPointer(GameStat) <= (130-cnt*2)) {SeenTiles(getWall(GameStat, WALL_TILECODE, 130-cnt*2))++}
#endif
	loop
return
#global
#endif


/* ��v�̂�������P���ɒ��ڂ��� */
/* ���Ƃŏ��� */

/* �U���łȂ����̃`�F�b�N�E�҂��v�𐔂��鏈�����܂� */
/* ���Ƃŏ��� */
std::unique_ptr<MachihaiInfo> chkFuriten(GameTable* gameStat, PLAYER_ID targetPlayer) {
	std::unique_ptr<MachihaiInfo> machihaiInfo(new MachihaiInfo);
	trace("�U�����ǂ����E�҂��v�𒲂ׂ܂��B");
	/* countseentiles�Ăяo�� */

	/* �������E���u�� */
	return machihaiInfo;
}
#if 0
	countseentiles SeenTiles, GameStat
	countTilesInHand TileCountCFTTmp, GameStat, targetPlayer
	dim MachihaiCount, TILE_NONFLOWER_STRICT_MAX+1: MachihaiTotal = 0: MachiMen = 0
	dim MachihaiFlag, TILE_NONFLOWER_STRICT_MAX+1
	FuritenFlag = 0
	tmpTsumoHai = TsumoTile(GameStat, targetPlayer)
	repeat TILE_NONFLOWER_STRICT_MAX,1
		MachihaiCount(cnt) = -1
		if (cnt\TILE_SUIT_STEP == 0) {continue}
		setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, targetPlayer, cnt
		Shanten = countshanten(GameStat, targetPlayer, SHANTEN_ALL)
		if (Shanten == -1) {
			MachiMen++: MachihaiFlag(cnt) = 1
			MachihaiCount(cnt) = (4 - SeenTiles(cnt) - TileCountCFTTmp(cnt))
			MachihaiTotal += (4 - SeenTiles(cnt) - TileCountCFTTmp(cnt))
			tmpHai = cnt
			repeat DiscardPointer(GameStat, targetPlayer), 1
				if ((getDiscard(GameStat, DISCARD_TILECODE, cnt, targetPlayer)\TILE_CODE_MAXIMUM) == tmpHai) {
					FuritenFlag = 1
				}
			loop
			if ((MeldPointer(GameStat, targetPlayer) > 0)&&(getRule(RULE_KAKAN_FURITEN) == 1)) {
				repeat MeldPointer(GameStat, targetPlayer), 1
					if (((getMeld(GameStat, MELD_TILECODE, cnt, targetPlayer)/MELD_TYPE_STEP) >= MELD_QUAD_ADDED_MIN)&&((getMeld(GameStat, MELD_TILECODE, cnt, targetPlayer)\TILE_CODE_MAXIMUM) == tmpHai)) {
						FuritenFlag = 1
					}
				loop
			}
			repeat DiscardPointer(GameStat, targetPlayer), 1
				if ((getDiscard(GameStat, DISCARD_TILECODE, cnt, targetPlayer)\TILE_CODE_MAXIMUM) == tmpHai) {
					FuritenFlag = 1
				}
			loop
		}
		setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, targetPlayer, tmpTsumoHai
	loop
	repeat NUM_OF_PLAYERS
		tmpooooo = cnt
		// �I�[�v�������҂̎�v��������
		if ((cnt != targetPlayer)&&(getRichiFlag(GameStat, RICHI_OPENFLAG, cnt) > 0)) {
			repeat NUM_OF_TILES_IN_HAND-1
				if (getHand(GameStat, HAND_TILECODE, cnt, tmpooooo) > 0) {
					if (MachihaiFlag(getHand(GameStat, HAND_TILECODE, cnt, tmpooooo)) == 1) {
						MachihaiCount(getHand(GameStat, HAND_TILECODE, cnt, tmpooooo))--
						MachihaiTotal--
					}
				}
			loop
		}
	loop
return
#global
#endif

/* �I�[�v�������̑҂��v */
void chkOpenMachi(GameTable* gameStat, PLAYER_ID targetPlayer) {
	// �I�[�v�����[�`�̑҂��v�����X�V����
	assert(gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile == NoTile);
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) {
		/* �ςȔv�Ōv�Z���Ȃ��悤�ɂ��܂��傤 */
		if (i % TILE_SUIT_STEP == 0) continue;
		if (i % TILE_SUIT_STEP > tileCode::RedDragon) continue;
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
__declspec(dllexport) void chkOpenMachi(GameTable* gameStat, int targetPlayer) {
	// �I�[�v�����[�`�̑҂��v�����X�V����(HSP�݊��p)
	chkOpenMachi(gameStat, (PLAYER_ID)targetPlayer);
}

/* �h����ݒ肷�� */
/* ���Ƃŏ����B�v���ւ̋L�^���߂Ƃ����邵�c�c */

/* ������̈ÞȂ̉� */
MJCORE bool chkAnkanAbility(GameTable* gameStat, PLAYER_ID targetPlayer) {
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

	/* ��v�𒲂ׂ� */
	TileCount tlCount = countTilesInHand(gameStat, targetPlayer);

	/* ��v��ޔ����� */
	HAND_TILES tmpHand;
	memcpy(tmpHand, gameStat->Player[targetPlayer].Hand, sizeof(HAND_TILES));

	/* �c�������v���S�������ĂȂ��Ȃ瓖�R�ɞȂ͕s�\ */
	/* ������ł��著��Ȃ͂ł��Ȃ��̂Ńc�������v�������ׂ�΂悢 */
	if (tlCount[gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile] < 4) {
		o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] �́A4�������Ă��܂���B"; debug(o.str());
		return false;
	} else {
		o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] �́A��v�ɍ��킹��4������܂��B"; debug(o.str());
	}

	/* ���v�ɏ��q�͂Ȃ��̂ł���ȍ~�̃`�F�b�N�������Ƃ��Ȃ��\�Ƃ킩�� */
	switch (gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
	case EastWind: case SouthWind: case WestWind: case NorthWind:
	case WhiteDragon: case GreenDragon: case RedDragon:
		o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] �͎��v�ł��B"; debug(o.str());
		return true;
	case Spring: case Summer: case Autumn: case Winter:
	case Plum: case Orchid: case Chrysanthemum: case Bamboo:
		/* �Ԕv��ȁH�@����k�� */
		o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] �͉Ԕv�ł��B"; debug(o.str());
		return false;
	default:
		o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] �͐��v�ł��B"; debug(o.str());
		break;
	}

	/* �c�������v�������ɂȂ肤��Ȃ�߂� */
	int tmpTakenCount = 0;
	for (int i = 0; i < (NUM_OF_TILES_IN_HAND-1); i++) {
		if (gameStat->Player[targetPlayer].Hand[i].tile ==
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile) {
				gameStat->Player[targetPlayer].Hand[i].tile = NoTile;
				tmpTakenCount++;
		}
		if (tmpTakenCount == 2) break;
	}
	gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;
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
	SHANTEN shanten = calcShanten(gameStat, targetPlayer, shantenAll);
	memcpy(gameStat->Player[targetPlayer].Hand, tmpHand, sizeof(HAND_TILES)); /* ��v�������߂� */
	if (shanten == 1) {
		o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] �́A�A�^�}���ł��B"; debug(o.str());
		return false;
	} else {
		o.str(""); o << "�c���v [" << std::setw(2) << std::setfill('0') <<
			gameStat->Player[targetPlayer].Hand[NUM_OF_TILES_IN_HAND - 1].tile <<
			"] �́A�A�^�}���ł͂���܂���B"; debug(o.str());
	}

	/* �ÞȂ���O�̑҂��v�𒲂ׂ� */
	/* chkfuriten�֐��Ăяo���������ɏ��� */
	
	/* �������E���u�� */
	return false;
}
#if 0
	/* �ÞȂ���O�̑҂��v�𒲂ׂ� */
	chkfuriten FuritenFlag, MachihaiFlag, MachihaiCount, MachihaiTotal, MachiMen, GameStat, targetPlayer
	dim tmpchkMachihaiFlag, TILE_NONFLOWER_STRICT_MAX+1
	repeat TILE_NONFLOWER_STRICT_MAX+1: tmpchkMachihaiFlag(cnt) = MachihaiFlag(cnt): loop
	/* �ÞȂ������Ƃ̑҂��v�𒲂ׂ� */
	repeat NUM_OF_TILES_IN_HAND
		if (getHand(GameStat, HAND_TILECODE, cnt, targetPlayer) == TsumoTile(GameStat, targetPlayer)) {
			setHand GameStat, HAND_TILECODE, cnt, targetPlayer, 0: tmptakencount++
		}
	loop
	setHand GameStat, HAND_TILECODE, TSUMOHAI_INDEX, targetPlayer, 0
	MeldPointerIncrement GameStat, targetPlayer
	chkfuriten FuritenFlag, MachihaiFlag, MachihaiCount, MachihaiTotal, MachiMen, GameStat, targetPlayer
	/* ��v�������߂� */
	MeldPointerDecrement GameStat, targetPlayer: repeat NUM_OF_TILES_IN_HAND /* ��v�������߂� */
		setHand GameStat, HAND_TILECODE, cnt, targetPlayer, tmpkanhand(cnt)
	loop
	/* �҂��v����v���Ȃ��Ȃ�߂� */
	MachiHaiDifference = 0
	repeat TILE_NONFLOWER_STRICT_MAX+1
		if (tmpchkMachihaiFlag(cnt) != MachihaiFlag(cnt)) {
			MachiHaiDifference = 1
		}
	loop
	if (MachiHaiDifference == 1) {
		debug "�Ȃ̑O��ő҂��v����v���܂���B"
		return 0
	}
	/* �����𖞂����Þ� */
	debug "�Ȃ̑O��ő҂��v����v���܂����B"
	return 1
#global
#endif
