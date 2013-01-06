#include "chktnpai.h"
#include "../../gametbl.h"
#include "../../utils.h"

namespace mihajong_graphic {

TableSubsceneCheckTenpai::TableSubsceneCheckTenpai(LPDIRECT3DDEVICE9 device) : TableSubsceneCallFadeProto(device) {
}

TableSubsceneCheckTenpai::~TableSubsceneCheckTenpai() {
}

void TableSubsceneCheckTenpai::ShowTenpaiFlag(PLAYER_ID player, int x, int y) {
	bool tenpaiflag = utils::isTenpai(GameStatus::gameStat(), player);
	if (tenpaiflag) ShowCallMsg(player, calltext::Tenpai, x, y); // ’®”v
	else if (GameStatus::gameStat()->Player.val[player].RichiFlag.RichiFlag)
		ShowCallMsg(player, calltext::Chonbo, x, y); // ƒm[ƒeƒ“ƒŠ[ƒ`‚µ‚Ä‚½ê‡‚Íö˜a‚Æ•\Ž¦
	else ShowCallMsg(player, calltext::Noten, x, y); // •s’®
}

void TableSubsceneCheckTenpai::Render() {
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sOpposite), TableSize / 2      ,                 192);
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sLeft    ),                 256, TableSize / 2      );
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sRight   ), TableSize     - 256, TableSize / 2      );
	ShowTenpaiFlag(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sSelf    ), TableSize / 2      , TableSize     - 192);
}

}
