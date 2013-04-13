#include "richibou.h"
#include "../../resource.h"
#include "../../sprite.h"
//#include "../../rule.h"
#include "../../utils.h"
//#include <cassert>

namespace mihajong_graphic {

using utils::playerRelative;

/* ÉäÅ[É`ñ_ï\é¶Ç∑ÇÈ */
void GameTableScreen::RichibouReconst::ShowRiichibou(const GameTable* gameStat) {
	RECT rectH = {72, 30, 216, 42,}, rectV = {30, 2, 42, 146,};
	for (PlayerID i = 0; i < Players; ++i) {
		if (!gameStat->Player[i].RichiFlag.RichiFlag) continue;
		switch (playerRelative(i, gameStat->PlayerID)) {
		case sSelf:
			SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
				tRichi, RiichiPosH, RiichiPosV, 144, 12,
				0xffffffff, &rectH, 72, 6);
			break;
		case sOpposite:
			SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
				tRichi, RiichiPosH, TableSize - RiichiPosV, 144, 12,
				0xffffffff, &rectH, 72, 6);
			break;
		case sLeft:
			SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
				tRichi, TableSize - RiichiPosV, RiichiPosH, 12, 144,
				0xffffffff, &rectV, 6, 72);
			break;
		case sRight:
			SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(
				tRichi, RiichiPosV, RiichiPosH, 12, 144,
				0xffffffff, &rectV, 6, 72);
			break;
		}
	}
}

void GameTableScreen::RichibouReconst::Render() {
	ShowRiichibou(GameStatus::gameStat());
}

GameTableScreen::RichibouReconst::RichibouReconst(GameTableScreen* parent) {
	caller = parent;
	caller->LoadTexture(&tRichi, MAKEINTRESOURCE(IDB_PNG_TENBOU));
}

GameTableScreen::RichibouReconst::~RichibouReconst() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (tRichi) tRichi->Release();
#endif
}

}
