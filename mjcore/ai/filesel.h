#ifndef FILESEL_H
#define FILESEL_H

#include <string>
#include <set>
#include <sstream>
#include <Windows.h>
#include "class.h"
#include "../func.h"

class aiscript::FileSelector {
private:
	static std::set<std::string> files; // AIのスクリプトファイル一覧をフルパスで格納
public:
	static void filelist(); // ファイル一覧を用意！
};

#endif
