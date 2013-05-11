#include "showdice.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../utils.h"

namespace mihajong_graphic {

using utils::playerRelative;

/* サイコロを表示する */
void GameTableScreen::DiceReconst::ShowDice(const GameTable* gameStat) {
	RECT rect1 = {
		static_cast<int32_t>((DiceWidth + DicePadding) * (gameStat->Dice[0].Number - 1)), static_cast<int32_t>((DiceHeight + DicePadding) * (gameStat->Dice[0].Direction    )),
		static_cast<int32_t>((DiceWidth + DicePadding) * (gameStat->Dice[0].Number    )), static_cast<int32_t>((DiceHeight + DicePadding) * (gameStat->Dice[0].Direction + 1)),
	};
	RECT rect2 = {
		static_cast<int32_t>((DiceWidth + DicePadding) * (gameStat->Dice[1].Number - 1)), static_cast<int32_t>((DiceHeight + DicePadding) * (gameStat->Dice[1].Direction    )),
		static_cast<int32_t>((DiceWidth + DicePadding) * (gameStat->Dice[1].Number    )), static_cast<int32_t>((DiceHeight + DicePadding) * (gameStat->Dice[1].Direction + 1)),
	};
	switch (playerRelative(gameStat->GameRound % Players, gameStat->PlayerID)) {
	case sSelf:
		SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
			tDice, DicePosH - (DiceWidth + DicePosInterstice) / 2, DicePosV,
			DiceWidth, DiceHeight, 0xffffffff, &rect1, DiceWidth / 2, DiceHeight / 2);
		SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
			tDice, DicePosH + (DiceWidth + DicePosInterstice) / 2, DicePosV,
			DiceWidth, DiceHeight, 0xffffffff, &rect2, DiceWidth / 2, DiceHeight / 2);
		break;
	case sOpposite:
		SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
			tDice, TableSize - DicePosH + (DiceWidth + DicePosInterstice) / 2, TableSize - DicePosV,
			DiceWidth, DiceHeight, 0xffffffff, &rect1, DiceWidth / 2, DiceHeight / 2);
		SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
			tDice, TableSize - DicePosH - (DiceWidth + DicePosInterstice) / 2, TableSize - DicePosV,
			DiceWidth, DiceHeight, 0xffffffff, &rect2, DiceWidth / 2, DiceHeight / 2);
		break;
	case sLeft:
		SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
			tDice, TableSize - DicePosV, DicePosH - (DiceWidth + DicePosInterstice) / 2,
			DiceWidth, DiceHeight, 0xffffffff, &rect1, DiceWidth / 2, DiceHeight / 2);
		SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
			tDice, TableSize - DicePosV, DicePosH + (DiceWidth + DicePosInterstice) / 2,
			DiceWidth, DiceHeight, 0xffffffff, &rect2, DiceWidth / 2, DiceHeight / 2);
		break;
	case sRight:
		SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
			tDice, DicePosV, TableSize - DicePosH - (DiceWidth + DicePosInterstice) / 2,
			DiceWidth, DiceHeight, 0xffffffff, &rect2, DiceWidth / 2, DiceHeight / 2);
		SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
			tDice, DicePosV, TableSize - DicePosH + (DiceWidth + DicePosInterstice) / 2,
			DiceWidth, DiceHeight, 0xffffffff, &rect1, DiceWidth / 2, DiceHeight / 2);
		break;
	}
}

void GameTableScreen::DiceReconst::Render() {
	ShowDice(GameStatus::gameStat());
}

GameTableScreen::DiceReconst::DiceReconst(GameTableScreen* parent) {
	caller = parent;
	caller->LoadTexture(&tDice, MAKEINTRESOURCE(IDB_PNG_DICE));
}

GameTableScreen::DiceReconst::~DiceReconst() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (tDice) tDice->Release();
#endif
}

}
