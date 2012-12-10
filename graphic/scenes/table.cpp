#include "table.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../gametbl.h"
#include "../rule.h"
#include <tuple>
#include <cassert>

namespace mihajong_graphic {

namespace {
	seatRelative inline playerRelative(PLAYER_ID targetPlayer, PLAYER_ID basePlayer) {
		return (seatRelative)((PLAYERS + targetPlayer - basePlayer) % PLAYERS);
	}
}
	
GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 1080, 1080); InitSprite(&sBorder);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 1080, 1080); InitSprite(&sBaize);
	Reconstruct(GameStatus::gameStat());
	/* éËîv */
	for (int i = 0; i < HandLength; i++) { /* ëŒñ ÇÃéËîv */
		TileTexture->NewTile(144+i   , WhiteDragon, Normal, HandPosH + ShowTile::VertTileWidth*i, HandPosV, UpsideDown, Upright);
	}
	for (int i = 0; i < HandLength; i++) { /* è„â∆ÇÃéËîv */
		TileTexture->NewTile(144+14+i, BambooOne, Normal, HandPosV, HandPosH + ShowTile::VertTileWidth*i, Clockwise, Upright);
	}
	for (int i = 0; i < HandLength; i++) { /* â∫â∆ÇÃéËîv */
		TileTexture->NewTile(144+28+i, CircleSeven, Normal, TableSize - HandPosV, HandPosH + ShowTile::VertTileWidth*i, Withershins, Upright);
	}
	for (int i = 0; i < HandLength; i++) { /* é©ï™ÇÃéËîv */
		TileTexture->NewTile(144+52+i, WhiteDragon, Normal, HandPosH + ShowTile::VertTileWidth*i, TableSize - HandPosV, Portrait, Upright);
	}
}

GameTableScreen::~GameTableScreen() {
	if (tBorder) tBorder->Release();
	if (sBorder) sBorder->Release();
	if (tBaize) tBaize->Release();
	if (sBaize) sBaize->Release();
}

