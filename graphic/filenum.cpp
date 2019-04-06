#include "filenum.h"
#include "resource.h"

namespace mihajong_graphic {

std::string dataFileName(int fileID) {
	switch (fileID) {
	case IDB_PNG_TBLBAIZE:
		return PKGDATADIR "/baize.png";
	case IDB_PNG_TBLBORDER:
		return PKGDATADIR "/frame.png";
	case IDB_PNG_SDBAR:
		return PKGDATADIR "/sidebar.png";
	case IDB_PNG_TILE:
		return PKGDATADIR "/tiles.png";
	case IDB_PNG_FONT:
		return PKGDATADIR "/font.png";
	case IDB_PNG_TITLE:
		return PKGDATADIR "/title.png";
	case IDB_PNG_BUTTON:
		return PKGDATADIR "/button.png";
	case IDB_PNG_FONT_HUGE:
		return PKGDATADIR "/largechr.png";
	case IDB_PNG_DICE:
		return PKGDATADIR "/dice.png";
	case IDB_PNG_FONT_SMALL:
		return PKGDATADIR "/smallchr.png";
	case IDB_PNG_TENBOU:
		return PKGDATADIR "/tenbou.png";
	case IDB_PNG_CHICHAMARK:
		return PKGDATADIR "/chiicha.png";
	case IDB_PNG_SCORE_INDICATOR:
		return PKGDATADIR "/score.png";
	case IDB_PNG_CALL_TEXT:
		return PKGDATADIR "/call.png";
	case IDB_PNG_CALL_DIGITS:
		return PKGDATADIR "/cdigits.png";
	case IDB_PNG_AGARI_WINDOW:
		return PKGDATADIR "/agariwnd.png";
	case IDB_PNG_SCORE_DIGITS:
		return PKGDATADIR "/sdigits.png";
	case IDB_PNG_CHECKBOX:
		return PKGDATADIR "/checkbox.png";
	case IDB_PNG_TILE_BLACK:
		return PKGDATADIR "/tiles_bk.png";
	case IDB_PNG_MOON_CLOCK:
		return PKGDATADIR "/clock.png";
	case IDB_CHARMAP_FONT:
		return PKGDATADIR "/font.chr";
	case IDB_CHARMAP_FONT_HUGE:
		return PKGDATADIR "/largechr.chr";
	case IDB_CHARMAP_FONT_SMALL:
		return PKGDATADIR "/smallchr.chr";
	case IDB_CHARMAP_CALL_DIGITS:
		return PKGDATADIR "/cdigits.chr";
	case IDB_CHARMAP_SCORE_DIGITS:
		return PKGDATADIR "/sdigits.chr";
	default:
		return "";
	}
}

}
