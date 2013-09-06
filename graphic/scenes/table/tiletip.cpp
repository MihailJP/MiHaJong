#include "tiletip.h"
#include "tehai.h"
#include "nakibtn.h"
#include "../../gametbl.h"
#include <cstring>
#include "../../utils.h"
#include <iomanip>

namespace mihajong_graphic {

GameTableScreen::TileTipReconst::TileTipReconst(GameTableScreen* parent) {
	caller = parent;
	myTextRenderer = new TextRenderer(parent->caller->getDevice());
	myTileRenderer = new ShowTile(parent->caller->getDevice());
}

GameTableScreen::TileTipReconst::~TileTipReconst() {
	delete myTileRenderer;
	delete myTextRenderer;
}

void GameTableScreen::TileTipReconst::reconstruct() {
	CodeConv::tstring tipText;
	int tileCursor = caller->tehaiReconst->getTileCursor();
	for (int i = 0; i < 14; ++i)
		myTileRenderer->DelTile(i);
	if ((tileCursor >= 0) && (!caller->tehaiReconst->isEnabled(tileCursor))) {
		tipText = _T("");
	} else if (tileCursor >= 0) {
		/* ‘Å”v‰¼’è */
		GameTable tmpGameStat; memcpy(&tmpGameStat, GameStatus::gameStat(), sizeof (GameTable));
		tmpGameStat.statOfMine().Hand[tileCursor].tile = tmpGameStat.statOfMine().Tsumohai().tile;
		tmpGameStat.statOfMine().Tsumohai().tile = NoTile;
		/*  */
		CodeConv::tostringstream o;
		utils::Shanten shanten = utils::calcShanten(&tmpGameStat, tmpGameStat.PlayerID, utils::shantenAll);
		if (shanten > 0) { // •s’®‚ÌŽž‚ÍŒü’®•\Ž¦
			o << (int)shanten << _T("Œü’®");
		} else {
			/* ‘Ò‚¿”v‚Ì”‚ð•\Ž¦ */
			MachihaiInfo machiInfo = utils::chkFuriten(&tmpGameStat, tmpGameStat.PlayerID);
			if ((tmpGameStat.gameType & RichiMJ) && (machiInfo.FuritenFlag))
				o << _T("[U’®]");
			if (machiInfo.MachihaiTotal == 0)
				o << _T("ƒ ƒJ ƒ‰");
			else
				o << _T("‚ ‚Æ") << std::setw(2) << std::setfill(_T(' ')) << (int)machiInfo.MachihaiTotal << _T("–‡");
			/* ‘Ò‚¿”v•\Ž¦ */
			int i = 0;
			for (int j = 1; j < TileNonflowerMax; ++j) {
				if (machiInfo.Machihai[j].MachihaiFlag) {
					myTileRenderer->NewTile(i, (TileCode)j, Normal,
						TipX + myTextRenderer->strWidthByPix(o.str()) + 20 + i * (ShowTile::VertTileWidth + 2),
						TipY + (ShowTile::VertTileHeight / 2) - 6,
						Portrait, Obverse,
						(machiInfo.Machihai[j].MachihaiCount > 0) ? 0xffffffff : 0xff7f7f7f);
					++i;
				}
			}
		}
		tipText = o.str();
	} else { // ”v‘I‘ðŽžˆÈŠO‚ÌƒtƒŠƒeƒ“•\Ž¦
		MachihaiInfo machiInfo = utils::chkFuriten(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		if ((GameStatus::gameStat()->gameType & RichiMJ) && (machiInfo.FuritenFlag || GameStatus::gameStat()->statOfMine().DoujunFuriten))
			tipText = _T("[U’®]");
		if (caller->buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) {
			TileCode discardTile = GameStatus::gameStat()->CurrentDiscard.tile;
			auto render = [this, &tipText] (unsigned tileID, TileCode tileCode) -> void {
				myTileRenderer->NewTile(tileID, tileCode, Normal,
					TipX + myTextRenderer->strWidthByPix(tipText) + 20 + tileID * (ShowTile::VertTileWidth + 2),
					TipY + (ShowTile::VertTileHeight / 2) - 6,
					Portrait, Obverse, 0xffffffff);
			};
			auto chkCursor = [this](ButtonReconst::ButtonID buttonID) -> bool {
				return (caller->buttonReconst->isEnabled(buttonID) &&
					(caller->buttonReconst->getCursor() == buttonID));
			};
			if (chkCursor(ButtonReconst::btnChii1)) {
				tipText += _T("•›˜I"); render(0, discardTile); render(1, TileCode(discardTile + 1)); render(2, TileCode(discardTile + 2));
			} else if (chkCursor(ButtonReconst::btnChii2)) {
				tipText += _T("•›˜I"); render(0, TileCode(discardTile - 1)); render(1, discardTile); render(2, TileCode(discardTile + 1));
			} else if (chkCursor(ButtonReconst::btnChii3)) {
				tipText += _T("•›˜I"); render(0, TileCode(discardTile - 2)); render(1, TileCode(discardTile - 1)); render(2, discardTile);
			} else if (chkCursor(ButtonReconst::btnPon)) {
				tipText += _T("•›˜I"); render(0, discardTile); render(1, discardTile); render(2, discardTile);
			} else if (chkCursor(ButtonReconst::btnKan)) {
				tipText += _T("•›˜I"); render(0, discardTile); render(1, discardTile); render(2, discardTile); render(3, discardTile);
			}
		}
	}
	myTextRenderer->NewText(0, tipText, TipX, TipY);
}

void GameTableScreen::TileTipReconst::Render() {
	myTextRenderer->Render();
	myTileRenderer->Render();
}

}
