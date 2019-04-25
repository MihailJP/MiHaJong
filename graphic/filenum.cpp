#include "filenum.h"
#include "resource.h"

namespace mihajong_graphic {

std::string dataFileName(int fileID) {
	switch (fileID) {
	case IDB_PNG_TBLBAIZE:
		return PKGDATADIR "/graphic/baize.png";
	case IDB_PNG_TBLBORDER:
		return PKGDATADIR "/graphic/frame.png";
	case IDB_PNG_SDBAR:
		return PKGDATADIR "/graphic/sidebar.png";
	case IDB_PNG_TILE:
		return PKGDATADIR "/graphic/tiles.png";
	case IDB_PNG_FONT:
		return PKGDATADIR "/graphic/font.png";
	case IDB_PNG_TITLE:
		return PKGDATADIR "/graphic/title.png";
	case IDB_PNG_BUTTON:
		return PKGDATADIR "/graphic/button.png";
	case IDB_PNG_FONT_HUGE:
		return PKGDATADIR "/graphic/largechr.png";
	case IDB_PNG_DICE:
		return PKGDATADIR "/graphic/dice.png";
	case IDB_PNG_FONT_SMALL:
		return PKGDATADIR "/graphic/smallchr.png";
	case IDB_PNG_TENBOU:
		return PKGDATADIR "/graphic/tenbou.png";
	case IDB_PNG_CHICHAMARK:
		return PKGDATADIR "/graphic/chiicha.png";
	case IDB_PNG_SCORE_INDICATOR:
		return PKGDATADIR "/graphic/score.png";
	case IDB_PNG_CALL_TEXT:
		return PKGDATADIR "/graphic/call.png";
	case IDB_PNG_CALL_DIGITS:
		return PKGDATADIR "/graphic/cdigits.png";
	case IDB_PNG_AGARI_WINDOW:
		return PKGDATADIR "/graphic/agariwnd.png";
	case IDB_PNG_SCORE_DIGITS:
		return PKGDATADIR "/graphic/sdigits.png";
	case IDB_PNG_CHECKBOX:
		return PKGDATADIR "/graphic/checkbox.png";
	case IDB_PNG_TILE_BLACK:
		return PKGDATADIR "/graphic/tiles_bk.png";
	case IDB_PNG_MOON_CLOCK:
		return PKGDATADIR "/graphic/clock.png";
	case IDB_PNG_SPLASH_SCREEN:
		return PKGDATADIR "/graphic/splash.png";
	case IDB_PNG_TITLE_BACKGROUND:
		return PKGDATADIR "/graphic/titlback.png";
	case IDB_CHARMAP_FONT:
		return PKGDATADIR "/graphic/font.chr";
	case IDB_CHARMAP_FONT_HUGE:
		return PKGDATADIR "/graphic/largechr.chr";
	case IDB_CHARMAP_FONT_SMALL:
		return PKGDATADIR "/graphic/smallchr.chr";
	case IDB_CHARMAP_CALL_DIGITS:
		return PKGDATADIR "/graphic/cdigits.chr";
	case IDB_CHARMAP_SCORE_DIGITS:
		return PKGDATADIR "/graphic/sdigits.chr";
	default:
		return "";
	}
}

}
