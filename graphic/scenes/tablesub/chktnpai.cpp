#include "chktnpai.h"
#include "../../gametbl.h"
#include "../../utils.h"
#include "../../rule.h"

namespace mihajong_graphic {

TableSubsceneCheckTenpai::TableSubsceneCheckTenpai(DevicePtr device) : TableSubsceneCallFadeProto(device) {
	memset(tenpaiflag, 0, sizeof tenpaiflag);
	txtRenderer = new TextRenderer(device);
	tileRenderer = new ShowTile(device);
	RecalcTenpaiFlag();
#ifndef _WIN32
	initialized = false;
#endif /*_WIN32*/
}

TableSubsceneCheckTenpai::~TableSubsceneCheckTenpai() {
	delete tileRenderer;
	delete txtRenderer;
}

void TableSubsceneCheckTenpai::CalculateTenpaiFlag(PlayerID player, int x, int y) {
	tenpaiflag[player] = utils::isTenpai(GameStatus::gameStat(), player);
	machiInfo[player] = utils::chkFuriten(GameStatus::gameStat(), player);
	if (tenpaiflag[player]) { // 聴牌
		int tile = 0;
		for (TileCode k = CharacterOne; k <= RedDragon; k = static_cast<TileCode>(k + 1)) {
			if (machiInfo[player].Machihai[k].MachihaiFlag) // 待ち牌になっている場合
				tileRenderer->NewTile(player * 9 + tile, Tile(k),
				x - 20 * (machiInfo[player].MachiMen - 1) + 40 * (tile),
				y + 20, TileDirection::portrait, TileSide::obverse), ++tile; // その牌を表示
		}
		if (machiInfo[player].FuritenFlag)
			txtRenderer->NewText(player, _T("(フリテン)"), x - 90, y + 40);
		else
			txtRenderer->NewText(player, _T("待ち"), x - 36, y + 40);
	}
}

void TableSubsceneCheckTenpai::ShowTenpaiFlag(PlayerID player, int x, int y) {
	if ((GameStatus::gameStat()->gameType & RichiMJ) && rules::chkRule("furiten_riichi", "no") && GameStatus::gameStat()->Player[player].RichiFlag.RichiFlag && machiInfo[player].FuritenFlag)
		ShowCallMsg(player, calltext::Chonbo, x, y - 40); // フリテン立直がチョンボになる場合
	if (tenpaiflag[player]) // 聴牌
		ShowCallMsg(player, calltext::Tenpai, x, y - 40);
	else if (GameStatus::gameStat()->Player[player].RichiFlag.RichiFlag)
		ShowCallMsg(player, calltext::Chonbo, x, y); // ノーテンリーチしてた場合は錯和と表示
	else if (GameStatus::gameStat()->Player[player].Hand[0]) // 配牌をもらっていない場合は除外
		ShowCallMsg(player, calltext::Noten, x, y); // 不聴
}

void TableSubsceneCheckTenpai::RecalcTenpaiFlag() {
	CalculateTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sOpposite), TableSize / 2      ,                 192);
	CalculateTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sLeft    ),                 256, TableSize / 2      );
	CalculateTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sRight   ), TableSize     - 256, TableSize / 2      );
	CalculateTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sSelf    ), TableSize / 2      , TableSize     - 192);
}

void TableSubsceneCheckTenpai::Render() {
#ifndef _WIN32
	if (!initialized) {
		RecalcTenpaiFlag();
		initialized = true;
	}
#endif /*_WIN32*/
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sOpposite), TableSize / 2      ,                 192);
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sLeft    ),                 256, TableSize / 2      );
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sRight   ), TableSize     - 256, TableSize / 2      );
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sSelf    ), TableSize / 2      , TableSize     - 192);
	txtRenderer->Render();
	tileRenderer->Render();
}

}
