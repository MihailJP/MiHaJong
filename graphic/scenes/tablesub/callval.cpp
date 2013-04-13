#include "callval.h"
#include "../../utils.h"
#include "../../calltext.h"
#include <cmath>
#include <cassert>
#include "../../event.h"

namespace mihajong_graphic {

TableSubsceneCallValue::TableSubsceneCallValue(DevicePtr device, const CodeConv::tstring& caption) : TableSubscene(device) {
	fontRenderer = new CallDigitRenderer(device);
	captionRenderer = nullptr;
	if (!caption.empty()) captionRenderer = new TableSubsceneMsg(device, caption);
}

TableSubsceneCallValue::~TableSubsceneCallValue() {
	delete fontRenderer;
	if (captionRenderer) delete captionRenderer;
}

const std::array<CodeConv::tstring, 17> TableSubsceneCallValue::digitName = {
	_T(""),
	_T("ñú"),     _T("â≠"),     _T("íõ"),     _T("ãû"),
	_T("ö¥"),     _T("û`"),     _T("èı"),     _T("ça"),
	_T("ä¿"),     _T("ê≥"),     _T("ç⁄"),     _T("ã…"),
	_T("çPâÕçπ"), _T("à¢ëmã_"), _T("ìﬂóRëº"), _T("ïsâ¬évãc"),
};

void TableSubsceneCallValue::ShowCall(PlayerID player, int x, int y) {
	fontRenderer->DelText(player);
	callvalue::CallValue c_val = callvalue::getVal(player);
	if ((c_val.Mantissa == 0) && (c_val.Exponent == 0)) return; // ï\é¶ÇµÇ»Ç¢èÍçá
	assert(c_val.Exponent >= 2u); // è¨êîÇ…ÇÕëŒâûÇµÇ»Ç¢

	const std::uint64_t curr = myTimer.elapsed();
	const int animationLength = 250000;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	const ArgbColor col = D3DCOLOR_ARGB(
		(curr >= animationLength) ? 255 :
		(int)std::pow((float)(curr * 255) / animationLength / 16.0f, 2),
		c_val.Mantissa > 0 ? 0xcc : 0xff,
		c_val.Mantissa < 0 ? 0xcc : 0xff,
		0xcc);
#else
	/* TODO: OpenGLÇ≈çƒé¿ëï */
	const ArgbColor col = 0xffffffff;
#endif

	CodeConv::tostringstream o; unsigned callLen = 0u;
	if (c_val.Mantissa != 0) {
		o << ((c_val.Mantissa > 0) ? _T('+') : _T('-')); // ïÑçÜ
		if (c_val.Exponent < 5u) { // 100Å`99900Ç‹Ç≈
			o << std::abs(c_val.Mantissa);
			if      (c_val.Exponent == 4u) o << _T("00");
			else if (c_val.Exponent == 3u) o << _T("0");
			callLen = o.str().size();
		} else { // ê¬ìVÉãÅ[Éãóp
			switch (c_val.Exponent % 4) {
			case 0:
				o << ((int)std::abs(c_val.Mantissa) / 100) << _T('.') << ((int)std::abs(c_val.Mantissa) % 100);
				break;
			case 1:
				o << ((int)std::abs(c_val.Mantissa) / 10) << _T('.') << ((int)std::abs(c_val.Mantissa) % 10);
				break;
			case 2:
				o << (int)std::abs(c_val.Mantissa);
				break;
			case 3:
				o << (int)std::abs(c_val.Mantissa) << _T('0');
				break;
			}
			callLen = o.str().size() + 1u;
			o << digitName[c_val.Exponent / 4];
		}
	} else {
		o << _T('0'); callLen = 1u;
	}
	fontRenderer->NewText(player, o.str(), x - 24 * callLen, y - 48, 1.0f, 1.0f, col);
}

void TableSubsceneCallValue::ShowAllCall() {
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sOpposite), TableSize / 2      ,                 192);
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sLeft    ),                 256, TableSize / 2      );
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sRight   ), TableSize     - 256, TableSize / 2      );
	ShowCall(utils::RelativePositionOf(GameStatus::gameStat()->PlayerID, sSelf    ), TableSize / 2      , TableSize     - 192);
}

void TableSubsceneCallValue::Render() {
	captionRenderer->Render();
	ShowAllCall();
	fontRenderer->Render();
}

void TableSubsceneCallValue::skipEvent() {
	ui::cancellableWait->set(1);
}

}
