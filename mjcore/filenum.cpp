#include "filenum.h"
#include "resource.h"

std::string dataFileName(int fileID) {
	switch (fileID) {
	case IDR_LZMA_STREA1:
		return PKGDATADIR "/mentz.dat.lzma";
	case IDR_LZMA_STREA2:
		return PKGDATADIR "/confitem.csv.lzma";
	case IDR_LZMA_STREA3:
		return PKGDATADIR "/confitem.ini.lzma";
	case IDR_LZMA_STREA4:
		return PKGDATADIR "/rankval.csv.lzma";
	case IDR_LZMA_STREA5:
		return PKGDATADIR "/prefitem.csv.lzma";
	case IDR_LZMA_STREA6:
		return PKGDATADIR "/prefitem.ini.lzma";
	default:
		return "";
	}
}
