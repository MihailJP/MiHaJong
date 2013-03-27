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
}

GameTableScreen::TileTipReconst::~TileTipReconst() {
	delete myTextRenderer;
}

void GameTableScreen::TileTipReconst::reconstruct() {
	CodeConv::tstring tipText;
	int tileCursor = caller->tehaiReconst->getTileCursor();
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
		if (shanten > 0) {
			o << (int)shanten << _T("Œü’®");
		} else {
			MachihaiInfo machiInfo = utils::chkFuriten(&tmpGameStat, tmpGameStat.PlayerID);
			if (machiInfo.FuritenFlag)
				o << _T("[U’®]");
			if (machiInfo.MachihaiTotal == 0)
				o << _T("ƒ ƒJ ƒ‰");
			else
				o << _T("‚ ‚Æ") << std::setw(2) << std::setfill(_T(' ')) << (int)machiInfo.MachihaiTotal << _T("–‡");
		}
		tipText = o.str();
	} else {
		MachihaiInfo machiInfo = utils::chkFuriten(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		if (machiInfo.FuritenFlag || GameStatus::gameStat()->statOfMine().DoujunFuriten)
			tipText = _T("[U’®]");
	}
	myTextRenderer->NewText(0, tipText, TipX, TipY);
}

void GameTableScreen::TileTipReconst::Render() {
	myTextRenderer->Render();
}

}
