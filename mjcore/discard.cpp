#include "discard.h"

#include "gametbl.h"

/* �]���̑Ŕv�R�[�h�ɒ��� */
int DiscardTileNum::toSingleInt() const {
	switch (this->type) {
		case Agari: return -1;
		case Kyuushu: return -2;
		case Disconnect: return -3;
		default: return (int)this->type * 20 + (int)this->id;
	}
}
/* �]���̑Ŕv�R�[�h����ϊ� */
DiscardTileNum DiscardTileNum::fromSingleInt(int val) {
	DiscardTileNum discardID;
	switch (val) {
	case -1:
		discardID.type = Agari; discardID.id = NUM_OF_TILES_IN_HAND - 1;
		break;
	case -2:
		discardID.type = Kyuushu; discardID.id = NUM_OF_TILES_IN_HAND - 1;
		break;
	case -3:
		discardID.type = Disconnect; discardID.id = NUM_OF_TILES_IN_HAND - 1;
		break;
	default:
		discardID.type = (discardType)(val / 20);
		discardID.id = val % 20;
		break;
	}
	return discardID;
}
