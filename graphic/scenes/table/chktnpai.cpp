#include "chktnpai.h"
#include "../../gametbl.h"
#include "../../utils.h"

namespace mihajong_graphic {

TableSubsceneCheckTenpai::TableSubsceneCheckTenpai(LPDIRECT3DDEVICE9 device) : TableSubsceneCallFadeProto(device) {
	txtRenderer = new TextRenderer(device);
	tileRenderer = new ShowTile(device);
}

TableSubsceneCheckTenpai::~TableSubsceneCheckTenpai() {
	delete tileRenderer;
	delete txtRenderer;
}

void TableSubsceneCheckTenpai::ShowTenpaiFlag(PLAYER_ID player, int x, int y) {
	bool tenpaiflag = utils::isTenpai(GameStatus::gameStat(), player);
	MachihaiInfo machiInfo = utils::chkFuriten(GameStatus::gameStat(), player);
	if (tenpaiflag) { // ’®”v
		ShowCallMsg(player, calltext::Tenpai, x, y - 40);
		int tile = 0;
		for (tileCode k = CharacterOne; k <= RedDragon; k = (tileCode)(k + 1)) {
			if (machiInfo.Machihai.val[k].MachihaiFlag) // ‘Ò‚¿”v‚É‚È‚Á‚Ä‚¢‚éê‡
				tileRenderer->NewTile(player * 9 + tile, k, Normal,
				x - 20 * (machiInfo.MachiMen - 1) + 40 * (tile++),
				y + 20, Portrait, Obverse); // ‚»‚Ì”v‚ð•\Ž¦
		}
		if (machiInfo.FuritenFlag)
			txtRenderer->NewText(player, _T("(ƒtƒŠƒeƒ“)"), x - 90, y + 40);
		else
			txtRenderer->NewText(player, _T("‘Ò‚¿"), x - 36, y + 40);
	}
	else if (GameStatus::gameStat()->Player.val[player].RichiFlag.RichiFlag)
		ShowCallMsg(player, calltext::Chonbo, x, y); // ƒm[ƒeƒ“ƒŠ[ƒ`‚µ‚Ä‚½ê‡‚Íö˜a‚Æ•\Ž¦
	else ShowCallMsg(player, calltext::Noten, x, y); // •s’®
}

void TableSubsceneCheckTenpai::Render() {
	tileRenderer->Render();
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sOpposite), TableSize / 2      ,                 192);
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sLeft    ),                 256, TableSize / 2      );
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sRight   ), TableSize     - 256, TableSize / 2      );
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sSelf    ), TableSize / 2      , TableSize     - 192);
	txtRenderer->Render();
}

}
