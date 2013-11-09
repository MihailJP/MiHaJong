#pragma once

#include <string>
#include <vector>
#include "class.h"

class aiscript::FileSelector {
private:
	static std::vector<std::string> files; // AIのスクリプトファイル一覧をフルパスで格納
public:
	static void filelist(); // ファイル一覧を用意！
	static std::string randomfile();
public: /* Monostate class: cannot be instantiated */
	FileSelector() = delete;
	FileSelector(const FileSelector&) = delete;
	FileSelector& operator= (const FileSelector&) = delete;
	~FileSelector() = delete;
};
