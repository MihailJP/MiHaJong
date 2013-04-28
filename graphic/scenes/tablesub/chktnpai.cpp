#include "chktnpai.h"
#include "../../gametbl.h"
#include "../../utils.h"

namespace mihajong_graphic {

TableSubsceneCheckTenpai::TableSubsceneCheckTenpai(DevicePtr device) : TableSubsceneCallFadeProto(device) {
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
	if (tenpaiflag[player]) { // ’®”v
		int tile = 0;
		for (TileCode k = CharacterOne; k <= RedDragon; k = (TileCode)(k + 1)) {
			if (machiInfo[player].Machihai[k].MachihaiFlag) // ‘Ò‚¿”v‚É‚È‚Á‚Ä‚¢‚éê‡
				tileRenderer->NewTile(player * 9 + tile, k, Normal,
				x - 20 * (machiInfo[player].MachiMen - 1) + 40 * (tile++),
				y + 20, Portrait, Obverse); // ‚»‚Ì”v‚ð•\Ž¦
		}
		if (machiInfo[player].FuritenFlag)
			txtRenderer->NewText(player, _T("(ƒtƒŠƒeƒ“)"), x - 90, y + 40);
		else
			txtRenderer->NewText(player, _T("‘Ò‚¿"), x - 36, y + 40);
	}
}

void TableSubsceneCheckTenpai::ShowTenpaiFlag(PlayerID player, int x, int y) {
	if (tenpaiflag[player]) // ’®”v
		ShowCallMsg(player, calltext::Tenpai, x, y - 40);
	else if (GameStatus::gameStat()->Player[player].RichiFlag.RichiFlag)
		ShowCallMsg(player, calltext::Chonbo, x, y); // ƒm[ƒeƒ“ƒŠ[ƒ`‚µ‚Ä‚½ê‡‚Íö˜a‚Æ•\Ž¦
	else ShowCallMsg(player, calltext::Noten, x, y); // •s’®
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