void GameTableScreen::ReconstructYamahai(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer) {
	std::tuple<unsigned, unsigned, unsigned, unsigned> yamahaiAttr;
	if (gameStat->gameType & AllSanma)
		yamahaiAttr = std::make_tuple(3, 108, 18, 102 - gameStat->ExtraRinshan);
	else if (rules::chkRule("flower_tiles", "no"))
		yamahaiAttr = std::make_tuple(0, 136, 17, 130);
	else if (rules::chkRule("flower_tiles", "8tiles"))
		yamahaiAttr = std::make_tuple(1, 144, 18, 130);
	else
		yamahaiAttr = std::make_tuple(2, 140, 18, 130);
	seatRelative tmpPlayerCode = playerRelative((gameStat->gameType & Sanma4) ? trueTargetPlayer : targetPlayer, gameStat->PlayerID);
	auto calcTileNum = [&gameStat, &targetPlayer, &yamahaiAttr](unsigned j) -> unsigned {
		const unsigned yamahaiMode = std::get<0>(yamahaiAttr);
		const unsigned dice = gameStat->Dice[0].Number + gameStat->Dice[1].Number;
		if ((gameStat->Dice[0].Number == 0) && (gameStat->Dice[1].Number == 0)) return 0; // ñ¢ê›íË
		assert((dice >= 2) && (dice <= 12));
		switch (yamahaiMode) {
		case 0:
			return
				((((targetPlayer + 3) % PLAYERS) * 34) + j * 2 + (104 * dice + 34) +
				((gameStat->gameType % PLAYERS)                       * 102)) % 136;
		case 1:
			return
				((((targetPlayer + 3) % PLAYERS) * 36) + j * 2 + (110 * dice + 36) +
				((gameStat->gameType % PLAYERS)                       * 108)) % 144;
		case 2:
			return (
				((((targetPlayer + 3) % PLAYERS) * 35) + j * 2 + (107 * dice + 35) +
				((gameStat->gameType % PLAYERS)                       * 105)) % 140
				) / 2 * 2;
		case 3:
			return
				((((targetPlayer + 2) % 3      ) * 36) + j * 2 + (74 * dice + 36) +
				(((gameStat->gameType - (gameStat->gameType / 4)) % 3)*  72)) % 108;
		default:
			throw _T("calcTileNum: Invalid mode!");
		}
	};
	auto getRinshanFlag = [&gameStat, &yamahaiAttr](unsigned ActiveTileNum) -> bool {
		const unsigned NumberOfTiles = std::get<1>(yamahaiAttr);
		bool RinshanFlag = false;
		for (unsigned i = 0; i <= 10; i += 2)
			if ((ActiveTileNum == i) && (gameStat->RinshanPointer < NumberOfTiles - i - 2)) RinshanFlag = true;
		return RinshanFlag;
	};
	switch (tmpPlayerCode) {
	case sOpposite:
		for (int i = (18 - std::get<2>(yamahaiAttr)) * 2; i < 36; i += 2) { /* ëŒñ ë§ÇÃéR */
			unsigned tileNum = calcTileNum((std::get<2>(yamahaiAttr) * 2 - i) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i); TileTexture->DelTile(i + 1);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile(      i    , gameStat->Deck[k + 1].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2 - 1),
				DeckPosV,                 UpsideDown, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile(      i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2 - 1),
				DeckPosV - TileThickness, UpsideDown, dora ? Obverse : Reverse);
		}
		break;
	case sLeft:
		for (int i = 0; i < std::get<2>(yamahaiAttr) * 2; i += 2) { /* è„â∆ë§ÇÃéR */
			unsigned tileNum = calcTileNum(i / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 36); TileTexture->DelTile(i + 37);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile( 36 + i    , gameStat->Deck[k + 1].tile, Normal, DeckPosV,
				DeckPosH                 + ShowTile::VertTileWidth * (i / 2), Clockwise, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile( 36 + i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosV,
				DeckPosH - TileThickness + ShowTile::VertTileWidth * (i / 2), Clockwise, dora ? Obverse : Reverse);
		}
		break;
	case sRight:
		for (int i = (18 - std::get<2>(yamahaiAttr)) * 2; i < 36; i += 2) { /* â∫â∆ë§ÇÃéR */
			unsigned tileNum = calcTileNum((std::get<2>(yamahaiAttr) * 2 - i) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 72); TileTexture->DelTile(i + 73);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile( 72 + i    , gameStat->Deck[k + 1].tile, Normal, TableSize - DeckPosV,
				DeckPosH                 + ShowTile::VertTileWidth * (i / 2 - 1), Withershins, Reverse);
			if ((gameStat->TilePointer <= k) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile( 72 + i + 1, gameStat->Deck[k    ].tile, Normal, TableSize - DeckPosV,
				DeckPosH - TileThickness + ShowTile::VertTileWidth * (i / 2 - 1), Withershins, dora ? Obverse : Reverse);
		}
		break;
	case sSelf:
		for (int i = 0; i < std::get<2>(yamahaiAttr) * 2; i += 2) { /* é©ï™ÇÃéR */
			unsigned tileNum = calcTileNum(i / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k  >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 108); TileTexture->DelTile(i + 109);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile(108 + i    , gameStat->Deck[k + 1].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2),
				TableSize - DeckPosV,                 Portrait, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile(108 + i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2),
				TableSize - DeckPosV - TileThickness, Portrait, dora ? Obverse : Reverse);
		}
		break;
	}
	return;
}

void GameTableScreen::Reconstruct(const GameTable* gameStat) {
	if (gameStat->gameType & Yonma) {
		for (PLAYER_ID i = 0; i < 4; i++)
			ReconstructYamahai(gameStat, i, i);
	} else if (gameStat->gameType & Sanma4) {
		PLAYER_ID tobePlayed[4][4] = {
			{0, 1, 2, 3}, {3, 1, 2, 0}, {1, 3, 2, 0}, {1, 2, 3, 0},
		};
		for (PLAYER_ID i = 0; i < 4; i++) {
			PLAYER_ID j(tobePlayed[gameStat->GameRound % PLAYERS][i]);
			if (j < 3) ReconstructYamahai(gameStat, j, i);
		}
	} else {
		for (PLAYER_ID i = 0; i < 3; i++)
			ReconstructYamahai(gameStat, i, i);
	}
}

void GameTableScreen::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // ÉoÉbÉtÉ@ÉNÉäÉA
	ShowSidebar();
	ShowSprite(sBaize, tBaize, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	ShowSprite(sBorder, tBorder, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	if (GameStatus::isModified())
		Reconstruct(GameStatus::gameStat());
	TileTexture->Render();
}

}
