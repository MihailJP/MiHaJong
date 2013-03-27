#include "tiletip.h"
#include "tehai.h"
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
			if (machiInfo.FuritenFlag)
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
		if (machiInfo.FuritenFlag || GameStatus::gameStat()->statOfMine().DoujunFuriten)
			tipText = _T("[U’®]");
	}
	myTextRenderer->NewText(0, tipText, TipX, TipY);
}

void GameTableScreen::TileTipReconst::Render() {
	myTextRenderer->Render();
	myTileRenderer->Render();
}

}
