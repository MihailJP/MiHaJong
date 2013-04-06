#include "filesel.h"

#include <sstream>
#include <windows.h>
#include "../func.h"
#include "../random.h"

std::vector<std::string> aiscript::FileSelector::files; // AIのスクリプトファイル一覧をフルパスで格納

/* ファイルリスト走査 */
void aiscript::FileSelector::filelist() {
	std::string confPath = confpath::confPath();
	if (confPath.empty()) confPath = ".";
	std::string scriptPath = confPath + std::string("\\ai");
	std::string scriptFiles = scriptPath + std::string("\\*.lua");
	files.clear();
	WIN32_FIND_DATAA finddat;
	info(_T("AIスクリプトを検索します"));

	/* 検索開始 */
	HANDLE h = FindFirstFileA(scriptFiles.c_str(), &finddat);
	if (h == INVALID_HANDLE_VALUE) {
		error(_T("ファイル検索できません！！")); return;
	}
	do { // 検索
		if (!strcmp(finddat.cFileName + strlen(finddat.cFileName) - 4, ".lua")) {
			CodeConv::tostringstream o; o << _T("検出: ") << finddat.cFileName; info(o.str().c_str());
			files.push_back(scriptPath + std::string("\\") + std::string(finddat.cFileName));
		}
	} while (FindNextFileA(h, &finddat));
	/* 検索完了！ */
	FindClose(h);
}

/* ファイルをランダムに選択 */
std::string aiscript::FileSelector::randomfile() {
	return files[RndNum::rnd(files.size())];
}
