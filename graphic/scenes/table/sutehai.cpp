#include "sutehai.h"
//#include "../../rule.h"
#include "../../utils.h"
#include <cassert>

namespace mihajong_graphic {

using utils::playerRelative;

/* ŽÌ”v‚Ì•\Ž¦ */
void GameTableScreen::SutehaiReconst::ReconstructSutehai_portrait(const GameTable* gameStat, PlayerID targetPlayer,
	unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos) {
		assert(gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile != NoTile);
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
		case sOpposite: /* ‘Î–Ê */
			TileTexture->NewTile(32 - tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				TableSize - DiscardPosH - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				UpsideDown, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sLeft: /* ã‰Æ */
			TileTexture->NewTile(33 + tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				DiscardPosH + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				Clockwise, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sRight: /* ‰º‰Æ */
			TileTexture->NewTile(98 - tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				TableSize - DiscardPosH - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				Withershins, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sSelf: /* Ž©•ª */
			TileTexture->NewTile(99 + tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				DiscardPosH + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				Portrait, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		}
		if ((tilePosCol >= DiscardLineLength) && (tilePosRow < 2)) {
			tilePosCol = 0; ++tilePosRow; shiftPos = false;
		}
}
void GameTableScreen::SutehaiReconst::ReconstructSutehai_rotated(const GameTable* gameStat, PlayerID targetPlayer,
	unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos) {
		assert(gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile != NoTile);
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
		case sOpposite: /* ‘Î–Ê */
			TileTexture->NewTile(32 - tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				TableSize - DiscardPosH - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				Clockwise, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sLeft: /* ã‰Æ */
			TileTexture->NewTile(33 + tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				DiscardPosH + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				Portrait, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sRight: /* ‰º‰Æ */
			TileTexture->NewTile(98 - tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				TableSize - DiscardPosH - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				UpsideDown, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sSelf: /* Ž©•ª */
			TileTexture->NewTile(99 + tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				DiscardPosH + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				Withershins, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		}
		shiftPos = true;
		if ((tilePosCol >= DiscardLineLength) && (tilePosRow < 2)) {
			tilePosCol = 0; ++tilePosRow; shiftPos = false;
		}
}
void GameTableScreen::SutehaiReconst::Reconstruct(const GameTable* gameStat, PlayerID targetPlayer) {
	unsigned tilePosCol = 0, tilePosRow = 0; bool shiftPosFlag = false, riichiFlag = false;
	for (unsigned tileID = 0; tileID < 33; ++tileID) {
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
			case sOpposite: TileTexture->DelTile(32 - tileID); break; /* ‘Î–Ê */
			case sLeft:     TileTexture->DelTile(33 + tileID); break; /* ã‰Æ */
			case sRight:    TileTexture->DelTile(98 - tileID); break; /* ‰º‰Æ */
			case sSelf:     TileTexture->DelTile(99 + tileID); break; /* Ž©•ª */
		}
	}
	for (unsigned i = 0; i < gameStat->Player.val[targetPlayer].DiscardPointer; ++i) {
		if ((gameStat->Player.val[targetPlayer].Discard[i + 1].dstat == discardRiichi) ||
			(gameStat->Player.val[targetPlayer].Discard[i + 1].dstat == discardRiichiTaken))
			riichiFlag = true;
		if ((gameStat->Player.val[targetPlayer].Discard[i + 1].dstat == discardNormal) ||
			(gameStat->Player.val[targetPlayer].Discard[i + 1].dstat == discardRiichi)) {
				if (riichiFlag) {
					ReconstructSutehai_rotated(gameStat, targetPlayer, i, tilePosCol, tilePosRow, shiftPosFlag);
					riichiFlag = false;
				} else {
					ReconstructSutehai_portrait(gameStat, targetPlayer, i, tilePosCol, tilePosRow, shiftPosFlag);
				}
		}
	}
}

void GameTableScreen::SutehaiReconst::Render() {
	TileTexture->Render();
}

GameTableScreen::SutehaiReconst::SutehaiReconst(GameTableScreen* parent) {
	caller = parent;
	TileTexture = new ShowTile(parent->caller->getDevice());
}

GameTableScreen::SutehaiReconst::~SutehaiReconst() {
	delete TileTexture;
}

}
