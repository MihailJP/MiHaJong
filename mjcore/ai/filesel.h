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
};
