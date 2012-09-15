#include "filesel.h"

std::vector<std::string> aiscript::FileSelector::files; // AIのスクリプトファイル一覧をフルパスで格納

/* ファイルリスト走査 */
void aiscript::FileSelector::filelist() {
	std::string confPath = confpath::confPath();
	if (confPath.empty()) confPath = ".";
	std::string scriptPath = confPath + std::string("\\ai");
	std::string scriptFiles = scriptPath + std::string("\\*.lua");
	files.clear();
	WIN32_FIND_DATA finddat;
	info("AIスクリプトを検索します");

	/* 検索開始 */
	HANDLE h = FindFirstFile(scriptFiles.c_str(), &finddat);
	if (h == INVALID_HANDLE_VALUE) {
		error("ファイル検索できません！！"); return;
	}
	do { // 検索
		if (!strcmp(finddat.cFileName + strlen(finddat.cFileName) - 4, ".lua")) {
			std::ostringstream o; o << "検出: " << finddat.cFileName; info(o.str().c_str());
			files.push_back(scriptPath + std::string("\\") + std::string(finddat.cFileName));
		}
	} while (FindNextFile(h, &finddat));
	/* 検索完了！ */
	FindClose(h);
}

/* ファイルをランダムに選択 */
std::string aiscript::FileSelector::randomfile() {
	return files[RndNum::rnd(files.size())];
}
