#include "filesel.h"

#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else  /*_WIN32*/
#include <dirent.h>
#endif /*_WIN32*/
#include "../func.h"
#include "../random.h"

std::vector<std::string> aiscript::FileSelector::files; // AIのスクリプトファイル一覧をフルパスで格納

/* ファイルリスト走査 */
void aiscript::FileSelector::filelist() {
	std::string confPath = confpath::confPath();
	if (confPath.empty()) confPath = ".";
#ifdef _WIN32
#ifdef GUOBIAO
	std::string scriptPath = confPath + std::string("\\gbai");
#else /* GUOBIAO */
	std::string scriptPath = confPath + std::string("\\ai");
#endif /* GUOBIAO */
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
#ifdef GUOBIAO
	std::string scriptPath = confPath + std::string("/gbai");
#else /* GUOBIAO */
	std::string scriptPath = confPath + std::string("/ai");
#endif /* GUOBIAO */
	files.clear();
	info(_T("AIスクリプトを検索します"));

	/* 検索開始 */
	DIR* dir = opendir(scriptPath.c_str());
	dirent* dirp;
	if (!dir) {
		error(_T("ファイル検索できません！！")); return;
	}
	while (dirp = readdir(dir)) { // 検索
		if (!strcmp(dirp->d_name + strlen(dirp->d_name) - 4, ".lua")) {
			CodeConv::tostringstream o; o << _T("検出: ") << (dirp->d_name); info(o.str().c_str());
			files.push_back(scriptPath + std::string("/") + std::string(dirp->d_name));
		}
	}
	/* 検索完了！ */
	closedir(dir);
#endif /*_WIN32*/
}

/* ファイルをランダムに選択 */
std::string aiscript::FileSelector::randomfile() {
	return files[RndNum::rnd(files.size())];
}
