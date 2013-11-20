﻿#include "discard.h"

#include "gametbl.h"

/* 従来の打牌コードに直す */
int DiscardTileNum::toSingleInt() const {
	switch (this->type) {
		case Agari: return -1;
		case Kyuushu: return -2;
		case Disconnect: return -3;
		default: return (int)this->type * TypeStep + (int)this->id;
	}
}
/* 従来の打牌コードから変換 */
DiscardTileNum DiscardTileNum::fromSingleInt(int val) {
	DiscardTileNum discardID;
	switch (val) {
	case -1:
		discardID.type = Agari; discardID.id = NumOfTilesInHand - 1;
		break;
	case -2:
		discardID.type = Kyuushu; discardID.id = NumOfTilesInHand - 1;
		break;
	case -3:
		discardID.type = Disconnect; discardID.id = NumOfTilesInHand - 1;
		break;
	default:
		discardID.type = (discardType)(val / 20);
		discardID.id = val % 20;
		break;
	}
	return discardID;
}
