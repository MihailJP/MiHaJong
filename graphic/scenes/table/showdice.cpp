#include "showdice.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../utils.h"
#include "../../rule.h"

namespace mihajong_graphic {

using utils::playerRelative;

/* サイコロを表示する */
void GameTableScreen::DiceReconst::ShowDice(const GameTable* gameStat) {
	const int diceID = ((gameStat->Dice[2].Number == 0) || (gameStat->Dice[3].Number == 0)) ? 0 : 2;
	const int dicePlayerOffset = ((gameStat->Dice[2].Number != 0) && (gameStat->Dice[3].Number != 0) && (gameStat->TilePointer == 0)) ?
		(gameStat->Dice[0].Number + gameStat->Dice[1].Number - 1) % Players : 0;
	const PlayerID dicePlayer = (gameStat->GameRound + dicePlayerOffset) %
		(gameStat->chkGameType(SanmaT) ? 3 : 4) +
		(gameStat->chkGameType(Sanma4) ? ((gameStat->Dice[0].Number + gameStat->Dice[1].Number - 1) / 3) : 0);
	const unsigned int diceFace1 = ((gameStat->gameType & GuobiaoMJ) && (gameStat->Dice[diceID + 0].Number == 4)) ? 7 : gameStat->Dice[diceID + 0].Number;
	const unsigned int diceFace2 = ((gameStat->gameType & GuobiaoMJ) && (gameStat->Dice[diceID + 1].Number == 4)) ? 7 : gameStat->Dice[diceID + 1].Number;
	const unsigned int diceDirection1 = ((gameStat->gameType & GuobiaoMJ) && (gameStat->Dice[diceID + 0].Direction == 3)) ? 4 : gameStat->Dice[diceID + 0].Direction;
	const unsigned int diceDirection2 = ((gameStat->gameType & GuobiaoMJ) && (gameStat->Dice[diceID + 1].Direction == 3)) ? 4 : gameStat->Dice[diceID + 1].Direction;
	const RECT rect1 = {
		static_cast<int32_t>((DiceWidth + DicePadding) * (diceFace1 - 1)), static_cast<int32_t>((DiceHeight + DicePadding) * (diceDirection1    )),
		static_cast<int32_t>((DiceWidth + DicePadding) * (diceFace1    )), static_cast<int32_t>((DiceHeight + DicePadding) * (diceDirection1 + 1)),
	};
	const RECT rect2 = {
		static_cast<int32_t>((DiceWidth + DicePadding) * (diceFace2 - 1)), static_cast<int32_t>((DiceHeight + DicePadding) * (diceDirection2    )),
		static_cast<int32_t>((DiceWidth + DicePadding) * (diceFace2    )), static_cast<int32_t>((DiceHeight + DicePadding) * (diceDirection2 + 1)),
	};
	switch (playerRelative(dicePlayer, gameStat->PlayerID)) {
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
