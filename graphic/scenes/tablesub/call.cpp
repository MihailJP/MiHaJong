#include "call.h"
#include "../../resource.h"
#include "../../sprite.h"
#include "../../utils.h"
#include "../../event.h"
#include "../table.h"
#include "../../matrix.h"

#ifdef None
#undef None
#endif

namespace mihajong_graphic {

// -------------------------------------------------------------------------

TableSubsceneCallProto::TableSubsceneCallProto(DevicePtr device) : TableSubscene(device) {
	LoadTexture(device, &tCall, MAKEINTRESOURCE(IDB_PNG_CALL_TEXT));
}

TableSubsceneCallProto::~TableSubsceneCallProto() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (tCall) tCall->Release();
#endif
}

void TableSubsceneCallProto::ShowAllCall() {
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, SeatRelative::opposite), TableSize / 2      ,                 192);
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, SeatRelative::left    ),                 256, TableSize / 2      );
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, SeatRelative::right   ), TableSize     - 256, TableSize / 2      );
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, SeatRelative::self    ), TableSize / 2      , TableSize     - 192);
}

void TableSubsceneCallProto::skipEvent() {
	ui::cancellableWait->set(1);
}

// -------------------------------------------------------------------------

TableSubsceneCallZoomProto::TableSubsceneCallZoomProto(DevicePtr device) : TableSubsceneCallProto(device) {
}

TableSubsceneCallZoomProto::~TableSubsceneCallZoomProto() {
}

/* 表示処理 */
void TableSubsceneCallZoomProto::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if (callType == calltext::CallType::none) return;
	const std::uint64_t curr = myTimer.elapsed();
	constexpr int animationLength = 250000;
	const float scale = (curr >= animationLength) ? 1.0f : pow(static_cast<float>(animationLength - static_cast<signed>(curr)) / 2.5e5f + 1.0f, 2);
	const ArgbColor col = static_cast<uint32_t>(
		(curr >= animationLength) ? 255 :
		static_cast<int>(pow(static_cast<float>(curr * 255) / animationLength / 16.0f, 2)))
		<< 24 | 0x00ffffff;
	const TransformMatrix matrix(getMatrix(static_cast<float>(x), static_cast<float>(y), scale, scale));
	const RECT rect = {
		0  , 96 * (static_cast<int>(callType)    ),
		384, 96 * (static_cast<int>(callType) + 1),
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(tCall, x, y, 384, 96, col, &rect, 192, 48, &matrix);
}

void TableSubsceneCallZoomProto::ShowCall(PlayerID player, int x, int y) {
	ShowCallMsg(player, calltext::getCall(player), x, y);
}

// -------------------------------------------------------------------------

TableSubsceneCall::TableSubsceneCall(DevicePtr device) : TableSubsceneCallZoomProto(device) {
}

TableSubsceneCall::~TableSubsceneCall() {
}

void TableSubsceneCall::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

TableSubsceneCallFadeProto::TableSubsceneCallFadeProto(DevicePtr device) : TableSubsceneCallProto(device) {
}

TableSubsceneCallFadeProto::~TableSubsceneCallFadeProto() {
}

/* 表示処理 */
void TableSubsceneCallFadeProto::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if (callType == calltext::CallType::none) return;
	const std::uint64_t curr = myTimer.elapsed();
	constexpr int animationLength = 250000;
	const ArgbColor col = static_cast<uint32_t>(
		(curr >= animationLength) ? 255 :
		static_cast<int>(pow(static_cast<float>(curr * 255) / animationLength / 16.0f, 2)))
		<< 24 | 0x00ffffff;
	const RECT rect = {
		0  , 96 * (static_cast<int>(callType)    ),
		384, 96 * (static_cast<int>(callType) + 1),
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(tCall, x, y, 384, 96, col, &rect, 192, 48);
}

void TableSubsceneCallFadeProto::ShowCall(PlayerID player, int x, int y) {
	ShowCallMsg(player, calltext::getCall(player), x, y);
}

// -------------------------------------------------------------------------

TableSubsceneCallFade::TableSubsceneCallFade(DevicePtr device) : TableSubsceneCallFadeProto(device) {
}

TableSubsceneCallFade::~TableSubsceneCallFade() {
}

void TableSubsceneCallFade::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

TableSubsceneCallCutProto::TableSubsceneCallCutProto(DevicePtr device) : TableSubsceneCallProto(device) {
}

TableSubsceneCallCutProto::~TableSubsceneCallCutProto() {
}

/* 表示処理 */
void TableSubsceneCallCutProto::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if (callType == calltext::CallType::none) return;
	const RECT rect = {
		0  , 96 * (static_cast<int>(callType)    ),
		384, 96 * (static_cast<int>(callType) + 1),
	};
	SpriteRenderer::instantiate(myDevice)->ShowSprite(tCall, x, y, 384, 96, 0xffffffff, &rect, 192, 48);
}

void TableSubsceneCallCutProto::ShowCall(PlayerID player, int x, int y) {
	ShowCallMsg(player, calltext::getCall(player), x, y);
}

// -------------------------------------------------------------------------

TableSubsceneCallCut::TableSubsceneCallCut(DevicePtr device) : TableSubsceneCallCutProto(device) {
}

TableSubsceneCallCut::~TableSubsceneCallCut() {
}

void TableSubsceneCallCut::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

TableSubsceneCallChankanPre::TableSubsceneCallChankanPre(DevicePtr device) : TableSubsceneCallCutProto(device) {
}

TableSubsceneCallChankanPre::~TableSubsceneCallChankanPre() {
}

/* 表示処理 */
void TableSubsceneCallChankanPre::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if ((callType != calltext::CallType::ron) && (callType != calltext::CallType::ronQualified))
		TableSubsceneCallCutProto::ShowCallMsg(player, callType, x, y);
}

void TableSubsceneCallChankanPre::Render() {
	ShowAllCall();
}

// -------------------------------------------------------------------------

TableSubsceneCallChankanRon::TableSubsceneCallChankanRon(DevicePtr device) : TableSubsceneCallProto(device) {
	zoomCall = new TableSubsceneCall(device);
	cutCall = new TableSubsceneCallCut(device);
}

TableSubsceneCallChankanRon::~TableSubsceneCallChankanRon() {
	delete zoomCall;
	delete cutCall;
}

/* 表示処理 */
void TableSubsceneCallChankanRon::ShowCallMsg(PlayerID player, calltext::CallType callType, int x, int y) {
	if ((callType == calltext::CallType::ron) || (callType == calltext::CallType::ronQualified))
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
