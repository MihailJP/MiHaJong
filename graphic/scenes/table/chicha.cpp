#include "chicha.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../gametbl.h"
#include "../../utils.h"

namespace mihajong_graphic {

using utils::playerRelative;

/* 起家マークを置く凹み */
void GameTableScreen::TrayReconst::ShowTray() {
	constexpr RECT rect1 = {TrayHLeft, TrayHTop, TrayHRight, TrayHBottom,};
	constexpr RECT rect2 = {TrayVLeft, TrayVTop, TrayVRight, TrayVBottom,};
	SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, TrayPosH, TrayPosV,
		TrayHWidth, TrayHHeight, 0xffffffff, &rect1, TrayHWidth / 2, TrayHHeight / 2);
	SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, TableSize - TrayPosH, TableSize - TrayPosV,
		TrayHWidth, TrayHHeight, 0xffffffff, &rect1, TrayHWidth / 2, TrayHHeight / 2);
	SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, TableSize - TrayPosV, TrayPosH,
		TrayVWidth, TrayVHeight, 0xffffffff, &rect2, TrayVWidth / 2, TrayVHeight / 2);
	SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, TrayPosV, TableSize - TrayPosH,
		TrayVWidth, TrayVHeight, 0xffffffff, &rect2, TrayVWidth / 2, TrayVHeight / 2);
}

/* 起家マークの表示 */
void GameTableScreen::TrayReconst::ShowChiicha(const GameTable* gameStat) {
	switch (playerRelative(0, gameStat->PlayerID)) {
	case SeatRelative::self:
		{
			const RECT rect = {
				static_cast<int32_t>((PlateWidthH + PlatePadding * 2) * (gameStat->GameRound / Players    ) + PlatePadding), static_cast<int32_t>((PlateHeightH + PlatePadding * 2) * (0    ) + PlatePadding),
				static_cast<int32_t>((PlateWidthH + PlatePadding * 2) * (gameStat->GameRound / Players + 1) - PlatePadding), static_cast<int32_t>((PlateHeightH + PlatePadding * 2) * (0 + 1) - PlatePadding),
			};
			SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, PlatePosH, PlatePosV,
				PlateWidthH, PlateHeightH, 0xffffffff, &rect, PlateWidthH / 2, PlateHeightH / 2);
		}
		break;
	case SeatRelative::opposite:
		{
			const RECT rect = {
				static_cast<int32_t>((PlateWidthH + PlatePadding * 2) * (gameStat->GameRound / Players    ) + PlatePadding), static_cast<int32_t>((PlateHeightH + PlatePadding * 2) * (1    ) + PlatePadding),
				static_cast<int32_t>((PlateWidthH + PlatePadding * 2) * (gameStat->GameRound / Players + 1) - PlatePadding), static_cast<int32_t>((PlateHeightH + PlatePadding * 2) * (1 + 1) - PlatePadding),
			};
			SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, TableSize - PlatePosH, TableSize - PlatePosV,
				PlateWidthH, PlateHeightH, 0xffffffff, &rect, PlateWidthH / 2, PlateHeightH / 2);
		}
		break;
	case SeatRelative::right:
		{
			const RECT rect = {
				static_cast<int32_t>((PlateWidthV + PlatePadding * 2) * (gameStat->GameRound / Players    ) + PlatePadding), static_cast<int32_t>((PlateHeightV + PlatePadding * 2) * (0    ) + PlatePadding + (PlateHeightH + PlatePadding * 2) * 2),
				static_cast<int32_t>((PlateWidthV + PlatePadding * 2) * (gameStat->GameRound / Players + 1) - PlatePadding), static_cast<int32_t>((PlateHeightV + PlatePadding * 2) * (0 + 1) - PlatePadding + (PlateHeightH + PlatePadding * 2) * 2),
			};
			SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, PlatePosV, TableSize - PlatePosH,
				PlateWidthV, PlateHeightV, 0xffffffff, &rect, PlateWidthV / 2, PlateHeightV / 2);
		}
		break;
	case SeatRelative::left:
		{
			const RECT rect = {
				static_cast<int32_t>((PlateWidthV + PlatePadding * 2) * (gameStat->GameRound / Players    ) + PlatePadding), static_cast<int32_t>((PlateHeightV + PlatePadding * 2) * (1    ) + PlatePadding + (PlateHeightH + PlatePadding * 2) * 2),
				static_cast<int32_t>((PlateWidthV + PlatePadding * 2) * (gameStat->GameRound / Players + 1) - PlatePadding), static_cast<int32_t>((PlateHeightV + PlatePadding * 2) * (1 + 1) - PlatePadding + (PlateHeightH + PlatePadding * 2) * 2),
			};
			SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, TableSize - PlatePosV, PlatePosH,
				PlateWidthV, PlateHeightV, 0xffffffff, &rect, PlateWidthV / 2, PlateHeightV / 2);
		}
		break;
	}
}

