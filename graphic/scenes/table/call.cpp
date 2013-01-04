#include "call.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../calltext.h"
#include "../../utils.h"
#include "../table.h"

namespace mihajong_graphic {

TableSubsceneCall::TableSubsceneCall(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	LoadTexture(device, &tCall, MAKEINTRESOURCE(IDB_PNG_CALL_TEXT), 384, 1632);
	if (FAILED(D3DXCreateSprite(device, &sCall)))
		throw _T("スプライトの生成に失敗しました");
}

TableSubsceneCall::~TableSubsceneCall() {
	if (tCall) tCall->Release();
	if (sCall) sCall->Release();
}

/* 表示処理 */
void TableSubsceneCall::ShowCall(PLAYER_ID player, int x, int y) {
	if (calltext::getCall(player) == calltext::None) return;
	const std::uint64_t curr = currTime();
	const int animationLength = 2500000;
	const float scale = (curr >= (startTime + animationLength)) ? 1.0f : std::pow((float)(animationLength - ((signed)curr - (signed)startTime)) / 2.5e6f + 1.0f, 2);
	const D3DCOLOR col = D3DCOLOR_ARGB(
		(curr >= (startTime + animationLength)) ? 255 :
		(int)std::pow((float)((curr - startTime) * 255) / animationLength / 16.0f, 2),
		0xff, 0xff, 0xff);
	D3DXMATRIX matrix, matrix1;
	D3DXMatrixIdentity(&matrix); D3DXMatrixIdentity(&matrix1);
	D3DXMatrixTranslation(&matrix1, (float)(-x), (float)(-y), 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, scale, scale, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixTranslation(&matrix1, (float)x, (float)y, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	RECT rect = {
		0  , 96 * (calltext::getCall(player)    ),
		384, 96 * (calltext::getCall(player) + 1),
	};
	SpriteRenderer::ShowSprite(sCall, tCall, x, y, 384, 96, col, &rect, 192, 48, &matrix);
}

void TableSubsceneCall::Render() {
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sOpposite), TableSize / 2      ,                 192);
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sLeft    ),                 256, TableSize / 2      );
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sRight   ), TableSize     - 256, TableSize / 2      );
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sSelf    ), TableSize / 2      , TableSize     - 192);
}

}
