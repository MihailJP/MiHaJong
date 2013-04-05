#include "call.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../utils.h"
#include "../../event.h"
#include "../table.h"

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubsceneCallProto::TableSubsceneCallProto(LPDIRECT3DDEVICE9 device) : TableSubscene(device) {
	LoadTexture(device, &tCall, MAKEINTRESOURCE(IDB_PNG_CALL_TEXT), 384, 1920);
}

TableSubsceneCallProto::~TableSubsceneCallProto() {
	if (tCall) tCall->Release();
}

void TableSubsceneCallProto::ShowAllCall() {
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sOpposite), TableSize / 2      ,                 192);
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sLeft    ),                 256, TableSize / 2      );
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sRight   ), TableSize     - 256, TableSize / 2      );
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sSelf    ), TableSize / 2      , TableSize     - 192);
}

void TableSubsceneCallProto::skipEvent() {
	ui::cancellableWait->set(1);
}

// -------------------------------------------------------------------------

TableSubsceneCallZoomProto::TableSubsceneCallZoomProto(LPDIRECT3DDEVICE9 device) : TableSubsceneCallProto(device) {
}

TableSubsceneCallZoomProto::~TableSubsceneCallZoomProto() {
}

/* 表示処理 */
void TableSubsceneCallZoomProto::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if (callType == calltext::None) return;
	const std::uint64_t curr = myTimer.elapsed();
	const int animationLength = 250000;
	const float scale = (curr >= animationLength) ? 1.0f : pow((float)(animationLength - (signed)curr) / 2.5e5f + 1.0f, 2);
	const D3DCOLOR col = D3DCOLOR_ARGB(
		(curr >= animationLength) ? 255 :
		(int)pow((float)(curr * 255) / animationLength / 16.0f, 2),
		0xff, 0xff, 0xff);
	D3DXMATRIX matrix, matrix1;
	D3DXMatrixIdentity(&matrix); D3DXMatrixIdentity(&matrix1);
	D3DXMatrixTranslation(&matrix1, (float)(-x), (float)(-y), 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, scale, scale, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixTranslation(&matrix1, (float)x, (float)y, 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	D3DXMatrixScaling(&matrix1, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f); D3DXMatrixMultiply(&matrix, &matrix, &matrix1);
	RECT rect = {
		0  , 96 * (callType    ),
		384, 96 * (callType + 1),
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(tCall, x, y, 384, 96, col, &rect, 192, 48, &matrix);
}

void TableSubsceneCallZoomProto::ShowCall(PlayerID player, int x, int y) {
	ShowCallMsg(player, calltext::getCall(player), x, y);
}

// -------------------------------------------------------------------------

TableSubsceneCall::TableSubsceneCall(LPDIRECT3DDEVICE9 device) : TableSubsceneCallZoomProto(device) {
}

TableSubsceneCall::~TableSubsceneCall() {
}

void TableSubsceneCall::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

TableSubsceneCallFadeProto::TableSubsceneCallFadeProto(LPDIRECT3DDEVICE9 device) : TableSubsceneCallProto(device) {
}

TableSubsceneCallFadeProto::~TableSubsceneCallFadeProto() {
}

/* 表示処理 */
void TableSubsceneCallFadeProto::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if (callType == calltext::None) return;
	const std::uint64_t curr = myTimer.elapsed();
	const int animationLength = 250000;
	const D3DCOLOR col = D3DCOLOR_ARGB(
		(curr >= animationLength) ? 255 :
		(int)pow((float)(curr * 255) / animationLength / 16.0f, 2),
		0xff, 0xff, 0xff);
	RECT rect = {
		0  , 96 * (callType    ),
		384, 96 * (callType + 1),
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(tCall, x, y, 384, 96, col, &rect, 192, 48);
}

void TableSubsceneCallFadeProto::ShowCall(PlayerID player, int x, int y) {
	ShowCallMsg(player, calltext::getCall(player), x, y);
}

// -------------------------------------------------------------------------

TableSubsceneCallFade::TableSubsceneCallFade(LPDIRECT3DDEVICE9 device) : TableSubsceneCallFadeProto(device) {
}

TableSubsceneCallFade::~TableSubsceneCallFade() {
}

void TableSubsceneCallFade::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

TableSubsceneCallCutProto::TableSubsceneCallCutProto(LPDIRECT3DDEVICE9 device) : TableSubsceneCallProto(device) {
}

TableSubsceneCallCutProto::~TableSubsceneCallCutProto() {
}

/* 表示処理 */
void TableSubsceneCallCutProto::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if (callType == calltext::None) return;
	RECT rect = {
		0  , 96 * (callType    ),
		384, 96 * (callType + 1),
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(tCall, x, y, 384, 96, 0xffffffff, &rect, 192, 48);
}

void TableSubsceneCallCutProto::ShowCall(PlayerID player, int x, int y) {
	ShowCallMsg(player, calltext::getCall(player), x, y);
}

// -------------------------------------------------------------------------

TableSubsceneCallCut::TableSubsceneCallCut(LPDIRECT3DDEVICE9 device) : TableSubsceneCallCutProto(device) {
}

TableSubsceneCallCut::~TableSubsceneCallCut() {
}

void TableSubsceneCallCut::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

TableSubsceneCallChankanPre::TableSubsceneCallChankanPre(LPDIRECT3DDEVICE9 device) : TableSubsceneCallCutProto(device) {
}

TableSubsceneCallChankanPre::~TableSubsceneCallChankanPre() {
}

/* 表示処理 */
void TableSubsceneCallChankanPre::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if ((callType != calltext::Ron) && (callType != calltext::RonQualified))
		TableSubsceneCallCutProto::ShowCallMsg(player, callType, x, y);
}

void TableSubsceneCallChankanPre::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

TableSubsceneCallChankanRon::TableSubsceneCallChankanRon(LPDIRECT3DDEVICE9 device) : TableSubsceneCallProto(device) {
	zoomCall = new TableSubsceneCall(device);
	cutCall = new TableSubsceneCallCut(device);
}

TableSubsceneCallChankanRon::~TableSubsceneCallChankanRon() {
	delete zoomCall;
	delete cutCall;
}

/* 表示処理 */
void TableSubsceneCallChankanRon::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if ((callType == calltext::Ron) || (callType == calltext::RonQualified))
		zoomCall->ShowCallMsg(player, callType, x, y);
	else
		cutCall->ShowCallMsg(player, callType, x, y);
}

void TableSubsceneCallChankanRon::ShowCall(PlayerID player, int x, int y) {
	ShowCallMsg(player, calltext::getCall(player), x, y);
}

void TableSubsceneCallChankanRon::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

}