/* ヤキトリマークの表示 */
void GameTableScreen::TrayReconst::ShowYakitori(const GameTable* gameStat) {
	for (PlayerID i = 0; i < Players; ++i) {
		if (!gameStat->Player[i].YakitoriFlag) continue;
		switch (playerRelative(i, gameStat->PlayerID)) {
		case SeatRelative::self:
			{
				constexpr RECT rect = {
					(PlateWidthH + PlatePadding * 2) * (PlateID_Yakitori    ) + PlatePadding, (PlateHeightH + PlatePadding * 2) * (0    ) + PlatePadding,
					(PlateWidthH + PlatePadding * 2) * (PlateID_Yakitori + 1) - PlatePadding, (PlateHeightH + PlatePadding * 2) * (0 + 1) - PlatePadding,
				};
				SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, YakitoriPosH, YakitoriPosV,
					PlateWidthH, PlateHeightH, 0xffffffff, &rect, PlateWidthH / 2, PlateHeightH / 2);
			}
			break;
		case SeatRelative::opposite:
			{
				constexpr RECT rect = {
					(PlateWidthH + PlatePadding * 2) * (PlateID_Yakitori    ) + PlatePadding, (PlateHeightH + PlatePadding * 2) * (1    ) + PlatePadding,
					(PlateWidthH + PlatePadding * 2) * (PlateID_Yakitori + 1) - PlatePadding, (PlateHeightH + PlatePadding * 2) * (1 + 1) - PlatePadding,
				};
				SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, TableSize - YakitoriPosH, TableSize - YakitoriPosV,
					PlateWidthH, PlateHeightH, 0xffffffff, &rect, PlateWidthH / 2, PlateHeightH / 2);
			}
			break;
		case SeatRelative::right:
			{
				constexpr RECT rect = {
					(PlateWidthV + PlatePadding * 2) * (PlateID_Yakitori    ) + PlatePadding, (PlateHeightV + PlatePadding * 2) * (0    ) + PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
					(PlateWidthV + PlatePadding * 2) * (PlateID_Yakitori + 1) - PlatePadding, (PlateHeightV + PlatePadding * 2) * (0 + 1) - PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
				};
				SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, YakitoriPosV, TableSize - YakitoriPosH,
					PlateWidthV, PlateHeightV, 0xffffffff, &rect, PlateWidthV / 2, PlateHeightV / 2);
			}
			break;
		case SeatRelative::left:
			{
				constexpr RECT rect = {
					(PlateWidthV + PlatePadding * 2) * (PlateID_Yakitori    ) + PlatePadding, (PlateHeightV + PlatePadding * 2) * (1    ) + PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
					(PlateWidthV + PlatePadding * 2) * (PlateID_Yakitori + 1) - PlatePadding, (PlateHeightV + PlatePadding * 2) * (1 + 1) - PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
				};
				SpriteRenderer::instantiate(caller->caller->getDevice())->ShowSprite(tChiicha, TableSize - YakitoriPosV, YakitoriPosH,
					PlateWidthV, PlateHeightV, 0xffffffff, &rect, PlateWidthV / 2, PlateHeightV / 2);
			}
			break;
		}
	}
}

void GameTableScreen::TrayReconst::Render() {
	ShowTray();
	ShowChiicha(GameStatus::gameStat());
	ShowYakitori(GameStatus::gameStat());
}

GameTableScreen::TrayReconst::TrayReconst(GameTableScreen* parent) {
	caller = parent;
	caller->LoadTexture(&tChiicha, MAKEINTRESOURCE(IDB_PNG_CHICHAMARK));
}

GameTableScreen::TrayReconst::~TrayReconst() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (tChiicha) tChiicha->Release();
#endif
}

}
