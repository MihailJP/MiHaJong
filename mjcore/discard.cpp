#include "discard.h"

#include "gametbl.h"

/* 従来の打牌コードに直す */
int DiscardTileNum::toSingleInt() const {
	switch (this->type) {
		case DiscardType::agari: return -1;
		case DiscardType::kyuushu: return -2;
		case DiscardType::disconnect: return -3;
		default: return static_cast<int>(this->type) * TypeStep + static_cast<int>(this->id);
	}
}
/* 従来の打牌コードから変換 */
DiscardTileNum DiscardTileNum::fromSingleInt(int val) {
	DiscardTileNum discardID;
	switch (val) {
	case -1:
		discardID.type = DiscardType::agari; discardID.id = NumOfTilesInHand - 1;
		break;
	case -2:
		discardID.type = DiscardType::kyuushu; discardID.id = NumOfTilesInHand - 1;
		break;
	case -3:
		discardID.type = DiscardType::disconnect; discardID.id = NumOfTilesInHand - 1;
		break;
	default:
		discardID.type = static_cast<DiscardType>(val / 20);
		discardID.id = val % 20;
		break;
	}
	return discardID;
}
