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
		/* 打牌仮定 */
		GameTable tmpGameStat(*GameStatus::gameStat());
		tmpGameStat.statOfMine().Hand[tileCursor] = tmpGameStat.statOfMine().Tsumohai();
		tmpGameStat.statOfMine().Tsumohai() = Tile();
		/*  */
		CodeConv::tostringstream o;
		utils::Shanten shanten = utils::calcShanten(&tmpGameStat, tmpGameStat.PlayerID, utils::ShantenType::all);
		if (shanten > 0) { // 不聴の時は向聴表示
			o << static_cast<int>(shanten) << _T("向聴");
		} else {
			/* 待ち牌の数を表示 */
			MachihaiInfo machiInfo = utils::chkFuriten(&tmpGameStat, tmpGameStat.PlayerID);
			if (tmpGameStat.chkGameType(GameTypeID::richiMJ) && (machiInfo.FuritenFlag))
				o << _T("[振聴]");
			if (machiInfo.MachihaiTotal == 0)
				o << _T("純 カ ラ");
			else
				o << _T("あと") << std::setw(2) << std::setfill(_T(' ')) << static_cast<int>(machiInfo.MachihaiTotal) << _T("枚");
			/* 待ち牌表示 */
			int i = 0;
			for (int j = 1; j < TileNonflowerMax; ++j) {
				if (machiInfo.Machihai[j].MachihaiFlag) {
					myTileRenderer->NewTile(i, Tile(static_cast<TileCode>(j), DoraCol::normal),
						TipX + myTextRenderer->strWidthByPix(o.str()) + 20 + i * (ShowTile::VertTileWidth + 2),
						TipY + (ShowTile::VertTileHeight / 2) - 6,
						TileDirection::portrait, TileSide::obverse,
						(machiInfo.Machihai[j].MachihaiCount > 0) ? 0xffffffff : 0xff7f7f7f);
					++i;
				}
			}
		}
		tipText = o.str();
	} else { // 牌選択時以外のフリテン表示
		MachihaiInfo machiInfo = utils::chkFuriten(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		if (GameStatus::gameStat()->chkGameType(GameTypeID::richiMJ) && (machiInfo.FuritenFlag || GameStatus::gameStat()->statOfMine().DoujunFuriten))
			tipText = _T("[振聴]");
		if (caller->buttonReconst->getButtonSet() == ButtonSet::normal) {
			TileCode discardTile = GameStatus::gameStat()->CurrentDiscard.tile;
			auto render = [this, &tipText] (unsigned tileID, TileCode tileCode) -> void {
				myTileRenderer->NewTile(tileID, Tile(tileCode),
					TipX + myTextRenderer->strWidthByPix(tipText) + 20 + tileID * (ShowTile::VertTileWidth + 2),
					TipY + (ShowTile::VertTileHeight / 2) - 6,
					TileDirection::portrait, TileSide::obverse, 0xffffffff);
			};
			auto chkCursor = [this](ButtonID buttonID) -> bool {
				return (caller->buttonReconst->isEnabled(buttonID) &&
					(caller->buttonReconst->getCursor() == buttonID));
			};
		}
	}
	myTextRenderer->NewText(0, tipText, TipX, TipY);
}

void GameTableScreen::TileTipReconst::Render() {
	myTextRenderer->Render();
	myTileRenderer->Render();
}

}
