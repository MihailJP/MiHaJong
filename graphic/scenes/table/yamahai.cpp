#include "yamahai.h"
#include "../../rule.h"
#include "../../utils.h"
#include <cassert>

namespace mihajong_graphic {

using utils::playerRelative;

/* 山牌の表示 */
void GameTableScreen::YamahaiReconst::Reconstruct(const GameTable* gameStat, PlayerID targetPlayer, PlayerID trueTargetPlayer) {
	std::tuple<unsigned, unsigned, unsigned, unsigned> yamahaiAttr;
	if (gameStat->gameType & AllSanma)
		yamahaiAttr = std::make_tuple(3, 108, 18, 102 - gameStat->ExtraRinshan);
	else if (rules::chkRule("flower_tiles", "no"))
		yamahaiAttr = std::make_tuple(0, 136, 17, 130);
	else if (rules::chkRule("flower_tiles", "8tiles"))
		yamahaiAttr = std::make_tuple(1, 144, 18, 130);
	else
		yamahaiAttr = std::make_tuple(2, 140, 18, 130);
	seatRelative tmpPlayerCode = playerRelative(trueTargetPlayer, gameStat->PlayerID);
	auto calcTileNum = [&gameStat, &targetPlayer, &yamahaiAttr](unsigned j) -> unsigned {
		const unsigned yamahaiMode = std::get<0>(yamahaiAttr);
		const unsigned dice = gameStat->Dice[0].Number + gameStat->Dice[1].Number;
		const unsigned dice2 = gameStat->Dice[2].Number + gameStat->Dice[3].Number;
		if ((gameStat->Dice[0].Number == 0) && (gameStat->Dice[1].Number == 0)) return 0; // 未設定
		assert((dice >= 2) && (dice <= 12));
		switch (yamahaiMode) {
		case 0:
			return
				((((targetPlayer + 3) % Players) * 34) + j * 2 + (104 * dice + 2 * dice2 + 34) +
				((gameStat->GameRound % Players)                        * 102)) % 136;
		case 1:
			return
				((((targetPlayer + 3) % Players) * 36) + j * 2 + (110 * dice + 2 * dice2 + 36) +
				((gameStat->GameRound % Players)                        * 108)) % 144;
		case 2:
			return (
				((((targetPlayer + 3) % Players) * 35) + j * 2 + (107 * dice + 2 * dice2 + 35) +
				((gameStat->GameRound % Players)                        * 105)) % 140
				) / 2 * 2;
		case 3:
			return
				((((targetPlayer + 2) % 3      ) * 36) + j * 2 + (74 * dice + 2 * dice2 + 36) +
				(((gameStat->GameRound - (gameStat->GameRound / 4)) % 3)*  72)) % 108;
		default:
			throw _T("calcTileNum: Invalid mode!");
		}
	};
	auto getRinshanFlag = [gameStat, yamahaiAttr](unsigned int a) -> std::function<bool (unsigned)> {
		const GameTable* const gameStat_ = gameStat;
		const std::tuple<unsigned, unsigned, unsigned, unsigned> yamahaiAttr_ = yamahaiAttr;
		return
		[gameStat_, yamahaiAttr_, a](unsigned ActiveTileNum) -> bool {
			const unsigned NumberOfTiles = std::get<1>(yamahaiAttr_);
			bool RinshanFlag = false;
			for (unsigned i = 0; i <= 10; i += 2)
				if ((ActiveTileNum == i) && (gameStat_->RinshanPointer < NumberOfTiles - i - a)) RinshanFlag = true;
			return RinshanFlag;
		};
	};
	auto getRinshanFlag2 = getRinshanFlag(2);
	auto getRinshanFlag1 = getRinshanFlag(1);
	const bool shorterWall = (std::get<0>(yamahaiAttr) == 2) && (gameStat->playerwind(trueTargetPlayer) % 2 == 1);
	switch (tmpPlayerCode) {
	case sOpposite:
		for (int i = (18 - std::get<2>(yamahaiAttr) + (shorterWall ? 1 : 0)) * 2; i < 36; i += 2) { /* 対面側の山 */
			unsigned tileNum = calcTileNum((34 - (i - (shorterWall ? 2 : 0))) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i); TileTexture->DelTile(i + 1);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag2(tileNum)))
				TileTexture->NewTile(      i    , gameStat->Deck[k + 1].tile, gameStat->Deck[k + 1].red, DeckPosH + ShowTile::VertTileWidth * (i / 2 - 1),
				DeckPosV,                 UpsideDown, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag1(tileNum)))
				TileTexture->NewTile(      i + 1, gameStat->Deck[k    ].tile, gameStat->Deck[k    ].red, DeckPosH + ShowTile::VertTileWidth * (i / 2 - 1),
				DeckPosV - TileThickness, UpsideDown, dora ? Obverse : Reverse);
		}
		break;
	case sLeft:
		for (int i = 0; i < (std::get<2>(yamahaiAttr) - (shorterWall ? 1 : 0)) * 2; i += 2) { /* 上家側の山 */
			unsigned tileNum = calcTileNum((i + (shorterWall ? 2 : 0)) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 36); TileTexture->DelTile(i + 37);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag2(tileNum)))
				TileTexture->NewTile( 36 + i    , gameStat->Deck[k + 1].tile, gameStat->Deck[k + 1].red, DeckPosV,
				DeckPosH                 + ShowTile::VertTileWidth * (i / 2), Clockwise, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag1(tileNum)))
				TileTexture->NewTile( 36 + i + 1, gameStat->Deck[k    ].tile, gameStat->Deck[k    ].red, DeckPosV,
				DeckPosH - TileThickness + ShowTile::VertTileWidth * (i / 2), Clockwise, dora ? Obverse : Reverse);
		}
		break;
	case sRight:
		for (int i = (18 - std::get<2>(yamahaiAttr) + (shorterWall ? 1 : 0)) * 2; i < 36; i += 2) { /* 下家側の山 */
			unsigned tileNum = calcTileNum((34 - (i - (shorterWall ? 2 : 0))) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 72); TileTexture->DelTile(i + 73);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag2(tileNum)))
				TileTexture->NewTile( 72 + i    , gameStat->Deck[k + 1].tile, gameStat->Deck[k + 1].red, TableSize - DeckPosV,
				DeckPosH                 + ShowTile::VertTileWidth * (i / 2 - 1), Withershins, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag1(tileNum)))
				TileTexture->NewTile( 72 + i + 1, gameStat->Deck[k    ].tile, gameStat->Deck[k    ].red, TableSize - DeckPosV,
				DeckPosH - TileThickness + ShowTile::VertTileWidth * (i / 2 - 1), Withershins, dora ? Obverse : Reverse);
		}
		break;
	case sSelf:
		for (int i = 0; i < (std::get<2>(yamahaiAttr) - (shorterWall ? 1 : 0)) * 2; i += 2) { /* 自分の山 */
			unsigned tileNum = calcTileNum((i + (shorterWall ? 2 : 0)) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k  >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 108); TileTexture->DelTile(i + 109);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag2(tileNum)))
				TileTexture->NewTile(108 + i    , gameStat->Deck[k + 1].tile, gameStat->Deck[k + 1].red, DeckPosH + ShowTile::VertTileWidth * (i / 2),
				TableSize - DeckPosV,                 Portrait, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag1(tileNum)))
				TileTexture->NewTile(108 + i + 1, gameStat->Deck[k    ].tile, gameStat->Deck[k    ].red, DeckPosH + ShowTile::VertTileWidth * (i / 2),
				TableSize - DeckPosV - TileThickness, Portrait, dora ? Obverse : Reverse);
		}
		break;
	}
	return;
}

void GameTableScreen::YamahaiReconst::Render() {
	TileTexture->Render();
}

GameTableScreen::YamahaiReconst::YamahaiReconst(GameTableScreen* parent) {
	caller = parent;
	TileTexture = new ShowTile(parent->caller->getDevice());
}

GameTableScreen::YamahaiReconst::~YamahaiReconst() {
	delete TileTexture;
}

}
