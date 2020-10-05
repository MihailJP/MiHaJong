#include "sutehai.h"
//#include "../../rule.h"
#include "../../utils.h"
#include <cassert>

namespace mihajong_graphic {

using utils::playerRelative;

/* 捨牌の表示 */
void GameTableScreen::SutehaiReconst::ReconstructSutehai_portrait(const GameTable* gameStat, PlayerID targetPlayer,
	unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos) {
		assert(gameStat->Player[targetPlayer].Discard[tileID + 1].tcode);
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
		case SeatRelative::opposite: /* 対面 */
			TileTexture->NewTile(32 - tileID,
				gameStat->Player[targetPlayer].Discard[tileID + 1].tcode,
				TableSize - DiscardPosH() - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				TileDirection::upsideDown, TileSide::obverse,
				gameStat->Player[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case SeatRelative::left: /* 上家 */
			TileTexture->NewTile(33 + tileID,
				gameStat->Player[targetPlayer].Discard[tileID + 1].tcode,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				DiscardPosH() + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				TileDirection::clockwise, TileSide::obverse,
				gameStat->Player[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case SeatRelative::right: /* 下家 */
			TileTexture->NewTile(98 - tileID,
				gameStat->Player[targetPlayer].Discard[tileID + 1].tcode,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				TableSize - DiscardPosH() - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				TileDirection::withershins, TileSide::obverse,
				gameStat->Player[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case SeatRelative::self: /* 自分 */
			TileTexture->NewTile(99 + tileID,
				gameStat->Player[targetPlayer].Discard[tileID + 1].tcode,
				DiscardPosH() + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				TileDirection::portrait, TileSide::obverse,
				gameStat->Player[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		}
		if ((tilePosCol >= DiscardLineLength()) && (tilePosRow < 2)) {
			tilePosCol = 0; ++tilePosRow; shiftPos = false;
		}
}
void GameTableScreen::SutehaiReconst::ReconstructSutehai_rotated(const GameTable* gameStat, PlayerID targetPlayer,
	unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos) {
		assert(gameStat->Player[targetPlayer].Discard[tileID + 1].tcode);
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
		case SeatRelative::opposite: /* 対面 */
			TileTexture->NewTile(32 - tileID,
				gameStat->Player[targetPlayer].Discard[tileID + 1].tcode,
				TableSize - DiscardPosH() - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				TileDirection::clockwise, TileSide::obverse,
				gameStat->Player[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case SeatRelative::left: /* 上家 */
			TileTexture->NewTile(33 + tileID,
				gameStat->Player[targetPlayer].Discard[tileID + 1].tcode,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				DiscardPosH() + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				TileDirection::portrait, TileSide::obverse,
				gameStat->Player[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case SeatRelative::right: /* 下家 */
			TileTexture->NewTile(98 - tileID,
				gameStat->Player[targetPlayer].Discard[tileID + 1].tcode,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				TableSize - DiscardPosH() - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				TileDirection::upsideDown, TileSide::obverse,
				gameStat->Player[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case SeatRelative::self: /* 自分 */
			TileTexture->NewTile(99 + tileID,
				gameStat->Player[targetPlayer].Discard[tileID + 1].tcode,
				DiscardPosH() + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				TileDirection::withershins, TileSide::obverse,
				gameStat->Player[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		}
		shiftPos = true;
		if ((tilePosCol >= DiscardLineLength()) && (tilePosRow < 2)) {
			tilePosCol = 0; ++tilePosRow; shiftPos = false;
		}
}
void GameTableScreen::SutehaiReconst::Reconstruct(const GameTable* gameStat, PlayerID targetPlayer) {
	unsigned tilePosCol = 0, tilePosRow = 0; bool shiftPosFlag = false, riichiFlag = false;
	for (unsigned tileID = 0; tileID < 33; ++tileID) {
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
			case SeatRelative::opposite: TileTexture->DelTile(32 - tileID); break; /* 対面 */
			case SeatRelative::left:     TileTexture->DelTile(33 + tileID); break; /* 上家 */
			case SeatRelative::right:    TileTexture->DelTile(98 - tileID); break; /* 下家 */
			case SeatRelative::self:     TileTexture->DelTile(99 + tileID); break; /* 自分 */
		}
	}
	for (unsigned i = 0; i < gameStat->Player[targetPlayer].DiscardPointer; ++i) {
		if (gameStat->Player[targetPlayer].Discard[i + 1].tcode) { // WORKAROUND: 何故かNoTileになる問題（花牌を親が抜いていて子が1巡目にポン）
			if ((gameStat->Player[targetPlayer].Discard[i + 1].dstat == DiscardStat::riichi) ||
				(gameStat->Player[targetPlayer].Discard[i + 1].dstat == DiscardStat::riichiTaken))
				riichiFlag = true;
			if ((gameStat->Player[targetPlayer].Discard[i + 1].dstat == DiscardStat::normal) ||
				(gameStat->Player[targetPlayer].Discard[i + 1].dstat == DiscardStat::riichi)) {
				if (riichiFlag) {
					ReconstructSutehai_rotated(gameStat, targetPlayer, i, tilePosCol, tilePosRow, shiftPosFlag);
					riichiFlag = false;
				} else {
					ReconstructSutehai_portrait(gameStat, targetPlayer, i, tilePosCol, tilePosRow, shiftPosFlag);
				}
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
