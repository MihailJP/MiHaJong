#include "filesel.h"

#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "../func.h"
#include "../random.h"

std::vector<std::string> aiscript::FileSelector::files; // AIのスクリプトファイル一覧をフルパスで格納

/* ファイルリスト走査 */
void aiscript::FileSelector::filelist() {
#ifdef _WIN32
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
#else /*_WIN32*/
	/* TODO: 未実装箇所 */
#endif /*_WIN32*/
}

/* ファイルをランダムに選択 */
std::string aiscript::FileSelector::randomfile() {
	return files[RndNum::rnd(files.size())];
}
